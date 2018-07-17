/* Regular expression processing.
 * Give it something like: ./bin "cabccbacbacab" "*c*cb*ab"
 * Do remeember to delimit using "" marks otherwise
 * shell interprets the args.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GAP_CHARACTER '*'
#define NULL_CHARACTER '\0'
#define TRUE    1
#define FALSE   0

char *get_next_sp(char *p, char **p_ptr) 
{
    char *p_init = *p_ptr;
    int p_size = 0;
    char *sub_pattern = NULL;
    int i = 0;

    while(**p_ptr == GAP_CHARACTER) {
        ++(*p_ptr);
    }
    
    p_init = *p_ptr;

    while(1) {
        if (**p_ptr == NULL_CHARACTER || **p_ptr == GAP_CHARACTER)
            break;
        ++(*p_ptr);
    }

    p_size = *p_ptr - p_init;
    sub_pattern = (char *) malloc (p_size + 2);
    memset(sub_pattern, 0, p_size + 1);

    while( p_init != *p_ptr) {
        sub_pattern[i++] = *p_init;
        p_init++;
    }

    return sub_pattern;
}

char *find_substr_with_gap(char *s_ptr, char *sub_pattern)
{
    char *sp_ptr = (sub_pattern[0] == GAP_CHARACTER) ? sub_pattern + 1 : sub_pattern;
    char *s_init = sp_ptr;

    printf("Find subpattern |%7s| in |%15s|: ", sub_pattern, s_ptr);
    while (*s_ptr != *sp_ptr && *s_ptr != NULL_CHARACTER) {
        (s_ptr)++;
    }

    if (*s_ptr == NULL_CHARACTER && *sp_ptr == NULL_CHARACTER) {
        return s_ptr;
    } else if (*s_ptr == NULL_CHARACTER) {
        return NULL;
    } else {
        while(1) {
            if (*sp_ptr == NULL_CHARACTER || *s_ptr == NULL_CHARACTER) {
                break;
            } else if (*s_ptr == *sp_ptr) {
                (s_ptr)++;
                sp_ptr++;
            } else {
                (s_ptr)++;
                sp_ptr = s_init;;
            }
        }

        if(*sp_ptr == NULL_CHARACTER) {
            return s_ptr;
        } else {
            return NULL;
        }
    }
}

int main (int argc, char *argv[])
{

    char *s = argv[1];
    char *p = argv[2];
    char *sp = NULL;

    printf("INIT: s=%p:\t     |%15s|\n\n",s, s);
    while(*p != NULL_CHARACTER) {
        sp = get_next_sp(argv[2], &p);
        s = find_substr_with_gap(s, sp);
        printf("found at: s=%p\n",s-strlen(sp));

        if(sp)
            free(sp);

        if (s == NULL) {
            printf("Not Found\n");
            return FALSE;
        }
    }

    printf("Found\n");
    return TRUE;
}
