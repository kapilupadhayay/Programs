/* right rotate
 * rr is implemented in terms of lr.
 */

#include <stdio.h>
#include <stdlib.h>

#define SEQ_SIZE 7
void lrotate(int seq[], int nr)
{
    int try = 0;
    int i = 0;
    int num_rotations  = nr % SEQ_SIZE;

    for (try=0; try < num_rotations; try++) {
        int tmp = seq[0];
        for (i = 0; i < SEQ_SIZE - 1; i++) {
            seq[i] = seq[i+1];
        }
        seq[i] = tmp;

    }
}

int main (int argc, char *argv[])
{
    int i = 0;
    int seq[7] = { 1, 2, 3, 4, 5, 6, 7 };
    /* lr and rr are modulo operations */
    lrotate(seq, SEQ_SIZE - atoi(argv[1]));
        for (i = 0; i < SEQ_SIZE ; i++)
            printf("%d ",seq[i]);
        printf("\n");

    return 0;
}
