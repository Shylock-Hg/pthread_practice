#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define THREAD_NUM 10

typedef struct thread_data {
	int tid;
	double stuff;
} thread_data_t;

void * thread_procedure(void * argv){
	assert(argv);

	thread_data_t * data = (thread_data_t*)argv;

	printf("Runing thread `%d`!\n", data->tid);

	pthread_exit(0);
}

int main(int argc __attribute__((unused)),
		char * argv[] __attribute__((unused))){

	pthread_t thread[THREAD_NUM];
	int i,rc;
	thread_data_t thread_data[THREAD_NUM];

	for(i=0; i<THREAD_NUM; i++){
		thread_data[i].tid = i;
		rc = pthread_create(thread+i, NULL, thread_procedure, thread_data+i);
		if(rc){
			fprintf(stderr, "Thread `%d` create failed with rc `%d`!\n", i, rc);
			return EXIT_FAILURE;
		}
	}

	for(i=0; i<THREAD_NUM; i++){
		pthread_join(thread[i], NULL);
	}

	return EXIT_SUCCESS;
}

