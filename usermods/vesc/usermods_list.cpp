#include "wled.h"
/*
 * Register your v2 usermods here!
 */
#ifdef USERMOD_VESC
#include "../usermods/vesc/usermod_vesc.h"
#endif



void registerUsermods()
{
#ifdef USERMOD_VESC
  usermods.add(new Usermodvesc());
#endif
}