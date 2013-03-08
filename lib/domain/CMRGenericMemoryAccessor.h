/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_GENERIC_MEMORY_ACCESSOR_H
#define CMR_GENERIC_MEMORY_ACCESSOR_H

/********************  HEADERS  *********************/
#include <cassert>
#include <cstdlib>
#include "../common/CMRDebug.h"
#include "../common/CMRGeometry.h"
#include "CMRDomainMemory.h"

/*********************  CLASS  **********************/
class CMRAbstractMemoryAccessor : public CMRDomainMemory
{
	public:
		CMRAbstractMemoryAccessor(void) {};
		CMRAbstractMemoryAccessor(CMRDomainMemory & memoryDomain) : CMRDomainMemory(memoryDomain) {};
		virtual ~CMRAbstractMemoryAccessor(void) {};
		virtual size_t getTypeSize(void) const = 0;
		virtual const void * getCell(int x,int y) const = 0;
		virtual void * getCell(int x,int y) = 0;
		virtual bool isContiguous(const CMRRect & rect) const = 0;
		virtual size_t copyFromBuffer ( const void* buffer, size_t size, const CMRRect & rect ) = 0;
		virtual size_t copyToBuffer ( void* buffer, size_t size, const CMRRect & rect ) const = 0;
		virtual size_t getBufferSize( const CMRRect & rect) const = 0;
};

/*********************  CLASS  **********************/
template <class DataType,class MemoryModel>
class CMRGenericMemoryAccessor : public CMRAbstractMemoryAccessor
{
	public:
		CMRGenericMemoryAccessor(void) {};
		CMRGenericMemoryAccessor(CMRDomainMemory & memoryDomain) : CMRAbstractMemoryAccessor(memoryDomain) {};
		virtual ~CMRGenericMemoryAccessor(void) {};
		size_t getTypeSize( void ) const;
		virtual bool isContiguous(const CMRRect & rect) const;
		virtual size_t copyFromBuffer ( const void* buffer, size_t size, const CMRRect & rect );
		virtual size_t copyToBuffer ( void* buffer, size_t size, const CMRRect & rect ) const;
		virtual size_t getBufferSize( const CMRRect & rect) const;
		virtual const void* getCell ( int x, int y ) const;
		virtual void* getCell ( int x, int y );
};

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
const void* CMRGenericMemoryAccessor<DataType,MemoryModel>::getCell ( int x, int y ) const
{
	//errors
	assert(ptrAbsPosition == CMRVect2D(0,0));
	assert(ptr != NULL);
	
	//return cell
	const DataType * typedPtr = (const DataType*) ptr;
	int id = MemoryModel::getCellId(x,y,memoryRect.width,memoryRect.height);
	return (typedPtr+id);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
void* CMRGenericMemoryAccessor<DataType,MemoryModel>::getCell ( int x, int y )
{
	//errors
	assert(ptrAbsPosition == CMRVect2D(0,0));
	assert(ptr != NULL);
	
	//return cell
	DataType * typedPtr = (DataType*)ptr;
	int id = MemoryModel::getCellId(x,y,memoryRect.width,memoryRect.height);
	return typedPtr+id;
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
bool CMRGenericMemoryAccessor<DataType,MemoryModel>::isContiguous ( const CMRRect& rect ) const
{
	return MemoryModel::isContiguous(this->memoryRect,rect);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
size_t CMRGenericMemoryAccessor<DataType,MemoryModel>::getBufferSize ( const CMRRect& rect ) const
{
	return rect.surface() * getTypeSize();
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
size_t CMRGenericMemoryAccessor<DataType,MemoryModel>::getTypeSize ( void ) const
{
	return sizeof(DataType);
}

/*******************  FUNCTION  *********************/
/** @TODO maybe prefer a generic version which deps only on model and type size, but not type **/
template <class DataType,class MemoryModel>
size_t CMRGenericMemoryAccessor<DataType,MemoryModel>::copyToBuffer ( void* buffer, size_t size, const CMRRect & rect ) const
{
	//errors
	assert(memoryRect.contains(rect));
	assert(size >= getBufferSize(rect));
	assert(ptr != NULL);
	
	//this is just that unit test does not cover this case
	debug_warning((ptrAbsPosition == CMRVect2D(0,0)),"Caution, this case wan't validate into test suite !");
	
	//local
	int origX = rect.x - ptrAbsPosition.x;
	int origY = rect.y - ptrAbsPosition.y;
	int memW = memoryRect.width;
	int memH = memoryRect.height;
	
	//ptrs
	DataType * typedBuffer = (DataType*) buffer;
	const DataType * typedDomain = (const DataType*) ptr;
	
	//copy
	for (int y = 0 ; y < rect.height ; y++)
		for (int x = 0 ; x < rect.width ; x++)
			typedBuffer[y*rect.width + x] = typedDomain[MemoryModel::getRelCellId(x + origX,y + origY,memW,memH,ptrAbsPosition.x,ptrAbsPosition.y)];

	//return current used size
	return getBufferSize(rect);
}

/*******************  FUNCTION  *********************/
/** @TODO maybe prefer a generic version which deps only on model and type size, but not type **/
template <class DataType,class MemoryModel>
size_t CMRGenericMemoryAccessor<DataType,MemoryModel>::copyFromBuffer ( const void* buffer, size_t size, const CMRRect & rect )
{
	//errors
	assert(memoryRect.contains(rect));
	assert(size >= getBufferSize(rect));
	assert(ptr != NULL);
	
	//this is just that unit test does not cover this case
	debug_warning((ptrAbsPosition == CMRVect2D(0,0)),"Caution, this case wan't validate into test suite !");
	
	//local
	int origX = rect.x - ptrAbsPosition.x;
	int origY = rect.y - ptrAbsPosition.y;
	int memW = memoryRect.width;
	int memH = memoryRect.height;
	
	//ptrs
	const DataType * typedBuffer = (DataType*) buffer;
	DataType * typedDomain = (DataType*) ptr;
	
	//copy
	for (int y = 0 ; y < rect.height ; y++)
		for (int x = 0 ; x < rect.width ; x++)
			typedDomain[MemoryModel::getRelCellId(x + origX,y + origY,memW,memH,ptrAbsPosition.x,ptrAbsPosition.y)] = typedBuffer[y*rect.width + x];

	//return current used size
	return getBufferSize(rect);
}

#endif //CMR_GENERIC_MEMORY_ACCESSOR_H
