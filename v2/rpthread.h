// File:	rpthread_t.h

// List all group member's name:
// username of iLab:
// iLab Server:

#ifndef RTHREAD_T_H
#define RTHREAD_T_H

#define _GNU_SOURCE

/* To use Linux pthread Library in Benchmark, you have to comment the USE_RTHREAD macro */
#define USE_RTHREAD 1

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ucontext.h>

#define STACK_SIZE SIGSTKSZ

typedef enum thread_status {RUNNING, READY, WAITING} thread_status;

typedef uint rpthread_t;

typedef struct threadControlBlock {
	/* add important states in a thread control block */
	rpthread_t id;         // thread Id
	thread_status status;   // thread status
	ucontext_t ctx;      // thread context
	void* stack;            // thread stack
	// thread priority
	// And more ...

	// YOUR CODE HERE
} tcb; 


/* mutex struct definition */
typedef struct rpthread_mutex_t {
	/* add something here */

	// YOUR CODE HERE
} rpthread_mutex_t;

/* define your data structures here: */
// Feel free to add your own auxiliary data structures (linked list or queue etc...)

/* Underlying linked list inside queue */
typedef struct rpthread_node {
        struct rpthread_node* next;
        tcb block;
} rpthread_node;

/* Queue of nodes holding tcb's */
typedef struct rpthread_queue {
        rpthread_node front;
        rpthread_node rear;
        int size;
} rpthread_q;


// YOUR CODE HERE

/* Function Declarations: */

/* create a new thread */
int rpthread_create(rpthread_t * thread, pthread_attr_t * attr, void
    *(*function)(void*), void * arg);

/* give CPU pocession to other user level threads voluntarily */
int rpthread_yield();

/* terminate a thread */
void rpthread_exit(void *value_ptr);

/* wait for thread termination */
int rpthread_join(rpthread_t thread, void **value_ptr);

/* initial the mutex lock */
int rpthread_mutex_init(rpthread_mutex_t *mutex, const pthread_mutexattr_t
    *mutexattr);

/* aquire the mutex lock */
int rpthread_mutex_lock(rpthread_mutex_t *mutex);

/* release the mutex lock */
int rpthread_mutex_unlock(rpthread_mutex_t *mutex);

/* destroy the mutex */
int rpthread_mutex_destroy(rpthread_mutex_t *mutex);

/* Scheduler functions */
static void scheduler();
static void sched_stcf();
static void sched_mlfq();

/* Initialize tcb and context for main thread. */
void init_ctcb(tcb* ctcb);

/* Initialize context for schedule function. */
void init_schedctx(ucontext_t* schedctx);

/* sets ID to rpthread_t type*/
void setid(rpthread_t* thread);

/* Queue functions. */
static int add(tcb* tcblock, rpthread_q* q);
static tcb dque(rpthread_q* q);

/* Timer functions. Start timer creates sighandler. */
void starttimer();
void stoptimer();

#ifdef USE_RTHREAD
#define pthread_t rpthread_t
#define pthread_mutex_t rpthread_mutex_t
#define pthread_create rpthread_create
#define pthread_exit rpthread_exit
#define pthread_join rpthread_join
#define pthread_mutex_init rpthread_mutex_init
#define pthread_mutex_lock rpthread_mutex_lock
#define pthread_mutex_unlock rpthread_mutex_unlock
#define pthread_mutex_destroy rpthread_mutex_destroy
#endif

#endif
