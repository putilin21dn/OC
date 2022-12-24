#include "zmq.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#include <iostream>
#include <queue>
#include <vector>
#include "header.hpp"
#include <ctime>
#include <cstdlib>

#define CHECK_ERROR(expr, stream, act) \
    do \
    { \
        int res = (expr); \
        if (res != 0) \
        { \
            std::cerr << stream; \
            act; \
        } \
    } while (0)

#define CHECK_ZMQ(expr, stream, act) \
    do \
    { \
        int res = (expr); \
        if (res == -1) \
        { \
            std::cerr << stream; \
            act; \
        } \
    } while (0) 



using namespace std;

string protocol = "tcp://localhost:";
int MIN_PORT;

void* async_node_thd(void*);


struct async_node
{
    int id;
    string port;
    bool active;
    async_node* L;
    async_node* R;
    pthread_mutex_t mutex;
    pthread_t thd;
    queue <vec> q;



    async_node(int i)
    {
        id = i;
        port = protocol + to_string(i);
        active = true;
        L = nullptr;
        R = nullptr;
        CHECK_ERROR(pthread_mutex_init(&mutex, NULL), "Error:" << i - MIN_PORT << ": Gateway mutex error\n", return);
        CHECK_ERROR(pthread_create(&thd, NULL, async_node_thd, this), "Error:" << i - MIN_PORT << ": Gateway thread error\n", return);
        CHECK_ERROR(pthread_detach(thd), "Error:" << i << ": Gateway thread error\n", return);
    }

    void make_query(vec V)
    {
        CHECK_ERROR(pthread_mutex_lock(&mutex), "Error:" << id - MIN_PORT << ": Gateway mutex lock error\n", active = false; return);
        q.push(V);
        CHECK_ERROR(pthread_mutex_unlock(&mutex), "Error:" << id - MIN_PORT << ": Gateway mutex unlock error\n", active = false);
    }

    ~async_node()
    {
        pthread_mutex_destroy(&mutex);
    }
};

async_node* find_node_exec(async_node* ptr, int id)
{
    if (ptr == nullptr)
        return nullptr;
    if (ptr->id > id)
        return find_node_exec(ptr->L, id);
    if (ptr->id < id)
        return find_node_exec(ptr->R, id);
    return ptr;
}

async_node* find_node_create(async_node* ptr, int id)
{
    if (ptr == nullptr)
        return nullptr;
    // cout << ptr->id << " " << id << '\n';
    if (ptr->L == nullptr && ptr->id > id)
        return ptr;
    if (ptr->R == nullptr && ptr->id < id)
        return ptr;
    if (ptr->id > id)
        return find_node_create(ptr->L, id);
    if (ptr->id < id)
        return find_node_create(ptr->R, id);
    return nullptr;
}

bool destroy_node(async_node*& ptr, int id)
{
    if (ptr == nullptr)
        return false;
    if (ptr->id > id)
        return destroy_node(ptr->L, id);
    if (ptr->id < id)
        return destroy_node(ptr->R, id);
    ptr->active = false;
    ptr->make_query({REMOVE});
    if (ptr->L != nullptr)
        destroy_node(ptr->L, ptr->L->id);
    if (ptr->R != nullptr)
        destroy_node(ptr->R, ptr->R->id);
    ptr = nullptr;
    return true;
}
bool ping(int);

void* async_node_thd(void* ptr)
{
    async_node* node = (async_node*)ptr;
    void* context = zmq_ctx_new();
    void* req = zmq_socket(context, ZMQ_REQ);
    CHECK_ZMQ(zmq_connect(req, node->port.c_str()), "Error: Connection with" << node->id - MIN_PORT << "\n",);
    while (node->active)
    {
        if (node->q.empty())
            continue;
        CHECK_ERROR(pthread_mutex_lock(&node->mutex), "Error:" << node->id - MIN_PORT << ": Gateway mutex lock error\n", node->active = false; break);
        vec V = node->q.front();
        node->q.pop();
        CHECK_ERROR(pthread_mutex_unlock(&node->mutex), "Error:" << node->id - MIN_PORT << ": Gateway mutex unlock error\n", node->active = false; break);

        // cout << V.ex << " " << V.id << " " << V.str << " " << V.lenstr << '\n';

        switch (V.ex)
        {   
            case CREATE:
            {
                zmq_msg_t msg;
                msg = vec2msg(V);
                CHECK_ZMQ(zmq_msg_send(&msg, req, 0), "Error:" << node->id - MIN_PORT << ": Message error\n", break);
                int pid;
                CHECK_ZMQ(zmq_recv(req, &pid, sizeof(int), 0), "Error:" << node->id - MIN_PORT << ": Message error\n", break);
                if (V.id < node->id)
                    node->L = new async_node(V.id);
                else
                    node->R = new async_node(V.id);
                cout << "Ok: " << pid << '\n';
                zmq_msg_close(&msg);
                break;
            }

            case EXEC:
            {
                zmq_msg_t msg;
                msg = vec2msg(V);
                CHECK_ZMQ(zmq_msg_send(&msg, req, 0), "Error:" << node->id - MIN_PORT << ": Message error\n", break);
                string ans;
                CHECK_ZMQ(zmq_msg_recv(&msg, req, 0), "Error:" << node->id - MIN_PORT << ": Message error\n", break);
                cout << "Ok:" << node->id - MIN_PORT << ':';
                ans = msg2str(msg);

                for(int i=0; i<ans.length();++i){
                    if(ans[i]!='#')
                        cout << ans[i];
                    else{
                        cout << " ";
                    }
                }

                cout << '\n';

                zmq_msg_close(&msg);
                break;
            }

            case REMOVE:
            {
                CHECK_ZMQ(zmq_send(req, &V.ex, sizeof(int), 0), "Error:" << node->id - MIN_PORT << ": Message error\n", break);
                int ans;
                CHECK_ZMQ(zmq_recv(req, &ans, sizeof(int), 0), "Error:" << node->id - MIN_PORT << ": Message error\n", break);
                break;
            }
        }
    }
    zmq_close(req);
    zmq_ctx_destroy(context);
    delete node;
    return NULL;
}

bool ping(int id)
{
    string port = protocol + to_string(id);
    string ping = "inproc://ping" + to_string(id);
    void* context = zmq_ctx_new();
    void *req = zmq_socket(context, ZMQ_REQ);

    zmq_socket_monitor(req, ping.c_str(), ZMQ_EVENT_CONNECTED | ZMQ_EVENT_CONNECT_RETRIED);
    void *soc = zmq_socket(context, ZMQ_PAIR);
    zmq_connect(soc, ping.c_str());
    zmq_connect(req, port.c_str());

    zmq_msg_t msg;
    zmq_msg_init(&msg);
    zmq_msg_recv(&msg, soc, 0);
    uint8_t* data = (uint8_t*)zmq_msg_data(&msg);
    uint16_t event = *(uint16_t*)(data);

    zmq_close(req);
    zmq_close(soc);
    zmq_msg_close(&msg);
    zmq_ctx_destroy(context);
    return event % 2;
}



async_node* tree = nullptr;

int main()
{
    srand( time(0) );
    MIN_PORT = 1024 + rand()%1000;

    while (true)
    {
        string command;
        cin >> command;
        if (command == "create")
        {
            int id;
            cin >> id;
            // cin >> str >> substr;
            id += MIN_PORT;
            vec V;
            V.ex = CREATE;
            V.id = id;

            if (tree == nullptr)
            {
                string id_str = to_string(id);
                int pid = fork();
                if (pid == 0)
                    CHECK_ERROR(execl("server", id_str.c_str(), NULL), "Error:" << id - MIN_PORT << ": Creating error\n", break);
                cout << "Ok: " << pid << '\n';
                tree = new async_node(id);
            }
            else
            {
                async_node* node = find_node_create(tree, id);
                if (node != nullptr){
                    // if (!ping(node->id))
                    // {
                    //     cerr << "Error:" << id - MIN_PORT << ": Parent is unavailable\n";
                    //     continue;
                    // }
                    cout << node->id << '\n';
                    node->make_query({CREATE, id});
                }
                else
                    cerr << "Error: Already exists\n";
            }
        }

        if (command == "exec")
        {
            int id;
            cin >> id;
            id += MIN_PORT;
            string str,substr;
            cin >> str >> substr;
            vec V;
            V.ex = EXEC;
            V.id = id;
            V.lenstr = str.length();
            V.str = str;
            V.lensubstr = substr.length();
            V.substr = substr;
            // if (!ping(id))
            // {
            //     cerr << "Error:" << id - MIN_PORT << ": Node is unavailable\n";
            //     continue;
            // }
            async_node* node = find_node_exec(tree, id);
            if (node != nullptr)
                node->make_query(V);
            else
                cerr << "Error:" << id - MIN_PORT << ": Not found\n";
        }

        if (command == "remove")
        {
            int id;
            cin >> id;
            id += MIN_PORT;
            if (!ping(id))
            {
                cerr << "Error:" << id - MIN_PORT << ": Node is unavailable\n";
                continue;
            }
            bool state = destroy_node(tree, id);
            if (state)
                cout << "Ok\n";
            else
                cerr << "Error: Not found\n";
        }

        if (command == "unsafe_remove")
        {
            int id;
            cin >> id;
            id += MIN_PORT;
            
            bool state = destroy_node(tree, id);
            if (state)
                cout << "Ok\n";
            else
                cerr << "Error: Not found\n";
        }
    }
}