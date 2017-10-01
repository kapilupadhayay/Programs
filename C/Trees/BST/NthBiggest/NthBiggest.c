#include "bst.h"

int get_value_from_user(void)
{
    int val = -1;
    printf ("Input a Value[0-1000] : "); 
    scanf("%d", &val);
    return val;
}

struct bt_node * create_bt_node (int val)
{
    struct bt_node *n = (struct bt_node *) 
                    calloc(1, sizeof(struct bt_node));
    if (n) {
        n->v = val;
        PRINTF("Created bt node for: %d\n", val);
        return n;
    }
    return NULL;
}


struct bt_node * add_bt_node (struct bt_node *r, int val) 
{
    if (!r) return create_bt_node(val);
    if (val < r->v)
        r->lt = add_bt_node(r->lt, val);
    else
        r->rt = add_bt_node(r->rt, val);
    return r;
}


void free_tree(struct bt_node *r) 
{
    if (r) {
        free_tree(r->lt);
        free_tree(r->rt);
        PRINTF("Freeing Root node: %d\n", r->v);
        free(r->lt);
    }
    
}
struct bt_node * search_bt_node(struct bt_node * r,
                                    int val)
{
    printf("Searching %d\n", val);
    if (r) {
        if (r->v == val) return r;
        if (val < r->v)
            return search_bt_node(r->lt, val);
        else
            return search_bt_node(r->rt, val);
    }
    return NULL;
}

struct bt_node * get_smallest_bt_node(struct bt_node *r)
{
    struct bt_node *successor = r;
    if (!r) return r;
    while (successor->lt) {
        successor = successor->lt;
    }
    return successor;
}

struct bt_node * delete_bt_node(struct bt_node *r, int val)
{
    if (!r) return NULL;
    if (val < r->v) {
        r->lt = delete_bt_node (r->lt, val);
    } else if (val > r->v) {
        r->rt = delete_bt_node(r->rt, val);
    } else {
        // r is at leaf
        if (r->lt == NULL && r->rt == NULL) {
            free(r);
            return NULL;
        }
        // r has two children
        if (r->lt != NULL && r->rt != NULL) {
            // inorder successor of r
            struct bt_node *s = get_smallest_bt_node(r->rt);
            r->v = s->v;
            r->rt = delete_bt_node(r->rt, s->v);
        } else if (r->lt != NULL) { // r has only left child
            struct bt_node *d = r->lt;
            free(r);
            return d;
        } else {                    // r has only right child
            struct bt_node *d = r->rt;
            free(r);
            return d;
        }
    }
    return r;
}

struct bt_node * Nth_biggest_bt_node(struct bt_node *r, int nth)
{

}


void loop_operation(struct bt_node *R)
{
    char n = 'z';
    printf ("i : Insert, d : delete, s : search, N : nth Bigges, n : nth Smallest, p : print tree\n");

    while (n != 'e') { // e means exit
        scanf(" %c",&n);
        printf("entered: %c\n",n);
        switch (n) {
            case 'i': { 
                add_bt_node(R, get_value_from_user());
                break;
            }
            case 'd': {
                delete_bt_node(R, get_value_from_user());
                break;
            }
             case 's': {
                struct bt_node *d = NULL;
                d = search_bt_node(R, get_value_from_user());
                if (d) 
                    printf("loacted at Node: %p:%d\n", d, d->v);
                else
                    printf("No such value\n");
                break;
            }       
            case 'n': {
                break;
            }
            case 'N': {
                break;
            }
            case 'p': {
                print_t(R);
                break;
            }
        }   // end switch
        printf ("i : Insert, d : delete, s : search, N : nth Biggest"
                ", n : nth Smallest, p : print tree\n");
    }
    printf("Bye bye\n");
}


int main(int argc, char *argv[])
{
    int i;
    struct bt_node *R = NULL;
    for (i = 1;i < argc ; i++) {
        PRINTF ("Add %d\n", atoi(argv[i]));
        R = add_bt_node(R, atoi(argv[i]));
    }
    // switch to interactive operations
    loop_operation(R);
    print_t(R);
    free_tree(R);
    return 0;
}
