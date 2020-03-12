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
void foo1();
void bar1();


void foo(){
        while (1){
                puts("Foo");
                rpthread_t thread;
                rpthread_create(&thread, NULL, (void *)&foo1, 0);
       } 
}

void bar(){
        while (1){
                puts("Bar");
                rpthread_t thread;
                rpthread_create(&thread, NULL, (void *)&bar1, 0);
        }
}

void foo1(){
        while (1){
                puts("Foo1111111111");
        }
}

void bar1(){
        while (1){
                puts("Bar11111111111");
        }
}

int main(int argc, char **argv) {

	/* Implement HERE */
        printf("Hello!\n");
        rpthread_t thread;
        rpthread_create(&thread, NULL, (void *)&foo, 0);
        bar();        
	return 0;
}
