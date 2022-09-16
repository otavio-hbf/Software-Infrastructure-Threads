#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex;
pthread_mutex_t mutex_inc;

class Candidato{

    private:
        int num_de_votos;
        float percentual;

    public:
        Candidato(){
            num_de_votos = 0;
            percentual = 0;
        }

        void getPercentual(int totalVotos){
            percentual = ((float)num_de_votos / (float)totalVotos) * 100;
        }

        void incVotos(){

            pthread_mutex_lock(&mutex_inc);
            num_de_votos++;
            pthread_mutex_unlock(&mutex_inc);

        }

        void printCandidato(int n){
            printf("candidato %d: %.2f porcento (%d votos)\n", n, percentual, num_de_votos);
        }
};


typedef struct ArgStruct{

    FILE* fptr;
    int* qtd_votos;
    Candidato* arrayDeCandidatos;
    char *nomeArquivo;


}ArgStruct;


//--------------tá pegando------------------
char* getFileName(int numArquivo){
    
    char numString[10];
    sprintf(numString, "%d", numArquivo);
    char extension[5];
    sprintf(extension, ".in");
    int tamanhoNome = strlen(numString) + strlen(extension) + 1;
    char* nome = (char*) malloc(tamanhoNome * sizeof(char));
    snprintf(nome, tamanhoNome, "%s%s", numString, extension);

    return nome;
}

//-------------------tá pegando---------------
void CreateFile(int numArquivo, int qtdCandidatos){

    char* nome = getFileName(numArquivo);

    FILE* file;    
    file = fopen(nome, "w"); 

    srand(time(0));
    for(int i=0; i < 1000 ; i++){
        int randNumber = (rand() % (qtdCandidatos + 1));
        fprintf(file, "%d\n", randNumber);
    }

    fclose(file);
}


void* ContaVoto(void* arguments){

    ArgStruct* arg = (ArgStruct*) arguments;
    int voto;

    // pthread_mutex_lock(&mutex);

    arg->fptr = fopen((arg->nomeArquivo),"r");
    printf("O arquivo %s está sendo contado\n", arg->nomeArquivo);
    //Essa provavelmente vai ser a função chamada pela thread
    while (!feof(arg->fptr)){

        *(arg-> qtd_votos)++; //vai precisar de mutex tbm né
        fscanf(arg->fptr, "%d", &voto);
        // printf("%d,", voto);
        arg-> arrayDeCandidatos[voto].incVotos(); //seria aqui que tá a região crítica ???? com certeza....

    }

    // pthread_mutex_unlock(&mutex);

}

int main(){

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_inc, NULL);

    //----------------tá pegando----------------------------------------------//
    int qtd_votos = 0, N, T , G;
    int* totalDeVotos = &qtd_votos;

    printf("Digite um número N de arquivos: ");
    scanf("%d", &N);
    printf("Digite um número T de threads, MENOR que o N de arquivos: ");
    scanf("%d", &T);    
    printf("Digite um número C de candidatos ao Governo: ");
    scanf("%d", &G);
    printf("\n\n\n");

    FILE* ponteiroArquivo[N];
    pthread_t thread[T]; // criando as threads
    Candidato candidatos[G + 1]; // c + 1

    for(int i=1; i <= N; i++){
        CreateFile(i, G);
    }

    int conclude_flag = 0;

    int j = 0;
    for(int i=1; i <= N; i++){
        if(j > T || N - j < T){
            for(int k=0; k < T ; k++){
                pthread_join(thread[k], NULL);
                j = 0;
            }
        }

        int* index = (int*) malloc(sizeof(int));
        *index = i;

        char* nome = (char*) malloc(sizeof(char*));
        nome = getFileName(*index);
        printf("thread %d para o arquivo %s\n" , j, nome); //debug
        
        ArgStruct* args = (ArgStruct*) malloc(sizeof(ArgStruct));
        *args = {ponteiroArquivo[*index-1], totalDeVotos, candidatos, nome};
        pthread_create(&thread[j], NULL, &ContaVoto, (void*) args);
        j++;

    }

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_inc);

    // for (int i = 0; i < G; i++) candidatos[i].getPercentual(qtd_votos);
    // for (int i = 0; i < G; i++) candidatos[i].printCandidato(i);

}
