#include <sys/types.h>
#include <sys/unistd.h>
#include <stdio.h>
void perror();

main() {

  if (setuid(23) == -1) {
    perror("Setuid failure");
  }
}
