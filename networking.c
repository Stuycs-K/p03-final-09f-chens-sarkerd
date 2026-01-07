#include "networking.h"

int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family=AF_INET;
  hints->ai_socktype=SOCK_STREAM;
  hints->ai_flags=AI_PASSIVE;
  int s = getaddrinfo(NULL,PORT,hints,&results);//results is the addy
  if(s) {
    perror("server_setup getaddrinfo");
    exit(99);
  }

  int clientd;//store the socket descriptor here
  clientd=socket(results->ai_family,results->ai_socktype,results->ai_protocol);//create socket
  err(clientd,"clientd socket creation error in server_setup");

  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error in server_setup");

  //bind the socket to address and port
  err(bind(clientd,results->ai_addr,results->ai_addrlen),"bind error server_setup");

  //set socket to listen state
  err(listen(clientd,10),"listen error server_setup");

  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);

  return clientd;
}

int server_tcp_handshake(int listen_socket){
    int client_socket;
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);
    //accept() the client connection
    client_socket = accept(listen_socket,(struct sockaddr *) &client_address, &sock_size);
    err(client_socket,"accept error server_tcp_handshake");
    return client_socket;
}

int client_tcp_handshake(char * server_address) {
  //setup structs for getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family=AF_INET;
  hints->ai_socktype=SOCK_STREAM;
  hints->ai_flags=0;
  int s = getaddrinfo(server_address,PORT,hints,&results);//results is the addy
  if(s) {
    perror("getaddrinfo on client_tcp_handshake");
    exit(99);
  }

  int serverd;//store the socket descriptor here
  serverd=socket(results->ai_family, results->ai_socktype, results->ai_protocol);//create the socket
  err(serverd,"serverd socket creation error in client_tcp_handshake");

  //connect() to the server
  err(connect(serverd, results->ai_addr, results->ai_addrlen),"connect error in client_tcp_handshake");

  free(hints);
  freeaddrinfo(results);

  return serverd;
}




void err(int i, char*message){
  if(i < 0){
	  printf("Error: %s - %s\n",message, strerror(errno));
  	exit(1);
  }
}
