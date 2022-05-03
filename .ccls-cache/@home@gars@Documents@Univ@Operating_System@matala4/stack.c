#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_SIZE 1024

// linked list node structure for the string linked list
typedef struct node {
  char string[MAX_SIZE];
  struct node *next;
} node;

// double linked list to dynamic memory allocation
typedef struct dynamic_mem {
  size_t size;
  struct dynamic_mem *next;
} dynamic_mem;

void *malloc_X(size_t size);
void free_X(void *ptr);

static const size_t align_to = 16;
static dynamic_mem dynamic_mem_top = {0, 0};

node *push(char *element, node *head) {
  if (strlen(element) < 1) {
    printf("ERROR:no string was given\n");
    fflush(stdout);
  }

  int size = strlen(element);
  node *temp = malloc_X(sizeof(node));
  memset(temp->string, 0, MAX_SIZE);
  memcpy(temp->string, element, size);
  temp->next = head;
  printf("DEBUG: end of push command with the word -%s-\n", temp->string);
  return temp;
}

char *show_top(node *head) {
  // case of empty stack
  if (!head) {
    printf("ERROR:the stack is empty\n");
    return NULL;
  } else {
    node *tmp = head;
    printf("OUTPUT:%s\n", tmp->string);
    printf("DEBUG: end of top display\n");
    return tmp->string;
  }
}

node *pop(node *head) {
  // case of empty stack
  if (!head) {
    printf("ERROR:the stack is empty\n");
    return NULL;
  } else {
    // create a new head
    node *newHead = head->next;
    // pop the element from stack
    free_X(head);
    return newHead;
    printf("DEBUG: end of pop command\n");
  }
}

void destroy_stack(node *p) {
  if (!p) {
    printf("ERROR:the stack is empty\n");
  } else {
    while (p != NULL) {
      node *next = p->next;
      free_X(p);
      p = next;
    }
    printf("DEBUG: end of stack destruction\n");
  }
}

void display(node *head) {
  if (!head) {
    printf("ERROR:the stack is empty\n");
  } else {
    node *tmp = head;
    int i = 1;
    while (tmp) {
      printf("%d-%s-\n", i, tmp->string);
      tmp = tmp->next;
      i++;
    }
    printf("DEBUG: end of display command\n");
  }
}

int firstWordI(int size, const char *line) {
  int i = 0;
  while (i < size) {
    if (line[i] == ' ') {
      break;
    }
    // printf("-%d-for letter %c\n",i,line[i]);
    i++;
  }

  return i;
}

void *malloc_X(size_t size) {
  // round the size of the block of mem to the nearest multiple of 16 > n
  size = (size + sizeof(dynamic_mem) + (align_to - 1)) & ~(align_to - 1);

  dynamic_mem *mem = dynamic_mem_top.next;
  dynamic_mem **top = &(dynamic_mem_top.next);
  while (mem != 0) {
    if (mem->size >= size) {
      *top = mem->next;
      return ((char *)mem) + sizeof(dynamic_mem);
    }
    top = &(mem->next);
    mem = mem->next;
  }
  mem = (dynamic_mem *)sbrk(size);
  mem->size = size;

  return ((char *)mem) + sizeof(dynamic_mem);
}

void free_X(void *ptr) {
  dynamic_mem *mem = (dynamic_mem *)(((char *)ptr) - sizeof(dynamic_mem));
  mem->next = dynamic_mem_top.next;
  dynamic_mem_top.next = mem;
}

void *calloc_X(size_t size) {
  // round the size of the block of mem to the nearest multiple of 16 > n
  size = (size + sizeof(dynamic_mem) + (align_to - 1)) & ~(align_to - 1);

  dynamic_mem *mem = dynamic_mem_top.next;
  dynamic_mem **top = &(dynamic_mem_top.next);
  while (mem != 0) {
    if (mem->size >= size) {
      *top = mem->next;
      memset(mem + sizeof(dynamic_mem), 0, size);
      return ((char *)mem) + sizeof(dynamic_mem);
    }
    top = &(mem->next);
    mem = mem->next;
  }
  mem = (dynamic_mem *)sbrk(size);
  memset(mem + sizeof(dynamic_mem), 0, size);
  mem->size = size;

  return ((char *)mem) + sizeof(dynamic_mem);
}
