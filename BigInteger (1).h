#ifndef BIGINTEGER_H
#define BIGINTEGER_H
struct BigInteger
{
    struct node*head,*tail;
    int size;
    int sign;
};
struct node
{
    int data;
    struct node*next,*prev;
};
struct BigInteger initialize(char*);
struct BigInteger add(struct BigInteger,struct BigInteger);
struct BigInteger sub(struct BigInteger,struct BigInteger);
struct BigInteger mul(struct BigInteger,struct BigInteger);
struct BigInteger div1(struct BigInteger,struct BigInteger);
struct BigInteger mod(struct BigInteger,struct BigInteger);
void display(struct BigInteger);
#endif