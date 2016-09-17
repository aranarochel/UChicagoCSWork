/*
 *
 * CMSC 23300 / 33300 - Networks and Distributed Systems
 *
 * main() code for chirc project
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include "reply.h"
#include "structs.h"
#include "functions.h"
#include "handlers.h"
#include "list.h"
#include "channels.h"
#include "dispatch-table.h"
/* Threading code borrows heavily from Borja's examples */

#ifdef MUTEX
pthread_mutex_t lock;
#endif


void *service_client(void *args);


int main(int argc, char *argv[])
{
  // Initialize user list for all threads to use
  global_user_list = new_list();
  global_channel_clist = new_clist();
  global_password = NULL;

  
  sigset_t new;
  sigemptyset (&new);
  sigaddset (&new, SIGPIPE);
  if (pthread_sigmask(SIG_BLOCK, &new, NULL) != 0)
    {
      perror("Unable to mask SIGPIPE");
      exit(-1);
    }


  char *port = "6667";
  char *passwd = NULL;

  pthread_t client_thread;
  user *user_info; // Struct that gets passed to the "service_client" function

  int opt;
  struct sockaddr_in serverAddr, clientAddr;
  int serverSocket;
  int clientSocket;
  int yes;

  socklen_t sinSize = sizeof(struct sockaddr_in);


  while ((opt = getopt(argc, argv, "p:o:h")) != -1)
    switch (opt)
    {
      case 'p':
        port = strdup(optarg);
        break;
      case 'o':
        passwd = strdup(optarg);
        break;
      default:
        printf("ERROR: Unknown option -%c\n", opt);
        exit(-1);
    }

  if (!passwd)
  {
    fprintf(stderr, "ERROR: You must specify an operator password\n");
    exit(-1);
  }


  // Initialize server socket address
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(port));
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  // Create socket and check for errors
  serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(serverSocket == -1) 
  {
    perror("Could not open socket");
    exit(-1);
  }

  // Make socket addr reusable and check for errors
  if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
  {
    perror("Socket setsockopt() failed");
    close(serverSocket);
    exit(-1);
  }

  // Bind socket to addr and check for errors
  if(bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
  {
    perror("Socket bind() failed");
    close(serverSocket);
    exit(-1);
  }

  // Make socket ready for listening and check for errors
  if(listen(serverSocket, 5) == -1)
  {
    perror("Socket listen() failed");
    close(serverSocket);
    exit(-1);
  }

  fprintf(stderr, "Waiting for a connection...\n");



/* =====================TEAM CODE======================== */


  while(1)
  {

    // accept connection and check for errors
    if( (clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &sinSize)) == -1)
    {
      perror("Socket accept() failed");
      close(serverSocket);
      exit(-1);
      continue;
    }

    socklen_t len;
    char hostname[128];
    
    if (gethostname(hostname, sizeof(hostname)) == -1) 
    {
      perror("gethostname() failed");
      return -1;
    }
    
    if (getpeername(clientSocket, (struct sockaddr*) &clientAddr, &len) == -1) 
    {
      perror("getpeername() failed");
      return -1;
    }
    
    char* clientIP = inet_ntoa(clientAddr.sin_addr);

    // create user struct to pass to service_client

    user_info = create_user("", "", "", clientSocket, 0, clientIP, hostname, passwd);

    add_to_list(global_user_list, user_info);

    if (pthread_create(&client_thread, NULL, service_client, (void*) user_info) != 0)
    {
      perror("Could not create client thread");
      free(user_info);
      close(clientSocket);
      close(serverSocket);
    }
  }
  free(passwd);
  
  return 0;
}



// function that will service a user
void *service_client(void *args)
{
  user *myuser;
  myuser = (user*) args;
  

  char partial_message[BUFFER_SIZE];
  char *client_message = NULL;

  cmdValueStruct cmdValueInstance = {"", "", ""};

  partial_message[0] = '\0';

  while(1)
    {

      // "receive_message()" returns a complete message and is stored in "client_message"
      // "client message will be NULL if we got an incomplete message. In this case,
      // "partial message" will hold the incomplete message until this func is called again
      client_message = receive_message(myuser->clientSocket,partial_message);
      
      // Debuggin message, which we can remove later 
      //printf("client_message: %s\n",client_message);

      // If we didn't get a partial message, proceed to parse
      if (client_message != NULL && client_message != '\0' && client_message != "")
      {

        // Parse_message takes the buffer and splits it into commands and values,
        // stuffing those into the cmdValueStruct struct, where they are more easily
        // accessable.
        // We then need to run boolean tests on the cmd portion, unless we come up with a better way
        // of passing that information through. Depending on how many commands we eventually have,
        // We may want to change the if/else to a switch/case
        cmdValueInstance = parse_message(client_message);

        int returnval = dispatch(cmdValueInstance, myuser, global_user_list);

      }
    }
    printf("out of while\n");
  
  close(myuser->clientSocket);
  pthread_exit(NULL);
  
}



