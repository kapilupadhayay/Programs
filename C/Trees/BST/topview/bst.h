#ifndef _BSTHEADER
#define _BSTHEADER
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct bt_node {
    struct bt_node *lt, *rt;
    int v;
};

/* The list that contains the pointer to
 * the nodes in the tree that are visible
 * as the top view.
 */
struct view_n {
    struct bt_node *bt;
    struct view_n *p;
    struct view_n *n;
    int lvl; /* level_t: level of  a node in tree*/
};

struct bt_node *create_bt_node(int );
/* sigma is the how much deviation from centeral axis.
 * The vertical axis passing through Root node of the 
 * tree forms the central axis. We follow the convention
 * like:
 *  i.  a left child is at sigma val -1 from its parent.
 *  ii. a right child at sigma val +1 from its parent.
 * Thus a node at two times left of Root node has sigma -2
 * similarly, a node which is at first Right then left is at 
 * sigma 0.
 * For the tree formed af the values provided:
 * 50 80 45 70 5 15 35 90 100 50 110
 *
 *	                 [0](050)[0] 
 *			 +----+---------+
 *		    [1](045)[-1]   [1](080)[+1]
 *	  +--------------+         +----+----+
 *   [2](005)[-2]             [2](070)[0] [2](090)[+2]
 *	  +----+                             +----+
 *	  [3](015)[-1]                       [3](100)[+3]
 *	       +----+                             +----+
 *		[4](035)[0]                        [4](110)[+4]
 *
 * Convention: 
 * A node in the tree above should be read as:
 * [Level_from_Root] (Node_Value) [Sigma_Sign Sigma_Value]
 * where the sign convention are mentioned in i. and ii. above.
 */   
void insert_view_node(struct view_n *, struct bt_node *, 
	    int sigma, int level_t);
void add_bt_node(struct bt_node *, struct bt_node *);
void del_bt_node(struct bt_node *, int );
void print_inorder(struct bt_node *);
void print_inorder_rev(struct bt_node *); // reverse in order
void print_t(struct bt_node *);
#endif
