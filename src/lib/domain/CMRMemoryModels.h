/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MEMORY_MODELS_H
#define CMR_MEMORY_MODELS_H

/********************  HEADERS  *********************/
#include <vector>
#include <cstdlib>
#include <cassert>
#include "CMRAbstractDomain.h"
#include "../common/CMRDebug.h"
#include "../common/CMRGeometry.h"
#include "CMRDomainMemory.h"

/*********************  CLASS  **********************/
struct CMRMemoryModelRowMajor
{
	static inline int getCellId(int x,int y,int width,int height);
	static inline bool isContiguous(const CMRRect & memoryRect, const CMRRect & rect);
	static inline const char * getName(void);
	//template <class T> int copyFromBuffer(T * mesh, const void* buffer, size_t size, const CMRRect & memoryRect, const CMRRect & rect);
	//template <class T> int copyToBuffer( const T * mesh,void* buffer, size_t size, const CMRRect & memoryRect, const CMRRect & rect);
};

/*******************  FUNCTION  *********************/
const char* CMRMemoryModelRowMajor::getName ( void )
{
	return "RowMajor";
}

/*******************  FUNCTION  *********************/
int CMRMemoryModelRowMajor::getCellId ( int x, int y, int width, int height )
{
	//errors
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	
	return y * width + x;
}

/*******************  FUNCTION  *********************/
bool CMRMemoryModelRowMajor::isContiguous ( const CMRRect& memoryRect, const CMRRect& rect )
{
	debug_warning(memoryRect.contains(rect),"Caution, isContiguous is used with rect which didn't fit in memoryRect !");
	return (rect.height == 1 || rect.width == memoryRect.width);
}

/*********************  CLASS  **********************/
struct CMRMemoryModelColMajor
{
	static inline int getCellId(int x,int y,int width,int height);
	static inline bool isContiguous(const CMRRect & memoryRect, const CMRRect & rect);
	static inline const char * getName(void);
	//template <class T> int copyFromBuffer(T * mesh, const void* buffer, size_t size, const CMRRect & memoryRect, const CMRRect & rect);
	//template <class T> int copyToBuffer( const T * mesh,void* buffer, size_t size, const CMRRect & memoryRect, const CMRRect & rect);
};

/*******************  FUNCTION  *********************/
const char* CMRMemoryModelColMajor::getName ( void )
{
	return "ColMajor";
}

/*******************  FUNCTION  *********************/
int CMRMemoryModelColMajor::getCellId ( int x, int y, int width, int height )
{
	//errors
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	
	return y + x * height;
}

/*******************  FUNCTION  *********************/
bool CMRMemoryModelColMajor::isContiguous ( const CMRRect& memoryRect, const CMRRect& rect )
{
	debug_warning(memoryRect.contains(rect),"Caution, isContiguous is used with rect which didn't fit in memoryRect !");
	return (rect.width == 1 || rect.height == memoryRect.height);
}

#endif // CMR_MEMORY_MODELS_H
