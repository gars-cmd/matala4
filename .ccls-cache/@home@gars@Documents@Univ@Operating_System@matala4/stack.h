#define MAX_SIZE 1024

typedef struct node
{
    char string[MAX_SIZE];
    struct node *next;
} node;

typedef struct dynamic_mem
{
    size_t size;
    struct dynamic_mem* next;
}dynamic_mem;

static const size_t align_to = 16;

int div_round(int a , int b);
void* malloc_X(size_t size);
void free_X(void* ptr);
void* calloc_X(size_t size);
node *push(char *element, node *head);
node *pop(node *head);
void destroy_stack(node *p);
void display(node *head);
char *show_top(node *head);
int firstWordI(int size, const char *line);
