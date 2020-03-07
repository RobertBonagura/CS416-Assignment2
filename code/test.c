#include <stdio.h>
#include <unistd.h>
//#include <pthread.h>
#include "rpthread.h"

/* A scratch program template on which to call and
 * test rpthread library functions as you implement
 * them.
 *
 * You can modify and use this program as much as possible.
 * This will not be graded.
 */

void foo();
void bar();


void foo(){
        while (1){
                puts("Foo");
        }
}

void bar(){
        while (1){
                puts("Bar");
        }
}

int main(int argc, char **argv) {

	/* Implement HERE */
        printf("Hello!\n");
        rpthread_t* thread;
        rpthread_create(thread, NULL, (void *)&foo, 0);
        bar();        
	return 0;
}
