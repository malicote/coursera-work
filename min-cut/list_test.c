#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_list_strings(node_t *head) {
  char *key = NULL;
  if(!head)
    return;

  while(head) {
    key = (char*)head->key;
    printf("[%s]->", (key) ? key : "<NULL>");
    head = head->next;
  }
  printf("[NULL]\n");
}

int compare_strings(void *lhs, void *rhs) {
  // Return 0 if one is null, but 1 if both are null
  if ((!lhs || !rhs)) {
    return lhs == rhs;
  }

  return !strcmp((char*)lhs, (char*)rhs);
}

void print_list_ints(node_t *head) {
  int *key = NULL;
  if(!head)
    return;

  while(head) {
    key = (int*)head->key;
    printf("[%d]->", (key) ? *key : -9999);
    head = head->next;
  }
  printf("[NULL]\n");
}

int compare_ints(void *lhs, void *rhs) {
  // Return 0 if one is null, but 1 if both are null
  if ((!lhs || !rhs)) {
    return lhs == rhs;
  }

  return (*(int*)lhs == *(int*)rhs);
}



int list_test_main(void) {
  /* INT Tests */ 
  int numbers[] = {1, 2, 3, 4, 5, 6};
  int non_numbers[] = {-1, -2, -3, -4};
  int num = 6;

  node_t *list = NULL;

  int i;
  for(i=0; i < num; i++) {
    insert_at_head(&list, &numbers[i]);
  }

  print_list_ints(list);
  
  for(i=num-1; i >= 0; i--) {
    insert_at_tail(&list, &numbers[i]);
  }
  
  print_list_ints(list);

  // Removal tests:
  node_t *found = find_key(list, &numbers[0], compare_ints);
  printf("Found a node: %d\n", (found) ? *(int*)found->key : -9999);

  found = find_key(list, &non_numbers[0], compare_ints);
  printf("%s find a node [val=%d]: %d\n", 
         (found) ? "DID" : "DID NOT", non_numbers[0], 
         (found) ? *(int*)found->key : -9999);

  node_t *list2 = NULL;
  for(i=0; (unsigned)i < sizeof(non_numbers)/sizeof(non_numbers[0]); i++)
  {
    insert_at_tail(&list2, &non_numbers[i]);
  }
  print_list_ints(list2);
  
  int removed = delete_nodes_by_key(&list, &numbers[0], compare_ints);
  printf("Removed %d of value = %d\n", removed, numbers[0]);
  print_list_ints(list);  
  removed = delete_nodes_by_key(&list, &numbers[1], compare_ints);
  printf("Removed %d of value = %d\n", removed, numbers[0]);
  print_list_ints(list); 

  append(list, list2);
  print_list_ints(list);

  printf("----------------------------------------------------\n\n");
  char *strings[ ] = { "StringA",
                       "My String",
                       "Funky char 7&&&",
                       "This is a really long string",
                       "Where",
                       "850-549-6209",
                       "0.1..111",
                       NULL,
                     };

  node_t *string_list = NULL;
  char **temp = strings;
  while(*temp) {
    insert_at_tail(&string_list, *temp);
    printf("%s\n", *temp);
    temp++;
  }
  print_list_strings(string_list);

  int val = delete_nodes_by_key(&string_list, strings[3], compare_strings);
  printf("Deleted '%s' [%d]:\n", strings[3], val);
  print_list_strings(string_list);
  return 0;
}
