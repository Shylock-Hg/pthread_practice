#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define THREAD_NUM 31477

typedef struct thread_data {
	int tid;
	double stuff;
} thread_data_t;

/* shared data */
typedef struct shared_data {
	double shared;
	pthread_mutex_t mutex;
} shared_data_t;

shared_data_t sh_data;

void * thread_procedure(void * argv){
	assert(argv);

	thread_data_t * data = (thread_data_t*)argv;

	printf("Runing thread `%d`!\n", data->tid);

	//< access to shared data
	pthread_mutex_lock(&sh_data.mutex);
	sh_data.shared += data->stuff;
	printf("Shared data value is %f!\n", sh_data.shared);
	pthread_mutex_unlock(&sh_data.mutex);

	pthread_exit(EXIT_SUCCESS);
}

int main(int argc __attribute__((unused)),
		char * argv[] __attribute__((unused))){

	pthread_t thread[THREAD_NUM];
	int i,rc;
	thread_data_t thread_data[THREAD_NUM];

	//< initialize shared data
	sh_data.shared = 0.0;
	pthread_mutex_init(&sh_data.mutex, NULL);

	for(i=0; i<THREAD_NUM; i++){
		thread_data[i].tid = i;
		thread_data[i].stuff = i;
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

