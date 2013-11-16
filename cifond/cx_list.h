#ifndef __CX_LIST_H__
#define __CX_LIST_H__
#include "cx_obj.h"

extern const char *cx_list_class_name;

struct cx_list_t;

struct cx_list_item_t {
  struct cx_list_t *list;
  cx_obj_ptr obj;
  struct cx_list_item_t *next;
  struct cx_list_item_t *prev;
};
typedef struct cx_list_item_t *cx_list_item_ptr;

struct cx_list_t { CX_O;
  cx_list_item_ptr front;
  cx_list_item_ptr back;
};
typedef struct cx_list_t *cx_list_ptr;

cx_list_ptr cx_list_create();
cx_list_item_ptr cx_list_insert(cx_list_ptr list, void *obj, cx_list_item_ptr before);
void cx_list_remove(cx_list_item_ptr item);

#define CX_LIST_ITEM(I,T) ((T##_ptr)(I->obj))

#endif /* ifndef __CX_LIST_H__ */

