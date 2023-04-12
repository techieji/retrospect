/* 
 * Retrospect Daemon script
 *
 * This script performs one action only: updating the .retro folder of all
 * relevant folders. It receives instructions from the main, user-controlled
 * executable `retro`. Spec:
 *    S: shutdown
 *    M: moniter (a folder)
 *    D: demoniter
 */

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

#define SAFE_CALL(call) do if ((call) < 0) perror_and_exit(#call); while (0)

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
