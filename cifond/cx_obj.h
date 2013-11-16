#ifndef __CX_OBJ_H__
#define __CX_OBJ_H__
#include "cx_types.h"

/*! convention: every object must have a cx_obj_t field named O at the very beginning
 * example:
 * struct {
 *  CX_O;
 * };
 */

/*! basic building block of CX */
typedef struct {
  /*! object class name */
  /*! constant, points to the same static memory for objects of the same class */
  /*! so it's generally safe to compare pointers directly */
  const char *class_name;

  /*! reference count */
  cx_u32_t refcount;

  /*! object destructor that cleans data */
  /*! if not NULL, must be called when this object is not needed anymore */
  void (*dtor)(void *);
} cx_obj_t;

#define CX_O cx_obj_t O

void *cx_retain(void *o);

void cx_release(void *o);

#endif /*ifndef __CX_OBJ_H__*/
