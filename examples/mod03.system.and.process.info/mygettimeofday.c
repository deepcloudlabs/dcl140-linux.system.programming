#include <sys/time.h>
#include <stdio.h>

main() {

  struct timeval tv;

  gettimeofday(&tv, NULL);
  printf("Time: seconds = %d microseconds = %d\n",
      tv.tv_sec, tv.tv_usec);
}
