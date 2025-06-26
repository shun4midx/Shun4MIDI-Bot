/********************************************
 * Copyright (c) 2025 Shun/翔海 (@shun4midx) *
 * Project: Shun4MIDI-Bot                   *
 * File Type: C++ file                      *
 * File: test_note.cpp                      *
 ****************************************** */
#include <iostream>
#include "note.h"

using namespace Shun4MIDI;

int main() {
    Note n1 = "Bb";
    Note n2 = "A#";

    if (n1 == n2) {
        std::cout << n1 << " is equal to " << n2 << std::endl;
    } else {
        std::cout << "Shun is very wrong, he can't get Bb = A#" << std::endl;
    }

    Note n3 = "G";
    n3 = n3.enharmonic('F');
    std::cout << "n3 rewritten with base note F is: " << n3 << std::endl;

    n3 = "Gb";
    std::cout << "The new n3 note is: " << n3 << std::endl;
    n3 = n3.enharmonic();
    std::cout << "n3 rewritten with unspecified base note is: " << n3 << std::endl;

    n3 = n3.enharmonic('B'); // Should return with error message that it's unrealistic

    return 0;
}