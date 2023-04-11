#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PATH "/tmp/socket"
#define MAX_BACKLOG 3         // Really no reason for concurrent connections

void perror_and_exit(char* msg) {
  perror(msg);
  exit(1);
}

int main() {
  int cfd;
  int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  char *s = "Testing 1 2 3";
  struct sockaddr_un myaddr, peer_addr;
  socklen_t peer_addr_size;
  if (fd < 0)
    perror("socket");
  memset(&myaddr, 0, sizeof(struct sockaddr_un));
  myaddr.sun_family = AF_UNIX;
  strncpy(myaddr.sun_path, PATH, sizeof(myaddr.sun_path) - 1);
  if (bind(fd, (struct sockaddr*) &myaddr, sizeof(struct sockaddr_un)) == -1)
    perror_and_exit("bind");
  if (listen(fd, MAX_BACKLOG) == -1)
    perror_and_exit("listen");
  
  /* Accept connections */
  puts("Waiting for connections...");
  cfd = accept(fd, (struct sockaddr*) &peer_addr, &peer_addr_size);
  if (cfd < 0)
    perror_and_exit("accept");
  puts("Connected");

  /* Process input */
  
  write(fd, "Echo: ", 6);
  char* c = malloc(sizeof(char));
  do {
    read(cfd, c, 1);
    write(fd, c, 1);
  } while (*c != '\0');
  if (close(fd) < 0)
    perror_and_exit("close");
  unlink("/tmp/socket");       // idk how to do this properly
  return 0;
}
