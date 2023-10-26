#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"BigInteger.h"


void addAtHead(struct node**head,struct node**tail,char val){
    struct node*temp=(struct node*)malloc(sizeof(struct node));
    temp->data=val-'0';
    temp->next=*head;
    temp->prev=NULL;
    if(*head) {
        (*head)->prev=temp;
    }
    *head=temp;
    if(!*tail) *tail=temp;
}


struct BigInteger initialize(char*str){
    int i=0,l=strlen(str);
    struct BigInteger temp;
    if(l > 0 && str[0] == '-'){ 
        temp.sign=-1;
        i++;
    }
    else temp.sign = 1;
    temp.head=temp.tail=NULL;
    temp.size=l-i;
    while(i<l){
        addAtHead(&(temp.head),&(temp.tail),str[i++]);
    }
    return temp;
}


void addAtTail(struct node**head,struct node**tail,int val){
    struct node*newNode=(struct node*)malloc(sizeof(struct node));
    newNode->data=val;
    newNode->next=NULL;
    newNode->prev=*tail;
    if(*tail==NULL){
        *head=*tail=newNode;
        return;
    }
    (*tail)->next=newNode;
    *tail=newNode;
}

int greaterMagnitude(struct BigInteger n1,struct BigInteger n2){
    if(n1.size > n2.size) return 1;
    if(n1.size < n2.size) return 0;
    if(n1.tail->data > n2.tail->data) return 1;
    struct node*l1=n1.tail,*l2=n2.tail;
    if(n1.sign == -1 && n2.sign == 1) return 0;
    while(l1 && l2){
        if(l1->data < l2->data) return 0;
        l1=l1->prev;
        l2=l2->prev;
    }
    return 1;
}
void listSubtract(struct node*l1,struct node*l2,struct BigInteger*n3){
    int b=0,x=0,d1,d2;
    while(l1 && l2){
        d1=l1->data;
        d2=l2->data;
        if(b){
            d1--;
            b=0;
        }
        if(d1 < d2){
            d1 += 10;
            b=1;
        }
        addAtTail(&(n3->head),&(n3->tail),d1-d2);
        n3->size += 1;
        l1=l1->next;
        l2=l2->next;
    }
    if(l2) l1=l2;
    while(l1){
        d1=l1->data;
        if(b){
            d1--;
            b=0;
        }
        addAtTail(&(n3->head),&(n3->tail),d1);
        n3->size += 1;
        l1=l1->next;
    }
}

void listAddition(struct node *l1,struct node *l2,struct BigInteger*n3){
    int c=0,x=0;
    while(l1 && l2){
        x=l1->data+l2->data+c;
        if(x<0) x=-x;
        addAtTail(&(n3->head),&(n3->tail),x%10);
        n3->size += 1;
        c=x/10;
        l1=l1->next;
        l2=l2->next;
    }
    if(l2) l1=l2;
    while(l1){
        x=l1->data+c;
        if(x<0) x=-x;
        addAtTail(&(n3->head),&(n3->tail),x%10);
        n3->size += 1;
        c=x/10;
        l1=l1->next;
    }
    if(c!=0){
        if(c<0) c=-c;
        addAtTail(&(n3->head),&(n3->tail),c);
        n3->size += 1;
    }
}


struct BigInteger copyBigint(struct BigInteger n1){
    struct BigInteger n2=initialize("");
    n2.sign=n1.sign;
    n2.size=n1.size;
    struct node*itr=n1.head;
    while(itr){
        addAtTail(&(n2.head),&(n2.tail),itr->data);
        itr=itr->next;
    }
    return n2;
}
void deleteList(struct node**head,struct node**tail){
    while(*head){
        struct node*temp=*head;
        *head=(*head)->next;
        free(temp);
    }
    *head=*tail=NULL;
}

struct BigInteger add(struct BigInteger n1,struct BigInteger n2){
    struct BigInteger n3=initialize("");
    if(n1.size == 0){
        n3=copyBigint(n2);
        return n3;
    }
    if(n2.size == 0){
        n3=copyBigint(n1);
        return n3;
    }
    if(greaterMagnitude(n1,n2) == 0){
        struct BigInteger temp=n1;
        n1=n2;
        n2=temp;
    }  
    n3.sign=n1.sign;
    if(n1.sign != n2.sign){
        listSubtract(n1.head,n2.head,&n3);
    }
    else{
        listAddition(n1.head,n2.head,&n3);
    }
    if(n3.tail->data == 0){
        struct node*temp;
        while(n3.tail && n3.tail != n3.head && n3.tail->data == 0){
            temp=n3.tail;
            n3.tail=n3.tail->prev;
            n3.size -= 1;
            free(temp);
        }
        if(n3.tail == n3.head && n3.head->data == 0) n3.sign=1;
    }
    return n3;
}
struct BigInteger sub(struct BigInteger n1,struct BigInteger n2){
    n2.sign *= -1;
    struct BigInteger n3=add(n1,n2);
    n2.sign *= -1;
    return n3;
}


struct BigInteger mul(struct BigInteger n1,struct BigInteger n2){
    struct BigInteger n3=initialize("");
    if(n1.head == NULL || n2.head == NULL) return n3;
    if(n1.tail->data == 0 || n2.tail->data == 0){
        return n3;
    }
    n3.sign=n1.sign*n2.sign;
    if(greaterMagnitude(n1,n2) == 0){
        struct BigInteger temp=n1;
        n1=n2;
        n2=temp;
    }
    struct node*l1=n1.head,*l2=n2.head,*head=NULL,*tail=NULL,*l3=NULL;
    int i=0;
    while(l2){
        for(int j=0;j<i;j++){
            addAtTail(&head,&tail,0);
        }
        i++;
        l1=n1.head;
        int carry=0;
        while(l1){
            int mul=l1->data*l2->data+carry;
            addAtTail(&head,&tail,mul%10);
            carry = mul/10;
            l1=l1->next;        
        }
        if(carry>0){
            addAtTail(&head,&tail,carry);
        }
        struct BigInteger temp=initialize("");
        listAddition(head,l3,&temp);
        deleteList(&head,&tail);
        head=tail=NULL;
        l3=temp.head;
        l2=l2->next;
    }
    n3.head=l3;
    n3.size=0;
    while(l3 && l3->next){
        l3=l3->next;
        n3.size += 1;
    }
    n3.tail=l3;
    n3.size += 1;
    return n3; 
}
int equal(struct BigInteger n1,struct BigInteger n2){
    if(n1.size != n2.size) return 0;
    struct node*l1=n1.head,*l2=n2.head;
    while(l1 && l2){
        if(l1->data != l2->data) return 0;
        l1=l1->next;
        l2=l2->next;
    }
    return 1;
}


int table(struct BigInteger n1,struct BigInteger n2){
    if(greaterMagnitude(n1,n2) == 0) return 0;
    char j='1';
    for(int i=1;i<=10;i++,j++){
        char str[2];
        str[0]=j;
        str[1]='\0';
        struct BigInteger n3=initialize(str);
        struct BigInteger n4=mul(n2,n3);
        deleteList(&(n3.head),&(n3.tail));
        if(greaterMagnitude(n4,n1)){
            if(equal(n4,n1)) return i;
            deleteList(&(n4.head),&(n4.tail));
            return i-1;
        }
        deleteList(&(n4.head),&(n4.tail));
    }
    return 1;
}


struct BigInteger div1(struct BigInteger n1,struct BigInteger n2){
    struct BigInteger n3=initialize("");
    if(n1.head == NULL || n2.head == NULL) return n3;
    if(n2.tail->data == 0){
        printf("Can't divide number by zero\n");
        return n3;
    }
    if(n1.tail->data == 0){
        return n3;
    }
    n3.sign=n1.sign/n2.sign;
    struct BigInteger copy1=initialize("");
    n2.sign=1;
    struct node*l1=n1.tail;
    while(l1){
        copy1.sign = 1;
        for(int i=0;i<n2.size && l1;i++){
            if(greaterMagnitude(copy1,n2)) break;
            addAtHead(&(copy1.head),&(copy1.tail),l1->data + '0');
            l1=l1->prev;
            copy1.size += 1;
        }
        
        if(greaterMagnitude(n2,copy1) && l1){
            copy1.size += 1;
            addAtHead(&(copy1.head),&(copy1.tail),l1->data+'0');
            l1=l1->prev;
            
        }
        int i=table(copy1,n2);
        addAtHead(&(n3.head),&(n3.tail),i+'0');
        n3.size += 1;
        char str[2]={i+'0','\0'};
        struct BigInteger il=initialize(str);
        struct BigInteger mlt=mul(n2,il);
        struct BigInteger temp=initialize("");
        listSubtract(copy1.head,mlt.head,&temp);
        if(temp.tail->data == 0){
            while(temp.tail && temp.tail != temp.head && temp.tail->data==0){
                temp.tail=temp.tail->prev;
                free(temp.tail->next);
                temp.tail->next=NULL;
                temp.size -= 1;
            }
        }
        deleteList(&(copy1.head),&(copy1.tail));
        deleteList(&(il.head),&(il.tail));
        deleteList(&(mlt.head),&(mlt.tail));
        if(temp.tail->data != 0){
            copy1=temp;
        }
        else copy1=initialize("");
    }
    return n3;
}


struct BigInteger mod(struct BigInteger n1,struct BigInteger n2){
    struct BigInteger n3=initialize("");
    if(n1.head == NULL || n2.head == NULL) return n3;
    if(n2.tail->data == 0){
        printf("Can't divide number by zero\n");
        return n3;
    }
    if(n1.tail->data == 0){
        n3=copyBigint(n1);
        return n3;
    }
    n3.sign=n1.sign;
    struct BigInteger copy1=initialize("");
    n2.sign=1;
    struct node*l1=n1.tail;
    while(l1){
        copy1.sign = 1;
        for(int i=0;i<n2.size && l1;i++){
            if(greaterMagnitude(copy1,n2)) break;
            addAtHead(&(copy1.head),&(copy1.tail),l1->data + '0');
            l1=l1->prev;
            copy1.size += 1;
        }
        
        if(greaterMagnitude(n2,copy1) && l1){
            copy1.size += 1;
            addAtHead(&(copy1.head),&(copy1.tail),l1->data+'0');
            l1=l1->prev;
            
        }
        int i=table(copy1,n2);
        char str[2]={i+'0','\0'};
        struct BigInteger il=initialize(str);
        struct BigInteger mlt=mul(n2,il);
        struct BigInteger temp=initialize("");
        listSubtract(copy1.head,mlt.head,&temp);
        if(temp.tail->data == 0){
            while(temp.tail && temp.tail != temp.head && temp.tail->data==0){
                temp.tail=temp.tail->prev;
                free(temp.tail->next);
                temp.tail->next=NULL;
                temp.size -= 1;
            }
        }
        if(!l1){
            n3=temp;
            n3.sign=n1.sign;
            break;
        }
        deleteList(&(copy1.head),&(copy1.tail));
        deleteList(&(il.head),&(il.tail));
        deleteList(&(mlt.head),&(mlt.tail));
        if(temp.tail->data != 0){
            copy1=temp;
        }
        else copy1=initialize("");
    }
    return n3;
}


void display(struct BigInteger n1){
    if(n1.size == 0) {
        printf("0\n");
        return;
    }

    if(n1.sign == -1) printf("-");
    struct node*itr=n1.tail;
    while(itr){
        printf("%d",itr->data);
        itr=itr->prev;
    }

    printf("\n");

}

