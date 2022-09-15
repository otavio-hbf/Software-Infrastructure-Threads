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

    // printf("thread row %d executando!\n", *index);

    int* detect_flag = (int*) malloc(sizeof(int));

    bool* check_array_row;
    check_array_row = (bool*) calloc(94, sizeof(bool));

    for(int j=0; j < arg->size; j++){
        int ascii_code = arg->matrix[*index][j];

        // printf("%c\n", ascii_code);
        
        if(!check_array_row[ascii_code - 33]) {check_array_row[ascii_code - 33] = 1;}
        else{
            *detect_flag = 1;
            break;
        };

    }
    free(arg->index);
    return (void*) detect_flag;
    
}

void* check_column(void* arguments){

    Arg* arg = (Arg*) arguments;
    int* index = arg -> index;

    // printf("thread column %d executando!\n", *index);

    int* detect_flag = (int*) malloc(sizeof(int));

    bool* check_array_row;
    check_array_row = (bool*) calloc(94, sizeof(bool));

    for(int j=0; j < arg->size; j++){

        int ascii_code = arg->matrix[j][*index];

        // printf("%c\n", ascii_code);
        
        if(!check_array_row[ascii_code - 33]) {check_array_row[ascii_code - 33] = 1;}
        else{
            *detect_flag = 1;
            break;
        };
    }
    
    free(arg->index);
    return (void*) detect_flag;

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
        // printf("thread_row [%d] created\n", *th_rows_id[i]);

    }


    int* th_columns_id[n];
    for(int i=0; i < n; i++){

        th_columns_id[i] = (int*) malloc(sizeof(int));
        *th_columns_id[i] = i;
        Arg* argument = (Arg*) malloc(sizeof(Arg));
        *argument = {square, n, th_columns_id[i]};
        pthread_create(&th_columns[i], NULL, &check_column, (void*) argument);
        // printf("thread_column [%d] created\n", *th_columns_id[i]);

    }


    int* flag_rows;
    int* flag_columns;
    int flag_latin = 0;

    for(int i=0; i < n; i++){
        pthread_join(th_rows[i], (void**) &flag_rows);
        

        if(*flag_rows){
            // printf("repetição na linha: %d\n", i);
            flag_latin = 1;
            break;
        }

        
    }

    for(int i=0; i < n; i++){
        pthread_join(th_columns[i], (void**) &flag_columns);

        if(*flag_columns){
            // printf("repetição na coluna: %d\n", i);
            flag_latin = 1;
            break;
        }

        
    }

    if(flag_latin)printf("\n sim. É um quadrado latino! :} \n");
    else printf("\n Não. Não é um quadrado latino... :(\n");
    pthread_mutex_destroy(&mutex);

}
