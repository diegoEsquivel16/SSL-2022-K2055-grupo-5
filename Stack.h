#ifndef STACK_H_
#define STACK_H_

struct stack* newStack(int capacity);
int size(struct stack *pt);
int isEmpty(struct stack *pt);
int isFull(struct stack *pt);
void push(struct stack *pt, int x);
int peek(struct stack *pt);
int pop(struct stack *pt);

#endif /* STACK_H_ */