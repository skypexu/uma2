all: main.c
	cc -D_GNU_SOURCE -O2 -g -o all main.c mtx.c rwlock.c smp.c sx.c systm.c task.c time.c uma_core.c -lpthread
clean:
	rm *.o all
