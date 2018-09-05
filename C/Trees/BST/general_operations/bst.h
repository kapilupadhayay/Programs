#ifndef _BSTHEADER
#define _BSTHEADER
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#ifdef DEBUG
#define PRINTF(X...) printf(X)
#else
#define PRINTF(X...)
#endif

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define TRUE 1
#define FALSE 0
struct bt_node {
    struct bt_node *lt, *rt;
    int v;
};

struct bt_node *create_bt_node(int );
struct bt_node * add_bt_node(struct bt_node *, int val);
void del_bt_node(struct bt_node *, int );
void print_inorder(struct bt_node *);
void print_inorder_rev(struct bt_node *); // reverse in order
void print_t(struct bt_node *);

struct bt_node * search_bt_node(struct bt_node * , int );

#define MAX_TREE_NODES 1024
typedef struct BFS_QUEUE {
  struct bt_node **H;
  struct bt_node **T;
  struct bt_node *Q[MAX_TREE_NODES];
} BFS_QUEUE, *PBFS_QUEUE;

static inline void BFS_enqueue(PBFS_QUEUE q, struct bt_node *n)
{
  if (q->H - q->T < MAX_TREE_NODES) {
    *(q->H) = n;
    if ((struct bt_node *)q->H++ >= (struct bt_node *)(q->Q + MAX_TREE_NODES)) 
        q->H = &(q->Q[0]);
  } else {
    PRINTF( "OVERFOLOW \n");
  }
}

static inline struct bt_node * BFS_dequeue(PBFS_QUEUE q)
{
    struct bt_node * n = NULL;
    if (q->H == q->T) {
        return NULL;
    } else {
        n  = *(q->T);
        if (q->T++ >= (q->Q + MAX_TREE_NODES)) 
            q->T = q->Q;
        return n;
    }
}

static inline int BFS_queue_empty(PBFS_QUEUE q)
{
    if (q->H == q->T) {
        return 1;
    }

    return 0;
}



#endif


