#include "c.h"
#include "port/pg_threads.h"

#include <stdio.h>

static pg_tss_t tss_id;

int
my_thread_body(void *arg)
{
	pg_tss_set(tss_id, (void *) 32);
	return 42;
}

void
my_tss_destructor(void *value)
{
	printf("destructor! %p\n", value);
}


int main()
{
	pg_thrd_t thread;
	int result;

	if (pg_tss_create(&tss_id, my_tss_destructor) != pg_thrd_success)
	{
		printf("boo\n");
		return 1;
	}

	printf("hello\n");
	if (pg_thrd_create(&thread, my_thread_body, NULL) != pg_thrd_success)
	{
		perror("error creating thread\n");
		return 1;
	}
	printf("created\n");

	pg_thrd_join(thread, &result);
	printf("thread returned %d\n", result);

	return 0;
}
