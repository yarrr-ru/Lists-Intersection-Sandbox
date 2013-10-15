#include <assert.h>

#include "structures/list.h"

int list_read_file(FILE * file, list_t * list) {
  // Read list size
  assert(file != NULL);
  if(fread(&(list->size), sizeof(list->size), 1, file) != 1) {
    return 0;
  }
  // Read data
  list->data = (uint32_t *) malloc(sizeof(*(list->data)) * list->size);
  assert(list->data != NULL);
  assert(fread(list->data, sizeof(*(list->data)), list->size, file) == list->size);
  return 1;
}

uint32_t list_sz(list_t * list) {
  return list->size;
}

uint32_t list_at(list_t * list, uint32_t index) {
  assert(index < list->size);
  return list->data[index];
}