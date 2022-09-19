#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

pthread_mutex_t mutex[N];
pthread_t philos_th[N];
int* forks; 


void printforks(){
    for(int i=0; i < N ; i++) printf("[%d]", forks[i]);
    printf("\n");
}

class Philosopher{

    private:
        int state;
        int index;

    public:

        Philosopher(){
            state = THINKING;
        }

        void set_index(int i){
            index = i;
        }

        void eats(){
            if(state == EATING){
                printf("Filosofo %d está comendo.", index);
                for(int i=0; i < 3; i++){
                    printf("%d: 'nhom (%d) \n'", index + 1, i+1);
                    sleep(1);
                }
                printf("\n");
            }

            state = THINKING;
        }


        void thinks(){
            if(state == THINKING){

                switch(index){
                    case 0: 
                        printf("1)O segredo da auto expansão ou valorização do capital se reduz ao seu poder de dispor\nde uma quantidade determinada de trabalho alheio não pago...\n\n");
                        sleep(2);
                        break;
                    case 1:
                        printf("2)Ninguém pode entrar duas vezes no mesmo rio, pois quando nele se entra novamente, não se encontra as mesmas águas,\ne o próprio ser já se modificou. Assim, tudo é regido pela dialética, a tensão e o revezamento dos opostos. Portanto, o real é sempre fruto da mudança, ou seja, do combate entre os contrários...\n\n");
                        sleep(2);
                        break;
                    case 2:
                        printf("3)Cogito ergo sum...\n\n");
                        sleep(2);
                        break;
                    case 3:
                        printf("4)Antes, a questão era descobrir se a vida precisava de ter algum significado para ser vivida.\nAgora, ao contrário, ficou evidente que ela será vivida melhor se não tiver significado.\n\n");
                        sleep(2);
                        break;
                    case 4:
                        printf("5)Para ver muita coisa é preciso despregar os olhos de si mesmo.\nNada neste mundo consome mais rapidamente o homem que a paixão pelo ressentimento...\n\n");
                        sleep(2);
                        break;

                }
            }
        }

        void gets_forks(){       

            if(state == THINKING){
                if(!forks[index]){
                    
                    pthread_mutex_lock(&mutex[index]);
                    forks[index] = index + 1; //marca o array de garfos com quem está o utilizando;
                    pthread_mutex_unlock(&mutex[index]);
                    printf("%d : 'Peguei o garfo %d.'\n\n", index + 1, index);
                    printforks();
                    state = HUNGRY;
                }
                else{
                    return;
                }
            }
            
            printf("%d : 'tentei pegar o garfo %d.'\n\n", index, index + 1);

            if(state == HUNGRY){
                if(!forks[index + 1] ){ //pra pegar outro garfo ele deve está livre e deve-se estar em posse do primeiro garfo
                    pthread_mutex_lock(&mutex[index + 1]);

                    if(index + 1 == N) forks[0] = index + 1;
                    else forks[index + 1] = index + 1; //marca o array de garfos com quem está o utilizando;

                    pthread_mutex_unlock(&mutex[index + 1]);
                    printforks();
                    state = EATING;
                }
                else{
                    if(forks[index] == index) forks[index] = 0; //libera o garfo que pode ter sido pego
                    state = THINKING;
                    return;
                }
            }

        }

        void puts_forks(){

            if(state == THINKING){
                if(index == N){
                    if(forks[index] == index + 1) forks[index] = 0;
                    if(forks[0] == index + 1) forks[index + 1] = 0;
                }
                else{
                    if(forks[index] == index + 1) forks[index] = 0;
                    if(forks[index + 1] == index + 1) forks[index + 1] = 0;
                }
                printforks();
            }

            return;
        }

        int getstate(){
            return state;
        }

};

typedef struct Arg{
    Philosopher philo;
    int index;

}Arg;

void* ToBeorNotToBe(void* argumentos){

    Arg* arg = (Arg*) argumentos;

    (arg->philo).set_index(arg->index);

    printf("o filósofo %d está no estado %d\n", arg->index + 1, arg->philo.getstate());

    while(true){

        (arg->philo).thinks();
        (arg->philo).gets_forks();
        (arg->philo).eats();
        (arg->philo).puts_forks();

    }
}

int main(){

    Philosopher philosophers[N];
    forks = (int*) calloc(N, sizeof(int));

    for(int i=0; i < N; i++){

        pthread_mutex_init(&mutex[i], NULL);

        int* index = (int*) malloc(sizeof(int));
        *index = i;

        Arg* arguments = (Arg*) malloc(sizeof(Arg));
        *arguments = {philosophers[*index], *index};

        pthread_create(&philos_th[*index], NULL, &ToBeorNotToBe, (void*) arguments);

    }

    for(int i=0; i < N; i++){
        int* index = (int*) malloc(sizeof(int));
        *index = i;
        pthread_join(philos_th[*index], NULL);
    }
}