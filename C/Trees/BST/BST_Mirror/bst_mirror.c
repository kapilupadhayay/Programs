#include "bst.h"

void swap_siblings(struct bt_node *p)
{
    struct bt_node *tmp = p->lt;
    p->lt = p->rt;
    p->rt = tmp;
}

void mirror_subroot(struct bt_node *R)
{
    if (R) {
	mirror_subroot(R->lt);
	mirror_subroot(R->rt);
	// swap L and R subtrees
	swap_siblings(R);
    }
}

void print_inorder_rev(struct bt_node *R)
{
    if(R) {
        print_inorder_rev(R->rt);
        printf("%d ", R->v);
        print_inorder_rev(R->lt);
    }
}

void print_inorder(struct bt_node *R)
{
    if (R) {
        print_inorder(R->lt);
        printf("%d ", R->v);
        print_inorder(R->rt);
    }
}

struct bt_node *create_bt_node(int val)
{
    struct bt_node *n = (struct bt_node *) malloc(sizeof(struct bt_node));
    if (n) {
        n->v = val;
        n->lt = NULL;
        n->rt = NULL;
    } else {
        return (struct bt_node *)NULL;
    }
    
    return n;
}

void add_bt_node(struct bt_node *R, struct bt_node *n)
{
#if 1
    if (R) {
        if (n->v < R->v) {
            add_bt_node(R->lt, n);
            if (!R->lt)
                R->lt = n;
        } else if  (n->v > R->v){
            add_bt_node(R->rt, n);
            if (!R->rt)
                R->rt = n;
        }
    }
#endif
#if 0
    if (n->v < R->v) {
        if (R->lt)
            add_bt_node(R->lt, n);
        else 
            R->lt = n;
    } else if (n->v > R->v) {
        if (R->rt)
            add_bt_node(R->rt, n);
        else
            R->rt = n;
    }    
#endif
}

/* As a matter to check for the correctness of the mirrored
 * tree imperically, the inorder traversal of the original 
 * tree and the reverse-inorder traversal of the mirrored 
 * should be same.
 */

int main (int argc, char * argv[])
{

    struct bt_node *R = NULL;
    if (argc > 1) {
	R =  create_bt_node(atoi(argv[1]));
        if (R) {
    	    for (int i=2; i < argc; i++) {
	        struct bt_node *n = create_bt_node(atoi(argv[i]));
                if (!n) {
                    printf("Failure in node creation. Exiting\n");
                    return 0;
                }
	        add_bt_node(R, n);
	    }
	    // Pictorial view of the tree. Uncomment print_t
	    //print_t(R);
            printf("\nInorder		    : ");
	    print_inorder(R);
            printf("\nInorder Rev	    : ");
	    print_inorder_rev(R);
	} else printf ("No R\n");
    }
    printf("\n");
    mirror_subroot(R);
    printf("\nMirror Inorder Rev  : ");
    print_inorder_rev(R);
    printf("\n");
    // Pictorial view of the tree. Uncomment print_t
    //print_t(R);
    
return 0;
}
