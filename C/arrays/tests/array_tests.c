#include <stdio.h>

int main () 
{
    int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int *first = array;
    int *last  = array + 1; 

    printf("Array: %20lu\nfirst: %20p\nlast : %20p\n", sizeof(array), first, last);

    printf ("last - first = %ld\n", last - first);
    printf ("%d\n", *first++);
    printf("Array: %20lu\nfirst: %20p\nlast : %20p\n", sizeof(array), first, last);
    printf ("last - first = %ld\n", last - first);
    return 0;
}
