#include <assert.h>

#include "structures/lists.h"

static void lists_read_file(FILE * file, lists_t * lists) {
  assert(file != NULL);
  // Get total lists count
  lists->size = 0;
  uint32_t list_size;
  while(fread(&list_size, sizeof(list_size), 1, file) == 1) {
    assert(fseek(file, list_size * sizeof(*(lists->data->data)), SEEK_CUR) == 0);
    ++(lists->size);
  }
  // Check that all file readed
  assert(feof(file) != 0);
  // Rewind file to begin
  rewind(file);
  // Allocate memory for lists
  lists->data = (list_t *) malloc(sizeof(*(lists->data)) * lists->size);
  assert(lists->data != NULL);
  // Read lists
  uint32_t list_id = 0;
  while(list_read_file(file, &(lists->data[list_id])) == 1) {
    ++list_id;
  }
  assert(list_id == lists->size);
  // Check that all file readed
  assert(feof(file) != 0);
}

void lists_read(const char * filename, lists_t * lists) {
  FILE * file = fopen(filename, "r");
  assert(file != NULL);
  lists_read_file(file, lists);
  assert(fclose(file) == 0);
}

uint32_t lists_sz(lists_t * lists) {
  return lists->size;
}

list_t lists_at(lists_t * lists, uint32_t index) {
  assert(index < lists->size);
  return lists->data[index];
}