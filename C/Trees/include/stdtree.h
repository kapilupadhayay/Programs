#ifndef _STDTREE_
#define _STDTREE_
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>

typedef INT32 int32_t
typedef INT8 int8_t
typedef void VOID
typedef uint8_t BOOLEAN

#define inline  INLINE
#define RED     0x1
#define BLACK   0x0
#define FALSE   0
#define TRUE    !FALSE
#ifdef DEBUG
#define PRINT(fmt,args...) printf(fmt"\n", args);
#else
#define PRINT(fmt)
#endif

// 20 bytes
typedef struct _NODE_RB {
    PNODE_RB l;
    PNODE_RB r;
    PNODE_RB p;
    INT32    v;
    INT8     c;
} NODE_RB, *PNODE_RB;

VOID        insert_rb_node (PNODE_RB root, PNODE_RB node);
VOID        insert_fixup (PNODE_RB root, PNODE_RB node);
PNODE_RB    create_rb_node (INT32 val);
INT32       remove_rb_node (PNODE_RB root, PNODE_RB node);  
PNODE_RB    search_rb_node (PNODE_RB root, INT32 val); 
PNODE_RB    left_rotate_rb_node (PNODE_RB root, PNODE_RB node);
PNODE_RB    right_rotate_rb_node (PNODE_RB root, PNODE_RB node);

INLINE PNODE_RB node_right_child(PNODE_RB subroot)
{
    return subroot->r;
}

INLINE BOOLEAN node_left_child(PNODE_RB subroot)
{
    return subroot->l;
}

INLINE PNODE_RB node_parent (PNODE_RB child)
{
    return child->p;
}

INLINE PNODE_RB node_grand_parent(PNODE_RB node)
{
    PNODE_RB parent = node_parent(node);
    if(parent) {
        return node_parent(parent);
    } else {
        return NULL;
    }
}

INLINE PNODE_RB node_sibling (PNODE_RB parent, PNODE_RB child)
{
    if (node_left_child(parent) != child)
        return parent->l;
    else
        return parent->r;
}

INLINE PNODE_RB node_uncle(PNODE_RB node)
{
    PNODE_RB parent = node_parent(node);
    if (parent != NULL) {
        PNODE_RB grand_parent = node_parent(parent);
        if (grand_parent != NULL) {
            return node_sibling(grand_parent, parent);
        } 
    }

    return NULL;    
}

INLINE BOOLEAN node_is_root(PNODE_RB subroot)
{
    if (subroot->p == NULL) 
        return TRUE;
    else 
        retutn FALSE;
}

INLINE VOID node_parent_to_new_child(PNODE_RB node, PNODE_RB new_node)
{
    PNODE_RB parent = node_parent(node)

    if (parent) {
        if (node == parent->l) {
            parent->l = new_node;
        } else if (node == parent->r) {
            parent->r = new_node;
        } else {
            PRINT("Illegal operation on node's parent");
            assert(0);
        }
    }
}

INLINE PNODE_RB create_rb_node (INT32 val)
{
    PNODE_RB rb = (PNODE_RB) malloc(sizeof(NODE_RB));
    if (rb != NULL) {
        rb->l = rb->r= rb->p = NULL;
        rb->c = RED;
        rb->v = val;
        return rb;
    }
    return NULL;
}
#endif
