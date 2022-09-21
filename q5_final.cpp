#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define N 5
#define LEFT (index + N - 1) % 5
#define RIGHT (index + 1) % 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

//------ Declarações uteis para a questão---------------------

pthread_mutex_t mutex_forks;
pthread_mutex_t mutex_terminal;
pthread_t philos_th[N];
pthread_cond_t condv[N];
int states[N];
int* forks;

//------ Funcoes ---------------------------------------------

void eats(int index){
    
    if (states[index] == EATING){
        pthread_mutex_lock(&mutex_terminal);
        printf("Filosofo %d está comendo: ", index + 1);
        printf("hmm! comidinha que delicia!!!\n\n");
        pthread_mutex_unlock(&mutex_terminal);

        sleep(1);

        pthread_mutex_lock(&mutex_forks);
        states[index] = THINKING;
        pthread_mutex_unlock(&mutex_forks);
    }
}

void thinks(int index){
  
    if (states[index] == THINKING){
        pthread_mutex_lock(&mutex_terminal);
        printf("O filósofo %d está pensando : ", index + 1);
        switch (index){
            case 0:
                printf("O segredo da auto expansão ou valorização do capital se reduz ao seu poder de dispor\nde uma quantidade determinada de trabalho alheio não pago...\n\n");
                sleep(1);
                break;
            case 1:
                printf("Ninguém pode entrar duas vezes no mesmo rio, pois quando nele se entra novamente,\nnão se encontra as mesmas águas, e o próprio ser já se modificou. Assim, tudo é regido pela dialética, a tensão e o revezamento dos opostos. Portanto, o real é sempre fruto da mudança, ou seja, do combate entre os contrários...\n\n");
                sleep(1);
                break;
            case 2:
                printf("Cogito ergo sum...\n\n");
                sleep(1);
                break;
            case 3:
                printf("Antes, a questão era descobrir se a vida precisava de ter algum significado para ser vivida.\nAgora, ao contrário, ficou evidente que ela será vivida melhor se não tiver significado.\n\n");
                sleep(1);
                break;
            case 4:
                printf("Para ver muita coisa é preciso despregar os olhos de si mesmo.\nNada neste mundo consome mais rapidamente o homem que a paixão pelo ressentimento...\n\n");
                sleep(1);
                break;
        }
        pthread_mutex_unlock(&mutex_terminal);
    }
}

void gets_forks(int index){

    pthread_mutex_lock(&mutex_forks);
    states[index] = HUNGRY;
    
    while(states[index] != HUNGRY || states[LEFT] == EATING || states[RIGHT] == EATING){ //enquanto ele não puder comer 
        pthread_cond_wait(&condv[index], &mutex_forks); // a thread fica esperando.

    }

    states[index] = EATING;
    printf("o filósofo %d pegou os garfos.\n", index + 1);
    pthread_mutex_unlock(&mutex_forks);

}

void puts_forks(int index){

    pthread_mutex_lock(&mutex_forks);
    states[index] = THINKING;
    printf("filosofo %d soltou os garfos.\n",index + 1);
    pthread_mutex_unlock(&mutex_forks);
    pthread_cond_signal(&condv[RIGHT]);
    pthread_cond_signal(&condv[LEFT]);
    
}

//--------------------Funcao chamada pelas threads ------------------------------------------

void *PhilFunction(void *arg) {

    int *index = (int *)arg;
    
    while (true){

        thinks(*index);
        gets_forks(*index);
        eats(*index);
        puts_forks(*index);

    }

    free(arg);
}

//----------------------------------------------------------------------------------------------

int main(){

    forks = (int *)calloc(N, sizeof(int));
    pthread_mutex_init(&mutex_forks, NULL);
    pthread_mutex_init(&mutex_terminal, NULL);

    for (int i = 0; i < N; i++){

        pthread_cond_init(&condv[i], NULL);
        states[i] = 0;

        int *index = (int *)malloc(sizeof(int));
        if(index == NULL) printf("Erro ao alocar memória!\n");
        else *index = i;

        if(pthread_create(&philos_th[*index], NULL, &PhilFunction, (void *)index)) printf("Erro ao criar thread!\n");

    }

    for (int i = 0; i < N; i++){
        if(pthread_join(philos_th[i], NULL)) printf("Erro ao juntar thread.\n");
        pthread_cond_destroy(&condv[i]);
    }

    pthread_mutex_destroy(&mutex_forks);
    pthread_mutex_init(&mutex_terminal, NULL);
}
