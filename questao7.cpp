#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex;

typedef struct Arg{
    int** matrix;
    int size;
    int* index;
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
    int* index = arg -> index;

    printf("thread %d executando!\n", *index);

    int* detect_flag = (int*) malloc(sizeof(int));

    bool* check_array_row;
    check_array_row = (bool*) calloc(94, sizeof(bool));

    for(int j=0; j < arg->size; j++){
        int ascii_code = arg->matrix[*index][j];

        printf("%c\n", ascii_code);
        
        if(!check_array_row[ascii_code - 33]) {check_array_row[ascii_code - 33] = 1;}
        else{
            *detect_flag = 1;
            break;
        };

    }
    free(arg->index);
    return (void*) detect_flag;
    
}

void check_column(){
    bool check_array_column [94]; 
}

int main(){

    pthread_mutex_init(&mutex, NULL);

    int n;
    printf("Qual o tamanho n da matriz ?");
    std::cin >> n;

    int** square = getMatrix(n);

    pthread_t th_rows[n];
    pthread_t th_columns[n];

    int* th_rows_id[n];

    for(int i=0; i < n; i++){

        th_rows_id[i] = (int*) malloc(sizeof(int));
        *th_rows_id[i] = i;
        Arg* argument = (Arg*) malloc(sizeof(Arg));
        *argument = {square, n, th_rows_id[i]};
        pthread_create(&th_rows[i], NULL, &check_row, (void*) argument);
        printf("thread_row [%d] created\n", *th_rows_id[i]);


    }
    int i = 0;
    int* flag;
    for(int i=0; i < n; i++){
        pthread_join(th_rows[i], NULL);

    }

    pthread_mutex_destroy(&mutex);

}
