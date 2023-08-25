#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

#define N_HACKERS 10
#define N_SERFS 10

static int hackers = 0;
static int serfs = 0;
static int kapetan=0;
static pthread_barrier_t barrier;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t hackerRed;
static sem_t serfsRed;

void *hacker(void *arg){

    sleep(rand()%15);
    int br_hakera=*(int*)arg;
    pthread_mutex_lock(&mutex);
    hackers++;
    if(hackers==4){
        for(int i=0;i<4;i++)sem_post(&hackerRed);
        hackers=0;
        kapetan=br_hakera;
    }
    else if(hackers==2 && serfs>=2){
        for(int i=0;i<2;i++){
            sem_post(&hackerRed);
            sem_post(&serfsRed);
        }
        serfs-=2;
        hackers=0;
        kapetan=br_hakera;
    }
    else pthread_mutex_unlock(&mutex);

    sem_wait(&hackerRed);

    printf("Hacker %d se ukrcao na brod\n",br_hakera);
    pthread_barrier_wait(&barrier);


    if(kapetan==br_hakera){
        kapetan=-1;
        printf("Kapetan broda je pokrenuo brod\n");
        pthread_mutex_unlock(&mutex);
    }
    free(arg);
}

void *serf(void *arg){

    sleep(rand()%15);
    int br_serfa=*(int*)arg;

    pthread_mutex_lock(&mutex);
    serfs++;
    if(serfs==4){
        for(int i=0;i<4;i++)sem_post(&serfsRed);
        serfs=0;
        kapetan=br_serfa;
    }
    else if(serfs==2 && hackers>=2){
        for(int i=0;i<2;i++){
            sem_post(&serfsRed);
            sem_post(&hackerRed);
        }
        hackers-=2;
        serfs=0;
        kapetan=br_serfa;
    }
    else pthread_mutex_unlock(&mutex);
    sem_wait(&serfsRed);

    printf("Serf %d se ukrcao na brod\n",br_serfa);
    pthread_barrier_wait(&barrier);

    if(kapetan==br_serfa){
        kapetan=-1;
        printf("Kapetan broda je pokrenuo brod\n");
        pthread_mutex_unlock(&mutex);
    }
    free(arg);
}

int main(){

    pthread_t linux_h[N_HACKERS];
    pthread_t microsoft_s[N_SERFS];
    sem_init(&hackerRed,0,0);
    sem_init(&serfsRed,0,0);
    pthread_barrier_init(&barrier, NULL, 4);
    srand(time(NULL));

    for(int i=0;i<N_HACKERS;i++){
        int *value=malloc(sizeof(int));
        *value=i;
        if(pthread_create(linux_h+i,NULL,hacker,value)!=0){
            perror("Greska pri kreiranju niti\n");
            return -1;
        }
    }

    for(int i=0;i<N_SERFS;i++){
        int *value=malloc(sizeof(int));
        *value=i;
        if(pthread_create(microsoft_s+i,NULL,serf,value)!=0){
            perror("Greska pri kreiranju niti\n");
            return -1;
        }
    }

for(int i=0;i<N_HACKERS;i++){
        if(pthread_join(linux_h[i], NULL)!=0) return -1;
        if(pthread_join(microsoft_s[i], NULL)!=0) return -1;
    }

    sem_destroy(&hackerRed);
    sem_destroy(&serfsRed);
    return 0;
}
