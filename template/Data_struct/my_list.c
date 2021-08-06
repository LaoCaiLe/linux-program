#include <stdio.h>
#include <stdlib.h>


struct node {
    int data;
    struct node *next;
};

//遍历链表
void list_print(struct node *p)
{
    while(p != NULL)
    {
        printf("%d ",p->data);
        p=p->next;
    }
    printf("\r\n");
}

//头插法
struct node *first_insert(struct node *first, int data)
{
    struct node *p = malloc(sizeof(struct node));

    p->data = data;
    p->next = NULL;

    if(first == NULL) return p;

    p->next = first;
    first = p;

    return first;
}

//尾插法
struct node *last_insert(struct node *first, int data)
{
    struct node *p = malloc(sizeof(struct node));
   
    p->data = data;
    p->next = NULL;

    if(first == NULL) return p;

    struct node *last;
    last = first;

    while(first->next != NULL)
    {
        first = first->next;
    }
    first->next = p;

    return last;

}

int main()
{
    int i;
    struct node *list;

    for(i=0; i<10; i++)
    {
        //list = last_insert(list,i+1);
        list = first_insert(list,i+1);
    }

    list_print(list);

}