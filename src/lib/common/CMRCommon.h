/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMMON_H
#define CMR_COMMON_H

/********************  MACRO  ***********************/
//used to make fiend classes to unit test private/protected members
#ifdef UNITTEST
	#ifdef UNIT_TEST_USER_FRIENDS
		#define ASSIST_UNIT_TEST( __class__ ) friend class __class__; UNIT_TEST_USER_FRIENDS
	#else
		#define ASSIST_UNIT_TEST( __class__ ) friend class __class__;
	#endif
#else
#define ASSIST_UNIT_TEST( __class__ )
#endif

/********************  MACRO  ***********************/
#define CMR_MPI_MASTER 0
#define CMR_MPI_ALL -1
#define CMR_ALL -1
#define CMR_CURRENT_STEP 0
#define CMR_PREV_STEP 1

/********************  ENUM  ************************/
/**
 * Synchronisation types.
**/
enum CMRCommType
{
	/** Send the data to the remote entity. **/
	CMR_COMM_SEND,
	/** Receive the data from the remote entity. **/
	CMR_COMM_RECV
};

/********************  ENUM  ************************/
enum CMRInitState
{
	CMR_INIT_STATE_NONE,
	CMR_INIT_STATE_INIT_DEPS,
	CMR_INIT_STATE_INIT,
	CMR_INIT_STATE_FINISHED,
	CMR_INIT_STATE_FINISH_DEPS,
};

/*******************  FUNCTION  *********************/
bool cmrIsMPIMaster(void);
int cmrGetMPIRank(void);
int cmrGetMPISize(void);
int cmrPGCD(int a, int b);

/*******************  FUNCTION  *********************/
/**
 * Quick template function to permute two values.
 * It require support of '=' operator and copy constructor.
 * @TODO move implementation in _impl.h file
**/
template<class T>
void cmrPermut(T & v1,T & v2)
{
	T tmp(v1);
	v1 = v2;
	v2 = tmp;
}

/*******************  FUNCTION  *********************/
template <class T>
T cmrMin(T v1,T v2)
{
	if (v1 < v2)
		return v1;
	else
		return v2;
}

/*******************  FUNCTION  *********************/
template <class T>
T cmrMax(T v1,T v2)
{
	if (v1 > v2)
		return v1;
	else
		return v2;
}

#endif //CMR_COMMON_H
