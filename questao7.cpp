#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <time.h>


int main(){
    int n;
    printf("Qual o tamanho n da matriz ?");
    std::cin >> n;

    char matriz[n][n];
    pthread_t th_horizontal[n];
    pthread_t th_vertical[n];

    
//----- Código que gera e printa uma matriz aleatória------------

    srand(time(0));
    for(int i=0; i < n ; i++){
        for(int j=0; j < n; j++){
            matriz[i][j] = (rand() % 93) + 33; //possibilita que ela seja criada apenas com caracteres visíveis
            printf("[%c]\t", matriz[i][j]);
        }
        printf("\n");
    }

    for(int i=0; i < n; i++){
        
    }

    
}