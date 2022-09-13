#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex;


class Candidato{

    private:
        int num_de_votos;
        float percentual;
        pthread_mutex_t mutex_inc;

    public:
        Candidato(){
            num_de_votos = 0;
            percentual = 0;
            pthread_mutex_init(&mutex_inc, NULL);
        }

        //vou criar um destrutor kkkk pthread_mutex_destroy(&mutex_inc);

        void getPercentual(int totalVotos){
            percentual = ((float)num_de_votos / (float)totalVotos) * 100;
        }

        void incVotos(){
            
            pthread_mutex_lock(&mutex_inc);
            num_de_votos++;
            pthread_mutex_unlock(&mutex_inc);
            
        }

        void printCandidato(int n){
            printf("candidato %d: %.2f%%(%d votos)\n", n, percentual, num_de_votos);
        }
};


typedef struct Arquivo{

    int tamanhoNome;
    char nome[20];
    
}Arquivo;

typedef struct ArgStruct{

    FILE* fptr;
    int* qtd_votos;
    Candidato* arrayDeCandidatos;
    char *nomeArquivo;
    

}ArgStruct;


//--------------tá pegando------------------
Arquivo getFileName(int numArquivo){
    Arquivo arq;

    char numString[10];
    sprintf(numString, "%d", numArquivo);
    char extension[5];
    sprintf(extension, ".in");
    arq.tamanhoNome = strlen(numString) + strlen(extension) + 1;
    snprintf(arq.nome, arq.tamanhoNome, "%s%s", numString, extension);

    return arq;
}

//----------------- precisa alterar -----------------------------------------
void CreateFile(int numArquivo, int qtdCandidatos){ // convém fazer com que essa função retorne um ponteiro de arquivo

    Arquivo arq = getFileName(numArquivo);
   
    FILE* file;    
    file = fopen((char*) arq.nome, "w"); 

    srand(time(0));
    for(int i=0; i < 1000 ; i++){
        int randNumber = (rand() % (qtdCandidatos + 1));
        fprintf(file, "%d\n", randNumber);
    }

    fclose(file);
}


void* ContaVoto(void* arguments){
    pthread_mutex_lock(&mutex);
       
    ArgStruct* arg = (ArgStruct*) arguments;
    int voto;

    
    
    arg->fptr = fopen((arg->nomeArquivo),"r");
    
    //Essa provavelmente vai ser a função chamada pela thread
    while (!feof(arg->fptr)){
       
        *(arg-> qtd_votos)++; //vai precisar de mutex tbm né
        fscanf(arg->fptr, "%d", &voto);
        printf("%d,", voto); //a contagem tá pegando!
        arg-> arrayDeCandidatos[voto].incVotos(); //seria aqui que tá a região crítica ???? com certeza....

    }
    printf("O arquivo %s está sendo contado\n", arg->nomeArquivo);

    pthread_mutex_unlock(&mutex);
    
}

int main(){
    pthread_mutex_init(&mutex, NULL);
    
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
        CreateFile(i, G); // create file precisa usaro array de ponteiro pra arquivos!!!!!
    }
    //---------------------------------------------------------------------------//

    
    //você não deve chamar o pthreadjoin junto com pthread_create porque assim as threads rodam sequencialmente
    //perdem o intuito. Se chamar em outro loop 

    // for(int i=0; i < T; i++) pthread_create(&thread[i], NULL, )  


    /// 8 arquivos 3 threads --> 8 mod 3 = 2, 7 mod 3 = 1 , 6 mod 3 = 0, 5 mod 3
    //devo dar join assim que todas as minhas threads forem executadas ? Pra em seguida, repetir o processo para os arquivos
    //que ainda não foram executados ? Será que posso fazer alguma coisa usando a subtração entre N e T e usar recursão ?

    // 4 arquivos
    // 3 threads
    //5 candidatos
    int conclude_flag = 0;
    while(N){ //n= 4 t = 3 // n=1 t=3
        
        for(int i=0; i < T && i < N; i++){
            Arquivo arq = getFileName(N-i);
            printf("thread %d para o arquivo %s\n" , i, arq.nome); //debug
            ArgStruct args = {ponteiroArquivo[N - i - 1], totalDeVotos, candidatos, (char*) arq.nome};
            pthread_create(&thread[i], NULL, ContaVoto, (void*) &args);
            // printf("thread %d\n", i);

        }
        
        for(int j=0; j < T && j < N; j++){
            pthread_join(thread[j], NULL);
        }

        if(N >= T){
            N = N - T; //4 - 3 = 1
        }
        else{
            N = 0;
        } 
        // printf("n: %d t: %d\n", N, T );

        
        //quando coloco esse trecho de código o único arquivo que aparenta ser lido é o 2 (usando o ex de 4 3 5)

    }

    pthread_mutex_destroy(&mutex);
    

    // for (int i = 0; i < G; i++) candidatos[i].getPercentual(qtd_votos);
    // for (int i = 0; i < G; i++) candidatos[i].printCandidato(i);

}
