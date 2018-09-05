#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 13
int main (int argc, char *argv[])
{
    int seq[ARRAY_SIZE] = { 12, 16, 22, 30, 35, 39, 42, 45, 48, 50, 53, 55, 56 };
    int *left  = seq;
    int *right = seq + ARRAY_SIZE;
    int k = 4;                //n closest
    int index = -1;
    int found = 0;
    int i;
    int number = atoi(argv[1]);

    for (i = 0; i < ARRAY_SIZE; i++) {
        printf ( "%d ", seq[i]);
        if (seq[i] == number) {
            index = i;
        }
    }

    printf ("\nnumber: %d\n", number);
    left  = seq + index - 1;
    right = seq + index + 1;

    //printf ("seq: %p index[%d] @: %p\n", seq, index, seq+index);
    while (found != k && right - left < ARRAY_SIZE) {
        if (left < seq) {
            printf(" %d ", *right++);
            found++;
            continue;
        }

        if (right >= seq + ARRAY_SIZE) {
            printf(" %d ", *left--);
            found++;
            continue;
        }

        //printf("left: %p(%d)\n righ:%p(%d)\n", left, *left, right, *right);
        if ((seq[index] - *left) < (*right - seq[index])) {
            printf(" %d ", *left--);
            found++;
        } else if ((seq[index] - *left) > (*right - seq[index])) {
            printf(" %d ", *right++);
            found++;
        } else {
            printf(" %d  %d", *left--, *right++);
            found += 2;
        }
    }
    printf ("\n");
}
