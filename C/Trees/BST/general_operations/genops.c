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
        PRINTF("%p : %d\n", n, val);
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

return NULL;
}

void print_inorder_tree (struct bt_node *root)
{
    if (root) {
        print_inorder_tree(root->lt);
        printf (" %d ",root->v);
        print_inorder_tree(root->rt);
    }
}

void print_preorder_tree (struct bt_node *root)
{
    if (root) {
        printf (" %d ",root->v);
        print_inorder_tree(root->lt);
        print_inorder_tree(root->rt);
    }
}

int get_subtree_height(struct bt_node *root)
{
    int lh = 0;
    int rh = 0;
    if (root) {

        printf ("%d \n", root->v);
        if (root->lt == root->rt)
            return 0;

        lh = get_subtree_height(root->lt) + 1;
        rh = get_subtree_height(root->rt) + 1;
        printf ("%d height: %d\n", root->v, max(lh,rh));
    }

    return max(lh, rh);
}


int is_balanced(struct bt_node *root)
{
    int lh = 0;
    int rh = 0;

    if (root) {
        if (root->lt == root->rt) {
            return TRUE;
        }

        if (root->lt)
            lh = get_subtree_height(root->lt) + 1;
        if (root->rt)
            rh = get_subtree_height(root->rt) + 1;

        printf ("root: %d lh: %d rh: %d\n", root->v, lh, rh);
        if (abs(lh - rh) <= 1 ) {
            return TRUE;
        }
    }

    return FALSE;
}

void level_order(PBFS_QUEUE Q, struct bt_node *root, int h)
{
    struct bt_node *last = NULL;
    int max_width = h;

    while (!BFS_queue_empty(Q)) {
        struct bt_node *cur = BFS_dequeue(Q);

        if (last && cur && cur->v < last->v) {
            printf("\n");
            h--;
         } 
         
         if (cur != NULL) {
            printf("%*d", max_width*h, cur->v);
         }
         
         if (cur->lt) {
            BFS_enqueue(Q, cur->lt);
         }
         
         if (cur->rt) {
            BFS_enqueue(Q, cur->rt);
         }

         last = cur;
    }
}

void print_level_order(struct bt_node *root)
{
    int h = get_subtree_height(root);
    BFS_QUEUE Q;

    BFS_enqueue(&Q, root);
    level_order(&Q, root, h);
}

/* For tree traversal during recursion, the aim is to keep a pointer head that
   always points to the previous node. Do not undermine leaf's null pointers 
    This function forms the ll attached through lt and rt pointers in the node.
   Also note that legend is  prev = lt and  next = rt from tree and ll view points.
 
   The problem statement is formally defined here:
   http://cslibrary.stanford.edu/109/TreeListRecursion.html
   and below is my solution to it.
   */

void tree_to_list (struct bt_node *root, struct bt_node **head)
{
    if (root) {
        tree_to_list(root->lt, head);
        
        if(*head) {
            /* This sets the next pointers in the ll. At this time the 
               recursion returns from completing the processing for the
               left node of the root. The head points to the previous 
               node in the root's left subtree.
            */
            (*head)->rt = root;
        }
        root->lt = *head;
        /* Move the head now to the next node in ll sequence, which is the 
           root. When the roots left subtree finishes, root is the next
           node in sequence.
        */
        *head    = root;
        tree_to_list(root->rt, head);
    }
}

struct bt_node *convert_tree_to_ll(struct bt_node *R)
{
    struct bt_node *leftest = R;
    struct bt_node *rightest= R;
    struct bt_node *head= NULL;

    while (leftest->lt) {
        leftest = leftest->lt;
    }

    while (rightest->rt) {
        rightest = rightest->rt;
    }

    tree_to_list(R, &head);
    leftest->lt = rightest;
    rightest->rt = leftest;

    /*
    printf("Tree Converted to Double LL Reverse: \n");
    head = rightest;
    do {
        printf("%d, ", head->v);
        head = head->lt;
    } while(head != rightest);
    */
    printf("\nTree Converted to Double LL: \n");
    head = leftest;
    do {
        printf("%d, ", head->v);
        head = head->rt;
    } while(head != leftest);
    printf("\n");

    return leftest;

}

void cleanse (struct bt_node *node)
{
    node->lt = node->rt = NULL;
}

int get_node_count_between(struct bt_node  *first, struct bt_node *last) 
{
    int count = 1;
    if (first == NULL) {
        return 0;
    }

    if (first == last) {
        return count;
    }

    do {
        count++;
    } while((first = first->rt) != last);

    return count;
}

struct bt_node * get_root_node (struct bt_node  *first, struct bt_node *last)
{
    int count = get_node_count_between(first, last);
    int mid = ((count % 2) ? (count / 2) + 1 : count/2) - 1;

    printf ("Nodes [%d, %d] count = %d mid = %d\n", first->v, last->v, count, mid);
    while (mid--) {
        first = first->rt;
    } 

    return first;
}

struct bt_node * ll_to_tree (struct bt_node *root, struct bt_node  *first, struct bt_node *last)
{
    printf("F: [%p], %d,     root: [%p], %d,    L: [%p], %d\n",
                first, first->v, root, root->v,  last->lt, last->v);
    if (first == root) {
        cleanse(root);
        if (root != last) {
            cleanse(last);
            root->rt = last;
        }
        return root;
    } else if (root == last) {
        cleanse(root);
        if (root != first) {
            cleanse(first);
            root->lt = first;
        }
        return root;
    } else {
        struct bt_node *rroot = NULL;
        struct bt_node *lroot = get_root_node(first, root->lt);
        root->lt    =   ll_to_tree(lroot, first, root->lt);
        rroot = get_root_node(root->rt, last);
        root->rt    =   ll_to_tree(rroot, root->rt, last);
    }

    return root;
}

struct bt_node *convert_ll_to_tree(struct bt_node *head)
{
    //struct bt_node *root = get_root_node(head, head->lt);
    //printf("F: [%p], %d,     root: [%p], %d,    L: [%p], %d\n",
    //            head, head->v, root, root->v,  head->lt, head->lt->v);
    return ll_to_tree(get_root_node(head, head->lt), head, head->lt);
}

void loop_operation(struct bt_node *R)
{
    char n = 'z';
    printf ("i : Insert, d : delete, s : search, N : nth Bigges, n : nth Smallest, p : print tree, h: height, b: balanced? t: toLinkList: l:lltoTree\n");

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
                printf("\n");
                //print_t(R);
                print_inorder_tree(R);
                printf("\n");
                break;
            }
            case 'h':
                printf("Height: %d\n", get_subtree_height(R));
                break;
            case 'b':
                printf(" %s\n", is_balanced(R) ? "Balanced\n" : "Not Balanced\n");
                break;
            case 't':
                convert_tree_to_ll(R);
                break;
            case 'l': {
                struct bt_node *head = convert_tree_to_ll(R);
                struct bt_node *treeRoot = convert_ll_to_tree(head);
                printf("LL converted to tree, inoder traversal:\n");
                print_inorder_tree(treeRoot);

                print_level_order(treeRoot);
                printf ("\n");
                break;
                }

        }   // end switch
        printf ("i : Insert, d : delete, s : search, N : nth Biggest"
                ", n : nth Smallest, p : print tree, h: height, b: balanced, t:ttoLL, l: lltoTree?\n");
    }
    printf("Bye bye\n");
}


int main(int argc, char *argv[])
{
    int i;
    struct bt_node *R = NULL;
    for (i = 1;i < argc ; i++) {
        //PRINTF ("Add %d\n", atoi(argv[i]));
        R = add_bt_node(R, atoi(argv[i]));
    }
    // switch to interactive operations
    loop_operation(R);
    print_t(R);
    free_tree(R);
    return 0;
}
