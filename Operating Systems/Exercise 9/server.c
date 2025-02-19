#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char** argv){

  char *ip = "127.0.0.1";
  int port = 5566;

  if (argc == 2){
    port = atoi(argv[1]);
  }

  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;

  // Create socket
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0){
    perror("[-]Socket error");
    exit(1);
  }

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  // Bind to the port number
  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("[-]Bind error");
    exit(1);
  }

  listen(server_sock, 5);

  while(1){
    // Accept
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);

    // Get message from the client and print it
    bzero(buffer, 1024);
    recv(client_sock, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);

    // Send message to client
    bzero(buffer, 1024);
    strcpy(buffer, "Welcome to the server running on REPTILIAN");
    send(client_sock, buffer, strlen(buffer), 0);

    close(client_sock);
  }

  return 0;
}
