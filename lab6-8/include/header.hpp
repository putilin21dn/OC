#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
#include <zmq.h>
#include <string.h>
#include <iostream>
using namespace std;

#define CREATE 1
#define EXEC 0
#define REMOVE -1

struct vec{
    int ex;
    int id;
    int lenstr;
    string str;
    int lensubstr;
    string substr;
};

zmq_msg_t vec2msg(vec V){
    
    zmq_msg_t msg;
    zmq_msg_init_size(&msg, 4 * sizeof(int) + V.lenstr+V.lensubstr);
    void * z = zmq_msg_data(&msg);
    memcpy(z, &V.ex, sizeof(int));
    memcpy(z = z + sizeof(int), &V.id, sizeof(int));
    cout << "TYT" << '\n';
    memcpy(z = z + sizeof(int), &V.lenstr, sizeof(int));
    memcpy(z = z + sizeof(int), V.str.c_str(), V.lenstr);
    memcpy(z = z + V.lenstr, &V.lensubstr, sizeof(int));
    memcpy(z = z + sizeof(int), V.substr.c_str(), V.lensubstr);
    return msg;

}


vec msg2vec(zmq_msg_t msg){
    void * z = zmq_msg_data(&msg);
    vec V;
    
    memcpy(&V.ex, z, sizeof(int));
    memcpy(&V.id, z=z+sizeof(int), sizeof(int));
    memcpy(&V.lenstr, z=z + sizeof(int), sizeof(int));
    char *  s = (char *)malloc(sizeof(char)*V.lenstr);
    memcpy(s, z = z + sizeof(int) , V.lenstr);
    V.str = s;
    free(s);
    memcpy(&V.lensubstr, z=z + V.lenstr, sizeof(int));
    s = (char *)malloc(sizeof(char)*V.lensubstr);
    memcpy(s, z=z+sizeof(int) , V.lensubstr);
    V.substr = s;
    free(s);
    return V;    

}


#endif