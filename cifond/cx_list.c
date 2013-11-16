#include "cx_list.h"

const char *cx_list_class_name = "cx_list";

static void cx_list_dtor(void *list) {
  cx_list_ptr this = (cx_list_ptr)list;
  cx_list_item_ptr item = this->front;
  while (item != NULL) {
    cx_release(item->obj);
    item = item->next;
    if (item->prev != NULL) free(item->prev);
  }
}

cx_list_ptr cx_list_create() {
  cx_list_ptr this = CX_CREATE(cx_list);
  this->front = this->back = NULL;
  return this;
}

cx_list_item_ptr cx_list_insert(cx_list_ptr list, void *obj,
  cx_list_item_ptr next) {
  cx_list_item_ptr prev = (next == NULL) ? list->back : next->prev;

  cx_list_item_ptr item = (cx_list_item_ptr)malloc(sizeof(struct cx_list_item_t));
  item->list = list;
  item->obj = cx_retain(obj);
  item->next = next;
  item->prev = prev;

  if (next == NULL) list->back = item; else next->prev = item;
  if (prev == NULL) list->front = item; else prev->next = item;
  return item;
}

void cx_list_remove(cx_list_item_ptr item) {
  if (item->next == NULL) item->list->back = item->prev;
  else item->next->prev = item->prev;

  if (item->prev == NULL) item->list->front = item->next;
  else item->prev->next = item->next;

  cx_release(item->obj);
  free(item);
}
