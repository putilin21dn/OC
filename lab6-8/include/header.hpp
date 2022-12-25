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
    memcpy(z = (void *)((unsigned long)z + sizeof(int)), &V.id, sizeof(int));
    memcpy(z = (void *)((unsigned long)z + sizeof(int)), &V.lenstr, sizeof(int));
    memcpy(z = (void *)((unsigned long)z + sizeof(int)), V.str.c_str(), sizeof(char)*V.lenstr);
    memcpy(z = (void *)((unsigned long)z + sizeof(char)*V.lenstr), &V.lensubstr, sizeof(int));
    memcpy(z = (void *)((unsigned long)z + sizeof(int)), V.substr.c_str(), sizeof(char)*V.lensubstr);
    return msg;

}

vec msg2vec(zmq_msg_t msg){
    void * z = zmq_msg_data(&msg);
    vec V;
    
    memcpy(&V.ex, z, sizeof(int));
    memcpy(&V.id, z=(void *)((unsigned long)z + sizeof(int)), sizeof(int));
    memcpy(&V.lenstr, z=(void *)((unsigned long)z + sizeof(int)), sizeof(int));
    char *  s = (char *)calloc(V.lenstr,sizeof(char));
    memcpy(s, z = (void *)((unsigned long)z + sizeof(int)) , V.lenstr);
    V.str = s;
    free(s);
    memcpy(&V.lensubstr, z=(void *)((unsigned long)z + sizeof(char)*V.lenstr), sizeof(int));
    s = (char *)calloc(V.lensubstr,sizeof(char));
    s = s + '\0';
    memcpy(s, z=(void *)((unsigned long)z + sizeof(int)) , V.lensubstr);
    V.substr = s;
    free(s);
    return V;    

}

zmq_msg_t str2msg(string str){
    zmq_msg_t msg;
    zmq_msg_init_size(&msg, str.length());
    void * z = zmq_msg_data(&msg);
    memcpy(z, str.c_str(), sizeof(char)*str.length());
    return msg;
}

string msg2str(zmq_msg_t msg){
    void * z = zmq_msg_data(&msg);
    int len = zmq_msg_size(&msg);
    char * s = (char *)calloc(len,sizeof(char));
    memcpy(s,z,len*sizeof(char));
    return s;   
}

#endif