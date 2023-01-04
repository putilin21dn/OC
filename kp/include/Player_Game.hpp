#ifndef PLAYERANDGAME_H
#define PLAYERANDGAME_H
#include <algorithm>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>

using namespace std;


class Player {
    public:
        string username;
        vector<vector<char>> field;
        bool turn;
        bool invite;
        Player() : field(12,  vector<char> (12, '.')), username(""), turn(false), invite(false) {}
        void ErasePlayer() {
            username = "";
            turn = false;
            invite = false;
        } 
};
class Game {
    public:
        string name;
        string password;
        bool connected;
        bool created;
        Game() : name(""), password(""), connected(false), created(false) {}
        void EraseGame() {
            name = "";
            password = "";
            connected = false;
            created = false;
        }
};
void Map ( vector< vector<char>> &field) {
    int j =- 1, k, v, l, x[2], y;
    srand(time(0));
    for (l = 4; l > 0; l--) {
        for (k = 5; k - l; k--) {
            v = 1&rand();
            do for (x[v] = 1 + rand() % 10, x[1 - v] = 1 + rand() % 7, y = j = 0; j - l; y |= field[x[0]][x[1]] != '.', x[1 - v]++, j++); while(y);
            x[1 - v] -= l + 1, field[x[0]][x[1]] = '/', x[v]--, field[x[0]][x[1]] = '/', x [v] += 2, field[x[0]][x[1]] = '/', x[v]--, x[1 - v]++;
            for (j = -1; ++j - l; field[x[0]][x[1]] = 'X', x[v]--, field[x[0]][x[1]] = '/', x[v] += 2, field[x[0]][x[1]] = '/', x[v]--, x[1 - v]++);
            field[x[0]][x[1]] = '/', x[v]--, field[x[0]][x[1]] = '/', x[v]+=2, field[x[0]][x[1]] = '/';
        }
    }
    for (int i = 0; i < 12; ++i) {
         replace(field[i].begin(), field[i].end(), '/', '.');
    }
}
void PrintField ( vector< vector<char>> &field) {
    cout << "  ";
    for (int i = 0; i<10; ++i){
        cout << char(int('A')+i) << " ";
    }
    cout << '\n';
    for (int i = 1; i < 11; ++i) {
        cout << i << " ";
        for (int j = 1; j < 11; ++j) {
            cout << field[i][j] << " ";
        }
        cout <<  '\n';
    }
}
bool WonGame ( vector< vector<char>> &field) {
    for (int i = 1; i < 11; ++i) {
        for (int j = 1; j < 11; ++j) {
            if (field[i][j] == 'X') {
                return false;
            }
        }
    }
    return true;
}
void PrepareField ( vector< vector<char>>& field) {
	for (int i = 0; i < 12; i++) {
		field[i].clear();
		field[i] =  vector<char>(12, '.');
	}
}
#endif