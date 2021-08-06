#include <stdio.h>
#include <stdlib.h>


struct node
{
    int data;
    struct node* next;
};

void list_show(struct node *l)
{
    while(l!=NULL)
    {
        printf("%d ",l->data);
        l=l->next;
    }
    printf("\r\n");
}
struct node *fast_insert(struct node *first,int data)
{
    struct node *p = malloc(sizeof(struct node));

    p->data = data;
    p->next = NULL;

    if(first==NULL)
    {
        return p;
    }
    else
    {
        p->next = first;
        first = p;
    }
    return first;
}

struct node *last_insert(struct node *first,int data)
{
    struct node* last ;
    last = first;

    struct node *p = malloc(sizeof(struct node)) ;
    p->data = data;
    p->next = NULL;

    if(first == NULL)//链表还没有第一个节点
	{
		//说明要插入的节点是第一个节点
		return p;
	}   
    
    
    while(last->next!= NULL)
    {
        last = last->next;
    }
    last->next = p;

    return first;
}


int main()
{
    struct node *l1;

    for(int i=0;i<10;i++)
    {
        l1 = last_insert(l1,i);
    }
    list_show(l1);
}