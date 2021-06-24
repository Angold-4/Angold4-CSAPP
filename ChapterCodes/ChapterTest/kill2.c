/* ---------------------------------------------------------------- */
/* PROGRAM  process-b.c:                                            */
/*   This program demonstrates the use of the kill() system call.   */
/* This process reads in commands and sends the corresponding       */
/* to process-a.  Note that process-a must run before process-b for */
/* process-b to retrieve process-a's pid through the shared memory  */
/* segment created by process-a.                                    */
/* ---------------------------------------------------------------- */

#include  <stdio.h>
#include  <sys/types.h>
#include  <signal.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>

void  main(void)
{
     pid_t   pid;
     key_t MyKey;
     int   ShmID;
     pid_t *ShmPTR;
     char  line[100], c;
     int   i;
     
     MyKey   = ftok(".", 's');          /* obtain the shared memory */
     ShmID   = shmget(MyKey, sizeof(pid_t), 0666);
     ShmPTR  = (pid_t *) shmat(ShmID, NULL, 0);
     pid     = *ShmPTR;                 /* get process-a's ID       */
     shmdt(ShmPTR);                     /* detach shared memory     */
     
     while (1) {                        /* get a command            */
          printf("Want to interrupt the other guy or kill it (i or k)? ");
          gets(line);
          for (i = 0; !(isalpha(line[i])); i++)
               ;
               c = line[i];
          if (c == 'i' || c == 'I') {   /* send SIGINT with kill()  */
               kill(pid, SIGINT);
               printf("Sent a SIGINT signal\n");
          }
          else if (c == 'k' || c == 'K') {
               printf("About to send a SIGQUIT signal\n");
               kill(pid, SIGQUIT);      /* send SIGQUIT with kill() */
               printf("Done.....\n");
               exit(0);
          }
          else
               printf("Wrong keypress (%c).  Try again\n", c);
     }
}
               
                          
