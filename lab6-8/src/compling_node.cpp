#include <zmq.h>
#include <unistd.h>
#include <string.h>
#include "header.hpp"
#include <iostream>
#include <vector>

#define CREATE 1
#define EXEC 0
#define REMOVE -1

#define CHECK_ERROR(expr) \
    do \
    { \
        int res = (expr); \
        if (res == -1){ \
            std::cout << errno << '\n'; \
            return -1;} \
    } while (0)

using namespace std;

vector<int> z_func(int lensubstr, string &s){
    int n = s.size();
    vector<int> z(n,0);
    vector <int> ans;
    int l=-1,r=-1;
    z[0]=n;
    for(int i=1;i<n;++i){
        if(i<=r){
            z[i]=min(z[i-l], r-i);
        }
        while((i+z[i]<n) && (s[z[i]]==s[i+z[i]])){
            ++z[i];
        }
        if(i+z[i]>r){
            r=i+z[i];
            l=i;
        }
    }
    for(int i=lensubstr; i<n;++i){
        if(z[i]==lensubstr){
            ans.push_back(i-lensubstr-1);
        }
    }
    return ans;
}


int main(int argc, char* argv[])
{
    int id = atoi(argv[0]);
    string port = "tcp://*:" + to_string(id);
    void *context = zmq_ctx_new();  
    void *responder = zmq_socket(context, ZMQ_REP);
    zmq_bind(responder, port.c_str());
    string id_str = to_string(id);

    cout << "HERE!" << '\n';
    while (true) 
    {
        zmq_msg_t msg;
        CHECK_ERROR(zmq_msg_init(&msg));
        // cout << "heresa" <<'\n';
        CHECK_ERROR(zmq_msg_recv(&msg, responder, 0));
        // cout << "heresdfs" <<'\n';
        vec V = msg2vec(msg);
        switch (V.ex)
        {
            case CREATE:
            {
                // cout << "here2" << '\n';
                int pid = fork();
                // cout << "here 2" << pid <<'\n';
                if (pid == -1)
                    return -1;
                if (pid == 0)
                    CHECK_ERROR(execl("server", id_str.c_str(), NULL));
                else
                    CHECK_ERROR(zmq_send(responder, &pid, sizeof(int), 0));
                break;
            }

            case EXEC:
            {
                cout << "here2" << '\n';
                string s = V.substr + "#" + V.str;
                vector <int> ans;
                ans = z_func(V.lensubstr,s);
                for(auto x : ans){
                    cout << x << " ";
                }
                cout << '\n';

                zmq_send(responder,&ans,sizeof(ans),0);
                break;
            }

            case REMOVE:
            {
                zmq_send(responder, &id, sizeof(int), 0);
                zmq_close(responder);
                zmq_ctx_destroy(context);
                return 0;
            }
        }
        CHECK_ERROR(zmq_msg_close(&msg));
    }
}