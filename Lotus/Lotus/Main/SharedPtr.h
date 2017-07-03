/*
 * ²Ã¼ô×ÔOgre, ÖÇÄÜÖ¸Õë
 */

#ifndef _Lotus_SharedPtr_H__
#define _Lotus_SharedPtr_H__

#include "Macro.h"

namespace Lotus {
	enum SharedPtrFreeMethod
	{
		/// Use OGRE_DELETE to free the memory
		SPFM_DELETE,
		/// Use OGRE_DELETE_T to free (only MEMCATEGORY_GENERAL supported)
		SPFM_DELETE_T,
		/// Use OGRE_FREE to free (only MEMCATEGORY_GENERAL supported)
		SPFM_FREE
	};

	template<class T> class SharedPtr
	{
	protected:
		T* pRep;
		unsigned int* pUseCount;
		SharedPtrFreeMethod useFreeMethod; // if we should use OGRE_FREE instead of OGRE_DELETE
	public:
		SharedPtr() : pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
		{

		}
		inline T& operator*() const { ASSERT(pRep); return *pRep; }
		inline T* operator->() const { ASSERT(pRep); return pRep; }
		inline T* get() const { return pRep; }
	};
}

#endif