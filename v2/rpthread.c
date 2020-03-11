// File:	rpthread.c

// List all group member's name:
// username of iLab:
// iLab Server:

#include "rpthread.h"

// INITAILIZE ALL YOUR VARIABLES HERE
// YOUR CODE HERE
rpthread_q queue;
tcb ctcb;
ucontext_t cctx;
int id_counter;
struct itimerval timer;


/* create a new thread */
int rpthread_create(rpthread_t * thread, pthread_attr_t * attr, 
                      void *(*function)(void*), void * arg) {
        if (timer.it_interval.tv_sec != 0 || timer.it_value.tv_usec != 0){
			stopTimer();
		}

		if (ctcb.id == 0 || ctcb.id == NULL){
			init_ctcb(&ctcb);
		}



       // create Thread Control Block

       // create and initialize the context of this thread
       // allocate space of stack for this thread to run
       // after everything is all set, push this thread int
       // YOUR CODE HERE
	
    return 0;
};

/* give CPU possession to other user-level threads voluntarily */
int rpthread_yield() {
	
	// change thread state from Running to Ready
	// save context of this thread to its thread control block
	// wwitch from thread context to scheduler context

	// YOUR CODE HERE
	return 0;
};

/* terminate a thread */
void rpthread_exit(void *value_ptr) {
	// Deallocated any dynamic memory created when starting this thread

	// YOUR CODE HERE
};


/* Wait for thread termination */
int rpthread_join(rpthread_t thread, void **value_ptr) {
	
	// wait for a specific thread to terminate
	// de-allocate any dynamic memory created by the joining thread
  
	// YOUR CODE HERE
	return 0;
};

/* initialize the mutex lock */
int rpthread_mutex_init(rpthread_mutex_t *mutex, 
                          const pthread_mutexattr_t *mutexattr) {
	//initialize data structures for this mutex

	// YOUR CODE HERE
	return 0;
};

/* aquire the mutex lock */
int rpthread_mutex_lock(rpthread_mutex_t *mutex) {
        // use the built-in test-and-set atomic function to test the mutex
        // if the mutex is acquired successfully, enter the critical section
        // if acquiring mutex fails, push current thread into block list and //  
        // context switch to the scheduler thread

        // YOUR CODE HERE
        return 0;
};

/* release the mutex lock */
int rpthread_mutex_unlock(rpthread_mutex_t *mutex) {
	// Release mutex and make it available again. 
	// Put threads in block list to run queue 
	// so that they could compete for mutex later.

	// YOUR CODE HERE
	return 0;
};


/* destroy the mutex */
int rpthread_mutex_destroy(rpthread_mutex_t *mutex) {
	// Deallocate dynamic memory created in rpthread_mutex_init

	return 0;
};

/* scheduler */
static void schedule() {
	// Every time when timer interrup happens, your thread library 
	// should be contexted switched from thread context to this 
	// schedule function

	// Invoke different actual scheduling algorithms
	// according to policy (STCF or MLFQ)

	// if (sched == STCF)
	//		sched_stcf();
	// else if (sched == MLFQ)
	// 		sched_mlfq();

	// YOUR CODE HERE

// schedule policy
#ifndef MLFQ
	// Choose STCF
#else 
	// Choose MLFQ
#endif

}

/* Preemptive SJF (STCF) scheduling algorithm */
static void sched_stcf() {
	// Your own implementation of STCF
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE
}

/* Preemptive MLFQ scheduling algorithm */
static void sched_mlfq() {
	// Your own implementation of MLFQ
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE
}

// Feel free to add any other functions you need

// YOUR CODE HERE

/* Initialize tcb and context for main thread. */
void init_ctcb(tcb* ctcb){
        void* cstack = malloc(sizeof(STACK_SIZE));
        if (cstack == NULL){
                perror("Failed stack allocation during init_ctcb");
                exit(1);
        }
        memset(cstack, 0, sizeof(STACK_SIZE));

        if (getcontext(&cctx) < 0){
                perror("Failed getcontext during init_ctcb");
                exit(1);
        }
        rpthread_t cid;
        setid(&cid);
        ctcb->id = cid;
        ctcb->status = RUNNING;
        ctcb->ctx = cctx;
        ctcb->stack = cstack;
        add(ctcb, &queue);
}

/* sets ID to rpthread_t type*/
void setid(rpthread_t* thread){
        id_counter++;
        *thread = (rpthread_t)id_counter;
        return;
}

/* QUEUE APIs*/

/* Add tcb into the queue*/
static int add(tcb* tcblock, rpthread_q* q){
        rpthread_node newtail;
        newtail.block = *tcblock;
        q->rear.next = &newtail;
        q->rear = newtail;
        q->size++;
        return 1;
        
}

/* Stop global timer */
void stoptimer(){
        timer.it_interval.tv_usec = 0;
        timer.it_interval.tv_sec = 0;
        timer.it_value.tv_usec = 0;
        timer.it_value.tv_usec = 0;
        setitimer(ITIMER_PROF, &timer, NULL);
        return;

