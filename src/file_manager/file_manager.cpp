/********************************************
 * Copyright (c) 2025 Shun/翔海 (@shun4midx) *
 * Project: Shun4MIDI-Bot                   *
 * File Type: C++ file                      *
 * File: file_manager.cpp                   *
 ****************************************** */

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "file_manager.h"

std::string filePath(std::string file_name, bool in_src) {
    if (!in_src) {
        return (std::filesystem::path(__FILE__).parent_path().parent_path().parent_path().parent_path()/"shun4midi_text_files"/file_name).string();
    } else {
        return (std::filesystem::path(__FILE__).parent_path().parent_path()/file_name).string();
    }
}

std::string read(std::string file_name, bool in_src) {
    std::ifstream curr_file(filePath(file_name, in_src));
    std::string curr_text = "";
    std::string text_line = "";

    while (getline(curr_file, text_line)) {
        curr_text = curr_text + (curr_text == "" ? "" : "\n") + text_line;
    }
    
    curr_file.close();

    return curr_text;
}

std::string readSingleLine(std::string file_name, bool in_src) {
    std::ifstream curr_file(filePath(file_name, in_src));
    std::string curr_text = "";
    std::string text_line = "";

    getline(curr_file, text_line);
    curr_text = curr_text + (curr_text == "" ? "" : "\n") + text_line;
    
    curr_file.close();

    return curr_text;
}

std::vector<std::string> readVector(std::string file_name, bool in_src) {
    std::ifstream curr_file(filePath(file_name, in_src));
    std::vector<std::string> curr_arr;
    std::string text_line = "";

    while (getline(curr_file, text_line)) {
        curr_arr.push_back(text_line);
    }
    
    curr_file.close();

    return curr_arr;
}

bool checkInstance(std::string file_name, std::string instance, bool in_src) {
    std::vector<std::string> curr_arr = readVector(file_name, in_src);
    return std::find(curr_arr.begin(), curr_arr.end(), instance) != curr_arr.end();
}

void write(std::string file_name, std::string text, bool in_src) {
    std::ofstream curr_file(filePath(file_name, in_src));
    curr_file << text;
    curr_file.close();
}

void deleteFile(std::string file_name, bool in_src) {
    remove(filePath(file_name, in_src).c_str());
}

bool checkFile(std::string file_name, bool in_src) {
    return std::filesystem::exists(filePath(file_name, in_src).c_str());
}