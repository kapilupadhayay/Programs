#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ARRAY_SIZE 16
void display_tupples(int *tupples, int num_ele, int cardinality)
{
    printf("("); 
    for ( int i = 0; i < num_ele; i++)  {
        if ((i % cardinality) == 0) { 
            printf("  %d,", tupples[i]); 
        } else {
            printf("%d,", tupples[i]);
        }
    }
    printf(")\n");
}

int intersects (int tupple1[], int cardinality1, int tupple2[], int cardinality2)
{
    //printf ("      tupple1: "); display_tupples(tupple1, cardinality1, cardinality1);
    //printf ("final tupple2: ");display_tupples(tupple2, cardinality2, cardinality2);

    for (int i = 0; i < cardinality1; i++) {
        int num_matched = 0;
        for (int j = 0; j < cardinality2; j++) {
            if ( tupple1[i] == tupple2[j]) {
                if (++num_matched == cardinality2)
                    //printf ("Above 2 tupples intersect...... YES \n");
                    return 1;   // intersects
            }
        }
    }

    //printf ("Above 2 tupples intersect...... NO \n");
    return 0; // mutually exclusive
}

uint64_t factorial (uint64_t n)
{
    if (n == 1) return 1;
    
    return n * factorial(n-1);
}

uint64_t nCr_tupples(uint64_t n, uint64_t r)
{
    if (n >= r) {
        printf ("%luC%lu = %lu\n",n,r, (factorial(n) /((factorial(n-r) * factorial(r)))));
        printf (" %lu\n",(factorial(n)));
        printf (" %lu\n",factorial(n-r));
        printf (" %lu\n",factorial(r) );
        return (factorial(n) /((factorial(n-r) * factorial(r))));
    } else {
        printf ("Error: n < r (%lu, %lu)\n", n, r);
    }
}

int sum_tupple(int *tupple, int cardinality)
{
    int i = 0;
    int sum = 0;
    for (int i = 0; i < cardinality; i++) 
        sum += tupple[i];

    return sum;
}


int * get_next_size_tupples (int seq[ARRAY_SIZE], int tupples[], int num_ele, int cardinality, int sum, int *next_tupple_nele)
{
    int tupple_i = 0;
    int seq_i = 0;
    int *next_tupples = NULL;
    int *next_tupples2 = NULL;
    *next_tupple_nele = 0;
    for (; tupple_i < num_ele; tupple_i += cardinality) {
        for (seq_i = 0; seq_i < ARRAY_SIZE; seq_i++) {
            int tmp [] = {seq[seq_i]};
            if (!intersects(tupples + tupple_i, cardinality, tmp, 1) && 
                    (sum_tupple(tupples + tupple_i, cardinality) + seq[seq_i]) == sum ) {
                int i = 0;
                next_tupples2 = (int *) realloc(next_tupples,(*next_tupple_nele + cardinality + 1) * sizeof(int)); 

                for (i = 0; i < cardinality; i++) {
                        next_tupples2[*next_tupple_nele + i] = tupples[tupple_i + i] ;
                }
                next_tupples2[*next_tupple_nele + i] = seq[seq_i];
                //printf("added (%d %d %d)\n", next_tupples2[*next_tupple_nele + i-2], next_tupples2[*next_tupple_nele + i-1], next_tupples2[*next_tupple_nele + i]);
                *next_tupple_nele += (cardinality + 1);
                next_tupples = next_tupples2;
            }
        }
    }

    return next_tupples;
}

int *get_2_tupples(int seq[ARRAY_SIZE], int *size)
{
    int cardinality = 2;    // num ele in tupple.
    int num_ele = nCr_tupples(ARRAY_SIZE, 2) * cardinality;
    int *tupples = (int *) malloc ( sizeof(int) * num_ele);
    //printf ("%dC%d: %d tupples\n", ARRAY_SIZE, 2, num_ele/cardinality);
    if (tupples) {
        int i = 0;
        int n = 0;
        for (; i < ARRAY_SIZE; i++) {
            int j = i + 1; 
            for (; j < ARRAY_SIZE; j++) {
                int k = n;
                tupples[n++] = seq[i];
                tupples[n++] = seq[j];
                //printf (" %d, %d\n",tupples [k], tupples[k+1]);
            }

        }
        *size = num_ele;
        return tupples;
    }
    
    return NULL;
}

int *get_all_solutions(int *tupples, int num_ele, int cardinality, int num_final_tupples)
{
    int tupple_i = 0;
    int tupple_f = 0;
    int *final_tupples = (int *) calloc (cardinality * num_final_tupples, sizeof(int));
    int num_tupples_in_final = 0;
    int i = 0;
    /*
    printf("("); 
    for ( int i = 0; i < num_ele; i++)  {
        if ((i % cardinality) == 0) { 
            printf("  %d,", tupples[i]); 
        } else {
            printf("%d,", tupples[i]);
        }
    }
    printf(")\n");
    */

    for (tupple_i = 0; tupple_i < num_ele; tupple_i = tupple_i + cardinality) {
        int tupple_intersects = 0;
        for (tupple_f = 0; tupple_f < (num_tupples_in_final * cardinality); tupple_f = (tupple_f + cardinality)) {
            //printf ("tupple_i: %d, tupple_f: %d num_tupples_in_final: %d\n", tupple_i, tupple_f, num_tupples_in_final);
            if (intersects(tupples + tupple_i, cardinality, final_tupples + tupple_f, cardinality)) {
                tupple_intersects = 1;
                break;
            }
        }
        //printf("...........................................................tupple_intersects: %d\n", tupple_intersects);
        if (!tupple_intersects) {
            int i = 0;
            //printf ("Adding tupple to final list: (");
            for (i=0; i < cardinality; i++) {
                final_tupples[num_tupples_in_final * cardinality + i] = tupples[ tupple_i + i ];
                printf ("%d,", final_tupples[num_tupples_in_final * cardinality + i]);
            }
            printf(")\n");
            num_tupples_in_final++;
        }

        if (num_tupples_in_final == num_final_tupples) {
            printf ("Found Solution\n");
            display_tupples(final_tupples, num_final_tupples * cardinality, cardinality);
            num_tupples_in_final = 0;
            memset(final_tupples, 0,cardinality * num_final_tupples * sizeof(int));
        }
    }

    return final_tupples;
}

int main() 
{
    int seq[ARRAY_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int tupple_array_size = 0;
    int cardinality = 2;
    int *tupples_n = get_2_tupples(seq, &tupple_array_size);
    int *tupples_nplus1 = NULL;
    int *final_tupples = NULL;
    int sum = 0;
    int final_cardinality = 4;
    int num_tupples_in_sol = 4;
    int seq_sz = sizeof(seq)/sizeof(seq[0]);
    for (int i = 0; i < seq_sz; i++) {
        sum += seq[i];
    }
    sum /= final_cardinality;
    printf ("Sum = %d\n", sum);
    printf ("Cardinality = %d\n", cardinality);
    display_tupples(tupples_n, tupple_array_size, cardinality);
    while (cardinality != final_cardinality) {
        tupples_nplus1 = get_next_size_tupples (seq, tupples_n, tupple_array_size, cardinality, sum, &tupple_array_size);
        free (tupples_n);
        tupples_n = tupples_nplus1;
        cardinality++;
        printf ("Cardinality = %d\n", cardinality );
        display_tupples(tupples_n, tupple_array_size, cardinality);
    }
    get_all_solutions(tupples_nplus1, tupple_array_size, final_cardinality, num_tupples_in_sol); //last 3 is num final tuples expected
    if (tupples_nplus1) {
        free (tupples_nplus1);
    }
}

#if 0
int main() 
{
    int seq[ARRAY_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int tupple_array_size = 0;
    int *tupples_2 = get_2_tupples(seq, &tupple_array_size);
    int *tupples_3 = NULL;
    int *final_tupples = NULL;

    tupples_3 = get_next_size_tupples (seq,tupples_2, tupple_array_size, 2, 15, &tupple_array_size);
    //display_tupples(tupples_3, tupple_array_size, 3);
    get_all_solutions(tupples_3, tupple_array_size, 3, 3); //last 3 is num final tuples expected

    if (tupples_2) {
        free (tupples_2);
    }
    if (tupples_3) {
        free (tupples_3);
    }

}
#endif
