#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>


pthread_mutex_t wrt,mutex;

int a=10,readcount=0;


void *reader(void * argu){

long int number;
number=(long int ) argu;
	pthread_mutex_lock(&mutex);
readcount++;
pthread_mutex_unlock(&mutex);

if(readcount==1)
	pthread_mutex_lock(&wrt);
	
	printf("\n Reader %ld is in  critical section",number);
	printf("\n Reader %ld is reading data %d",number,a);
	
	sleep(1);
	pthread_mutex_lock(&mutex);
	readcount--;
	pthread_mutex_lock(&mutex);
	
	if(readcount==0)
	pthread_mutex_unlock(&wrt);
	
	printf("\nReader %ld is in the left  critical section",number);




}
void * writers(void *argu)
{
	long int number;
number=(long int ) argu;

	pthread_mutex_lock(&wrt);
	printf("\nWriters %ld is in  critical section",number);
	printf("\n Writers %ld have written data as %d",number,++a);
	sleep(1);
	
	pthread_mutex_unlock(&wrt);
	printf("\nReader %ld is in left critical section",number);
		
}

int main()
{
pthread_t r [20],w[10];
long int i,j;
int nord,nowrt;
pthread_mutex_init(&wrt,NULL);
pthread_mutex_init(&mutex,NULL);

printf("Enter the number reader and writers");

scanf("%d %d",&nord,&nowrt);



for(i=0;i<nord;i++)
pthread_create(&r[i],NULL,reader,(void *)i);
for(j=0;j<nowrt;j++)
pthread_create(&w[j],NULL,writers,(void *)j);



for(i=0;i<nord;i++)
pthread_join(r[i],NULL);
for(j=0;j<nowrt;j++)
pthread_join(w[j],NULL);
return 0;
}

