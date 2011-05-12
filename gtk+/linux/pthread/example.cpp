#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* thread(void* p)
{
	for(int i = 0; i < 3; i++)
	{
		printf("This is a pthread.\n");
		sleep(1);
	}
	return NULL;
}

int main(void)
{
	pthread_t		id;
	int			i;
	int			ret;

	ret = pthread_create(&id, NULL, thread, NULL);	
	if(0 != ret)
	{
		printf("Create pthread error\n");
		exit(1);
	}

	for(int i = 0; i < 3; i++)
	{
		printf("This is the main process.\n");
		sleep(1);
	}

	pthread_join(id, NULL);
	return 0;
}
