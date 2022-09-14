#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <time.h>

typedef struct Arg{
    int** matrix;
    int size;
    int index;
}Arg;

int** getMatrix(int n){

    //----- Alocação Dinâmica da matriz quadrada --------------------

    int** matrix = (int**) malloc( n * sizeof(int*));
    for(int i=0; i < n; i++) matrix[i] = (int*) malloc(n * sizeof(int));

    //----- Código que gera e printa uma matriz aleatória------------

    srand(time(0));
    for(int i=0; i < n ; i++){
        for(int j=0; j < n; j++){
            matrix[i][j] = (rand() % 93) + 33; //possibilita que ela seja criada apenas com caracteres visíveis
            printf("[%c]\t", matrix[i][j]);
        }
        printf("\n");
    }

    return matrix;

}

void* check_row(int** matrix, int size, int row){

    int* detect_flag = (int*) malloc(sizeof(int));

    bool* check_array_row;
    check_array_row = (bool*) calloc(93, sizeof(bool));

    for(int j=0; j < size; j++){
        int ascii_code = matrix[row][j];
        
        if(!check_array_row[ascii_code]) {check_array_row[ascii_code] = 1;}
        else{
            *detect_flag = 1;
            j = size;
        };

    }

    return (void*) detect_flag;
    
}

void check_column(){
    bool check_array_column [93]; 
}

int main(){

    int n;
    printf("Qual o tamanho n da matriz ?");
    std::cin >> n;

    int** square = getMatrix(n);

    pthread_t th_rows[n];
    pthread_t th_columns[n];

    for(int i=0; i < n; i++){
        Arg argument = {square, n, i};
        pthread_create(&th_rows[i], NULL, &check_row, (void*) &argument);
    } 


    

    
    
}
