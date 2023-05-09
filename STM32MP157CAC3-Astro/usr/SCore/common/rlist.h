#ifndef __RLIST_H__
#define __RLIST_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * Double List structure
 */
    struct rlist_node
    {
        struct rlist_node *next; /**< point to next node. */
        struct rlist_node *prev; /**< point to prev node. */
    };
    typedef struct rlist_node rlist_t; /**< Type for lists. */

    /**
 * Single List structure
 */
    struct rslist_node
    {
        struct rslist_node *next; /**< point to next node. */
    };
    typedef struct rslist_node rslist_t; /**< Type for single list. */

#define r_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

/**
 * @brief initialize a list object
 */
#define RLIST_OBJECT_INIT(object) \
    {                             \
        &(object), &(object)      \
    }

    /**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
    inline void rlist_init(rlist_t *l)
    {
        l->next = l->prev = l;
    }

    /**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
    inline void rlist_insert_after(rlist_t *l, rlist_t *n)
    {
        l->next->prev = n;
        n->next = l->next;

        l->next = n;
        n->prev = l;
    }

    /**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
    inline void rlist_insert_before(rlist_t *l, rlist_t *n)
    {
        l->prev->next = n;
        n->prev = l->prev;

        l->prev = n;
        n->next = l;
    }

    /**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
    inline void rlist_remove(rlist_t *n)
    {
        n->next->prev = n->prev;
        n->prev->next = n->next;

        n->next = n->prev = n;
    }

    /**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
    inline int rlist_isempty(const rlist_t *l)
    {
        return l->next == l;
    }

    /**
 * @brief get the list length
 * @param l the list to get.
 */
    inline unsigned int rlist_len(const rlist_t *l)
    {
        unsigned int len = 0;
        const rlist_t *p = l;
        while (p->next != l)
        {
            p = p->next;
            len++;
        }

        return len;
    }

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define rlist_entry(node, type, member) \
    r_container_of(node, type, member)

/**
 * rlist_for_each - iterate over a list
 * @pos:	the rlist_t * to use as a loop cursor.
 * @head:	the head for your list.
 */
#define rlist_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * rlist_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the rlist_t * to use as a loop cursor.
 * @n:		another rlist_t * to use as temporary storage
 * @head:	the head for your list.
 */
#define rlist_for_each_safe(pos, n, head)                  \
    for (pos = (head)->next, n = pos->next; pos != (head); \
         pos = n, n = pos->next)

/**
 * rlist_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define rlist_for_each_entry(pos, head, member)                 \
    for (pos = rlist_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head);                                \
         pos = rlist_entry(pos->member.next, typeof(*pos), member))

/**
 * rlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define rlist_for_each_entry_safe(pos, n, head, member)          \
    for (pos = rlist_entry((head)->next, typeof(*pos), member),  \
        n = rlist_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head);                                 \
         pos = n, n = rlist_entry(n->member.next, typeof(*n), member))

/**
 * rlist_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define rlist_first_entry(ptr, type, member) \
    rlist_entry((ptr)->next, type, member)

#define RSLIST_OBJECT_INIT(object) \
    {                              \
        NULL                       \
    }

    /**
 * @brief initialize a single list
 *
 * @param l the single list to be initialized
 */
    inline void rslist_init(rslist_t *l)
    {
        l->next = NULL;
    }

    inline void rslist_append(rslist_t *l, rslist_t *n)
    {
        struct rslist_node *node;

        node = l;
        while (node->next)
            node = node->next;

        /* append the node to the tail */
        node->next = n;
        n->next = NULL;
    }

    inline void rslist_insert(rslist_t *l, rslist_t *n)
    {
        n->next = l->next;
        l->next = n;
    }

    inline unsigned int rslist_len(const rslist_t *l)
    {
        unsigned int len = 0;
        const rslist_t *list = l->next;
        while (list != NULL)
        {
            list = list->next;
            len++;
        }

        return len;
    }

    inline rslist_t *rslist_remove(rslist_t *l, rslist_t *n)
    {
        /* remove slist head */
        struct rslist_node *node = l;
        while (node->next && node->next != n)
            node = node->next;

        /* remove node */
        if (node->next != (rslist_t *)0)
            node->next = node->next->next;

        return l;
    }

    inline rslist_t *rslist_first(rslist_t *l)
    {
        return l->next;
    }

    inline rslist_t *rslist_tail(rslist_t *l)
    {
        while (l->next)
            l = l->next;

        return l;
    }

    inline rslist_t *rslist_next(rslist_t *n)
    {
        return n->next;
    }

    inline int rslist_isempty(rslist_t *l)
    {
        return l->next == NULL;
    }

/**
 * @brief get the struct for this single list node
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define rslist_entry(node, type, member) \
    r_container_of(node, type, member)

/**
 * rslist_for_each - iterate over a single list
 * @pos:    the rslist_t * to use as a loop cursor.
 * @head:   the head for your single list.
 */
#define rslist_for_each(pos, head) \
    for (pos = (head)->next; pos != NULL; pos = pos->next)

/**
 * rslist_for_each_entry  -   iterate over single list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your single list.
 * @member: the name of the list_struct within the struct.
 */
#define rslist_for_each_entry(pos, head, member)                 \
    for (pos = rslist_entry((head)->next, typeof(*pos), member); \
         &pos->member != (NULL);                                 \
         pos = rslist_entry(pos->member.next, typeof(*pos), member))

/**
 * rslist_first_entry - get the first element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define rslist_first_entry(ptr, type, member) \
    rslist_entry((ptr)->next, type, member)

/**
 * rslist_tail_entry - get the tail element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define rslist_tail_entry(ptr, type, member) \
    rslist_entry(rslist_tail(ptr), type, member)

    /**@}*/

#ifdef __cplusplus
}
#endif

#endif /* __RLIST_H__ */
