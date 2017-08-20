#ifndef _BSTHEADER
#define _BSTHEADER
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct bt_node {
    struct bt_node *lt, *rt;
    int v;
};

struct bt_node *create_bt_node(int );
void add_bt_node(struct bt_node *, struct bt_node *);
void del_bt_node(struct bt_node *, int );
void print_inorder(struct bt_node *);
void print_inorder_rev(struct bt_node *); // reverse in order
void print_t(struct bt_node *);
#endif
