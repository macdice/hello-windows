#include "c.h"
#include "port/pg_threads.h"

#include <stdio.h>

int
my_thread_body(void *arg)
{
	return 42;
}

int main()
{
	pg_thrd_t thread;
	int result;

	if (pg_thrd_create(&thread, my_thread_body, NULL) != pg_thrd_success)
	{
		perror("error creating thread\n");
		return 1;
	}

	pg_thrd_join(thread, &result);
	printf("thread return %d\n", result);

	return 0;
}
