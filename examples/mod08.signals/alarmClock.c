#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>	/* For time declarations. */

#define REPEAT_RATE	2	/* seconds. */

static char beep =  '\007';

void handler(int i) {
  write (STDOUT_FILENO, &beep, sizeof(beep));
}

main(int argc, char *argv[]) {
  struct itimerval it;	/* Structure for loading the timer. */
  sigset_t mask;		/* Mask for blocking signals. */
  int seconds;

  if ((argc != 2) || ((seconds = atoi(argv[1])) <= 0)) {
    fprintf (stderr, "Usage: %s <seconds>\n", argv[0]);
    exit (1);
  }

  /* Set up mask of signals to block.
  Block all except the SIGALRM and ^C. */
  sigfillset(&mask);
  sigdelset(&mask,SIGALRM);
  sigdelset(&mask,SIGINT);

  /* Install the handler to receive the alarm signal. */
  (void)sigset(SIGALRM, handler);

  /* Init data structure to load into setitimer. */
  it.it_value.tv_sec = seconds;
  it.it_value.tv_usec = 0;
  it.it_interval.tv_sec = REPEAT_RATE;
  it.it_interval.tv_usec = 0;

  /* Load and set timer. */
  if (setitimer(ITIMER_REAL, &it, (struct itimerval *)NULL) == -1) {
    perror("setitimer");
    exit(1);
  }

  /* Pause indefinitely.  sigsuspend() will return after each alarm, so
  put it into an infinite loop, to go forever. */
  while (1) {
    sigsuspend(&mask);
  }
}
