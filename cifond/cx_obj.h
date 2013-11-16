#ifndef __CX_OBJ_H__
#define __CX_OBJ_H__
#include "cx_types.h"

/*! convention: every object must have a cx_obj_t field named O at the very beginning
 * example:
 * struct { CX_O;
 *   ...
 * };
 */

/*! basic building block of CX */
struct cx_obj_t {
  /*! object class name */
  /*! constant, points to the same static memory for objects of the same class */
  /*! so it's generally safe to compare pointers directly */
  const char *class_name;

  /*! reference count */
  cx_u32_t refcount;

  /*! object destructor that cleans data */
  /*! if not NULL, must be called when this object is not needed anymore */
  void (*dtor)(void *);
};
typedef struct cx_obj_t *cx_obj_ptr;
#define CX_O struct cx_obj_t O

void *cx_create(cx_size_t size, const char *classname, void (*dtor)(void*));
void *cx_retain(void *o);
void cx_release(void *o);

#define CX_CREATE(T) ((T##_ptr)cx_create(sizeof(struct T##_t), T##_class_name, T##_dtor))
#define CX_RETAIN(V,T) ((T##_ptr)cx_retain(V))

#endif /*ifndef __CX_OBJ_H__*/
