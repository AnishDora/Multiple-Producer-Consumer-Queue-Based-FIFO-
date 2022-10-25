#include<iostream>
#include<pthread.h>
#include<cstdlib>
#include<unistd.h>
#include<time.h>
#include<vector>
#include<fstream>
using namespace std;
int *queue;
int length=0;
int add=0;
int consume=0;
pthread_mutex_t mut;
pthread_cond_t condEmpty;
pthread_cond_t condFull;
void *producer(void *ptr1)
{
    int size=*(int *)ptr1;
    while(1)
    {   
        pthread_mutex_lock(&mut);
        while (length>size-1) 
        {cout<<"Queue is full! Waiting for Consumers"<<endl;
        pthread_cond_wait(&condFull, &mut);}
        queue[add] = rand()%100;
        cout<<"Producer:"<<pthread_self()<<" "<<"producing->"<<queue[add]<<endl;
        add = (add + 1) % size;
        length++;
        pthread_mutex_unlock(&mut);
        pthread_cond_broadcast(&condEmpty);
    }
    pthread_exit(NULL);
}
void *consumer(void *ptr1)
{
    int size=*(int *)ptr1;
    while(1)
    {
        pthread_mutex_lock(&mut);
        while (length == 0) {
        cout<<"Queue is empty! Waiting for Producers"<<endl;
        pthread_cond_wait(&condEmpty, &mut);
        }
        int b = queue[consume];
        consume = (consume + 1) % size;
        length--;
        cout<<"Consumers:"<<pthread_self()<<" "<<"consuming->"<<b<<endl;
        pthread_mutex_unlock(&mut);
        pthread_cond_broadcast(&condFull);
    }
    pthread_exit(NULL);
}
int main(int n, char *argv[])
{
    int sizequeue=0;
    int producers=0;
    int consumers=0;
    int optional=0;
    vector<int> v;
    int num;
    for(std::string line; std::getline(std::cin,line);)
    {   
        string temp = ""; 
        for (int i = 0; i < line.size(); ++i) {
        if ((line[i] >= '0' && line[i] <= '9') ) {
            temp = temp + line[i];
        }
        }
        if(temp=="")
        {   
            continue;
        }
        else{
        num=stoi(temp);
        v.push_back(num);
    }
    }
    sizequeue=v[0];
    producers=v[1];
    consumers=v[2];
    if(sizequeue<=0)
    {
cout<<"Invalid size of Queue"<<endl;
        return 0;
    }
    if(producers==0)
    {
        cout<<"Queue will be empty! Invalid Input"<<endl;
        return 0;
    }
    if(consumers<=0)
    {
        cout<<"Invalid Input No Consumers"<<endl;
        return 0;
    }
    queue=new int[sizequeue];
    int count=producers+consumers;
    pthread_t tr[count];
    int tid;
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&condEmpty, NULL);
    pthread_cond_init(&condFull, NULL);
    for(tid=1;tid<producers;tid++){
        pthread_create(&tr[tid],NULL,producer,&sizequeue);
    }
    int x=1;
    for(tid=producers;tid<count;tid++)
    {
        pthread_create(&tr[tid],NULL,consumer,&sizequeue);
    }
    for(int i = 1; i <= count; i++)
    {
        pthread_join(tr[i], NULL);
    }
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&condEmpty);
    pthread_cond_destroy(&condFull);
    pthread_exit(0);
}