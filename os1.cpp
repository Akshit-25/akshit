#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>

#include<semaphore.h>
 
#define N 5
 
time_t end_time;/*end time*/
sem_t mutex,Students,TA;/*Three semaphors*/
int count=0;/*The number of Students waiting for haircuts*/
 
void TA(void *arg);
void customer(void *arg);
 
int main(int argc,char *argv[])
{
 pthread_t id1,id2;
 int stus=0;
 end_time=time(NULL)+20;/*TA Shop Hours is 20s*/
 
 /*Semaphore initialization*/
 sem_init(&mutex,0,1);
 sem_init(&Students,0,0);
 sem_init(&TA,0,1);
 
 /*TA_thread initialization*/
 stus=pthread_create(&id1,NULL,(void *)TA,NULL);
 if(stus!=0)
  perror("create TA is failure!\n");
 /*Customer_thread initialization*/
 stus=pthread_create(&id2,NULL,(void *)customer,NULL);
 if(stus!=0)
  perror("create Students is failure!\n");
 
 /*Customer_thread first blocked*/
 pthread_join(id2,NULL);
 pthread_join(id1,NULL);
 
 exit(0);
}
 
void TA(void *arg)/*TA Process*/
{
 while(time(NULL)<end_time || count>0)
 {
  sem_wait(&Students);/*P(Students)*/            
  sem_wait(&mutex);/*P(mutex)*/
  count--;
  printf("TA:help the student,count is:%d.\n",count);
  sem_post(&mutex);/*V(mutex)*/
  sem_post(&TA);/*V(TA)*/
  sleep(3);       
 }
}
 
void customer(void *arg)/*Students Process*/
{
 while(time(NULL)<end_time)
 {
  sem_wait(&mutex);/*P(mutex)*/
  if(count<N)
  {
   count++;
   printf("students:add count,count is:%d\n",count);
   sem_post(&mutex);/*V(mutex)*/
   sem_post(&customer);/*V(Students)*/
   sem_wait(&TA);/*P(TA)*/
  }
  else
   /*V(mutex)*/
   /*If the number is full of Students,just put the mutex lock let go*/
   sem_post(&mutex);
  sleep(1);
 }
}	

