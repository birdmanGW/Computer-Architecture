#include<stdio.h>
#include<stdlib.h>
#include<string.h>
 
typedef struct 
{
   int *contents;
   int maxSize;
   int top;
   
} stackT;

void StackInit(stackT *stackP, int maxSize)
{
    int *newContents;
    
    newContents = (int *)malloc(sizeof(int) * maxSize);
    
    if (newContents == NULL) 
    {
        fprintf(stderr, "Insufficient memory to initialize stack.\n");
        exit(1);
    }
    
    stackP->contents = newContents;
    stackP->maxSize = maxSize;
    stackP->top = -1;
}

void StackClear(stackT *stackP)
{
    free(stackP->contents);
    
    StackInit(stackP, 8);
} 

int StackIsEmpty(stackT *stackP)
{
    return stackP->top < 0;
}

void StackPush(stackT *stackP, int element)
{

    
    if (stackP->top >= stackP->maxSize - 1)
    {
        fprintf(stderr, "Can't push element on stack: stack is full.\n");
        return;
    }
    
    stackP->contents[++stackP->top] = element;
//debugg//  printf("successfully pushed %d on the stack in place %d, maxSize: %d\n",element, stackP->top, stackP->maxSize-1);
    
}

int StackPop(stackT *stackP)
{
    if (StackIsEmpty(stackP))
    {
        fprintf(stderr, "Can't pop element from stack: stack is empty.\n");
        return 0;
    }
    
    return stackP->contents[stackP->top--];
}

void OpAdd(stackT *stackP)
{
    int op1, op2, ans;
    
    if (!StackIsEmpty(stackP)) 
    {
        op1 = StackPop(stackP);
    } else {return;}
    
    if (!StackIsEmpty(stackP))
    {
        op2 = StackPop(stackP);
    }
    else
    {
        StackPush(stackP, op1);
        return;
    }
    
    ans = op1 + op2;
    
    if (-999 <= ans && ans <= 999)
    {
        StackPush(stackP, ans);
    } 
    else
    {
        printf("Answer is out-of-bounds.\n");
        StackPush(stackP, op2);
        StackPush(stackP, op1);
    }
    
}

void OpSub(stackT *stackP)
{
    int op1, op2, ans;
    
    if (!StackIsEmpty(stackP)) 
    {
        op1 = StackPop(stackP);
    } else {return;}
    
    if (!StackIsEmpty(stackP))
    {
        op2 = StackPop(stackP);
    }
    else
    {
        StackPush(stackP, op1);
        return;
    }
    
    ans = op1 - op2;
    
    if (-999 <= ans && ans <= 999)
    {
        StackPush(stackP, ans);
    } 
    else
    {
        printf("Answer is out-of-bounds.\n");
        StackPush(stackP, op2);
        StackPush(stackP, op1);
    }
    
}

void OpMult(stackT *stackP)
{
    int op1, op2, ans;
    
    if (!StackIsEmpty(stackP)) 
    {
        op1 = StackPop(stackP);
    } else {return;}
    
    if (!StackIsEmpty(stackP))
    {
        op2 = StackPop(stackP);
    }
    else
    {
        StackPush(stackP, op1);
        return;
    }
    
    
    ans = op1 * op2;
    
    if (-999 <= ans && ans <= 999)
    {
        StackPush(stackP, ans);
    } 
    else
    {
        printf("Answer is out-of-bounds.\n");
        StackPush(stackP, op2);
        StackPush(stackP, op1);
    }
}

void OpDisplay(stackT *stackP)
{
    int ans;
    
    if(!StackIsEmpty(stackP))
    {
        ans = StackPop(stackP);    
    }
    else
    {
        printf("Nothing to display here.\n");
        return;
    }
    
    
    printf("%d\n", ans);
    StackPush(stackP, ans);
}



void main ()
{
    stackT stack;
    StackInit(&stack, 9);
    
    char input[256];
    int op;
    
    while (1) 
    {

        printf("Enter a command: "); 
        fgets(input, 256, stdin);
        
        
        if (input[0] == 'X') 
        {
            printf("Halting processor.\n");
            exit(1);
        } 
        else if (input[0] == 'C')
        {
            StackClear(&stack);
        }
        else if (input[0] == '+')
        {
            OpAdd(&stack);
        }
        else if (input[0] == '-')
        {
            OpSub(&stack);
        }
        else if (input[0] == '*')
        {
            OpMult(&stack);
        }
        else if (input[0] == 'D')
        {
            OpDisplay(&stack);
        }
        else
        {
            op = atoi (input);
            
            if (op >= 1000 || op <= -1000 )
            {
                printf("Too many digits.\n");
                
            }
            else
            {
                StackPush(&stack, op);
            }
        }
        
    }
}