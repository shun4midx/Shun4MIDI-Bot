/********************************************
 * Copyright (c) 2025 Shun/翔海 (@shun4midx) *
 * Project: Shun4MIDI-Bot                   *
 * File Type: C++ file                      *
 * File: note.cpp                           *
 ****************************************** */

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <stdexcept>
#include <unordered_map>
#include <math.h>
#include "note.h"

using namespace Shun4MIDI;
static const std::regex parseStrRegex(R"(^([A-Ga-g])([ud]?)(x|[b#]{0,2})(\d+)?$)");

// ======== PRELIMINARIES ========= //
// get() functions
int Note::Semitones() const {
    return semitones;
}

int Note::Micro_Offset() const {
    return micro_offset;
}

char Note::Letter() const {
    return letter;
}

int Note::Alteration() const {
    return alteration;
}

int Note::Octave() const {
    return octave;
}

// Constructors
Note::Note() { // Default constructor
    semitones = 0;
    micro_offset = 0;
    letter = 'C';
    alteration = 0;
    octave = 1;
}

Note::Note(const char* note_name)
  : Note(std::string(note_name))
{}

Note::Note(const std::string& note_name) {
    parseName(note_name);
} 

Note::Note(int _semitones, int _alteration, int _micro_offset) {
    semitones = _semitones;
    alteration = _alteration;
    micro_offset = _micro_offset;

    // Find letter and octave (Probably from semitones)
    setLetterOctave();
}

Note& Note::operator=(const Note& _note) {
    semitones = _note.semitones;
    micro_offset = _note.micro_offset;
    letter = _note.letter;
    alteration = _note.alteration;
    octave = _note.octave;
    return *this;
}

Note& Note::operator=(const char* note_name) {
    parseName(note_name);
    return *this;
}

Note& Note::operator=(const std::string& note_name) {
    parseName(note_name);
    return *this;
}

// ======== METHODS ========= //
Note Note::semitoneToNote(double _semitones, int _micro_offset, bool prefer_sharps) {
    // Differentiate 0.5 based on prefer_sharps
    int temp_micro_offset = 0; // Micro-offset that we would add
    int int_semitones = (int)_semitones;
    if (_semitones * 2 - int_semitones * 2 == 0.5 && _semitones >= 0) { // Has 0.5
        if (prefer_sharps) { // 1.5 -> 1 + 0.5
            temp_micro_offset = 1;
        } else { // 1.5 -> 2 - 0.5
            temp_micro_offset = -1;
            int_semitones += 1;
        }
    } else if (_semitones * 2 - int_semitones * 2 != 0 || _semitones < 0) { // Invalid
        throw std::invalid_argument("Invalid semitone number inputted: " + std::to_string(_semitones));
    }

    std::pair<std::pair<char, int>, int> letter_octave_offset_pair = findLetterOctave(int_semitones, 0, prefer_sharps);

    Note new_note;
    new_note.semitones = int_semitones;
    new_note.micro_offset = temp_micro_offset;
    new_note.letter = letter_octave_offset_pair.first.first;
    new_note.alteration = letter_octave_offset_pair.second;
    new_note.octave = letter_octave_offset_pair.first.second;

    // Make note most "simplified"
    Note ans_note = new_note.enharmonic((prefer_sharps ? 1 : -1));
    return ans_note;
}

std::string Note::name(bool explicit_natural, std::vector<Note> same_measure_notes) const { // Sometimes we want to use 'n' for natural explicitly
    std::string output = "";

    // Letter
    output += letter;

    // Microtone
    if (micro_offset == 1) {
        output += "u";
    } else if (micro_offset == -1) {
        output += "d";
    } else {
        if (micro_offset == 0 && alteration == 0 && explicit_natural) {
            // Check if same measure notes share any others that aren't natural right before (starting from the right then check to the left)
            for (int i = same_measure_notes.size() - 1; i >= 0; --i) {
                Note measure_note = same_measure_notes[i];
                if (measure_note.octave == octave && measure_note.letter == letter && (alteration != 0 || micro_offset != 0)) { // Need 'n'
                    output += "n";
                    break;
                }
            }
        }
    }

    // Sharp/Flat
    if (alteration == 1) {
        output += "#";
    } else if (alteration == -1) {
        output += "b";
    } else if (alteration == 2) {
        output += "x";
    } else if (alteration == -2) {
        output += "bb";
    }

    // Octave
    output += std::to_string(octave);

    return output;
}

int Note::noteToSemitone(char _letter, int _alteration, int _octave) {
    // Each octave adds 12 semitones
    // Throw exceptions
    int letter_semitone;
    if (letterToSemitone.find(_letter) == letterToSemitone.end()) {
        throw std::invalid_argument("Invalid letter: " + std::string(1, _letter));
    } else {
        letter_semitone = letterToSemitone.at(_letter);
    }

    if (!validOctave(_letter, _octave)) {
        throw std::out_of_range(("Out of range letter octave combination: " + std::string(1, _letter)) + std::to_string(_octave));
    }

    // Calculate
    int ans = letter_semitone + (_octave - 1) * 12 + _alteration;

    if (ans < -3) { // Less than A0
        throw std::out_of_range("Out of range note requested.");
    }
    
    return ans;
}

Note Note::enharmonic(char base_letter, int prefer_alteration) const { // X for default, also we ofc assume the 'simplest' enharmonic for default
    // Catch invalid inputs
    if (base_letter != 'X' && (base_letter < 'A' || base_letter > 'G')) {
        throw std::invalid_argument("Invalid base letter requested: " + std::string(1, base_letter));
    }

    // Otherwise
    typedef struct candidate {
        char _letter;
        int _alteration;
        int _micro_offset;
        int _symbol_count;
        int _octave;
        double _diff;
    } Candidate;
    std::vector<Candidate> cands;

    double target = semitones + micro_offset * 0.5; // Current note

    // Generate all candidates from the 7 letters
    for (char L : {'C', 'D', 'E', 'F', 'G', 'A', 'B'}) {
        if (base_letter != 'X' && base_letter != L) {
            continue;
        }

        // Calculate diff
        int cand_octave = octave;
        int letter_semitones = letterToSemitone.at(L) + 12 * cand_octave;
        double diff = target - (double)(letter_semitones);

        // Wrap diff
        while (abs(diff) > 6) {
            if (diff > 6) {
                diff -= 12;
                ++cand_octave;
            }

            if (diff < -6) {
                diff += 12;
                --cand_octave;
            }
        }

        // Determine valididty of cand_octave
        if (!validOctave(L, cand_octave)) {
            continue;
        }

        // Split diff into whole semitone vs microtone
        int alter, micro;

        // Split s.t. alter and micro are the same sign
        if (diff == 0) {
            alter = 0;
            micro = 0;
        } else if (diff > 0) {
            alter = (int)(diff * 2) / 2;
            micro = (int)(diff * 2) % 2;
        } else {
            alter = (int)(diff * -2) / 2 * -1;
            micro = ((int)(diff * -2) % 2) * -1;
        }

        // Drop nonsensical candidates
        if (abs(alter) > 2 || abs(micro) > 1) {
            continue;
        }

        // Measure symbol count
        int sym_count = abs(alter) + abs(micro);

        // Push
        cands.push_back({L, alter, micro, sym_count, cand_octave, fabs(diff)});
    }

    // Filter by base letter and preferred alteration
    if (prefer_alteration > 0) {
        cands.erase(
            std::remove_if(cands.begin(), cands.end(), [&](auto &c){ return c._alteration < 0 || c._micro_offset < 0; }),
            cands.end()
        );
    }

    if (prefer_alteration < 0) {
        cands.erase(
            std::remove_if(cands.begin(), cands.end(), [&](auto &c){ return c._alteration > 0 || c._micro_offset > 0; }),
            cands.end()
        );
    }

    if (cands.empty()) {
        throw std::runtime_error("No reasonable enharmonic found for " + this->name() + " with base letter " + std::string(1, base_letter));
    }

    // Pick best by error then by symbol count
    Candidate best = *min_element(cands.begin(), cands.end(),
        [&](auto &a, auto &b){
            if (a._diff != b._diff) {
                return a._diff < b._diff; // Smallest pitch error first
            }
            return a._symbol_count < b._symbol_count; // Rhen simplest accidentals
    });

    // Build result
    Note ans;
    ans.semitones = semitones;
    ans.micro_offset = best._micro_offset;
    ans.letter = best._letter;
    ans.alteration = best._alteration;
    ans.octave = best._octave;
    return ans;
}

bool Note::operator==(const Note& _note) const {
    return (2 * semitones + micro_offset == 2 * _note.semitones + _note.micro_offset);
}

bool Note::operator==(const std::string& note_name) const {
    return (*this == Note(note_name));
}

namespace Shun4MIDI {
std::ostream& operator<<(std::ostream& o, const Note& note) {
    o << note.name();
    return o;
}
}

// ======== HELPER FUNCTIONS ======== //
void Note::parseName(const std::string& note_name) {
    // Parse
    std::smatch regex; // {entire string, letter, quarter accidental, accidental, octave}
    if (!std::regex_match(note_name, regex, parseStrRegex)) {
        throw std::invalid_argument("Invalid note format: " + note_name);
    }

    // Letter and octave
    letter = std::toupper(regex[1].str()[0]);

    std::string octave_str = regex[4].str();
    

    if (octave_str.length() == 0) {
        octave = 4; // Default octave 
    } else {
        octave = std::stoi(octave_str);
        if (octave < 0) {
            throw std::invalid_argument("Invalid octave: " + std::to_string(octave));
        } else if (octave == 0 && letter > 'B') {
            throw std::invalid_argument(("Invalid octave for letter: " + std::string(1, letter)) + std::to_string(octave));
        }
    }

    // Quarter-tone suffix
    std::string micro_accidental = regex[2];
    if (micro_accidental == "d") {
        micro_offset = -1;
    } else if (micro_accidental == "u") {
        micro_offset = 1;
    } else if (micro_accidental.length() > 1) {
        throw std::invalid_argument("Invalid microtonal accidental: " + micro_accidental);
    } else {
        micro_offset = 0;
    }

    // Accidentals
    std::string accidental = regex[3];
    alteration = 0;
    for (int i = 0; i < accidental.length(); ++i) {
        if (accidental[i] == 'b') {
            --alteration;
        } else if (accidental[i] == '#') {
            ++alteration;
        } else if (accidental[i] == 'x') {
            alteration += 2;
        }
    }

    // Compute raw semitones
    if (letterToSemitone.find(letter) != letterToSemitone.end()) {
        semitones = letterToSemitone.at(letter) + 12 * octave + alteration;
    } else {
        // Handle invalid letter case
        throw std::invalid_argument("Invalid letter provided");
    }
}

std::pair<std::pair<char, int>, int> Note::findLetterOctave(int _semitones, int _alteration, bool prefer_sharps) {
    std::pair<char, int> letter_octave_pair;

    int base_semitones = _semitones - _alteration;

    // Check for normal iteration
    for (int i = 0; i < 7; ++i) {
        if ((base_semitones - semitoneToLetterPair[i].first) % 12 == 0) { // Correct note
            letter_octave_pair.first = semitoneToLetterPair[i].second;
            letter_octave_pair.second = (int)((base_semitones - semitoneToLetterPair[i].first) / 12) + 1; // Begin from octave 1 not 0

            std::pair<std::pair<char, int>, int> letter_octave_offset_pair;
            letter_octave_offset_pair.first = letter_octave_pair;
            letter_octave_offset_pair.second = _alteration;

            return letter_octave_offset_pair;
        }
    }

    // If not, then depending on prefer_sharps, redo it again for the correct note
    base_semitones += (prefer_sharps ? -1 : 1);
    for (int i = 0; i < 7; ++i) {
        if ((base_semitones - semitoneToLetterPair[i].first) % 12 == 0) { // Correct note
            letter_octave_pair.first = semitoneToLetterPair[i].second;
            letter_octave_pair.second = (int)((base_semitones - semitoneToLetterPair[i].first) / 12) + 1; // Begin from octave 1 not 0

            std::pair<std::pair<char, int>, int> letter_octave_offset_pair;
            letter_octave_offset_pair.first = letter_octave_pair;
            letter_octave_offset_pair.second = (prefer_sharps ? 1 : -1);

            return letter_octave_offset_pair;
        }
    }

    throw std::runtime_error("Cannot find letter octave");
}

void Note::setLetterOctave() {
    std::pair<char, int> letter_octave_pair = findLetterOctave(semitones, alteration).first;
    letter = letter_octave_pair.first;
    octave = letter_octave_pair.second;
}

bool Note::validOctave(char _letter, int _octave) {
    // Isolate invalid letters
    if (_letter < 'A' || _letter > 'G' || _octave > 8) {
        return false;
    }

    // Otherwise
    return (_octave > 0) || (_octave > 0 && _letter <= 'B');
}