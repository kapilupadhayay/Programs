#include <stdio.h>
#include <stdlib.h>

typedef struct BST_NODE {
    struct BST_NODE *l;
    struct BST_NODE *r;
    int pos;
    int v;
}BST_NODE, *PBST_NODE;

PBST_NODE BST_create(int v) 
{
    PBST_NODE n = (PBST_NODE) malloc (sizeof(BST_NODE));
    if (n)
        n->v = v;
    return n;
}

void BST_insert (PBST_NODE root, int v)
{
    if (root) {
        if ( v <= root->v) {
            if (root->l) {
                BST_insert(root->l, v);
            } else {
                root->l = BST_create(v);
            }
        } else {
            if (root->r) {
                BST_insert(root->r, v);
            } else {
                root->r = BST_create(v);
            }
        }
    }
}

void mark_nodes (PBST_NODE root, int *num)
{
    if (root) {
        if (root->l)
            mark_nodes(root->l, num);

        root->pos = (*num)++;

        if(root->r)
            mark_nodes(root->r, num);
    }
}

void display_nth_largest(PBST_NODE root, int pos) 
{
     if (root) {
        display_nth_largest(root->l, pos);
        if (pos == root->pos)
            printf("%dth Largest node has: %d ",pos, root->v);
        display_nth_largest(root->r, pos);
    }
   
}

void BST_display (PBST_NODE root)
{
    if (root) {
        BST_display(root->l);
        printf("%d ", root->v);
        BST_display(root->r);
    }
}

#define LEFT_SIDE 0
#define RIGHT_SIDE 1
void display_side_view(PBST_NODE root, int level, int *max_level, int side)
{
    if (root) {
        if (*max_level < level) {
            printf(" %d", root->v);
            *max_level = level;
        }
        if (side == RIGHT_SIDE) {
            display_side_view(root->r, level+1, max_level, side);
            display_side_view(root->l, level+1, max_level, side);
        } else {
            display_side_view(root->l, level+1, max_level, side);
            display_side_view(root->r, level+1, max_level, side);
        }
    }
}

int main() 
{
    int num = 0;
    int level = 1;
    int max_level = 0;
    PBST_NODE root = BST_create(10);
    BST_insert(root, 8);
    BST_insert(root, 15);
    BST_insert(root, 7);
    BST_insert(root, 9);
    BST_insert(root, 13);
    BST_insert(root, 17);
    BST_insert(root, 16);

    BST_display(root);
    printf("\nLEFT_SIDE view:\n");
    display_side_view(root, level, &max_level, LEFT_SIDE);
    printf("\nRIGHT_SIDE view:\n");
    display_side_view(root, level, &max_level, RIGHT_SIDE);
    printf("\n");
}
