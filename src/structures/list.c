#include <assert.h>

#include "structures/list.h"

int list_read_file(FILE * file, list_t * list, bool check_list) {
  // Read list size
  assert(file != NULL);
  if(fread(&(list->size), sizeof(list->size), 1, file) != 1) {
    return 0;
  }
  // Read data
  list->data = (uint32_t *) malloc(sizeof(*(list->data)) * list->size);
  assert(list->data != NULL);
  assert(fread(list->data, sizeof(*(list->data)),
         list->size, file) == list->size);
  // Check data
  if(check_list) {
    for(size_t i = 1; i < list->size; i++) {
      assert(list->data[i - 1] < list->data[i]);
    }
  }
  return 1;
}

size_t list_sz(const list_t * list) {
  return list->size;
}

uint32_t list_at(const list_t * list, uint32_t index) {
  assert(index < list->size);
  return list->data[index];
}

uint32_t * list_begin(const list_t * list) {
  return list->data;
}

uint32_t * list_end(const list_t * list) {
  return list->data + list->size;
}