#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <cassert>
#include <cstring>
#include <vector>
#include "MappedFile.hpp"
#include "CommonMutex.hpp"
#include "Player_Game.hpp"
#include <algorithm>
#include <sys/stat.h>
#include <fstream>

using namespace std;

MappedFile mapped_file;
CommonMutex mutex;
string nickname;
string username, password;
bool playing = false;
string current_game = "";

void SendMessage (const string &message) {
    if (pthread_mutex_lock(mutex.ptr) != 0) {
        cout << "An error while locking mutex has been detected!" << '\n';
        exit(EXIT_FAILURE);
    }
    memset(mapped_file.data, '\0', _MAPPED_SIZE);
    sprintf(mapped_file.data, "%s", message.c_str());
    pthread_mutex_unlock(mutex.ptr);
}

bool ReceiveAnswer() {
    if (mapped_file.data[0] != 'T' || mapped_file.data[1] != 'O' || mapped_file.data[2] != _MSG_SEP) {
        return false;
    }
    string message = mapped_file.data;
    vector<string> server_commands;
    string strings = "";
    // считывание из мапы

    for (int i = 0; i < message.size(); i++) {
        if (message[i] == _MSG_SEP) {
            server_commands.push_back(strings);
            strings = "";
        } 
        else {
            strings.push_back(message[i]);
        }
    }
    
    if(server_commands[1] == "print_self"){
        for (int i=2; i<server_commands.size();++i){
            cout << server_commands[i] << '\n';
        }
        return true;
    }
    else if(server_commands[1] == "print_oppon"){
        for (int i=2; i<server_commands.size();++i){
            cout << server_commands[i] << '\n';
        }
        return true;
    }
    
    else if (server_commands[1] == nickname) {
        if (pthread_mutex_lock(mutex.ptr) != 0) {
            perror("Error locking mutex\n");
            return -1;
        }
        memset(mapped_file.data, '\0', _MAPPED_SIZE);
        pthread_mutex_unlock(mutex.ptr);
        if (server_commands[2] == "gamecreated") {
            playing = true;
            cout << "Created successfully!" << '\n';
            cout << "You are a player №1, cause you have created the game. Your field has been prepared!" << '\n';
            return true;
        }
        if (server_commands[2] == "connected") {
            cout << "Connected sucessfully" << '\n';
            cout << "You are a player №2, cause you have connected to the game. Your field has been prepared!" << '\n';
            playing = true;
            return true;
        }
        if (server_commands[2] == "checked") {
            cout << "Connected sucessfully" << '\n';
            cout << "You are a player №2, cause you have connected to the game. Your field has been prepared!" << '\n';
            current_game = server_commands[3];
            password = server_commands[4];
            playing = true;
            return true;
        }
        
        if (server_commands[2] == "notatgame") {
            playing = true;
            cout << "You can't play without another player!" << '\n';;
            return true;
        }
        if (server_commands[2] == "gamenotexists") {
            cout << "Game with this name not exists" << '\n';
            playing = false;
            current_game = "";
            return true;
        }
        if (server_commands[2] == "wrongpassword") {
            cout << "Wrong password has been detected!" << '\n';
            playing = false;
            current_game = "";
            return true;
        }
        if (server_commands[2] == "notyourturn") {
            cout << "It's not your turn now!" << '\n';
            playing = true;
            return true;
        }
        if (server_commands[2] == "youwounded") {
            playing = true;
            cout << "You have wounded enemy's ship! Please enter coordinates again!" << '\n';
            return true;
        }
        if (server_commands[2] == "youmissed") {
            playing = true;
            cout << "Unfortunately you have missed! Now it's your enemy's turn!" << '\n';
            return true;
        }
        if (server_commands[2] == "youkilled") {
            playing = true;
            cout << "Congrats, you have KILLED enemy's ship! Please enter coordinates again!" << '\n';
            return true;
        }
        if (server_commands[2] == "zeroplaces") {
            playing = false;
            cout << "Sorry, but you can not create a game or connect to existing game. There are not free places!" << '\n';
            return true;
        }
        if (server_commands[2] == "yourepeated") {
            playing = true;
            cout << "You have already entered these coordinates! Please enter something new." << '\n';
            return true;
        }
        if (server_commands[2] == "disconnected") {
            cout << "You have successfully disconnected from the server!" << '\n';
            playing = false;
            return true;
        }
        if (server_commands[2] == "youwon") {
            cout << "YOU WON THE GAME!" << '\n';
            playing = false;
            return true;
        }
        else {
            cout  << "Warning: unknown message has been detected!" << '\n';
            playing = false;
            return true;
        }
        return true;
    }
    else if (server_commands[1] == username)
    {
        if(server_commands[2] == "invited"){
            cout << "Invited successfully!\n";
            return true;
        }
    }
    
    return false;
}

void Help() {
    cout << "Follow next rules: " << '\n';
    cout << '\t' << "create for creating a new game" << '\n';
    cout << '\t' << "connect for connecting to the server" << '\n';
    cout << '\t' << "shoot for shooting at enemy's ship" << '\n';
    cout << '\t' << "print for checking your field" << '\n';
    cout << '\t' << "invite for sending invite to opponent" << '\n';
    cout << '\t' << "check invite" << '\n';
    cout << '\t' << "disconnect for leaving from the server" << '\n';
    cout << '\t' << "quit for leaving from the program" << '\n';
    cout << '\t' << "help for checking rules" << '\n';
}

int main() {
    mapped_file.fd = shm_open(_BUFFER_NAME, O_RDWR, _SHM_OPEN_MODE);
    if (mapped_file.fd == -1 ) {
        perror("An error while shm_open has been detected!\n");
        return -1;
    }
    mutex = shared_mutex_init(_MUTEX_NAME);
    mapped_file.data = (char*)mmap(0, _MAPPED_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mapped_file.fd, 0);
    if (mapped_file.data == MAP_FAILED) {
        perror("An error while mmaping has been detected!\n");
    }
    cout << "Welcome to the SeaBattle! Please enter your nickname: " << '\n';
    cout << "> ";
    cin >> nickname;
    cout << "Hello, " << nickname << "!\n";
    Help();
    string command;
    string on = "ON";
    string gamename;
    while (cout << "> " && cin >> command) {
        if (!playing && command == "create") {
            cin >> gamename >> password;
            current_game = gamename;
            string server_message = on + _MSG_SEP + nickname + _MSG_SEP + "create" + _MSG_SEP + gamename + _MSG_SEP + password + _MSG_SEP;
            SendMessage (server_message);
            bool hasnotanswer = true;
            while (hasnotanswer) {
                hasnotanswer = !ReceiveAnswer();
            }
        }

        else if (playing && command == "create") {
            cin >> gamename >> password;
            cout << "Can't create a new game, you are playing now! Please enter another command!" << '\n';
            continue;
        }
        else if (!playing && command == "connect") {
            cin >> gamename >> password;
            current_game = gamename;
            string server_message = on + _MSG_SEP + nickname + _MSG_SEP + "connect" + _MSG_SEP + gamename + _MSG_SEP + password + _MSG_SEP;
            SendMessage (server_message);
            bool hasnotanswer = true;
            while (hasnotanswer) {
                hasnotanswer = !ReceiveAnswer();
            }
        } 
        else if (playing && command == "connect") {
            cin >> gamename >> password;
            cout << "Can't connect to a new game, you've already connected! Please enter another command!" << '\n';
            continue;
        }
        else if (playing && command == "print") {
            string server_message = on + _MSG_SEP + nickname + _MSG_SEP + "print_self" + _MSG_SEP;
            SendMessage (server_message);
            cout << "Your field!\n";
            bool hasnotanswer = true;
            while (hasnotanswer) {
                hasnotanswer = !ReceiveAnswer();
            }
            server_message = on + _MSG_SEP + nickname + _MSG_SEP + "print_oppon" + _MSG_SEP;
            SendMessage (server_message);
            cout << "\nOpponent's field!\n";
            hasnotanswer = true;
            while (hasnotanswer) {
                hasnotanswer = !ReceiveAnswer();
            }
        }
        else if (playing && command == "shoot") {
            int number;
			char letter;
            cin >> letter >> number;
            if ((!((letter >= 'A') && (letter <= 'J'))) || ((number < 1) || (number > 10))) {
                cout << "Please enter letter between A and J and number between 1 and 10!" << '\n';
                continue;
            }
            else {
                
                string server_message = on + _MSG_SEP + nickname + _MSG_SEP + "shoot" + _MSG_SEP + current_game + _MSG_SEP + letter + _MSG_SEP + to_string(number) + _MSG_SEP;
                SendMessage (server_message);
                bool hasnotanswer = true;
                while (hasnotanswer) {
                    hasnotanswer = !ReceiveAnswer();
                }
            }
        }
        else if (playing && command == "invite") {
            
            cin >> username;
            string server_message = on + _MSG_SEP + username + _MSG_SEP + "invite" + _MSG_SEP + gamename + _MSG_SEP + password + _MSG_SEP;
            SendMessage (server_message);
            bool hasnotanswer = true;
            while (hasnotanswer) {
                hasnotanswer = !ReceiveAnswer();
            }
        }
        else if (!playing && command == "check") {
            string server_message = on + _MSG_SEP + nickname + _MSG_SEP + "check" + _MSG_SEP;
            SendMessage (server_message);
            bool hasnotanswer = true;
            while (hasnotanswer) {
                hasnotanswer = !ReceiveAnswer();
            }
        }
        else if (!playing && command == "shoot") {
            int number;
            char letter;
            cin >> letter >> number;
            cout << "You are not in the game right now. Please create a game or connect to the existing one!" << '\n';
            continue;
        }
        else if (playing && command == "disconnect") {
            string server_message = on + _MSG_SEP + nickname + _MSG_SEP + "disconnect" + _MSG_SEP + current_game + _MSG_SEP;
            SendMessage (server_message);
            bool hasnotanswer = true;
            while (hasnotanswer) {
                hasnotanswer = !ReceiveAnswer();
            }
        }
        else if (command == "help") {
            Help();
        } 
        else if (!playing && command == "quit") {
            break;
        }
        else {
            cout << "Wrong input!" << '\n';
        }
    }
    return 0;
}
