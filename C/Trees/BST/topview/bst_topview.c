#include "bst.h"

struct view_n *new_view_n(struct bt_node *bt, int l)
{
    struct view_n *vn = (struct view_n *) malloc(sizeof(struct view_n));
    if (vn) {
	vn->p = NULL;
	vn->n = NULL;
	vn->bt = bt;
	vn->lvl = l;
	return vn;
    }
    return NULL;
}

void print_view_list(struct  view_n *pivot) 
{
    struct view_n *t = pivot;
    if (t) {
	 while (t->p) t = t->p;
	 while (t) {
		printf ("%p:: %d, ", t, t->bt->v);
		t = t->n;
	}
    }
}

void insert_view_node(struct view_n *pivot, 
		struct bt_node *bt, int sigma, int level_t)
{
    if (pivot == NULL) 
	pivot = new_view_n(bt, level_t); /* Root node level is 0 */
    else if (sigma < 0) {
	int sigmaVal = abs(sigma);
	struct view_n *add_left = pivot;
	while (sigmaVal--) add_left = add_left->p;
	if (add_left->p) {
	    /* already a tree node is added at this sigma value.
             * But the new value suggested by the algorithm may contains
             * level value lesser than whats present in the view node.
             * We will replace the view node's pointer to this bt_node, as 
             * this is at a lesser level value hence closer to the root node
             * hence shadows the view nodes bt node.
             */
	    if (add_left->lvl > level_t) {
		add_left->bt = bt;
		add_left->lvl=level_t;
	    }
	} else {
	    add_left->p = new_view_n(bt, level_t);
	    add_left->p->n = add_left;
	}
    } else {
	int sigmaVal = abs(sigma);
	struct view_n *add_rt = pivot;
	while (sigmaVal--) add_rt = add_rt->n;
	if (add_rt->n) {
	    if (add_rt->lvl > level_t) {
		add_rt->bt = bt;
		add_rt->lvl=level_t;
	    }    
	} else {
	    add_rt->n = new_view_n(bt, level_t);
	    add_rt->n->p = add_rt;
	}
    }

    print_view_list(pivot);
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
}

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
	} else printf ("No R\n");
    }
    printf("\n");
    
return 0;
}
