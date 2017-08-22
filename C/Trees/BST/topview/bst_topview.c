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
    return (struct view_n *)NULL;
}

void print_view_list(struct  view_n *pivot, int debug) 
{
    struct view_n *t = pivot;
    if (t) {
	 while (t->p) t = t->p;
	 if (debug) {
	    while (t) {
		printf ("\t[P(%p) view: %p] bt(%p): %d, lvl: %d\n", pivot, t, t->bt, t->bt->v, t->lvl);
		t = t->n;
	    }
	} else {
	    while (t) {
		printf ("%d ", t->bt->v);
		t = t->n;
	    }
	}
    printf("\n");
    }
}

void insert_view_node(struct view_n *pivot, 
		struct bt_node *bt, int sigma, int level_t)
{

    PRINTF("INSERT: bt: %p,v=%d, sigma: %d, level: %d\n\n", bt, bt->v, sigma, level_t);
    if (sigma < 0) {
	int sigmaVal = abs(sigma) - 1;
	struct view_n *add_left = pivot->p;
	struct view_n *stalker  = pivot;
	while (sigmaVal--) {
	    if (add_left) {
		add_left = add_left->p;
		stalker = stalker->p;
	    }
	}   
	if (add_left) {
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
	    add_left = stalker;
	    add_left->p = new_view_n(bt, level_t);
	    add_left->p->n = add_left;
	}
    } else if (sigma > 0){
	int sigmaVal = abs(sigma) - 1;
	struct view_n *add_rt = pivot->n;
	struct view_n *stalker= pivot;
	while (sigmaVal--) {
	    if (add_rt) {
		add_rt = add_rt->n;
		stalker = stalker->n;
	    }
	}
	if (add_rt) {
	    if (add_rt->lvl > level_t) {
		add_rt->bt = bt;
		add_rt->lvl=level_t;
	    }    
	} else {
	    add_rt = stalker;
	    add_rt->n = new_view_n(bt, level_t);
	    add_rt->n->p = add_rt;
	}
    }

#if DEBUG
    print_view_list(pivot, 1);
#endif
}

void create_tree_top_view(struct bt_node *R, struct view_n *pivot, int sigma, int level)
{
    if (R) {
	insert_view_node(pivot, R, sigma, level);
	create_tree_top_view(R->lt, pivot, --sigma, ++level);
	level--; sigma++;
	create_tree_top_view(R->rt, pivot, ++sigma, ++level);
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
    
    //printf(" Created btnode(%p), value: %d\n", n, n->v);
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
#if 0
    test_insert_view(); return 0;
#endif

    struct bt_node *R = NULL;
    struct view_n *pivot = NULL;
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
	    print_t(R);
            printf("\nInorder		    : ");
	    print_inorder(R);
	} else printf ("No R\n");
    }
    pivot = new_view_n(R, 0);
    create_tree_top_view(R, pivot , 0 , 0);
    printf("\nTop View	    : ");
    print_view_list(pivot, 0);
    printf ("\n\nNote: The tree drawn above may not be uniform. Recommended \n"
            " is that, it be drawn on a paper properly to have the right view.\n");
    return 0;
}



#if DEBUG
void test_insert_view()
{
    struct view_n *pivot = NULL;
    int sigmaplus = 1;
    int sigmaminus = -1;
    int level = 1;
    struct bt_node *bt = create_bt_node(0);
    pivot = new_view_n(bt, 0);
    PRINTF("PIVOT: %p\n", pivot);
    for (int i=1; i < 10; i++) {
	struct bt_node *bt = create_bt_node(i+10);
	if (i % 2) 
	    insert_view_node(pivot, bt, sigmaminus--, level);
	else
	    insert_view_node(pivot, bt, sigmaplus++, level);
	level++;
    }
    sigmaplus = 1;
    sigmaminus = -1;
    level = -10;
    PRINTF(" REINSERTS..............\n");
    for (int i=1; i < 15; i++) {
	struct bt_node *bt = create_bt_node(-i);
	if (i % 2) {
	    insert_view_node(pivot, bt, sigmaminus--, level);
	} else {
	    insert_view_node(pivot, bt, sigmaplus++, level);
	}
	level++;
    }

    printf("\n");
}
#endif

