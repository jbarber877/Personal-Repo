#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char ** argv){

  char *ip = "127.0.0.1";
  int port = 5566;

  if (argc == 2){
    port = atoi(argv[1]);
  }

  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;

  // Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("[-]Socket error");
    exit(1);
  }

  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(ip);

  // Connect to server
  connect(sock, (struct sockaddr*)&addr, sizeof(addr));

  // Send message to server
  bzero(buffer, 1024);
  strcpy(buffer, "Jared Barber : 1234 - 5678");
  send(sock, buffer, strlen(buffer), 0);

  // Get message from client and print it
  bzero(buffer, 1024);
  recv(sock, buffer, sizeof(buffer), 0);
  printf("%s\n", buffer);

  // Disconnect from the server
  close(sock);

  return 0;
}
