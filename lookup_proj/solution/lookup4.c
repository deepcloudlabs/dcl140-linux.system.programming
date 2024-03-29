/*
 * lookup4 : This file does no looking up locally, but instead asks
 * a server for the answer. Communication is by message queues.
 * The protocol is : messages of type 1 are meant for the server.
 * The client only reads messages whose type matches their own pid.
 * The message queue key is what is passed as resource.
 */

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
  static ClientMessage snd;
  ServerMessage rcv;
  static int qid;
  static int pid;
  static int first_time = 1;

  if (first_time) { /* open message queue */
    first_time = 0;

    /* Prepare our ClientMessage structure. */
    pid = getpid();
    sprintf(snd.content.id,"%d",pid);
    snd.type = 1L;

    /* Open the message queue.  Use resource pointer value as key. */
    if ((qid = msgget(strtol(resource,(char **)NULL,0),0)) == -1)
      return UNAVAIL;
  }

  /* Send server the word to be found ; await reply */
  strcpy(snd.content.word,sought->word);
  if (msgsnd(qid,(struct msgbuf *)&snd,sizeof(Client),0) == -1) {
    return UNAVAIL;
  }

  if (msgrcv(qid,(struct msgbuf *)&rcv,TEXT,pid,0) == -1) {
    return UNAVAIL;
  }

  strcpy(sought->text,rcv.text);

  /* Server returns XXXX when it cannot find request. */
  if (strcmp(rcv.text,"XXXX") != 0)
    return FOUND;

  return NOTFOUND;
}
