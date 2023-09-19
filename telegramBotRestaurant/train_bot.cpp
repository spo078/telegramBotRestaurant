#include <stdio.h>
#include <iostream>
#include <tgbot/tgbot.h>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <boost/bind/bind.hpp>
using namespace TgBot;
using namespace std;
using namespace boost::placeholders;


vector<string> bot_commands = {"start", "location", "get", "info", "policy", "menu", "reserve", "order", "exit"};
void save(string message) {
    ofstream savefile("reserve.txt", ios::app); // Open the file in append mode

    if (!savefile) {
        cerr << "Error: Unable to open save.txt" << endl;
        return;
    }

    savefile << message << endl; // Write the message to the file
    savefile.close(); // Close the file
}
void save_order(string message) {
    ofstream savefile("order.txt", ios::app); // Open the file in append mode

    if (!savefile) {
        cerr << "Error: Unable to open save.txt" << endl;
        return;
    }

    savefile << message << endl; // Write the message to the file
    savefile.close(); // Close the file
}

bool isCommandInText(const std::string& command, const std::string& text) {
    // Check if "/"+command is found within text.
    return text.find("/" + command) != std::string::npos;
}

void createOneColumnKeyboard(const vector<string>& buttonStrings, ReplyKeyboardMarkup::Ptr& kb)
{
  for (size_t i = 0; i < buttonStrings.size(); ++i) {
    vector<KeyboardButton::Ptr> row;
    KeyboardButton::Ptr button(new KeyboardButton);
    button->text = buttonStrings[i];
    row.push_back(button);
    kb->keyboard.push_back(row);
  }
}

void createKeyboard(const vector<vector<string>>& buttonLayout, ReplyKeyboardMarkup::Ptr& kb)
{
  for (size_t i = 0; i < buttonLayout.size(); ++i) {
    vector<KeyboardButton::Ptr> row;
    for (size_t j = 0; j < buttonLayout[i].size(); ++j) {
      KeyboardButton::Ptr button(new KeyboardButton);
      button->text = buttonLayout[i][j];
      row.push_back(button);
    }
    kb->keyboard.push_back(row);
  }
}

int main() {
    Bot bot("6581380007:AAFtcHjK1LE13hOCRH44JL47p5za2XYKrIs");
    const string photoFilePath = "menu_dale.jpg";
    const string photoMimeType = "image/jpeg";
    const string photoFilePath_3 = "order_aigen.jpg";
    const string photoFilePath_4 = "reserve_aigen.jpg";
    

    //reply-keyboard stuff

    //ReplyKeyboardMarkup::Ptr keyboardMult(new ReplyKeyboardMarkup);
    //createKeyboard({"/menu", "/order", "/reserve", "/info", "/location", "/file"}, keyboardMult);

    //keyboard stuff
    ReplyKeyboardMarkup::Ptr keyboardOneCol(new ReplyKeyboardMarkup);
    createOneColumnKeyboard({"/order", "/info", "/exit"}, keyboardOneCol);
     ReplyKeyboardMarkup::Ptr keyboardMult(new ReplyKeyboardMarkup);
    createKeyboard({
      {"/menu", "/order",},
      {"/info", "/location", "/policy"},
      //{"On", "Off"},
      //{"Back"},
      //{"Info", "About", "Map", "Etc"}
    }, keyboardMult);
    ReplyKeyboardMarkup::Ptr keyboardOrder(new ReplyKeyboardMarkup);
    createKeyboard({
      {"Pasta Carbonara ", "Margarita Pizza", "Orange Juice"},
      {"Vegeterian Salad", "Ceasar Salad", "/return"},
      //{"On", "Off"},
      //{"Back"},
      //{"Info", "About", "Map", "Etc"}
    }, keyboardOrder);
    


    vector<BotCommand::Ptr> commandsbar;
    BotCommand::Ptr cmdbar(new BotCommand);
    cmdbar->command = "menu";
    cmdbar->description = "Our Menu";

    commandsbar.push_back(cmdbar);

    cmdbar = BotCommand::Ptr(new BotCommand);
    cmdbar->command = "order";
    cmdbar->description = "Order Something";
    commandsbar.push_back(cmdbar);

    cmdbar = BotCommand::Ptr(new BotCommand);
    cmdbar->command = "info";
    cmdbar->description = "Information";
    commandsbar.push_back(cmdbar);
    

    bot.getApi().setMyCommands(commandsbar);

    vector<BotCommand::Ptr> vectCmd;
    vectCmd = bot.getApi().getMyCommands();

    // Thanks Pietro Falessi for code
    /*InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0;
    InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
    checkButton->text = "check";
    checkButton->callbackData = "check";
    row0.push_back(checkButton);
    keyboard->inlineKeyboard.push_back(row0);*/
    string photoFilePath_2 = "welcome_dale.jpg";
    // reservatopn date
    std::map<int64_t, std::string> reservationDates;
    //std::map<int64_t, std::string>
    //user order
    std::map<std::int64_t, std::string> userOrders;
    bot.getEvents().onCommand("start", [&bot, &keyboardMult, &photoFilePath_2, &photoMimeType, &keyboardOneCol](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi, happy to see you in our restaurant I'm desgined to help you with your order " + message->chat->firstName);
        bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath_2, photoMimeType));
        bot.getApi().sendMessage(message->chat->id, "Select Option", false, 0, keyboardMult);
        
    });
    bot.getEvents().onCommand("info", [&bot, &keyboardMult](Message::Ptr message) {
            bot.getApi().sendMessage(message->chat->id, "All the information you can find about our restaurant by following the link <link>", false, 0, keyboardMult);
    });
    bot.getEvents().onCommand("menu", [&bot, &photoFilePath, &photoMimeType, &keyboardMult, &keyboardOneCol](Message::Ptr message) {
            bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath, photoMimeType));
            bot.getApi().sendMessage(message->chat->id, "Use commands bar on corner", false, 0, keyboardOneCol);

    }); 
    /*bot.getEvents().onNonCommandMessage([&bot, &photoFilePath, &photoMimeType, &keyboardMult, &keyboardOneCol](Message::Ptr message) {
    // Check if the user provided the /menu command.
    if (StringTools::startsWith(message->text, "/menu")) {
        // User only typed /menu, so send the menu and instructions.
        bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath, photoMimeType));
        bot.getApi().sendMessage(message->chat->id, "Use commands /order and /reserve or the command bar on the right corner.", false, 0, keyboardMult);
    } else  {
        // User provided an additional command or a regular message, handle it here.
        // You can add more command handling logic as needed.
        // For example, you can check if the user typed "/order" or "/reserve" and handle those commands.
        // Here, we'll send a message indicating that the provided command is not recognized.
        bot.getApi().sendMessage(message->chat->id, "Unrecognized command. Use /menu to see the menu, /order to order your meals and /reserve for reservation", false, 0, keyboardMult);
    }
});*/
bot.getEvents().onCommand("exit", [&bot, &keyboardMult, &photoMimeType, &photoFilePath, &keyboardOneCol](Message::Ptr message) {
    bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath, photoMimeType));
    bot.getApi().sendMessage(message->chat->id, "Select Option", false, 0, keyboardMult);
});
bot.getEvents().onCommand("return", [&bot, &keyboardMult, &photoMimeType, &photoFilePath, &keyboardOneCol](Message::Ptr message) {
    bot.getApi().sendMessage(message->chat->id, "Select Option", false, 0, keyboardOneCol);
});


     bot.getEvents().onCommand("reserve", [&bot, &keyboardMult, &reservationDates, &photoMimeType, &photoFilePath_4, &keyboardOneCol](Message::Ptr message) {
        bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath_4, photoMimeType));
        bot.getApi().sendMessage(message->chat->id, "Please enter date you want reservation - mm/dd and time");
        bot.getApi().sendMessage(message->chat->id, "Type your order or select another option", false, 0, keyboardMult);
        //for (const auto& command : bot_commands) {
            //if (isCommandInText(command, message->text) == true) {
             //   return;  // Exit the loop when a command is found.
            //}
            //else {
                reservationDates[message->chat->id] = "";
           // }
        //}
       // for (const auto& command : bot_commands) {
            //if (!isCommandInText(command, message->text)) {
            
            //}
            //else {
            // return;
            //}
       // }
        
    });
    bot.getEvents().onCommand("policy", [&bot, &keyboardMult](Message::Ptr message){
        bot.getApi().sendMessage(message->chat->id, "Here is our policy");
        bot.getApi().sendDocument(message->chat->id, "https://secure.efficientforms.com/uploads/ehx/policies/4eb9a3b9-5282-4fbb-8b7c-7b999e6f9f86.PDF");
        bot.getApi().sendMessage(message->chat->id, "Select option", false, 0, keyboardMult);

    });
    // Set up an event handler for any message
    bot.getEvents().onAnyMessage([&bot, &reservationDates, &keyboardMult, &keyboardOneCol](Message::Ptr message) {
        // Check if the user is in the state of providing a reservation date
        if (reservationDates.find(message->chat->id) != reservationDates.end()) {
            // Store the reservation date entered by the user
            reservationDates[message->chat->id] = message->text;
             save("Reservation date set to: " + message->text  + ". Username - " + message->chat->username);
            // Respond to the user and reset the state
            
            bot.getApi().sendMessage(message->chat->id, "Reservation date set to: " + message->text);
            bot.getApi().sendMessage(message->chat->id, "Select option", false, 0, keyboardMult);
           
            
            // Remove the user from the reservation state
            reservationDates.erase(message->chat->id);
        } //else {
            // Handle regular messages or other commands
            //bot.getApi().sendMessage(message->chat->id, "Received your message: " + message->text);
        //}
        
        
    });

    bot.getEvents().onCommand("order", [&bot, &photoFilePath_3, &photoMimeType, &userOrders, &keyboardOrder, &keyboardOneCol](Message::Ptr message) {
        // Check if the user has an existing order.
        //if (userOrders.find(message->chat->id) == userOrders.end()) {
        // User doesn't have an order yet.
            bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath_3, photoMimeType));
            //bot.getApi().sendMessage(message->chat->id, "The most important moment of your visit! \nType meal names you want to try today and number of your table number");
            bot.getApi().sendMessage(message->chat->id, "Type your order or select another option", false, 0, keyboardOrder);

                userOrders[message->chat->id] = "";
            
         
    });


    bot.getEvents().onAnyMessage([&bot, &userOrders, &keyboardMult, &keyboardOneCol, &keyboardOrder](Message::Ptr message) {
        // Check if the user is in the state of providing a reservation date
        if (userOrders.find(message->chat->id) != userOrders.end()) {
            // Store the reservation date entered by the user
            userOrders[message->chat->id] = message->text;
            // Respond to the user and reset the state
            // bool orderSent = false; // Initialize a flag to false

        while (message->text != "/return") {
            if (message->text == "Margarita Pizza" || message->text == "Pasta Carbonara" || message->text == "Vegeterian Salad" || message->text == "Orange Juice" || message->text == "Ceasar Salad") {
                bot.getApi().sendMessage(message->chat->id, "Your order: " + message->text);
                save_order("Order and table number: " + message->text  + ". Username - " + message->chat->username);
                // orderSent = true;  // Set the flag to true after sending the order
                bot.getApi().sendMessage(message->chat->id, "Select an option", false, 0, keyboardOrder);
                return;
            } 
            else if(message->text == "/order" || message->text == "/reserve") {
                return; //or break
            }
            else{
                bot.getApi().sendMessage(message->chat->id, "Wrong Message please use commands bar or type meal names we have acces to", false, 0, keyboardOrder);
                return;


            }
        }
            
            // Remove the user from the reservation state
            userOrders.erase(message->chat->id);
        }
        
    });
    bot.getEvents().onCommand("location", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Here is our restaurant Location" + message->chat->username);
        bot.getApi().sendLocation(message->chat->id, 40.016869, -105.279617);
    });
    
    
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        
    });

    bot.getEvents().onAnyMessage([&bot, &keyboardMult](Message::Ptr message) {
        bool isCommandFound = false;
        for (const auto& command : bot_commands) {
            if (isCommandInText(command, message->text)) {
                isCommandFound = true;
                break;  // Exit the loop when a command is found.
            }
        }

        if (!isCommandFound && message->text != "Margarita Pizza" && message->text != "Pasta Carbonara" && message->text != "Vegeterian Salad" && message->text != "Ceaser Salad" && message->text != "Orange Juice") {
            bot.getApi().sendMessage(message->chat->id, "Please Open Command bar or use command /menu, /order, /info, /location dear " + message->chat->username, false, 0, keyboardMult);
        }
    });
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}