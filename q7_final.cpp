#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <time.h>

typedef struct Arg{
    int** matrix;
    int size;
    int* index;
}Arg;

//--- Funções --------------------------------------------------------------

//Função acessório
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

//----- Função chamada por parte das threads ---------------------------------------------------------------
void* check_row(void * arguments){ // Uma função que faz com que uma thread cheque repetições em uma linha

    Arg* arg = (Arg*) arguments;
    int* index = arg -> index;

    int* detect_flag = (int*) malloc(sizeof(int));
    if(detect_flag == NULL){ printf("Erro em alocação de memória.\n"); exit(1); }

    bool* check_array_row;
    check_array_row = (bool*) calloc(94, sizeof(bool)); // alocação do array que vai ser usado para checagem
    if(check_array_row == NULL){ printf("Erro em alocação de memória.\n"); exit(1);}

    for(int j=0; j < arg->size; j++){
        int ascii_code = arg->matrix[*index][j]; //a variação ocorre nas linhas

        if(!check_array_row[ascii_code - 33]) {check_array_row[ascii_code - 33] = 1;}
        else{
            *detect_flag = 1;
            break;
        };
    }

    free(arguments);
    free(check_array_row);
    return (void*) detect_flag;
}
        
//----- Função chamada por outra parte das threads ----------------------------------------------------------
void* check_column(void* arguments){ // Uma função que faz com que uma thread cheque repetições em uma coluna

    Arg* arg = (Arg*) arguments;
    int* index = arg -> index;

    int* detect_flag = (int*) malloc(sizeof(int));
    if(detect_flag == NULL){ printf("Erro em alocação de memória.\n"); exit(1); }

    bool* check_array_row;
    check_array_row = (bool*) calloc(94, sizeof(bool)); // alocação do array que vai ser usado para checagem
    if(check_array_row == NULL){ printf("Erro em alocação de memória.\n"); exit(1);}

    for(int j=0; j < arg->size; j++){

        int ascii_code = arg->matrix[j][*index]; //a variação ocorre nas colunas
        if(!check_array_row[ascii_code - 33]) {check_array_row[ascii_code - 33] = 1;}
        else{
            *detect_flag = 1;
            break;
        };
    }

    free(arguments);
    free(check_array_row);
    return (void*) detect_flag;
}
//-----------------------------------------------------------------------------------------------------------

int main(){

//------- Recebe entrada do usuário-------------------------------
    int n;
    printf("Qual o tamanho n da matriz ?");
    std::cin >> n;

//------- Cria uma matriz de tamanho correspondente---------------
    int** square = getMatrix(n);

//------- Cria um array de pthread_t de tamanho n para as linhas --------------------
//-------       e  outro de mesmo tamanho para as colunas.        -------------------

    pthread_t th_rows[n];
    pthread_t th_columns[n];

//------- Cria as threads que vão checar as linhas   ---------------------------------
    int* th_rows_id[n];
    for(int i=0; i < n; i++){

        th_rows_id[i] = (int*) malloc(sizeof(int));
        if(th_rows_id[i] == NULL) printf("erro de alocação.\n");
        else *th_rows_id[i] = i;
        
        Arg* argument = (Arg*) malloc(sizeof(Arg));
        if(argument == NULL) printf("erro de alocação.\n");
        else *argument = {square, n, th_rows_id[i]};

        if(pthread_create(&th_rows[i], NULL, &check_row, (void*) argument)) printf("Erro ao criar thread.\n");
    }

//------- Cria as threads que vão checar as colunas ---------------------------------
    int* th_columns_id[n];
    for(int i=0; i < n; i++){

        th_columns_id[i] = (int*) malloc(sizeof(int));
        if(th_columns_id[i] == NULL) printf("erro de alocação.\n");
        else *th_columns_id[i] = i;
        
        Arg* argument = (Arg*) malloc(sizeof(Arg));
        if(argument == NULL) printf("erro de alocação.\n");
        else *argument = {square, n, th_columns_id[i]};

        pthread_create(&th_columns[i], NULL, &check_column, (void*) argument);
    }

//----- Dá join nas threads e recebe o resultado das funções, para checar se ------------------
//----- a matriz gerada é um quadrado latino, e assim printar a conclusão.   ------------------

    int* flag_rows;
    int* flag_columns;
    int flag_latin = 0;

    for(int i=0; i < n; i++){
        if(pthread_join(th_rows[i], (void**) &flag_rows));
        if(pthread_join(th_columns[i], (void**) &flag_columns));        

        if(*flag_rows || *flag_columns){
            flag_latin = 1;
            break;
        }
    }

    if(flag_latin)printf("\n sim. É um quadrado latino! :} \n");
    else printf("\n Não. Não é um quadrado latino... :(\n");
}
