#ifndef __CX_ITERATOR_H__
#define __CX_ITERATOR_H__

#include "cx_obj.h"

CX_OBJ_BEGIN(cx_iterator)
  void (*next)(struct cx_iterator_t *it);
  cx_obj_ptr item;
  cx_obj_ptr impl;
CX_OBJ_END(cx_iterator)

#define CX_ITERATOR_GET(it,type) ((type)((it)->item))
#define CX_ITERATOR_NEXT(it) (it)->next(it)

#endif /*ifndef __CX_ITERATOR_H__*/
