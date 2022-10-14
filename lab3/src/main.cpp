#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>

using namespace std;

struct Data {
    
    vector<vector<long double> > &mas1;
    vector<vector<long double> > &mas2;
    vector<vector<long double> > &res1;
    vector<vector<long double> > &res2;
    
    int w_f;
    int thread_num;
    int thread_count;
};


void * multik(void *args){
    
    vector<vector<long double>>&mas1 = ((Data *)args)->mas1;
    vector<vector<long double>>&mas2 = ((Data *)args)->mas2;
    vector<vector<long double>>&res1 = ((Data *)args)->res1;
    vector<vector<long double>>&res2 = ((Data *)args)->res2;
    
    int thread_num = ((Data *)args)->thread_num;  // номер потока
    int th_count = ((Data *)args)->thread_count;  // количество потоков
    int cols = mas1[0].size();
    int offset = ((Data *)args)->w_f/2; 
    int rows = mas1.size();
    


    for (int th_row = thread_num; th_row < rows; th_row += th_count) {
        
        for (int th_col = 0; th_col < cols; th_col++) {
            
            long double max = mas1[th_row][th_col];
            long double min = mas2[th_row][th_col];
            for (int i = th_row - offset; i < th_row + offset + 1; i++) {
                for (int j = th_col - offset; j < th_col + offset + 1; j++) {
                    long double curr1, curr2;
                    if ((i < 0) || (i >= rows) || (j < 0) || (j >= cols)) {
                        curr1 = 0;
                        curr2 = 0;
                    } else {
                        curr1 = mas1[i][j];
                        curr2 = mas2[i][j];
                    }
                    if (curr1 > max) {
                        max = curr1;
                    }
                    if (curr2 < min) {
                        min = curr2;
                    }
                }
            }
            
            res1[th_row][th_col] = max;
            res2[th_row][th_col] = min;
        }

    }

    return NULL;
}

int main(int argc,char* argv[]){
    int N,w_f;
    
    cin >> N;
    
    vector<vector<long double>> mas1(N, vector<long double>(N,0));
    vector<vector<long double>> mas2(N, vector<long double>(N,0));
    int x;
    for(int i=0; i<N; ++i){
        for(int j=0;j<N;++j){
            cin >> x;
            mas1[i][j] = x;
            mas2[i][j] = x;
        }
    }


    cin >> w_f;
    int max_thread = atoi(argv[1]);
    vector<vector<long double> >res1(N, vector<long double>(N,0));
    vector<vector<long double> >res2(N, vector<long double>(N,0));

    vector<Data> data = vector<Data>(max_thread,{mas1,mas2,res1,res2,w_f,0,0});
    vector<pthread_t> threads = vector<pthread_t>(max_thread);
    
    for(int k=0; k<atoi(argv[2]); ++k){
        int i=0,j=0;
        for(int p=0; p<max_thread; p++){
            
            
            data[p].thread_count = max_thread;
            data[p].thread_num = p;
            if(p>=N*N){
                break;
            }

            if(int err = pthread_create(&threads[p],NULL,multik, (void *)&data[p]))
            {
                std::cout << "Thread create error: " << err << '\n';
                return -1;
            }

            
        }

        //join
        for(int i = 0; i < max_thread; i++) {
            if (pthread_join(threads[i],NULL) != 0) {
                cout << "Can't wait for thread\n";
            }
        }

        //swap
        for(int i = 0; i < max_thread; i++) { 
            swap(data[i].mas1,data[i].res1);
            swap(data[i].mas2,data[i].res2);
        }
    }

    for(int i = 0; i < max_thread; i++) { 
            swap(data[i].mas1,data[i].res1);
            swap(data[i].mas2,data[i].res2);
        }


    cout << "Filtr narashivania \n";
    for(int i=0; i<N; ++i){
        for(int j=0;j<N;++j){
            cout << data[0].res1[i][j] << " ";
            
        }
        cout << '\n';
    }
    
    cout << "\nFiltr erosii  \n";
    for(int i=0; i<N; ++i){
        for(int j=0;j<N;++j){
            cout << data[0].res2[i][j] << " ";
        }
        cout << '\n';
    }
    
}