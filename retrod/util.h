#include "retrod.h"

void perror_and_exit(char* msg) {
  perror(msg);
  exit(1);
}
