#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

pthread_t Students[3];
pthread_t TA;

int ChairsCount = 0;
int CurrentIndex = 0;

sem_t TA_Sleep;
sem_t Student_Sem;
sem_t ChairsSem[3];
pthread_mutex_t ChairAccess;

void *TA_Activity();
void *Student_Activity(void *threadID);

int main(int argc, char* argv[])
{
	int number_of_students = 3;
	int id;
	srand(time(NULL));


	sem_init(&TA_Sleep, 0, 0);
	sem_init(&Student_Sem, 0, 0);
	for(id = 0; id < 3; ++id)
		sem_init(&ChairsSem[id], 0, 0);

	pthread_mutex_init(&ChairAccess, NULL);


	pthread_create(&TA, NULL, TA_Activity, NULL);
	for(id = 0; id < number_of_students; id++)
		pthread_create(&Students[id], NULL, Student_Activity,(void*) (long)id);


	pthread_join(TA, NULL);
	for(id = 0; id < number_of_students; id++)
		pthread_join(Students[id], NULL);

	return 0;
}

void *TA_Activity()
{
	while(1)
	{
		sem_wait(&TA_Sleep);
		printf("***********TA has been awakened by a student.*************\n");

		while(1)
		{

			pthread_mutex_lock(&ChairAccess);
			if(ChairsCount == 0)
			{

				pthread_mutex_unlock(&ChairAccess);
				break;
			}

			sem_post(&ChairsSem[CurrentIndex]);
			ChairsCount--;
			printf("Student left his/her chair. Remaining Chairs %d\n", 3 - ChairsCount);
			CurrentIndex = (CurrentIndex + 1) % 3;
			pthread_mutex_unlock(&ChairAccess);


			printf("\t TA is currently helping the student.\n");
			sleep(5);
			sem_post(&Student_Sem);
			usleep(1000);
		}
	}
}

void *Student_Activity(void *threadID)
{
	int ProgrammingTime;

	while(1)
	{
		printf("Student %ld is doing programming assignment.\n", 1+ (long)threadID);
		ProgrammingTime = rand() % 10 + 1;
		sleep(ProgrammingTime);

		printf("Student %ld needs help from the TA\n", 1+(long)threadID);

		pthread_mutex_lock(&ChairAccess);
		int count = ChairsCount;
		pthread_mutex_unlock(&ChairAccess);

		if(count < 3)
		{
			if(count == 0)
				sem_post(&TA_Sleep);
			else
				printf("Student %ld sat on a chair waiting for the TA to finish. \n", 1+(long)threadID);

			pthread_mutex_lock(&ChairAccess);
			int index = (CurrentIndex + ChairsCount) % 3;
			ChairsCount++;
			printf("Student sat on chair.Chairs Remaining: %d\n", 3 - ChairsCount);
			pthread_mutex_unlock(&ChairAccess);

			sem_wait(&ChairsSem[index]);
			printf("\t Student %ld is getting help from the TA. \n", 1+(long)threadID);
			sem_wait(&Student_Sem);
			printf("Student %ld left TA room.\n",(long)threadID);
		}
		else
			printf("Student %ld will return at another time. \n", 1+(long)threadID);

	}
}
