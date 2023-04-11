#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
  int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  char *s = "Testing 1 2 3";
  struct socketaddr_in servAddr;
}
