#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "stack.h"
#include <signal.h>
#include <assert.h>



//------------------------------------PARAMS PART-----------------------------------------------------------------

#define MAX_SIZE 1024
#define PORT "3490"
#define BACKLOG 10


// initialization of the stack
node *top = NULL;

//initialization of the mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handler_connect(int *client);
void stack_call(int *client, char *command);
void *get_in_addr(struct sockaddr *sa);


int main(int argc, char const *argv[])
{

    // ------------------------------SERVER PART-------------------------------------------------------------------
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1)
    { // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }
        int pt;
        pthread_t thread;

        pt = (pthread_create(&thread, NULL, handler_connect, &new_fd) != 0);
        if (pt != 0)
        {
            perror("thread");

            return 1;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("DEBUG:server: got connection from %s\n", s);
    }
    


}


//--------------------------------------------HANDLE PART--------------------------------------------------------


void *handler_connect(int *client)
{

    pthread_mutex_lock(&mutex);
    
    int socketC = *client;
    char ans[MAX_SIZE];
    memset(ans, 0, MAX_SIZE);
    int recept;
    recept = recv(socketC, ans, MAX_SIZE, 0);
    if (recept == -1)
    {
        perror("Reception...");
        exit(1);
    }
    ans[recept] = '\0';
    stack_call(client, ans);
    pthread_mutex_unlock(&mutex);

    close(client);
    pthread_exit(NULL);
    return NULL;
}

//---------------------------------------------STACK PART----------------------------------------------------------------

void stack_call(int *client, char *command)
{
    int index, rest, cmd_size;

    printf("DEBUG:wait for command from the client\n");

    cmd_size = strlen(command);
    index = firstWordI(cmd_size, command);
    rest = cmd_size - index;

    // get the command
    char cmd[index];
    memset(cmd, 0, index + 1);
    strncpy(cmd, command, index);
    fflush(stdout);

    // if there is not parameter to the command
    if (rest == 0)
    {
        // show the top of the stack
        if (strcmp(cmd, "TOP") == 0)
        {
            int tmp;
            char *tosend = show_top(top);
            if (tosend == NULL)
            {
                send(*client, "ERROR:the stack is empty", 24, 0);
            }

            else if ((tmp = send(*client, tosend, MAX_SIZE, 0)) == -1)
            {
                perror("ERROR:TOP...");
            }
            else
            {
                printf("DEBUG:send succeed\n");
                fflush(stdout);
            }
        }
        // delete the top element of the stack
        else if (strcmp(cmd, "POP") == 0)
        {
            top = pop(top);
        }
        // show the entire stack
        else if (strcmp(cmd, "SHOW") == 0)
        {
            display(top);
        }
        // destroy the entire stack
        else if (strcmp(cmd, "RESET") == 0)
        {
            destroy_stack(top);
            top = NULL;
        }
        // exit the program
        else if (strcmp(cmd, "EXIT") == 0)
        {
            exit(EXIT_SUCCESS);
        }
		// to run a simple test
		else if (strcmp(cmd,"TEST") == 0) 
		{
			node* t = NULL;
			printf("DEBUG:TEST:we will add to the test stack the word [test1]\n");
			t= push("test1", t);
			printf("DEBUG:TEST:let's see what we have in the top of the stack \n");
			char* display = show_top(t);
			printf("DEBUG:TEST:we  got %s\n",display);
			assert(strcmp(display,"test1")==0);
			printf("DEBUG:TEST:we will add to the test stack the word [test2]\n");
			t= push("test2", t);
			printf("DEBUG:TEST:let's test what we have in the top of the stack \n");
			char* display2 = show_top(t);
			assert(strcmp(display2,"test2")==0);
			printf("DEBUG:TEST:now let's pop an element\n");
			pop(t);
			char* display3 = show_top(t);
			printf("DEBUG:TEST:let's test what we have in the top of the stack \n");
			assert(strcmp(display3,"test1"));
		}
        // if a not valid/recognized command was typed
        else
        {
            printf("DEBUG:No command recognized\n");
        }
    }
    // the case of a push command
    else
    {
        char data[rest];
        memset(data, 0, rest);
        strncpy(data, &command[index + 1], rest - 1);
        if (strcmp(cmd, "PUSH") == 0)
        {
            top = push(data, top);
        }
    }
    // }
}

//---------------------------------------UTILS FUNCTIONS-----------------------------
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}


