/********************************************
 * Copyright (c) 2025 Shun/翔海 (@shun4midx) *
 * Project: Shun4MIDI-Bot                   *
 * File Type: Header file                   *
 * File: file_manager.h                     *
 ****************************************** */

 #pragma once
 #include <iostream>
 #include <vector>
 #include <fstream>
 #include <filesystem>
 
 std::string filePath(std::string file_name, bool in_src = false); // Returns file path given file_name and in_src
 std::string read(std::string file_name, bool in_src = false); // Reads a txt file and returns it, in_src is if it is in /src
 std::string readSingleLine(std::string file_name, bool in_src = false); // Reads the first line of a txt file and returns it, in_src is if it is in /src
 std::vector<std::string> readVector(std::string file_name, bool in_src = false); // Reads a txt file and returns it as a vector
 bool checkInstance(std::string file_name, std::string instance, bool in_src = false); // Checks for if there's a line containing instance
 
 void write(std::string file_name, std::string text, bool in_src = false); // Writes text in a txt file, in_src is if it is in /src
 
 void deleteFile(std::string file_name, bool in_src = false); // Deletes a txt file, in_src is if it is in /src
 
 bool checkFile(std::string file_name, bool in_src = false); // Checks if a txt file exists