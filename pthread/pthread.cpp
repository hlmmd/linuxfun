#include <stdio.h>

#include <pthread.h>

__attribute__((constructor)) void foo()  
{  
    printf("hello world\n");  
}  

class printclass{
public:
    printclass()
    {
        printf("hello world\n");
    }

};

printclass p;

int peach = 99999;

#define PICK_A 2
#define PICK_B 3

pthread_mutex_t lock;

void *mythread1(void *count1)
{

    do
    {
        pthread_mutex_lock(&lock);
        if (peach < PICK_A)
            break;
        peach -= PICK_A;
        *(int *)count1 += PICK_A;
        pthread_mutex_unlock(&lock);
    } while (1);
    pthread_mutex_unlock(&lock);
}

void *mythread2(void *count1)
{
    do
    {
        pthread_mutex_lock(&lock);
        if (peach < PICK_B)
            break;
        peach -= PICK_B;
        *(int *)count1 += PICK_B;
        pthread_mutex_unlock(&lock);
    } while (1);
    pthread_mutex_unlock(&lock);
    pthread_exit(count1);
}

int main()
{

    pthread_t pt1, pt2;

    int count1 = 0, count2 = 0;
    int ret = pthread_create(&pt1, NULL, mythread1, &count1);
    pthread_create(&pt2, NULL, mythread2, &count2);

    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);

    printf("%d %d %d\n", count1, count2, peach);

    return 0;
}