/*
  COMP 3430 Operating Systems
  mq_test.c
  (C) Computer Science, University of Manitoba
  
  Test of POSIX message queue
  
  Each message queue is identified by a name of the form /somename;
  that is, a null-terminated string of up to NAME_MAX (i.e., 255)
  characters consisting of an initial slash, followed by one or more
  characters, none of which are slashes. Two processes can operate on
  the same queue by passing the same name to mq_open.
  
  Messages are transferred to and from a queue using mq_send and
  mq_receive. When a process has finished using the queue, it closes
  it using mq_close, and when the queue is no longer required, it can
  be deleted using mq_unlink.
  
  POSIX message queues have kernel persistence: if not removed by
  mq_unlink, a message queue will exist until the system is shut down.
  
  A message queue descriptor is a reference to an open message queue
  description. After a fork, a child inherits copies of its parent’s
  message queue descriptors, and these descriptors refer to the same
  open message queue descriptions as the corresponding descriptors in
  the parent.
  
  On Linux, message queues are created in a virtual file system. This
  file system is rooted at /dev/mqueue. The message queues on the
  system can be viewed and manipulated using the commands usually used
  for files (e.g., ls(1) and rm(1)).

  The contents of each file in the directory consist of a single line
  containing information about the queue:

           $ cat /dev/mqueue/mymq
           QSIZE:129     NOTIFY:2    SIGNO:0    NOTIFY_PID:8260
  
  The struct mq_attr structure is defined as:

           struct mq_attr {
               long mq_flags;       // Flags: 0 or O_NONBLOCK
               long mq_maxmsg;      // Max. # of messages on queue
               long mq_msgsize;     // Max. message size (bytes)
               long mq_curmsgs;     // # of messages currently in queue
           };

  
  to compile: gcc -lrt mq_test.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>                     // For O_* constants
#include <sys/wait.h>
#include <mqueue.h>

#define MSGSZ 1024
#define NUMMSGS 10
#define PERMS 0644                     // (octal) permissions rw-r--r--
#define FLAGS O_RDWR | O_CREAT

int main (int argc, char *argv[]) {
  int i;
  struct mq_attr attr;                 // attributes for our message queue
  mqd_t mq;                            // a message queue descriptor
  char buf[MSGSZ+1];                   // buffer for our message
  char mqname[10];                     // buffer for creating message queue name
  pid_t pid;                           // place to store process ID
  unsigned prio;                       // place to store message priority
  int status;                          // place to store child return status
   
  // Set up attributes for the message queue
  attr.mq_maxmsg = 10;                 // maximum number of messages in queue
  attr.mq_msgsize = MSGSZ;             // maximum message size
  attr.mq_flags = 0;                   // blocking operations

  // Create a unique message queue name using userid
  sprintf(mqname, "/%s", getenv("USER"));
  printf("Message queue name is %s.\n\n", mqname);

  // attempt to open a message queue, and create it if necessary
  if (-1==(mq = mq_open (mqname, FLAGS, PERMS, &attr))) 
    perror("mq_open croaked");

  // fork a child process. It will inherit the open message queue
  if ((pid=fork())) {
    // parent process
    for (i=0;i<NUMMSGS;i++) {
      sprintf(buf,"Parent sending message num %d.\0",i);
      printf("Parent sent: <%s>.\n",buf);
      if (mq_send(mq, buf, strlen(buf), 2)== -1) 
        perror("mq_send croaked");
    }// for
  }else{
    // child process
    sleep(1);
    for (i=0;i<NUMMSGS;i++) {
      mq_receive(mq, buf, MSGSZ, &prio);
      printf("Child received: <%s>.\n",buf);
    }// for
    mq_close(mq);
    exit(0);
  }// end of child process
  
  // parent will close and remove the message queue
  waitpid(pid,&status,0);
  mq_close(mq);
  printf("End of processing.\n");
  exit(0);
}// end parent

