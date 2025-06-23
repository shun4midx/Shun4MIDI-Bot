/********************************************
 * Copyright (c) 2025 Shun/翔海 (@shun4midx) *
 * Project: Shun4MIDI-Bot                   *
 * File Type: Header file                   *
 * File: note.h                             *
 ****************************************** */

#pragma once
#include <string>
#include <unordered_map>
#include <vector>

const std::unordered_map<char, int> letterToIdx = {
    {'C', 0}, 
    {'D', 1}, 
    {'E', 2}, 
    {'F', 3}, 
    {'G', 4}, 
    {'A', 5}, 
    {'B', 6}
};

const std::vector<char> noteOrder = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};

const std::unordered_map<char, int> letterToSemitone = {
    {'C', 0}, 
    {'D', 2}, 
    {'E', 4}, 
    {'F', 5}, 
    {'G', 7}, 
    {'A', 9}, 
    {'B', 11}
};

const std::unordered_map<int, char> semitoneToLetter = {
    {0, 'C'}, 
    {2, 'D'},
    {4, 'E'},
    {5, 'F'},
    {7, 'G'},
    {9, 'A'},
    {11, 'B'}
};

const std::pair<int, char> semitoneToLetterPair[7] = {
    {0, 'C'}, 
    {2, 'D'},
    {4, 'E'},
    {5, 'F'},
    {7, 'G'},
    {9, 'A'},
    {11, 'B'}
};

namespace Shun4MIDI {

class Note {
public:
    // ======== PRELIMINARIES ========= //
    // get() functions
    int Semitones() const;
    int Micro_Offset() const;
    char Letter() const;
    int Alteration() const;
    int Octave() const;

    // Constructors
    Note();
    Note(const char* note_name);
    Note(const std::string& note_name);
    Note(int _semitones, int _alteration, int _micro_offset = 0);
    Note& operator=(const Note& _note);
    Note& operator=(const char* note_name);
    Note& operator=(const std::string& note_name);

    // ======== METHODS ========= //
    static Note semitoneToNote(double _semitones, int _micro_offset = 0, bool prefer_sharps = true); // It just makes more sense to have a preference here in my opinion? Feel free to use .enharmonic() to make it no preference most 'simplified'
    std::string name(bool explicit_natural = false, std::vector<Note> same_measure_notes = {}) const; // Returns the note letter name in string
    static int noteToSemitone(char _letter, int _alteration = 0, int _octave = 4); // e.g. "A, 4, 0" would output the amount of semitones 'A4' is from C1
    Note enharmonic(char base_letter = 'X', int prefer_alteration = 0) const; // prefer_alteration < 0 (flats), 0 (no pref, just swap), > 0 (sharp); X for default

    // Use '==' for 'enharmonic'
    bool operator==(const Note& _note) const;
    bool operator==(const std::string& note_name) const;

    // Print to ostream
    friend std::ostream& operator<<(std::ostream& o, const Note& note);

private:
    // ======= VARIABLES ======= //
    int semitones; // Semitones from C1 of the "main note" (i.e. w/o micro-offset)
    int micro_offset; // Offset from the note determined from _semitones
    char letter; // 'A'-'G'
    int alteration; // b or # from the character "letter", can range from -2 to +2 but must be int
    int octave;

    // ======== HELPER FUNCTIONS ======== //
    void parseName(const std::string& note_name);
    static std::pair<std::pair<char, int>, int> findLetterOctave(int _semitones, int _alteration = 0, bool prefer_sharps = true); // Letter, octave, offset
    void setLetterOctave(); // Return letter and octave
    static bool validOctave(char _letter, int _octave);
};

} // namespace Shun4MIDI