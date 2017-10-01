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





#endif


