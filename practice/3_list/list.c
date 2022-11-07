#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define MAXSIZE 1024
#define TRUE 1
#define FALSE 0

typedef int Status;
typedef int ElemType;

typedef struct Node
{
    ElemType data;
    struct Node *next;
}Node, *LinkList;

//Creat Head node，next point to NULL
Status InitList(LinkList * L)
{
    *L = (LinkList)malloc(sizeof(Node));
    if(!(*L))
    {
        printf("init error\n");
        return -1;
    }
    (*L)->next = NULL;
    return 1;
}

//Calculate the length of the list , except the header
int ListLength(LinkList L)
{
    int i=0;
    LinkList p=L->next; 
    while(p)                        
    {
        i++;
        p=p->next;
    }
    return i;
}

//insert elem before i
Status ListInsert(LinkList *L,int i,ElemType e)
{ 
    int j;
    LinkList p,s;
    p = *L;   
    j = 1;
    while (p && j < i)     //find i node
    {
        p = p->next;
        j++;
    } 
    if (!p || j > i) 
        return 0;   /* 第i个元素不存在 */
    s = (LinkList)malloc(sizeof(Node)); 
    s->data = e;  
    s->next = p->next;      /* 将p的后继结点赋值给s的后继  */
    p->next = s;          /* 将s赋值给p的后继 */
    return 1;
}

//删除第i个数据元素，用elem返回其值
Status ListDelete(LinkList *L,int i,ElemType *e) 
{ 
    int j;
    LinkList p,q;
    p = *L;
    j = 1;
    while (p->next && j < i)    /* 遍历寻找第i个元素 */
    {
        p = p->next;
        j++;
    }
    if (!(p->next) || j > i) 
        return 0;           /* 第i个元素不存在 */
    q = p->next;
    p->next = q->next;          /* 将q的后继赋值给p的后继 */
    *e = q->data;               /* 将q结点中的数据给e */
    free(q);                    /* 让系统回收此结点，释放内存 */
    return 1;
}

//遍历输出
Status ListTraverse(LinkList L)
{
     LinkList p=L->next;
    // while(p)
    // {
    //     printf("%d\t",p->data);
    //     p=p->next;
    // }
    // printf("\n");
    // return 1;
    printf("Traverse output\n");
    int i = 1;
    for(p = L->next; p != NULL; p = p->next)
    {
        printf("data%d is %d\n", i,p->data);
        i++;
    }
    printf("Linklist length = %d\n\n",ListLength(L));
    return 1;
}

//清空链表
Status ClearList(LinkList *L)
{ 
    LinkList p,q;
    p=(*L)->next;           
    while(p)                
    {
        q=p->next;
        free(p);
        p=q;
    }
    (*L)->next=NULL;       
    return 1;
}

//创建一个链表，尾插法
void CreateList(LinkList *L, int n) 
{
    LinkList p,r;
    int i;
    srand(time(0));                     
    *L = (LinkList)malloc(sizeof(Node)); 
    r=*L;                                
    for (i=0; i<n; i++) 
    {
        p = (Node *)malloc(sizeof(Node)); 
        p->data = rand()%100+1;           
        r->next=p;                        
        r = p;                            
    }
    r->next = NULL;                       
}

//
void ListSort(LinkList *L, int n)
{
    int i,j;
    LinkList p;
    ElemType temp;
    //LinkList p = L->next;
    Status flag=TRUE;           /* flag用来作为标记 */
    for(i = n-1; i > 0; i--) 
    {
        p =(*L)->next;
        flag = FALSE;             
        for(j = 0; j < i; j++)
        {
            if(p->data > p->next->data)
            {
                temp = p->data;
                p->data = p->next->data;
                p->next->data = temp;
                 flag=TRUE;     /* 如果有数据交换，则flag为true */
            }
                p = p->next;
        }
        if(flag == FALSE)
            break;
    }
}


int main()
{
    LinkList L;
    ElemType elem;
    int i;
    InitList(&L);
    printf("insert 1 2 3 4 5\n");
    for(i =1; i <= 5; i++)
    {
        ListInsert(&L, i, i);
    }
    ListTraverse(L);

    printf("Delete 1 2 3  \n");
    for(i = 0; i < 3; i++)
    {
        ListDelete(&L, 1, &elem);
    }
    ListTraverse(L);
    
    printf("Create List\n");
    CreateList(&L, 10);
    ListTraverse(L);

    printf("bubble sort \n");
    ListSort(&L, 10);
    ListTraverse(L);
    printf("Clear List\n");
    ClearList(&L);
    printf("Linklist length = %d\n",ListLength(L));
}