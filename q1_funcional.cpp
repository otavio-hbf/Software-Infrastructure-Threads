#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_mutex_t mutex_inc;
int qtd_votos;

//----------Estruturas de Dados importantes------------------------------

class Candidato{

    public:
        int num_de_votos;
        float percentual;

        Candidato(){
            num_de_votos = 0;
            percentual = 0;
        }

        void getPercentual(){
            percentual = ((float)num_de_votos / (float)qtd_votos) * 100;
        }

        void incVotos(){

            pthread_mutex_lock(&mutex_inc);
            num_de_votos++;
            pthread_mutex_unlock(&mutex_inc);

        }

        void printCandidato(int n){
            printf("candidato %d: %.2f% (%d votos)\n", n, percentual, num_de_votos);
        }
};


typedef struct ArgStruct{

    FILE* fptr;
    Candidato* arrayDeCandidatos;
    char *nomeArquivo;


}ArgStruct;

//------Funções que criam arquivos-----------------------------------------

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

void CreateFile(int numArquivo, int qtdCandidatos){

    char* nome = getFileName(numArquivo);

    FILE* file;    
    file = fopen(nome, "w"); 
    time_t
    srand((time(NULL)*numArquivo * qtdCandidatos));
    for(int i=0; i < 1000 ; i++){
        int randNumber = (rand() % (qtdCandidatos + 1));
        fprintf(file, "%d\n", randNumber);
    }

    fclose(file);
}

//-------Função chamada pela thread--------------------------------

void* ContaVoto(void* arguments){

    ArgStruct* arg = (ArgStruct*) arguments;
    int voto;

    pthread_mutex_lock(&mutex);

    arg->fptr = fopen((arg->nomeArquivo),"r");
    printf("O arquivo %s está sendo contado\n", arg->nomeArquivo); ////debug


    while (!feof(arg->fptr)){

        qtd_votos++; 
        fscanf(arg->fptr, "%d", &voto);
        arg-> arrayDeCandidatos[voto].incVotos();
        

    }
    pthread_mutex_unlock(&mutex);
    free(arguments);

}

//---------------------------------------------------------------------------

int main(){

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_inc, NULL);
    
    //-------Recebe o input do usuário para customizar a questão----------------

    int  N, T , G;
    qtd_votos = 0;

    printf("Digite um número N de arquivos: ");
    scanf("%d", &N);
    printf("Digite um número T de threads, MENOR que o N de arquivos: ");
    scanf("%d", &T);    
    printf("Digite um número C de candidatos ao Governo: ");
    scanf("%d", &G);
    printf("\n\n");

    //--------- Cria os arrays uteis para a questão-----------------------------

    FILE* ponteiroArquivo[N];
    pthread_t thread[T]; // criando as threads
    Candidato candidatos[G + 1]; // c + 1

    //-------- Cria os arquivos de votação -------------------------------------

    for(int i=1; i <= N; i++){
        CreateFile(i, G);
    }

    //-------- Cria as threads e designa cada thread para um arquivo até que os arquivos sejam todos lidos--------

    int conclude_flag = 0;
    int j = 0, count = 0, n_join = 0;
    int repeat = (int) ceil(((float)N/(float)T));

    while(repeat--){
        for(int i=0; i < T && N - j > 0; i++){
        
            char* nome = (char*) malloc(sizeof(char*));
            nome = getFileName(j+1);
            printf("thread %d para o arquivo %s\n" , (j % T), nome); //debug
            
            ArgStruct* args = (ArgStruct*) malloc(sizeof(ArgStruct));
            if(args == NULL) printf("Erro ao alocar memória!\n");
            else *args = {ponteiroArquivo[j], candidatos, nome};

            if(pthread_create(&thread[(j % T)], NULL, &ContaVoto, (void*) args)) printf("Erro ao criar thread");

            j++;
            n_join++;
        }

        for(int k=0; k < n_join; k++) pthread_join(thread[k], NULL);
        n_join = 0;

    }

    //--------Calcula e mostra os resultados ------------------------------------------------------------

    qtd_votos -=  N; candidatos[0].num_de_votos -= N; //correção de zeros contados a mais no arquivos

    printf("\n\n");
    for (int i = 0; i < G; i++) candidatos[i].getPercentual();
    for (int i = 0; i < G; i++) candidatos[i].printCandidato(i);

    int maior_index = 0, maior_num = 0;
    for(int i=0; i < G; i++){ //seleciona o candidato com mais votos.
        if(candidatos[i].num_de_votos > maior_num){
            maior_index = i;
            maior_num = candidatos[i].num_de_votos;
        } 
    }
    printf("O candidato %d ganhou a eleição!\n", maior_index);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_inc);
    
}
