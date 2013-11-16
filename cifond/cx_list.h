#ifndef __CX_LIST_H__
#define __CX_LIST_H__
#include "cx_obj.h"

extern const char *cx_list_class_name;

struct cx_list_st;

struct cx_list_item_st {
  struct cx_list_st *list;
  cx_obj_t *obj;
  struct cx_list_item_st *next;
  struct cx_list_item_st *prev;
};
typedef struct cx_list_item_st cx_list_item_t;

struct cx_list_st {
  CX_O;
  cx_list_item_t *front;
  cx_list_item_t *back;
};
typedef struct cx_list_st cx_list_t;

cx_list_t *cx_list_make();
cx_list_item_t *cx_list_insert(cx_list_t *list, void *obj, cx_list_item_t *before);
void cx_list_remove(cx_list_item_t *item);

#define CX_LIST_ITEM(I,T) ((T*)(I->obj))

#endif /* ifndef __CX_LIST_H__ */

