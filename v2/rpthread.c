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
ucontext_t schedctx;
int id_counter;
struct itimerval timer;
struct sigaction sa;

/* create a new thread */
int rpthread_create(rpthread_t * thread, pthread_attr_t * attr, 
                      void *(*function)(void*), void * arg) {
        if (timer.it_interval.tv_sec != 0 || timer.it_value.tv_usec != 0){
			stoptimer();
		}
       
	    // Create Thread Control Block for calling thread
		if (ctcb.id == 0 ){
			init_ctcb(&ctcb);
		}

		// Allocate space for stack for this tcb
	    void* stack = malloc(sizeof(STACK_SIZE));
	    if (stack == NULL){
		    perror("Failed stack allocation in rpthread_create");
		    exit(1);
	    }
	    memset(stack, 0, sizeof(STACK_SIZE));
        
		// Create and initialize the context of this new thread
	    ucontext_t ctx; 
	    if (getcontext(&ctx) < 0){
		    perror("Failed during getcontext in rpthread_create");
		    exit(1);
	    }
	    ctx.uc_link = NULL;
	    ctx.uc_stack.ss_sp = stack;
	    ctx.uc_stack.ss_size = STACK_SIZE;
	    ctx.uc_stack.ss_flags = 0;
	    makecontext(&ctx, (void (*)(void)) function, 1, arg);
       
        // after everything is all set, push this thread into q
		tcb tcblock;
		if (thread == NULL){
			thread = malloc(sizeof(rpthread_t));
			if (thread == NULL){
				perror("Failed allocation for thread_t in setid");
				exit(1);
			}
		}
		memset(thread, 0, sizeof(rpthread_t));
		setid(thread);
		tcblock.id = *thread;
		tcblock.status = READY;
		tcblock.ctx = ctx;
		tcblock.stack = stack;
		add(&tcblock, &queue);

		// swap to scheduler context
        if (schedctx.uc_stack.ss_sp == NULL){
                init_schedctx(&schedctx);
        }
		/**
		// Allocate space for stack for this tcb
	    void* cstack = malloc(sizeof(STACK_SIZE));
	    if (cstack == NULL){
		    perror("Failed stack allocation in rpthread_create");
		    exit(1);
	    }
	    memset(cstack, 0, sizeof(STACK_SIZE));
        
		// Create and initialize the context of this new thread
	    if (getcontext(&cctx) < 0){
		    perror("Failed during getcontext in rpthread_create");
		    exit(1);
	    }
	    cctx.uc_link = NULL;
	    cctx.uc_stack.ss_sp = cstack;
	    cctx.uc_stack.ss_size = STACK_SIZE;
	    cctx.uc_stack.ss_flags = 0;
	*/
	    swapcontext(&cctx, &schedctx);
        return 1;
}

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
	stoptimer();
	ctcb.status = READY;
	ctcb.ctx = cctx;
	add(&ctcb, &queue);
	// Invoke different actual scheduling algorithms
	// according to policy (STCF or MLFQ)

	// if (sched == STCF)
	//		sched_stcf();
	// else if (sched == MLFQ)
	// 		sched_mlfq();

	// YOUR CODE HERE
	sched_stcf();

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
	ctcb = dque(&queue);
	ctcb.status = RUNNING;
	cctx = ctcb.ctx;
	starttimer();
	setcontext(&cctx);
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
}

/* Initialize context for scheduler function */
void init_schedctx(ucontext_t* schedctx){
        void* stack = malloc(sizeof(STACK_SIZE));
        if (stack == NULL ){
                perror("Failed allocation during makeschedctx");
                exit(1);
        }
        memset(stack, 0, sizeof(STACK_SIZE));
        if (getcontext(schedctx) < 0){
                perror("Failed during getcontext(schedctx)");
                exit(1);
        }
        // Setup scheduler context
        schedctx->uc_link = NULL;
        schedctx->uc_stack.ss_sp = stack;
        schedctx->uc_stack.ss_size = STACK_SIZE;
        schedctx->uc_stack.ss_flags = 0;
        makecontext(schedctx, (void *)&schedule, 0);
        return;
}

/* sets ID to rpthread_t type*/
void setid(rpthread_t* thread){
		if (thread == NULL){
			thread = malloc(sizeof(rpthread_t));
			if (thread == NULL){
				perror("Failed allocation for thread_t in setid");
				exit(1);
			}
		}
		id_counter++;
        *thread = (rpthread_t)id_counter;
        return;
}

/* QUEUE APIs*/

/* Add tcb into the queue*/
static int add(tcb* tcblock, rpthread_q* q){
        
		rpthread_node* newtail = malloc(sizeof(rpthread_node));
		if (newtail == NULL) {
			perror("Failed allocation of new node in add()");
			exit(1);
		}
		memset(newtail, 0, sizeof(rpthread_node));

		newtail->next = NULL;
		newtail->block = *tcblock;

		q->rear.next = newtail;
		q->rear = *newtail;
		if (q->size == 0){
			q->front = q->rear;
		}
		else if (q->size == 1){
			q->front.next = &(q->rear);
		}
		q->size++;
		//free(newtail);
        return 1;
        
}

/* Dequeue function for rpthread_queue */
static tcb dque(rpthread_q* q){
        if (q->size == 0){
                perror("Removing from empty queue");
                exit(1);
        }
        rpthread_node front = q->front;
		tcb tcblock = front.block;
        q->front = *(front.next);
		q->size--;
        return tcblock;
}

/* Start global timer */
void starttimer(){
		if (sa.sa_handler == NULL){
			sa.sa_handler = &schedule;
			sigaction(SIGPROF, &sa, NULL);
		}
        timer.it_interval.tv_usec = 0;
        timer.it_interval.tv_sec = 1;
        timer.it_value.tv_usec = 0;
        timer.it_value.tv_usec = 1;
        setitimer(ITIMER_PROF, &timer, NULL);
        return;
}

/* Stop global timer */
void stoptimer(){
        timer.it_interval.tv_usec = 0;
        timer.it_interval.tv_sec = 0;
        timer.it_value.tv_usec = 0;
        timer.it_value.tv_usec = 0;
        setitimer(ITIMER_PROF, &timer, NULL);
        return;
}
