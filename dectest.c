#include <stdio.h>
#include <stdlib.h>

int main()
{
		 int arr[5] = {1,2,3,4,5};
		 int index = 4;
		 printf("array element 3: %d", arr[--index]);
		 printf("index is now 3? : %d", index);
		 return 0;
}
