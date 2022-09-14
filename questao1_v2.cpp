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

void* check_row(void * arguments){

    Arg* arg = (Arg*) arguments;


    int* detect_flag = (int*) malloc(sizeof(int));

    bool* check_array_row;
    check_array_row = (bool*) calloc(93, sizeof(bool));

    for(int j=0; j < arg->size; j++){
        int ascii_code = arg->matrix[arg->index][j];
        printf("%c\n", ascii_code);
        
        if(!check_array_row[ascii_code]) {check_array_row[ascii_code] = 1;}
        else{
            *detect_flag = 1;
            j = arg->size;
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
        printf("thread_row [%d] created\n", i);
    }

    int* flag;
    for(int i=0; i < n; i++){
        pthread_join(th_rows[i], (void**) &flag);
        printf("%d\n", *flag);
    } 
}
