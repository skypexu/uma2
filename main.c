#include "uma/uma.h"
#include <pthread.h>
#include <stdio.h>

static pthread_once_t uma_start_once = PTHREAD_ONCE_INIT;
static uma_zone_t iobuf_zone;
static void do_start_uma()
{
    uma_default_startup();
    iobuf_zone = uma_zcreate("iobuf", 8192,
         NULL, NULL, NULL, NULL, UMA_ALIGN_PTR, UMA_ZONE_LARGE_KEG);
}

static inline void start_uma()
{
    pthread_once(&uma_start_once, do_start_uma);
}

void *thr(void *arg)
{
#define N 1000
	void *b[N];
	int i, j;

	printf("start thread %p\n", pthread_self());
	for (j = 0; j < 1000000; ++j) {
		for (i = 0; i < N; ++i)  {
			b[i] = uma_zalloc(iobuf_zone, 0);
		}
		for (i = 0; i < N; ++i)  {
			uma_zfree(iobuf_zone, b[i]);
		}
	}
	printf("exit thread %p\n", pthread_self());
}

int main()
{
#define TD 100
	pthread_t td[TD];

	start_uma();

	for (int i = 0; i < TD; ++i)
		pthread_create(&td[i], NULL, thr, NULL);
	for (int i = 0; i < TD; ++i)
		pthread_join(td[i], NULL);
}


