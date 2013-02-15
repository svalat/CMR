/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRMPICommFactory.h"
#include "CMRMPIComm.h"

/*******************  FUNCTION  *********************/
CMRMPICommFactory::CMRMPICommFactory ( int sendToRank, int recvFromRank, int tagBase ) 
{
	this->sendRank = sendToRank;
	this->recvRank = recvFromRank;
	this->tagBase = tagBase;
}

/*******************  FUNCTION  *********************/
CMRComm* CMRMPICommFactory::createComm ( CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType )
{
	return new CMRMPIComm(this,domain,rect,commType);
}

/*******************  FUNCTION  *********************/
int CMRMPICommFactory::getRecvRank ( void ) const
{
	return recvRank;
}

/*******************  FUNCTION  *********************/
int CMRMPICommFactory::getSendRank ( void ) const
{
	return sendRank;
}

/*******************  FUNCTION  *********************/
int CMRMPICommFactory::getTagBase ( void ) const
{
	return tagBase;
}