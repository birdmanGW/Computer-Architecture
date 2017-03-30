#include<stdio.h>

void main() 
{
    char input[256];
    int count = 0;
    int i;
    
    printf("Enter a string: ");
    fgets(input, 256, stdin);
    
    for ( i = 0; i < 256; i++) 
    {
        if (input[i] == ' ') 
        {
            count++;
        }
      
    }
    
    printf("You entered %d words.\n", count);
    
}