#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/memlayout.h"
#include "user/user.h"

/* Example: ordering executions of critical sections 
 * 
 * Suppose that we have three processes: A, B, C 
 *
 *   A: non-critical code; critical section A; non-critical code
 *   B: non-critical code; critical section B; non-critical code
 *   C: non-critical code; critical section C; non-critical code
 * 
 * We need to order execution of critical sections in a particular order:
 *
 *   critical section A > critical section B > critical section C
 * 
 * Solution using semaphores: 
 *
 * Define three semaphores Sa, Sb, Sc. 
 * Initialise Sa=1, Sb=0, Sc=0
 * Change code of A,B,C as follows:
 *
 *   A: non-critical code; wait(Sa); critical section A; signal(Sb); non-critical code
 *   B: non-critical code; wait(Sb); critical section B; signal(Sc); non-critical code
 *   C: non-critical code; wait(Sc); critical section C; signal(Sa); non-critical code
 *
 * Initially, only process A will be able to proceed through its critical section, 
 * because Sa is initialised to 1. Processes B and C will sleep on semaphores Sb and Sc.
 * As process A leaves the critical section, it will signal (increment) semaphore Sb waking up 
 * process B and allowing it to proceed throught its critical section. When process B 
 * leaves its critical section, it will signal (increment) semaphore Sc waking up process 
 * C and allowing it to proceed throught its critical section.
 */
 
enum semaphores {
  Sa = 0,
  Sb = 1,
  Sc = 2
};

void A()
{
   // non-critical code
   printf("AAAAAAA");

   sem_wait(Sa,1);
   // critical section A
   printf("\nA's critical section\n");
   sem_post(Sb,1);

   // non-critical code
   exit(0);
}

void B()
{  
   // non-critical code 
   printf("BBBBBB");
	
   sem_wait(Sb,1);
   // critical section B
   printf("\nB's critical section\n");
   sem_post(Sc,1);
   
   // non-critical code
   exit(0);
}  

void C()
{
   // non-critical code
   printf("CCCCCC");
   
   sem_wait(Sc,1);
   // critical section C
   printf("\nC's critical section\n");
   sem_post(Sa,1);

   // non-critical code
   exit(0);
}

int
main(int argc, char *argv[])
{
  // initialise semaphores 
  sem_open(Sa,1);
  sem_open(Sb,0);
  sem_open(Sc,0);

  // create processes
  if (fork() == 0) C(); // process C
  if (fork() == 0) B(); // process B
  if (fork() == 0) A(); // process A

  // wait for the 3 child processes to terminate
  wait(0); 
  wait(0);
  wait(0);


  // release semaphore objects
  sem_close(Sa);
  sem_close(Sb);
  sem_close(Sc);

  return 0;
}
