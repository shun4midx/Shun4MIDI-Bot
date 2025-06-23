# Shun4MIDI Library Documentation
Although this is not officially a library, this is the repository where I mainly develop working on my Shun4MIDI library.

The point of the Shun4MIDI project is for musicians to have a decent tool to use, such as the Shun4MIDI Discord bot, and for programmers like me to have an easy-to-use library for musical programming, almost as if music is simply a built-in data-type. This is mainly done by overloading operators and some (not so good) OOP design, as this is my first ever big project that uses OOP.

## My Coding Conventions
It may be easier to reference my code if you know my coding conventions. Here is how I type the following structures in most of my code:
 - Classes: `MyStyle`
 - Functions: `myStyle`
 - Variables: `my_style`
 - Variables as parameters with the same name as class variables: `_my_style`
 - File Names: `my_style`

I also use `idx` in place of "index" in most of my code, as you can tell from my obsession with "idx" with my username being "shun4m***idx***".

## `Note` class
For example, for the `Note` class, I would support code such as:
```
Note a = "C#4";
std::cout << a << "is equal to Db4? " << (a == "Db4 ? "Yes" : "No");
```
With the intended output being `C#4 is equal to Db4? Yes`

There is also the support to print out more specific forms of `Note` objects, or even their enharmonics, using these two methods:
```
std::string name(bool explicit_natural = false, std::vector<Note> same_measure_notes = {}) const;
    
Note enharmonic(char base_letter = 'X', int prefer_alteration = 0) const;
```
Here, the `name()` function returns the note letter name in string. It checks if there is a need to add an 'n' sign depending on the `same_measure_notes`, if `explicit_natural` is `true`.

Here, the `enharmonic()` function returns an enharmonic `Note` object. THe variable `prefer_alteration` would mean the answer prefers flats if it's < 0, prefers sharps if it's > 0, and has no preference if it's 0. If `base_letter` is 'X', then there is no preference in base letter for the `Note` returned.

## `Interval` and `Chord` classes
As of right now, only `Note` has been developed, but in the future, I intend to support the following, for example:
```
Note a = "C4";
Note b = "G4";

Interval i = b - a; // Interchangable with a - b, i is now the "Perfect 5th"
Chord c = a + b; // c is now the chord of C4 and G4
```