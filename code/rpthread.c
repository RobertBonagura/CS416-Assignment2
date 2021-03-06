// File:	rpthread.c

// List all group member's name:
// username of iLab:
// iLab Server:

#include "rpthread.h"

// INITAILIZE ALL YOUR VARIABLES HERE
// YOUR CODE HERE
rpthread_q* queue;
tcb* ctcb;
ucontext_t* cctx;
ucontext_t* sched_ctx;
int id_counter;
struct itimerval* timer;
struct sigaction* sa;

/* create a new thread */
int rpthread_create(rpthread_t * thread, pthread_attr_t * attr, 
                      void *(*function)(void*), void * arg) {
        if (timer != NULL){
                stoptimer();
        }
        // First, check if main thread has tcb initialized
        if (ctcb == NULL){
                init_ctcb(&ctcb);
        }
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
        // Setup context of newly created thread
        ucp->uc_link = NULL;
        ucp->uc_stack.ss_sp = stack;
        ucp->uc_stack.ss_size = STACK_SIZE;
        ucp->uc_stack.ss_flags = 0;
        makecontext(ucp, (void (*)(void)) function, 1, arg);
        
        // after everything is all set, push this thread into q
        setid(&thread);
        tcblock->id = thread;
        tcblock->status = READY;
        tcblock->ucp = ucp;
        tcblock->stack = stack;
        add(&tcblock, &queue);
        
        // swap to scheduler context 
        if (sched_ctx == NULL){
                init_schedctx(&sched_ctx);
        }
        swapcontext(cctx, sched_ctx);
        return 0;
};

/* give CPU possession to other user-level threads voluntarily */
int rpthread_yield() {
        stoptimer();
        if (cctx == NULL) {
                printf("Can't call rphtread_yield() on a single thread\n");
                printf("Call rpthread_create() first\n");
                return(-1);

        }
        // Change thread state from Running to Ready
	ctcb->status = READY;
        // Save context of this thread to its thread control block        
        if (getcontext(cctx) < 0){
                perror("Failed during getcontext");
                exit(1);
        }
        ctcb->ucp = cctx;
        // Setup scheduler context
        if (sched_ctx == NULL){
                init_schedctx(&sched_ctx);
        }
        swapcontext(cctx, sched_ctx);
        starttimer();
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
        /* First update context of ctcb so that it contains context
           of thread just before swap was called.                   */
        ctcb->status = READY;
        ctcb->ucp = cctx;
        add(&ctcb, &queue);
	// Every time when timer interrup happens, your thread library 
	// should be contexted switched from thread context to this 
	// schedule function

	// Invoke different actual scheduling algorithms
	// according to policy (STCF or MLFQ)

	// if (sched == STCF)
	//		sched_stcf();

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
        ctcb->status = RUNNING;
        cctx = ctcb->ucp;
        starttimer();
        setcontext(cctx);
}

/* Preemptive MLFQ scheduling algorithm */
static void sched_mlfq() {
	// Your own implementation of MLFQ
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE
}

// Feel free to add any other functions you need


/* Initialize tcb and context for main thread */
void init_ctcb(tcb** ctcb){
        *ctcb = malloc(sizeof(tcb));
        ucontext_t* cctx = malloc(sizeof(ucontext_t));
        void* cstack = malloc(sizeof(STACK_SIZE));
        if (*ctcb == NULL | cctx == NULL | cstack == NULL){
                perror("Failed allocation during init_ctcb");
                exit(1);
        }
        memset(*ctcb, 0, sizeof(tcb));
        memset(cctx, 0, sizeof(ucontext_t));
        memset(cstack, 0, sizeof(STACK_SIZE));

        if (getcontext(cctx) < 0){
                perror("Failed during getcontext");
                exit(1);
        }
        rpthread_t* cid;
        setid(&cid);
        (*ctcb)->id = cid;
        (*ctcb)->status = RUNNING;
        (*ctcb)->ucp = cctx;
        (*ctcb)->stack = cstack;
        add(ctcb, &queue);
}


/* Initialize context for scheduler function */
void init_schedctx(ucontext_t** sched_ctx){
        *sched_ctx = malloc(sizeof(ucontext_t));
        void* stack = malloc(sizeof(STACK_SIZE));
        if (*sched_ctx == NULL | stack == NULL ){
                perror("Failed allocation during makeschedctx");
                exit(1);
        }
        memset(*sched_ctx, 0, sizeof(ucontext_t));
        memset(stack, 0, sizeof(STACK_SIZE));
        if (getcontext(*sched_ctx) < 0){
                perror("Failed during getcontext(schedctx)");
                exit(1);
        }
        // Setup scheduler context
        (*sched_ctx)->uc_link = NULL;
        (*sched_ctx)->uc_stack.ss_sp = stack;
        (*sched_ctx)->uc_stack.ss_size = STACK_SIZE;
        (*sched_ctx)->uc_stack.ss_flags = 0;
        makecontext(*sched_ctx, (void *)&schedule, 0);
        return;
}

/* sets ID to rpthread_t type*/
int setid(rpthread_t** thread){
        *thread = malloc(sizeof(rpthread_t));
        if (*thread == NULL){
                perror("Failed allocation during setid");
                exit(1);
        }
        memset(*thread, 0, sizeof(rpthread_t));
        if (id_counter <= 0){
                id_counter = 0;
        }
        id_counter++;
        **thread = id_counter;
        return **thread;
}



/* queue APIs*/
static void init_q(rpthread_q** q){
        *q = malloc(sizeof(rpthread_q));
        rpthread_node* front = malloc(sizeof(rpthread_node));
        rpthread_node* next = malloc(sizeof(rpthread_node));
        tcb* thread = malloc(sizeof(tcb));
        if (*q == NULL | front == NULL || next == NULL || thread == NULL){
                perror("Could not allocate queue");
                exit(1);
        }
        memset(*q, 0, sizeof(rpthread_q));
        memset(front, 0, sizeof(rpthread_node));
        memset(next, 0, sizeof(rpthread_node));
        memset(thread, 0, sizeof(tcb));
       
        rpthread_q queue;
        **q = queue; 
        front->thread = thread;
        front->next = next;
        (*q)->front = front; 
        (*q)->rear = front;
        return;
}

static int add(tcb** tcblock, rpthread_q** q){
        if (*q == NULL){
                init_q(q);
        }
        rpthread_node* newtail = malloc(sizeof(rpthread_node));
        rpthread_node* newnext = malloc(sizeof(rpthread_node));
        if (newtail == NULL || newnext == NULL){
                perror("Could not allocate add API");
                exit(1);
        }
        memset(newtail, 0, sizeof(rpthread_node));
        memset(newnext, 0, sizeof(rpthread_node));
        newtail->next = newnext;
        newtail->thread = *tcblock;
        (*q)->rear->next = newtail;
        (*q)->rear = newtail;
        (*q)->size++;
        return 1;
        
}

/* Dequeue function for rpthread_queue */
static tcb* dque(rpthread_q** q){
        if (*q == NULL || (*q)->size == 0){
                perror("Removing from empty queue");
                exit(1);
        }
        rpthread_node* front = (*q)->front;
        tcb* tcblock = front->thread;
        free(front);
        (*q)->front = front->next;
        (*q)->size--;
        return tcblock;
}

/* Start global timer */
void starttimer(){
        if (sa == NULL){
                memset(sa, 0, sizeof(struct sigaction));
                sa->sa_handler = &schedule;
                sigaction(SIGPROF, sa, NULL);
        }
        if (timer == NULL){
                memset(timer, 0, sizeof(struct itimerval));
        }
        timer->it_interval.tv_usec = 0;
        timer->it_interval.tv_sec = 1;
        timer->it_value.tv_usec = 0;
        timer->it_value.tv_usec = 1;
        setitimer(ITIMER_PROF, timer, NULL);
        return;
}

/* Stop global timer */
void stoptimer(){
        timer->it_interval.tv_usec = 0;
        timer->it_interval.tv_sec = 0;
        timer->it_value.tv_usec = 0;
        timer->it_value.tv_usec = 0;
        setitimer(ITIMER_PROF, timer, NULL);
        return;
}




