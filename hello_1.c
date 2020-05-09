#include "stdio.h"

void main(void)
{
    int a;
    printf("Enter a interger number:");
    scanf("%d",&a);
    if (a > 0)
    {
        printf("Hello, Big, a= %d\n",a);
    }
    else
    {
        printf("Hello,small,a=%d\n",a);
    }
    printf("Press any key to exit ...\n");
    getchar();

}