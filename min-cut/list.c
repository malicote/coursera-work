#include "list.h"

void insert_at_head(node_t** head, void *key) {
  if (!head)
    return;

  node_t* tmp = malloc(sizeof(node_t));
  if (!tmp)
    return;

  tmp->key = key;
  tmp->next = *head;
  *head = tmp;
}
void insert_at_tail(node_t** head, void *key) {
  if (!head)
    return;

  if (!(*head)) {
    insert_at_head(head, key);
    return;
  }
  node_t *n = *head;

  while(n->next)
    n = n->next;

  n->next = malloc(sizeof(node_t));
  if(n->next) {
    n->next->key = key;
    n->next->next = NULL;
  }
}


//TODO: Remove edge case for head node removal.
//TODO: Memory leak for key.  May need to free that too.
void* delete_node(node_t **head, node_t *node_to_delete) {
 void *key = NULL;

 if(!head || !(*head) || !node_to_delete)
      return key;

  node_t *n = *head;

  // Check head first
  if (n == node_to_delete) {
    key = n->key;
    *head = n->next;
    free(node_to_delete);
    return key;
  }
  //TODO: What if first node is one to delete!
  while(n->next && n->next != node_to_delete)
    n = n->next;

  if (n->next) {
    // Here n->next is the node to delete
    key = n->next->key;
    n->next = n->next->next;
    free(node_to_delete);
  }

  return key;
}


// TODO: Don't have to use next node for deletion here.
int
delete_nodes_by_key(node_t **head, void *key,
                    int (*compare)(void *node_key, void *search_key)) 
{
  int removal_count = 0;

  if (!head || !(*head) || !compare)
    return 0;

  node_t *n = *head;

  while(n) {
    if(compare(n->key, key)) {
      delete_node(head, n);
      removal_count++;
    }
    n = n->next;
  }

  return removal_count;
}


int
append(node_t *head_A, node_t *head_B) {
  if (!head_A)
    return -1;

  // Nothind to append.
  if (!head_B)
    return 0;

  while (head_A->next)
    head_A = head_A->next;

  head_A->next = head_B;
  head_B = NULL;
  return 0;
}

node_t*
find_key(node_t *head, void *key, int (*compare)(void *lhs, void *rhs)) {
  if(!head || !compare)
    return NULL;
 
  while (head != NULL && !compare(head->key, key)) {
    head = head->next;
  }
  return head;
}

inline node_t* next_item(node_t *node) {
  return (node) ? node->next : NULL;
}

void **
arrayitize(node_t *head, int *size, int *count) {
  assert(head && size && count);
  
  if(!size) {
    size = malloc(sizeof(int));
    *size = 16;
  } else if (*size < 1) {
    *size = 16;
  }
  
  void **key_array = malloc(*size * sizeof(void *));
  assert(key_array);

  int last_idx = 0;
  while(head) {
    if(last_idx == *size - 1) {
      *size *= 2;
      key_array = realloc(key_array, *size * sizeof(void *));
      assert(key_array);
    }
    key_array[last_idx] = head->key;
    head = head->next;
    last_idx++;
  }
 
  *count = last_idx; 
  return key_array;
}
