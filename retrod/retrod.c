#include "retrod.h"

int main() {
  int cfd;
  int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  char *s = "Testing 1 2 3";
  struct sockaddr_un myaddr, peer_addr;
  socklen_t peer_addr_size;
  if (fd < 0)
    perror_and_exit("socket");
  memset(&myaddr, 0, sizeof(struct sockaddr_un));
  myaddr.sun_family = AF_UNIX;
  strncpy(myaddr.sun_path, PATH, sizeof(myaddr.sun_path) - 1);
  SAFE_CALL(bind(fd, (struct sockaddr*) &myaddr, sizeof(struct sockaddr_un)));
  SAFE_CALL(listen(fd, MAX_BACKLOG));
  
  /* Accept connections */
  puts("Waiting for connections...");
  cfd = accept(fd, (struct sockaddr*) &peer_addr, &peer_addr_size);
  if (cfd < 0)
    perror_and_exit("accept");
  puts("Connected");

  /* Process input */
  
  write(fd, "Ping", 5); 
  char* buf = malloc(1024 * sizeof(char));
  read(cfd, buf, 1024);
  SAFE_CALL(shutdown(fd, SHUT_RD));
  SAFE_CALL(close(fd));
  SAFE_CALL(unlink("/tmp/socket"));
  return 0;
}
