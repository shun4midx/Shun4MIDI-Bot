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
#include "note/note.h"

using namespace Shun4MIDI;

int main() {
    std::srand(unsigned(time(NULL)));

    dpp::cluster bot(read("bot_token.txt"), dpp::i_default_intents | dpp::i_message_content);

    // Main Program
    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        const std::string cmd = event.command.get_command_name();

        // ======== SHUN/SHUN4MIDI BASIC INFO ======= //
        if (cmd == "shun_names") {
            event.reply("Shun\n翔海\nShun/翔海\nしゅう\nしゅん\nShun4mi");
        } else if (cmd == "shun_projects") {
            event.reply("Shun4MIDI Music Tool (shun4midi_en, shun4midi_jp)\nShun4miBot");
        } else if (cmd == "who_am_i") {
            event.reply("I am a bot made by Shun (@shun4midx) to be a helpful music theory tool for composers!");   
        } else if (cmd == "notation" || cmd == "help") {
            event.reply("I read inputs in the following manner!\n**Default Octave:** Every note you input would be defaulted to the fourth octave, so please specify the octave of your note if it's not 4!\n**Letter Notes:** C D E F G A B\n**Solfège:** Do Re Mi Fa Sol La Ti (with respect to any tonal center)\n**Accidentals:** b = Flat, # = Sharp, n = Natural, bb = Double Flat, x = Double Sharp, d = Half Flat, u = Half Sharp\n**Entering Microtones:** Please enter in the most simplified form possible, and put the quarter-tone microtone in front of any other accidentals. For example, input `Cu#` not `C#u`");
        }

        try {
            // ======== BASIC NOTE FUNCTIONALITIES ======= //
            if (cmd == "enharmonic") {
                std::string note_str = std::get<std::string>(event.get_parameter("note"));
                std::string preference_str = std::get<std::string>(event.get_parameter("preference"));

                int preference;

                if (preference_str == "Sharps") {
                    preference = 1;
                } else if (preference_str == "Flats") {
                    preference = -1;
                } else if (preference_str == "None") {
                    preference = 0;
                } else {
                    throw std::invalid_argument("Invalid preferred alteration: " + preference_str);
                }

                event.reply(Note(note_str).enharmonic('X', preference).name());

            } else if (cmd == "enharmonic_from_base_letter") {
                std::string note_str = std::get<std::string>(event.get_parameter("note"));
                std::string base_letter_str = std::get<std::string>(event.get_parameter("base_letter"));
                std::string preference_str = std::get<std::string>(event.get_parameter("preference"));

                int preference;

                if (preference_str == "Sharps") {
                    preference = 1;
                } else if (preference_str == "Flats") {
                    preference = -1;
                } else if (preference_str == "None") {
                    preference = 0;
                } else {
                    throw std::invalid_argument("Invalid preferred alteration: " + preference_str);
                }

                if (base_letter_str.length() != 1) {
                    event.reply("Invalid base letter: " + base_letter_str);
                } else {
                    char base_letter_char = base_letter_str[0];
                    event.reply(Note(note_str).enharmonic(base_letter_char, preference).name());
                }

            } else if (cmd == "are_enharmonic") {
                std::string note1_str = std::get<std::string>(event.get_parameter("note1"));
                std::string note2_str = std::get<std::string>(event.get_parameter("note2"));

                if (Note(note1_str) == Note(note2_str)) {
                    event.reply("Yes!");
                } else {
                    event.reply("No");
                }
            }
        } catch (const std::exception& e) {
            event.reply(e.what());
        }
    });

    // ========= LISTENING ======== //
    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        // ======== MY SPECIAL FUNCTIONS ======== //
        // Carry on only if it's not from the bot itself
        if (event.msg.author.format_username() != read("bot_username.txt") && event.msg.author.format_username() != read("conflicting_bot_username.txt")) {
            // std::cout << "Detected message" << std::endl;
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
        bot.set_presence(dpp::presence(dpp::ps_idle, dpp::at_game, "Tsunamis on MIDIs! :D"));

        if (dpp::run_once<struct register_bot_commands>()) {
            // ======= SHUN/SHUN4MIDI BASIC INFO ======== //
            bot.global_command_create(dpp::slashcommand("shun_names", "Outputs all forms of Shun's names", bot.me.id));
            bot.global_command_create(dpp::slashcommand("shun_projects", "Outputs all of Shun's current projects he's working on", bot.me.id));
            bot.global_command_create(dpp::slashcommand("who_am_i", "Who am I?", bot.me.id));
            bot.global_command_create(dpp::slashcommand("notation", "What are my conventions for musical notation?", bot.me.id));
            bot.global_command_create(dpp::slashcommand("help", "How do you use this bot?", bot.me.id));

            // ======== BASIC NOTE FUNCTIONALITIES ======= //
            bot.global_command_create(dpp::slashcommand("enharmonic", "Outputs the most simplest enharmonic note to your inputted note", bot.me.id).add_option(dpp::command_option(dpp::co_string, "note", "Your note you want to find the enharmonic note to", true)).add_option(dpp::command_option(dpp::co_string, "preference", "Accidental preference", true).set_auto_complete(true)));
            bot.global_command_create(dpp::slashcommand("enharmonic_from_base_letter", "Outputs the most simplest enharmonic note using your base letter to your note", bot.me.id).add_option(dpp::command_option(dpp::co_string, "note", "Your note you want to find the enharmonic note to", true)).add_option(dpp::command_option(dpp::co_string, "base_letter", "The base letter", true)).add_option(dpp::command_option(dpp::co_string, "preference", "Accidental preference", true).set_auto_complete(true)));
            bot.global_command_create(dpp::slashcommand("are_enharmonic", "Determines whether two notes are enharmonic", bot.me.id).add_option(dpp::command_option(dpp::co_string, "note1", "Your first note", true)).add_option(dpp::command_option(dpp::co_string, "note2", "Your second note", true)));
        }
    });

    // ======== AUTOCOMPLETE ======= //
    bot.on_autocomplete([&bot](const dpp::autocomplete_t & event) {
        for (auto & opt : event.options) {
            if (!opt.focused) {
                continue;
            }

            const std::string cmd = event.command.get_command_name();
            const std::string prefix = std::get<std::string>(opt.value);

            // Prepare the response builder
            dpp::interaction_response resp(dpp::ir_autocomplete_reply);

            // if (cmd == "enharmonic" || cmd == "enharmonic_from_base_letter" || cmd == "are_enharmonic") {
                static std::vector<std::string> auto_items = {"Sharps", "Flats", "None"};
                for (std::string item : auto_items) {
                    if (item.rfind(prefix, 0) == 0) { // Matches prefix
                        resp.add_autocomplete_choice(dpp::command_option_choice(item, item));
                    }
                }
            // }

            // Send the reply (must use event.command.id & token)
            bot.interaction_response_create(event.command.id, event.command.token, resp);
            break;
        }
    });

    bot.start(dpp::st_wait);

    return 0;
}