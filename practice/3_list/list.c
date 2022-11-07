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

//Creat Head node��next point to NULL
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
        return 0;   /* ��i��Ԫ�ز����� */
    s = (LinkList)malloc(sizeof(Node)); 
    s->data = e;  
    s->next = p->next;      /* ��p�ĺ�̽�㸳ֵ��s�ĺ��  */
    p->next = s;          /* ��s��ֵ��p�ĺ�� */
    return 1;
}

//ɾ����i������Ԫ�أ���elem������ֵ
Status ListDelete(LinkList *L,int i,ElemType *e) 
{ 
    int j;
    LinkList p,q;
    p = *L;
    j = 1;
    while (p->next && j < i)    /* ����Ѱ�ҵ�i��Ԫ�� */
    {
        p = p->next;
        j++;
    }
    if (!(p->next) || j > i) 
        return 0;           /* ��i��Ԫ�ز����� */
    q = p->next;
    p->next = q->next;          /* ��q�ĺ�̸�ֵ��p�ĺ�� */
    *e = q->data;               /* ��q����е����ݸ�e */
    free(q);                    /* ��ϵͳ���մ˽�㣬�ͷ��ڴ� */
    return 1;
}

//�������
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

//�������
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

//����һ������β�巨
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
    Status flag=TRUE;           /* flag������Ϊ��� */
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
                 flag=TRUE;     /* ��������ݽ�������flagΪtrue */
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