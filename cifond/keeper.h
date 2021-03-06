#ifndef __KEEPER_H__
#define __KEEPER_H__

#include "cx_array.h"
#include "track_info.h"

CX_OBJ_BEGIN(keeper)
  cx_obj_ptr impl;
CX_OBJ_END(keeper)

/*! \brief create a keeper that is a sqlite database stored in filename
 *
 * \warning thread-safe when used only in one thread simultaneously
 */
keeper_ptr keeper_create(const char *filename);

/*! \brief create a copy for using in another thread
 */
keeper_ptr keeper_copy(keeper_ptr keeper);

cx_array_ptr keeper_track_find(keeper_ptr kpr, cx_string_ptr needle,
  int offset, int count);

track_info_ptr keeper_track_get(keeper_ptr keeper, track_id_t id);

void keeper_track_insert(keeper_ptr kpr, track_info_ptr track);

#endif /*ifndef __KEEPER_H__*/
