
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Mutex Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MUTEX_H__
#define __JAM_MUTEX_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t						mutex_t;


	/** ·ÖÅä»¥³â¾ä±ú */
	JAM_API mutex_t jam_mutex_alloc();


	/** ¹Ø±Õ»¥³â¾ä±ú */
	JAM_API void jam_mutex_free(mutex_t lock);


	/** Ëø¶¨ */
	JAM_API void jam_mutex_lock(mutex_t lock);


	/** ½âËø */
	JAM_API void jam_mutex_unlock(mutex_t lock);


	/** ·Ç×èÈûËø */
	JAM_API bool jam_mutex_trylock(mutex_t lock);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MUTEX_H__ */
