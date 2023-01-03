#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
#include "MappedFile.hpp"
#include "CommonMutex.hpp"
#include "Player_Game.hpp"
#include <fstream>




int main() {
    // creator, connector - players
    Player creator;
    Player connector;
    Game game;
    MappedFile mapped_file;
    string client_message = "";
    mapped_file.fd = shm_open(_BUFFER_NAME, O_RDWR | O_CREAT, _SHM_OPEN_MODE);
    if (mapped_file.fd == -1) {
        perror("sem_open error");
        return -1;
    }
    if (ftruncate(mapped_file.fd, _MAPPED_SIZE) == -1) {
        perror("ftruncate error");
        return -1;
    }
    mapped_file.data = (char *)mmap(NULL, _MAPPED_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mapped_file.fd, 0);
    if (mapped_file.data == MAP_FAILED) {
        perror("mmap error");
        return -1;
    }
    
    memset(mapped_file.data, '\0', _MAPPED_SIZE);
    CommonMutex mutex = shared_mutex_init(_MUTEX_NAME);
    if (mutex.created == 0) {
        cout << "Mutex has been created!" << endl;
    } 
    else {
        errno = 0;
    }
    cout << "Server is working now! Please start a game and it will be displayed here!" << endl;
    while (true) {
        if (mapped_file.data[0] == EOF) {
            break;
        }
        if (mapped_file.data[0] == '\0') {
            continue;
        }
        if (!(mapped_file.data[0] == 'O' && mapped_file.data[1] == 'N' &&
              mapped_file.data[2] == _MSG_SEP)) {
            continue;
        }
        cout << "Locking mutex" << endl;
        if (pthread_mutex_lock(mutex.ptr) != 0) {
            perror("Error locking mutex\n");
            return -1;
        }
        client_message = mapped_file.data;
        cout << "Has received next message from client: " << client_message << '\n';
        memset(mapped_file.data, '\0', _MAPPED_SIZE);
        vector<string> client_commands;
        string strings = ""; 
        //write client_command
        for (int i = 0; i < client_message.size(); ++i) {
            if (client_message[i] == _MSG_SEP) {
                client_commands.push_back(strings);
                strings = "";
            } 
            else {
                strings.push_back(client_message[i]);
            }
        }
        
        if (client_commands[2] == "create") {
            if (game.created || game.name == client_commands[3]) {
                
                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "zeroplaces" + _MSG_SEP;
                sprintf(mapped_file.data, "%s", player_message.c_str());
                cout << "Sending to client next message: " << player_message << '\n';
            }
            else {
                game.created = true;
                creator.turn = true;
                connector.turn = false;
                creator.username = client_commands[1];
                Map(creator.field);
                // cout << "creator\n";
                // PrintField(creator.field);
                game.name = client_commands[3];
                game.password = client_commands[4];
                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "gamecreated" + _MSG_SEP;
                sprintf(mapped_file.data, "%s", player_message.c_str());
                cout << "Sending to client next message: " << player_message << '\n';
            }
        }
        else if (client_commands[2] == "connect") {
            if (game.connected) {
                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "zeroplaces" + _MSG_SEP;
                sprintf(mapped_file.data, "%s", player_message.c_str());
                cout << "Sending to client next message: " << player_message << '\n';
            }
            else {
                if (game.name == client_commands[3]) {
                    if (game.password == client_commands[4]) {
                        game.connected = true;
                        connector.turn = false;
                        creator.turn = true;
                        connector.username = client_commands[1];
                        Map(connector.field);
                        // cout << "connector\n";
                        // PrintField(connector.field);
                        string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "connected" + _MSG_SEP;
                        sprintf(mapped_file.data, "%s", player_message.c_str());
                        cout << "Sending to client next message: " << player_message << '\n';
                    }  
                    else {
                        game.connected = false;
                        string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "wrongpassword" + _MSG_SEP;
                        sprintf(mapped_file.data, "%s", player_message.c_str());
                        cout << "Sending to client next message: " << player_message << '\n';
                    }
                }
                else {
                    game.connected = false;
                    string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "gamenotexists" + _MSG_SEP;
                    sprintf(mapped_file.data, "%s", player_message.c_str());
                    cout << "Sending to client next message:" << player_message << '\n';
                }
            }
        }
        else if (client_commands[2] == "invite"){
            if (game.connected) {
                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "zeroplaces" + _MSG_SEP;
                sprintf(mapped_file.data, "%s", player_message.c_str());
                cout << "Sending to client next message: " << player_message << '\n';
            }
            else{
                game.name = client_commands[3];
                game.password = client_commands[4];
                connector.invite = true;
                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "invited" + _MSG_SEP;
                sprintf(mapped_file.data, "%s", player_message.c_str());
                cout << "Sending to client next message: " << player_message << '\n';
            }
        }
        else if(client_commands[2] == "check"){
            if(connector.invite){
                game.connected = true;
                connector.turn = false;
                creator.turn = true;
                connector.username = client_commands[1];
                Map(connector.field);
                // cout << "connector\n";
                // PrintField(connector.field);
                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "checked" + _MSG_SEP + game.name + _MSG_SEP + game.password + _MSG_SEP;
                sprintf(mapped_file.data, "%s", player_message.c_str());
                cout << "Sending to client next message: " << player_message << '\n';
            }
        }
        else if (client_commands[2] == "shoot") {
            if (!game.connected) {
                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "notatgame" + _MSG_SEP;
                sprintf(mapped_file.data, "%s", player_message.c_str());
                cout << "Sending to client next message: " << player_message << '\n';
            }
            // shoot connector
            if (client_commands[1] == connector.username) {
                if (connector.turn && !creator.turn) {  // check try
                    if (game.name == client_commands[3]) {
                        int number = stoi(client_commands[5]);
                        string l = client_commands[4];
                        char letter = l[0];
                        // check position maybe
                        if (creator.field[number][int(letter) - int('A') + 1] == 'X' && 
                        (creator.field[number][int(letter) - int('A') + 2] == '.' || creator.field[number][int(letter) - int('A') + 2] == 'm' || creator.field[number][int(letter) - int('A') + 2] == 'w') && 
                        (creator.field[number - 1][int(letter) - int('A') + 1] == '.' || creator.field[number - 1][int(letter) - int('A') + 1] == 'm' || creator.field[number - 1][int(letter) - int('A') + 1] == 'w') && 
                        (creator.field[number - 1][int(letter) - int('A') + 2] == '.' || creator.field[number - 1][int(letter) - int('A') + 2] == 'm' || creator.field[number - 1][int(letter) - int('A') + 2] == 'w') &&
                        (creator.field[number + 1][int(letter) - int('A') + 1] == '.' || creator.field[number + 1][int(letter) - int('A') + 1] == 'm' || creator.field[number + 1][int(letter) - int('A') + 1] == 'w') &&
                        (creator.field[number + 1][int(letter) - int('A') + 2] == '.' || creator.field[number + 1][int(letter) - int('A') + 2] == 'm' || creator.field[number + 1][int(letter) - int('A') + 2] == 'w')) {
                            creator.field[number][int(letter) - int('A') + 1] = 'w';
                            connector.wounds++;
                            connector.kills++;
                            connector.turn = true;
                            creator.turn = false;
                            if (WonGame(creator.field)) {
                                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youwon" + _MSG_SEP;
                                sprintf(mapped_file.data, "%s", player_message.c_str());
                                cout << "Sending to connector next message:" << player_message << '\n';
                                connector.wins++;
                                creator.loses++;
                                creator.ErasePlayer();
                                connector.ErasePlayer();
                                PrepareField(creator.field);
                                PrepareField(connector.field);
                                game.EraseGame();
                                
                            }
                            else {
                                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youkilled" + _MSG_SEP;
                                sprintf(mapped_file.data, "%s", player_message.c_str());
                                cout << "Sending to client next message:" << player_message << '\n';
                            }
                        }
                        else if (creator.field[number][int(letter) - int('A') + 1] == 'w' || creator.field[number][int(letter) - int('A') + 1] == 'm') {
                            connector.turn = true;
                            creator.turn = false;
                            string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "yourepeated" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message:" << player_message << '\n';  
                        }
                        else if (creator.field[number][int(letter) - int('A') + 1] == 'X' && 
                        creator.field[number][int(letter) - int('A') + 2] == 'X' && 
                        (creator.field[number - 1][int(letter) - int('A') + 1] == '.' || creator.field[number - 1][int(letter) - int('A') + 1] == 'm' || creator.field[number - 1][int(letter) - int('A') + 1] == 'w') && 
                        (creator.field[number - 1][int(letter) - int('A') + 2] == '.' || creator.field[number - 1][int(letter) - int('A') + 2] == 'm' || creator.field[number - 1][int(letter) - int('A') + 2] == 'w') &&
                        (creator.field[number + 1][int(letter) - int('A') + 1] == '.' || creator.field[number + 1][int(letter) - int('A') + 1] == 'm' || creator.field[number + 1][int(letter) - int('A') + 1] == 'w') &&
                        (creator.field[number + 1][int(letter) - int('A') + 2] == '.' || creator.field[number + 1][int(letter) - int('A') + 2] == 'm' || creator.field[number + 1][int(letter) - int('A') + 2] == 'w')) {
                            creator.field[number][int(letter) - int('A') + 1] = 'w';
                            connector.wounds++;
                            connector.turn = true;
                            creator.turn = false;
                            string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youwounded" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        else if (creator.field[number][int(letter) - int('A') + 1] == 'X' && (creator.field[number][int(letter) - int('A') + 2] == '.' || creator.field[number][int(letter) - int('A') + 2] == 'm' || creator.field[number][int(letter) - int('A') + 2] == 'w') && 
                        creator.field[number - 1][int(letter) - int('A') + 1] == 'X' && 
                        (creator.field[number - 1][int(letter) - int('A') + 2] == '.' || creator.field[number - 1][int(letter) - int('A') + 2] == 'm' || creator.field[number - 1][int(letter) - int('A') + 2] == 'w') &&
                        (creator.field[number + 1][int(letter) - int('A') + 1] == '.' || creator.field[number + 1][int(letter) - int('A') + 1] == 'm' || creator.field[number + 1][int(letter) - int('A') + 1] == 'w') &&
                        (creator.field[number + 1][int(letter) - int('A') + 2] == '.' || creator.field[number + 1][int(letter) - int('A') + 2] == 'm' || creator.field[number + 1][int(letter) - int('A') + 2] == 'w')) {
                            creator.field[number][int(letter) - int('A') + 1] = 'w';
                            connector.wounds++;
                            connector.turn = true;
                            creator.turn = false;
                            string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youwounded" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        else if (creator.field[number][int(letter) - int('A') + 1] == 'X' && 
                        (creator.field[number][int(letter) - int('A') + 2] == '.' || creator.field[number][int(letter) - int('A') + 2] == 'm' || creator.field[number][int(letter) - int('A') + 2] == 'w') && 
                        (creator.field[number - 1][int(letter) - int('A') + 1] == '.' || creator.field[number - 1][int(letter) - int('A') + 1] == 'm' || creator.field[number - 1][int(letter) - int('A') + 1] == 'w') && 
                        (creator.field[number - 1][int(letter) - int('A') + 2] == '.' || creator.field[number - 1][int(letter) - int('A') + 2] == 'm' || creator.field[number - 1][int(letter) - int('A') + 2] == 'w') &&
                        creator.field[number + 1][int(letter) - int('A') + 1] == 'X' &&
                        (creator.field[number + 1][int(letter) - int('A') + 2] == '.' || creator.field[number + 1][int(letter) - int('A') + 2] == 'm' || creator.field[number + 1][int(letter) - int('A') + 2] == 'w')) {
                            creator.field[number][int(letter) - int('A') + 1] = 'w';    
                            connector.wounds++;
                            connector.turn = true;
                            creator.turn = false;
                            string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youwounded" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        else if (creator.field[number][int(letter) - int('A') + 1] == 'X' && creator.field[number + 1][int(letter) - int('A') + 1] == 'X') {
                            creator.field[number][int(letter) - int('A') + 1] = 'w';    
                            connector.wounds++;
                            connector.turn = true;
                            creator.turn = false;
                            string player_message = to + _MSG_SEP + client_commands[1] +_MSG_SEP + "youwounded" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        else if (creator.field[number][int(letter) - int('A') + 1] == '.') {
                            connector.misses++;
                            connector.turn = false;
                            creator.turn = true;
                            creator.field[number][int(letter) - int('A') + 1] = 'm';
                            string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youmissed" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        cout << "Current state of " << creator.username << "'s field is: " << '\n';
                        PrintField(creator.field);
                    }
                    else {
                        string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "gamenotexists" + _MSG_SEP;
                        sprintf(mapped_file.data, "%s", player_message.c_str());
                        cout << "Sending to client next message: " << player_message << '\n';
                    }
                }
                else {
                    string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "notyourturn" + _MSG_SEP;
                    sprintf(mapped_file.data, "%s", player_message.c_str());
                    cout << "Sending to client next message: " << player_message << '\n';
                }
            }
            // shoot creator
            else if (client_commands[1] == creator.username) {
                if (creator.turn && !connector.turn) {
                    if (game.name == client_commands[3]) {
                        int number = std::stoi(client_commands[5]);
                        string l = client_commands[4];
                        char letter = l[0];
                        // wounded
                        if (connector.field[number][int(letter) - int('A') + 1] == 'X' && 
                        (connector.field[number][int(letter) - int('A') + 2] == '.' || connector.field[number][int(letter) - int('A') + 2] == 'm' || connector.field[number][int(letter) - int('A') + 2] == 'w') && 
                        (connector.field[number - 1][int(letter) - int('A') + 1] == '.' || connector.field[number - 1][int(letter) - int('A') + 1] == 'm' || connector.field[number - 1][int(letter) - int('A') + 1] == 'w') && 
                        (connector.field[number - 1][int(letter) - int('A') + 2] == '.' || connector.field[number - 1][int(letter) - int('A') + 2] == 'm' || connector.field[number - 1][int(letter) - int('A') + 2] == 'w') &&
                        (connector.field[number + 1][int(letter) - int('A') + 1] == '.' || connector.field[number + 1][int(letter) - int('A') + 1] == 'm' || connector.field[number + 1][int(letter) - int('A') + 1] == 'w') &&
                        (connector.field[number + 1][int(letter) - int('A') + 2] == '.' || connector.field[number + 1][int(letter) - int('A') + 2] == 'm' || connector.field[number + 1][int(letter) - int('A') + 2] == 'w')) {
                            connector.field[number][int(letter) - int('A') + 1] = 'w';
                            creator.kills++;
                            creator.wounds++;
                            creator.turn = true;
                            connector.turn = false;
                            if (WonGame(connector.field)) {
                                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youwon" + _MSG_SEP;
                                sprintf(mapped_file.data, "%s", player_message.c_str());
                                cout << "Sending to creator next message: " << player_message << '\n';
                                creator.wins++;
                                connector.loses++;  
                                creator.ErasePlayer();
                                connector.ErasePlayer();
                                PrepareField(creator.field);
                                PrepareField(connector.field);
                                game.EraseGame();
                            }
                            else {
                                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youkilled" + _MSG_SEP;
                                sprintf(mapped_file.data, "%s", player_message.c_str());
                                cout << "Sending to client next message: " << player_message << '\n';
                            }
                        }
                        else if (connector.field[number][int(letter) - int('A') + 1] == 'w' || connector.field[number][int(letter) - int('A') + 1] == 'm') {
                            creator.turn = true;
                            connector.turn = false;
                            string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "yourepeated" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        else if (connector.field[number][int(letter) - int('A') + 1] == 'X' && 
                        connector.field[number][int(letter) - int('A') + 2] == 'X' && 
                        (connector.field[number - 1][int(letter) - int('A') + 1] == '.' || connector.field[number - 1][int(letter) - int('A') + 1] == 'm' || connector.field[number - 1][int(letter) - int('A') + 1] == 'w') && 
                        (connector.field[number - 1][int(letter) - int('A') + 2] == '.' || connector.field[number - 1][int(letter) - int('A') + 2] == 'm' || connector.field[number - 1][int(letter) - int('A') + 2] == 'w') &&
                        (connector.field[number + 1][int(letter) - int('A') + 1] == '.' || connector.field[number + 1][int(letter) - int('A') + 1] == 'm' || connector.field[number + 1][int(letter) - int('A') + 1] == 'w') &&
                        (connector.field[number + 1][int(letter) - int('A') + 2] == '.' || connector.field[number + 1][int(letter) - int('A') + 2] == 'm' || connector.field[number + 1][int(letter) - int('A') + 2] == 'w')) {
                            connector.field[number][int(letter) - int('A') + 1] = 'w';
                            creator.wounds++;
                            creator.turn = true;
                            connector.turn = false;
                            string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youwounded" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        else if (connector.field[number][int(letter) - int('A') + 1] == 'X' && (connector.field[number][int(letter) - int('A') + 2] == '.' || connector.field[number][int(letter) - int('A') + 2] == 'm' || connector.field[number][int(letter) - int('A') + 2] == 'w') && 
                        connector.field[number - 1][int(letter) - int('A') + 1] == 'X' && 
                        (connector.field[number - 1][int(letter) - int('A') + 2] == '.' || connector.field[number - 1][int(letter) - int('A') + 2] == 'm' || connector.field[number - 1][int(letter) - int('A') + 2] == 'w') &&
                        (connector.field[number + 1][int(letter) - int('A') + 1] == '.' || connector.field[number + 1][int(letter) - int('A') + 1] == 'm' || connector.field[number + 1][int(letter) - int('A') + 1] == 'w') &&
                        (connector.field[number + 1][int(letter) - int('A') + 2] == '.' || connector.field[number + 1][int(letter) - int('A') + 2] == 'm' || connector.field[number + 1][int(letter) - int('A') + 2] == 'w')) {
                            connector.field[number][int(letter) - int('A') + 1] = 'w';
                            creator.wounds++;
                            creator.turn = true;
                            connector.turn = false;
                            string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youwounded" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        else if (connector.field[number][int(letter) - int('A') + 1] == 'X' && 
                        (connector.field[number][int(letter) - int('A') + 2] == '.' || connector.field[number][int(letter) - int('A') + 2] == 'm' || connector.field[number][int(letter) - int('A') + 2] == 'w') && 
                        (connector.field[number - 1][int(letter) - int('A') + 1] == '.' || connector.field[number - 1][int(letter) - int('A') + 1] == 'm' || connector.field[number - 1][int(letter) - int('A') + 1] == 'w') && 
                        (connector.field[number - 1][int(letter) - int('A') + 2] == '.' || connector.field[number - 1][int(letter) - int('A') + 2] == 'm' || connector.field[number - 1][int(letter) - int('A') + 2] == 'w') &&
                        connector.field[number + 1][int(letter) - int('A') + 1] == 'X' &&
                        (connector.field[number + 1][int(letter) - int('A') + 2] == '.' || connector.field[number + 1][int(letter) - int('A') + 2] == 'm' || connector.field[number + 1][int(letter) - int('A') + 2] == 'w')) {
                            connector.field[number][int(letter) - int('A') + 1] = 'w';
                            creator.wounds++;
                            creator.turn = true;
                            connector.turn = false;
                            string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youwounded" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        else if (connector.field[number][int(letter) - int('A') + 1] == 'X' && connector.field[number + 1][int(letter) - int('A') + 1] == 'X') {
                            connector.field[number][int(letter) - int('A') + 1] = 'w';    
                            connector.wounds++;
                            connector.turn = true;
                            creator.turn = false;
                            string player_message = to + _MSG_SEP + client_commands[1] +_MSG_SEP + "youwounded" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        else if (connector.field[number][int(letter) - int('A') + 1] == '.') {
                            creator.misses++;
                            creator.turn = false;
                            connector.turn = true;
                            connector.field[number][int(letter) - int('A') + 1] = 'm';
                            string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "youmissed" + _MSG_SEP;
                            sprintf(mapped_file.data, "%s", player_message.c_str());
                            cout << "Sending to client next message: " << player_message << '\n';
                        }
                        cout << "Current state of " << connector.username << "'s field is: " << '\n';
                        PrintField(connector.field);
                    }
                    else {
                        string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "gamenotexists" + _MSG_SEP;
                        sprintf(mapped_file.data, "%s", player_message.c_str());
                        cout << "Sending to client next message: " << player_message << '\n';
                    }
                }
                else {
                    creator.turn = false;
                    string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "notyourturn" + _MSG_SEP;
                    sprintf(mapped_file.data, "%s", player_message.c_str());
                    cout << "Sending to client next message: " << player_message << '\n';
                }
            }
        }
        else if(client_commands[2]=="print_self"){
           vector< vector<char>> field;
            if (client_commands[1] == connector.username) {
                field = connector.field;
                
            }
            else{
                field = creator.field;
            }
            string player_message = to + _MSG_SEP + "print_self" +_MSG_SEP + "  ";
                
            for (int i = 0; i<10; ++i){
                player_message = player_message + char(int('A')+i) + " ";
            }
            player_message = player_message + _MSG_SEP;
            for (int i = 1; i < 11; ++i) {
                player_message = player_message + to_string(i)  + " ";
                for (int j = 1; j < 11; ++j) {
                    player_message = player_message + field[i][j] + " ";
                }
                player_message = player_message + _MSG_SEP;
            }
            sprintf(mapped_file.data, "%s", player_message.c_str());
            cout << "Sending to client next message: " << "your field" << '\n';
        }
        else if(client_commands[2]=="print_oppon"){
           vector< vector<char>> field;
            if (client_commands[1] != connector.username) {
                field = connector.field;
                
            }
            else{
                field = creator.field;
            }
            string player_message = to + _MSG_SEP + "print_oppon" +_MSG_SEP + "  ";
                
                for (int i = 0; i<10; ++i){
                    player_message = player_message + char(int('A')+i) + " ";
                }
                player_message = player_message + _MSG_SEP;
                for (int i = 1; i < 11; ++i) {
                    player_message = player_message + to_string(i)  + " ";
                    for (int j = 1; j < 11; ++j) {
                        if(field[i][j] != 'X')
                            player_message = player_message + field[i][j] + " ";
                        else
                            player_message = player_message + "." + " ";
                    }
                    player_message = player_message + _MSG_SEP;
                }
                sprintf(mapped_file.data, "%s", player_message.c_str());
                cout << "Sending to client next message: " << "opponent's field" << '\n';
        }



        else if (client_commands[2] == "disconnect") {
            if (client_commands[1] == creator.username) {
                creator.turn = false;
                connector.turn = true;
                game.connected = false;
                string player_message = to + _MSG_SEP + client_commands[1] + _MSG_SEP + "disconnected" + _MSG_SEP;
                sprintf(mapped_file.data, "%s", player_message.c_str());
                std::cout << "Sending to client next message: " << player_message << std::endl;
            }
            else {
                creator.turn = true;
                connector.turn = false;
                game.connected = false;
                string player_message = to + _MSG_SEP + connector.username + _MSG_SEP + "disconnected" + _MSG_SEP;
                sprintf(mapped_file.data, "%s", player_message.c_str());
                std::cout << "Sending to client next message: " << player_message << '\n';
            }
        }
        
        pthread_mutex_unlock(mutex.ptr);
        cout << "Unlocked mutex" << '\n';
    }
    if (shared_mutex_destroy(mutex) == -1) {
        perror("An error while destroying mutex has been detected!\n");
        return -1;
    }
    if (shm_unlink(_BUFFER_NAME) == -1) {
        perror("An error while unlink mutex has been detected!\n");
        return -1;
    }
    return 0;
}