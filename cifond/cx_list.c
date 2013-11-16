#include "cx_list.h"

const char *cx_list_class_name = "cx_list";

static void cx_list_dtor(void *list) {
  cx_list_t *this = (cx_list_t*)list;
  cx_list_item_t *item = this->front;
  while (item != NULL) {
    cx_release(item->obj);
    item = item->next;
    if (item->prev != NULL) free(item->prev);
  }
}

cx_list_t *cx_list_make() {
  cx_list_t *list = (cx_list_t*)malloc(sizeof(cx_list_t));
  list->O.class_name = cx_list_class_name;
  list->O.refcount = 1;
  list->O.dtor = cx_list_dtor;
  list->front = list->back = NULL;
}

cx_list_item_t *cx_list_insert(cx_list_t *list, void *obj,
  cx_list_item_t *next) {
  cx_list_item_t *prev = (next == NULL) ? list->back : next->prev;

  cx_list_item_t *item = (cx_list_item_t*)malloc(sizeof(cx_list_item_t));
  item->list = list;
  item->obj = cx_retain(obj);
  item->next = next;
  item->prev = prev;

  if (next == NULL) list->back = item; else next->prev = item;
  if (prev == NULL) list->front = item; else prev->next = item;
  return item;
}

void cx_list_remove(cx_list_item_t *item) {
  if (item->next == NULL) item->list->back = item->prev;
  else item->next->prev = item->prev;

  if (item->prev == NULL) item->list->front = item->next;
  else item->prev->next = item->next;

  cx_release(item->obj);
  free(item);
}
