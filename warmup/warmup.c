#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <ucontext.h>

#define STACK_SIZE SIGSTKSZ

ucontext_t* octx;
ucontext_t* nctx;
ucontext_t* cctx;
ucontext_t* _ctx;

void foo();
void bar();
void switchContext(int);
struct itimerval* gettimer();


void  foo(){ 
        nctx = octx;
        getcontext(cctx);
        while (1) {
                puts("Foo");	
        }
}

void bar(){
        nctx = octx;
        getcontext(cctx);
        while (1) {
                puts("Bar");
        }
}

void switchContext(int signum){
        puts("Context switch!");
        //sleep(1);
        octx = cctx;
        //nctx = octx;
        //octx = cctx_foo;
        swapcontext(cctx, nctx);
        printf("reached end of switch context\n");
        sleep(2);
}

struct itimerval* gettimer(){
        
        struct itimerval timer;
        timer.it_interval.tv_usec = 1110;
        timer.it_interval.tv_sec = 0;
        
        timer.it_value.tv_usec = 1110;
        timer.it_value.tv_sec = 0;

        struct itimerval* ptr = &timer;
        return ptr;
        //return timer;
}

int main(int argc, char **argv) {
	
        if (argc != 1) {
                printf(": USAGE Program Name and no Arguments expected\n");
                exit(1);
        }  

        ucontext_t cctx_foo, cctx_bar; 
    
        if (getcontext(&cctx_foo) < 0){ 
                perror("getcontext");
                exit(1);
        }   
        
        if (getcontext(&cctx_bar) < 0){
                perror("getcontext_bar:");
                exit(1);
        }
        
        void *stack1=malloc(STACK_SIZE);
        void *stack2=malloc(STACK_SIZE);
        if (stack1 == NULL || stack2 == NULL){
                perror("Failed to allocate stack");
                exit(1);
        } 

        /* Setup context that we are going to use */
        cctx_foo.uc_link=NULL;
        cctx_foo.uc_stack.ss_sp=stack1;
        cctx_foo.uc_stack.ss_size=STACK_SIZE;
        cctx_foo.uc_stack.ss_flags=0;
        
        cctx_bar.uc_link = NULL;
        cctx_bar.uc_stack.ss_sp=stack2;
        cctx_bar.uc_stack.ss_size=STACK_SIZE;
        cctx_bar.uc_stack.ss_flags=0;

        makecontext(&cctx_foo, (void *)&foo, 0);
        makecontext(&cctx_bar, (void *)&bar, 0);
        puts("Successfully modified context for both foo and bar");
        
        cctx = &cctx_foo;
        octx = &cctx_bar;
        
        // Use sigaction to register signal handler
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = &switchContext;
        sigaction(SIGPROF, &sa, NULL);
        
        // Start the timer
        struct itimerval* timer_ptr = gettimer();
        setitimer(ITIMER_PROF, timer_ptr, NULL);

        foo();

        return 0;  
        
}

