#include <stdio.h>
#include <pthread.h>

#define GREEN 0
#define YELLOW 1
#define RED 2

pthread_t rua_a;
pthread_t rua_b;
pthread_cond_t aguardo_a;
pthread_cond_t aguardo_b;
int sinal_a;
int sinal_b;

void* carro(){
    while(sinal_a != GREEN){
        pthread_cond_wait();
    }

}

int main(){
    
}