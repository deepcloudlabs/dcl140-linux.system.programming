#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

main() {

  /* Send SIGUSR1 signal to all processes in
  process group */
  sigsend(1, getpgid(getpid()), SIGUSR1);
}
