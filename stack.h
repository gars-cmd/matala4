#define MAX_SIZE 1024

typedef struct node
{
    char string[MAX_SIZE];
    struct node *next;
} node;


node* push(char *element, node *head);
node *pop(node *head);
void destroy_stack(node *p);
void show_top(node* head);
void display(node *head);
int firstWordI(int size,const char* line);