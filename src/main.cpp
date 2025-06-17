/********************************************
 * Copyright (c) 2025 Shun/翔海 (@shun4midx) *
 * Project: Shun4MIDI-Bot                   *
 * File Type: C++ file                      *
 * File: main.cpp                           *
 ****************************************** */

#include <iostream>
#include <format>
#include <cstdio>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>
#include <dpp/dpp.h>
#include <dpp/message.h>
#include <dpp/user.h>
#include <dpp/cluster.h>
#include "file_manager/file_manager.h"

int main() {
    std::srand(unsigned(time(NULL)));

    dpp::cluster bot(read("bot_token.txt"), dpp::i_default_intents | dpp::i_message_content);

    // Main Program
    bot.on_log(dpp::utility::cout_logger());
    
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        // ======== SHUN/SHUN4MIDI BASIC INFO ======= //
        if (event.command.get_command_name() == "shun_names") {
            event.reply("Shun\n翔海\nShun/翔海\nしゅう\nしゅん\nShun4mi");
        } else if (event.command.get_command_name() == "shun_projects") {
            event.reply("Shun4MIDI Music Tool (shun4midi_en, shun4midi_jp)\nShun4miBot");
        } else if (event.command.get_command_name() == "who_am_i") {
            event.reply("I am a bot made by Shun (@shun4midx) to be a helpful music theory tool for composers!");   
        } else if (event.command.get_command_name() == "notation") {
            event.reply("**Letter Notes:** C D E F G A B\n**Solfège:** Do Re Mi Fa Sol La Ti (with respect to any tonal center)\n**Accidentals:** b = Flat, # = Sharp, n = Natural, bb = Double Flat, x = Double Sharp, d = Half Flat, u = Half Sharp");
        }
    });

    // ========= LISTENING ======== //
    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        // ======== MY SPECIAL FUNCTIONS ======== //
        // Carry on only if it's not from the bot itself
        if (event.msg.author.format_username() != read("bot_username.txt") && event.msg.author.format_username() != read("conflicting_bot_username.txt")) {
            std::cout << "Detected message" << std::endl;
            std::string message = event.msg.content;

            // ======== Shun4MIDI-Bot Says (My function only) ======= //
            if (message.find("?bot_say") == 0 && checkInstance("bot_say_auths.txt", event.msg.author.username)) {
                std::string command = "?bot_say";
                std::string quote = message.substr(command.length() + 1, message.length() - command.length());
                bot.message_delete(event.msg.id, event.msg.channel_id);
                event.send(quote);
            }

            // ======== AUTORESPONDER ======== //
            std::string og_message = message;
            std::transform(message.begin(), message.end(), message.begin(), ::tolower);
            if (message.find("shun4midi") != std::string::npos || message.find("tsunami") != std::string::npos || message.find("shunami") != std::string::npos) {
                event.reply("Omg me mention! :D I love Tsunamis and MIDIs! :ocean::musical_keyboard:", true);
            }
        }
    });

    // ======== INIT PART OF THE CODE ======== //
    bot.on_ready([&bot](const dpp::ready_t& event) {
        // Bot status
        bot.set_presence(dpp::presence(dpp::ps_idle, dpp::at_game, "Daydreaming about Tsunami MIDIs!"));

        if (dpp::run_once<struct register_bot_commands>()) {
            // ======= SHUN/SHUN4MIDI BASIC INFO ======== //
            bot.global_command_create(dpp::slashcommand("shun_names", "Outputs all forms of Shun's names", bot.me.id));
            bot.global_command_create(dpp::slashcommand("shun_projects", "Outputs all of Shun's current projects he's working on", bot.me.id));
            bot.global_command_create(dpp::slashcommand("who_am_i", "Who am I?", bot.me.id));
            bot.global_command_create(dpp::slashcommand("notation", "What are my conventions for musical notation?", bot.me.id));
        }
    });

    bot.start(dpp::st_wait);

    return 0;
}
