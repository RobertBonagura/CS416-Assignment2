// File:	rpthread.c

// List all group member's name:
// username of iLab:
// iLab Server:

#include "rpthread.h"

// INITAILIZE ALL YOUR VARIABLES HERE
// YOUR CODE HERE
rpthread_q* queue;
int id;

/* create a new thread */
int rpthread_create(rpthread_t * thread, pthread_attr_t * attr, 
                      void *(*function)(void*), void * arg) {
        // Create Thread Control Block
        // Create and initalize the context of this thread
        // Allocate space of stack for this thread to run
        tcb* tcblock = malloc(sizeof(tcb));
        ucontext_t* ucp = malloc(sizeof(ucontext_t));
        void* stack = malloc(sizeof(STACK_SIZE));
        if (tcblock == NULL || ucp == NULL || stack == NULL){
                perror("Failed allocation during pthread_create");
                exit(1);
        }
        memset(tcblock, 0, sizeof(tcb));
        memset(ucp, 0, sizeof(ucontext_t));
        memset(stack, 0, sizeof(STACK_SIZE));

        if (getcontext(ucp) < 0){
                perror("Failed during getcontext");
                exit(1);
        }
        // Setup context
        ucp->uc_link = NULL;
        ucp->uc_stack.ss_sp = stack;
        ucp->uc_stack.ss_size = STACK_SIZE;
        ucp->uc_stack.ss_flags = 0;
        makecontext(ucp, (void (*)(void)) function, 1, arg);
        
        // after everything is all set, push this thread into q
        setid(thread);
        tcblock->id = thread;
        tcblock->status = READY;
        tcblock->ucp = ucp;
        tcblock->stack = stack;
        add(thread, queue);	

        return 0;
};

/* give CPU possession to other user-level threads voluntarily */
int rpthread_yield() {
	
        // Change thread state from Running to Ready
	// Save context of this thread to its thread control block
	// switch from thread context to scheduler context

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
	
	// Wait for a specific thread to terminate
	// De-allocate any dynamic memory created by the joining thread
  
	// YOUR CODE HERE
	return 0;
};

/* initialize the mutex lock */
int rpthread_mutex_init(rpthread_mutex_t *mutex, 
                          const pthread_mutexattr_t *mutexattr) {
	//Initialize data structures for this mutex

	// YOUR CODE HERE
	return 0;
};

/* aquire the mutex lock */
int rpthread_mutex_lock(rpthread_mutex_t *mutex) {
        // use the built-in test-and-set atomic function to test the mutex
        // When the mutex is acquired successfully, enter the critical section
        // If acquiring mutex fails, push current thread into block list and 
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


/* sets ID to rpthread_t type*/
int setid(rpthread_t* thread){
        thread = malloc(sizeof(rpthread_t));
        if (thread == NULL){
                perror("Could not allocate pthread in setid");
                exit(1);
        }
        memset(thread, 0, sizeof(rpthread_t));
        *(thread) = (rpthread_t)id;
        id++;
}

/* queue APIs*/
static void init_q(rpthread_q* q){
        q = malloc(sizeof(rpthread_q));
        rpthread_node* front = malloc(sizeof(rpthread_node));
        rpthread_t* thread = malloc(sizeof(rpthread_t));
        if (q == NULL || front == NULL || thread == NULL){
                perror("Could not allocate queue");
                exit(1);
        }
        memset(q, 0, sizeof(rpthread_q));
        memset(front, 0, sizeof(rpthread_node));
        memset(thread, 0, sizeof(rpthread_t));
        rpthread_node* back = front;
        return;
}

static int add(rpthread_t* thread, rpthread_q* q){
        if (q == NULL){
                init_q(q);
        }
        rpthread_node* newtail = malloc(sizeof(rpthread_node));
        if (newtail == NULL){
                perror("Could not allocate add API");
                exit(1);
        }
        memset(newtail, 0, sizeof(rpthread_node));
        q->rear->next = newtail;
        newtail->thread = thread;
        q->rear = newtail;
        q->size++;
        return 1;
        
}

/* Dequeue function for rpthread_queue */
static rpthread_t* dque(rpthread_q* q){
        
        if (q == NULL || q->size == 0){
                int err = -1;
                rpthread_t* thread_err = &err;
                return thread_err;
        }
        rpthread_node* front = queue->front;
        rpthread_t* thread = front->thread;
        
        queue->front = front->next;
        queue->size--;
        free(front);
        return thread;
}
