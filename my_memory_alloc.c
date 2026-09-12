#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

typedef struct sblock{
  size_t size;
  int free;
  struct sblock *next_block;
}mblock;

static mblock *head = NULL;
static mblock *find_free_heap(size_t size){
  mblock *current = head;
  while (current != NULL){
    if (current->free == 0 && current->size  >= size)
    {
      return current;
    }
    current = current->next_block;
  }
  return NULL;
}

static mblock *make_free_hep(size_t size){
  mblock *block = sbrk(sizeof(mblock) + size);
  if (block == (void *)-1){
    return NULL;
  }
  block->size = size;
  block->free = 0;
  block->next_block = NULL;
  return block;
}

void *my_malloc(size_t size){
  if (size == 0){
    return NULL;
  }
  mblock *block = find_free_heap(size);
  if (block != NULL){
    block->free = 1;
    return (void *)(block + 1);
  }
  block = make_free_hep(size);
  if (head == NULL){
    head = block;
  }
  else{
    mblock *current = head;
    while (current->next_block != NULL){
      current = current->next_block;
    }
    current->next_block = block;
    block->free = 1;
    
  }
  return (void *)(block + 1);
}
void free_my_memory(void *ptr)
{
    if (ptr == NULL)
        return;

    mblock *block = (mblock *)ptr - 1;

    block->free = 0;
}

int main(){
  int *p = my_malloc(sizeof(int));
  *p = 42;
  printf("%d",*p);
  free_my_memory(p);
  return 0;
}

