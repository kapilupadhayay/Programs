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

int main() 
{
    int num = 0;
    PBST_NODE root = BST_create(10);
    BST_insert(root, 8);
    BST_insert(root, 15);
    BST_insert(root, 7);
    BST_insert(root, 9);
    BST_insert(root, 13);
    BST_insert(root, 17);

    BST_display(root);
    mark_nodes(root, &num);

    printf("num nodes: %d\n", num);

    display_nth_largest(root, num-5);
    printf("\n");
    display_nth_largest(root, num-2); 
    printf("\n");
    display_nth_largest(root, num-6); 
    printf("\n");
    display_nth_largest(root, num-9); 
    printf("\n");

}
