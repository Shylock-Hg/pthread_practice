#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

static const int MAX_COND_COUNT = 30;

typedef struct cond {
	int count;
	pthread_cond_t  cond;
	pthread_mutex_t mutex;
} cond_t;

cond_t condition = {
	.count = 0,
	.cond = PTHREAD_COND_INITIALIZER,
	.mutex = PTHREAD_MUTEX_INITIALIZER,
};

void * thr_procedure0(void * argv __attribute__((unused))){

	pthread_t self = pthread_self();

	//< code block before waiting cond
	
	pthread_mutex_lock(&condition.mutex);
	while(condition.count < MAX_COND_COUNT){  //!< wait for count to MAX
		printf("Thread `%lu` wait count\n", self);
		pthread_cond_wait(&condition.cond, &condition.mutex);
	}
	pthread_mutex_unlock(&condition.mutex);

	//< code block after waiting cond
	
	pthread_exit(EXIT_SUCCESS);
}

void * thr_procedure1(void * argv __attribute__((unused))){

	pthread_t self = pthread_self();

	pthread_mutex_lock(&condition.mutex);

	condition.count = MAX_COND_COUNT;
	printf("Thread `%lu` change count\n", self);

	if(condition.count == MAX_COND_COUNT){
		pthread_mutex_unlock(&condition.mutex);
		pthread_cond_signal(&condition.cond);
	}else{
		pthread_mutex_unlock(&condition.mutex);
	}

	pthread_exit(EXIT_SUCCESS);
}

int main(int argc __attribute__((unused)),
		char * argv[] __attribute__((unused))){

	pthread_t thr[2];

	pthread_create(thr+0, NULL, thr_procedure0, NULL);
	pthread_create(thr+1, NULL, thr_procedure1, NULL);

	for(int i=0; i<sizeof(thr)/sizeof(thr[1]); i++){
		pthread_join(thr[i], NULL);
	}

	return EXIT_SUCCESS;
}

