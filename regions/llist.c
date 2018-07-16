#include <stdlib.h>

#include "llist.h"
#include "debug.h"

llist *ll_add_item(llist *list, void *content, ll_comparator *comp) {
    struct ll_node *node = list;
    struct ll_node *prev = NULL;
    while (node && (*comp)(node->content, content)) {
        prev = node;
        node = node->next;
    }
    struct ll_node *new_node = (struct ll_node *) malloc(sizeof (*new_node));
    if (!new_node) return NULL;
    new_node->content = content;
    new_node->next = node;
    debug("%p", prev);
    if (node) {
        if (prev) {
            prev->next = new_node;
        } else {
            return new_node;
        }
    } else {
        if (prev) {
            prev->next = new_node;
        } else {
            return new_node;
        }
    }
    return list;
}

int ll_length(llist *list) {
    int i = 0;
    for (struct ll_node *node = list; node; node = node->next) {
        ++i;
    }
    return i;
}
