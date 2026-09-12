#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct node{
  size_t data;
  struct node *next_node;
} nodes;

static nodes *head = NULL;

void add(size_t data){
  
  nodes *new_node = malloc(sizeof(nodes) + sizeof(data));
  if (head == NULL){
    head = new_node;
    new_node->data = data;
    new_node->next_node = NULL;
  }
  else{
    nodes *current = head;
    while (current->next_node != NULL){
      current = current->next_node;
    }
    current->next_node = new_node;
    new_node->data = data;
    new_node->next_node = NULL;
  }
}

void travelsal(){
  if (head == NULL){
    return;
  }
  nodes *currents = head;
  while (currents->next_node != NULL){
    printf("%d",(int)currents->data);
    currents = currents->next_node;
  }

}

int main(){
  add(1);
  add(2);
  add(3);
  add(4);
  add(5);
  travelsal();
  return 0;
}
