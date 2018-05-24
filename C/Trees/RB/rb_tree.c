#include "stdtree.h"
//PNODE_RB    create_rb_node (INT32 val);
//INT32       remove_rb_node (PNODE_RB root, PNODE_RB node);  
//PNODE_RB    search_rb_node (PNODE_RB root, INT32 val); 


VOID left_rotate_rb_node (PNODE_RB root, PNODE_RB node)
{
    PNODE_RB rChild = NULL;

    if (subRoot == NULL) 
        return;

    if ((rChild = node_right_child(node)) != NULL) {
        rChild->p = node->p; 
        if (node_is_root(node)) {
            root = rChild;
        } else {
            node_parent_to_new_child(node, rChild);
        }
        if (rChild->l != NULL) {
            rChild->l->p = node;
        }
        node->p   = rChild;
        node->r   = rChild->l;
        rChild->l = node;
    }
}

VOID right_rotate_rb_node (PNODE_RB root, PNODE_RB node)
{
    PNODE_RB lChild = NULL;

    if (subRoot == NULL) 
        return;

    if ((lChild = node_left_child(node)) != NULL) {
        lChild->p = node->p; 
        if (node_is_root(node)) {
            root = lChild;
        } else {
            node_parent_to_new_child(node, lChild);
        }
        if (lChild->l != NULL) {
            lChild->r->p = node;
        }
        node->p   = lChild;
        node->l   = lChild->r;
        lChild->r = node;
    }
}

VOID insert_rb_node (PNODE_RB root, INT32 val);
{
    if (root != NULL) {
        PNODE_RB pWalk  = root;
        PNODE_RB pStalk = pWalk;
        while (pWalk != NULL) {
            pStalk = pWalk;
            if (val < pWalk->v) {
                pWalk = pWalk->l;
            } else {
                pWalk = pWalk->r;
            }
        }
        if (val < pStalk->v) {
            pStalk->l = create_rb_node(val);
            insert_fixup(root, pStalk->l);
        } else {
            pStalk->r = create_rb_node(val);
            insert_fixup(root, pStalk->r);
        }
    } else {
        root = create_rb_node(val);
    }
}

VOID insert_fixup(PNODE_RB root, PNODE_RB node)
{
    while(node_parent(node)->c == RED) {
        PNODE_RB uncle = node_uncle(node);
        if (uncle != NULL) {
            /* case 1: node's uncle is RED */
            if (uncle->c == RED) {
                /* If node's parent is left child */
                if (node_grand_parent(node)->l == node_parent(node)) {
                    node_parent(node)->c = node_uncle(node)->c = BLACK;
                } else {
                    /* node's parent is right child */
                    node_parent(node)->c = node_uncle(node)->c = BLACK;
                }
                node = node_grand_parent(node);
            } else {
                /* Case 2: Node's uncle is black and node is right child*/
                node = node_parent(node);
                left_rotate_rb_node(node, node->r);
                /* Case 3: Node's uncle is black and node is left child*/
            }
        } else {
            PRINT("Can't fixup, uncle Null, %p", node);
        }
    }

}

int main () {

return 0;
}
