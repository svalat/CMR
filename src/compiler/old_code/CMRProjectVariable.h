/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_VARIABLE_H
#define CMR_PROJECT_VARIABLE_H


#include "CMRProjectEntity.h"
#include "parsor/ParsorBasics.h"

/********************  NAMESPACE  *******************/
namespace CMRCompilerOld
{

/*********************  CLASS  **********************/
class CMRProjectVariable : public CMRProjectEntity
{
	public:
		CMRProjectVariable ( const std::string& latexName, const std::string& longName, const std::string& type );
		void addDim( int size, const std::string& name, int start = 0);
		void printCPPCode(void) const;
		virtual std::ostream& genUsageCCode(std::ostream& out, const CMRProjectContext& context, CMRCompiler::LatexEntityOld& entity) const;
		std::string getTypeWithDims(void) const;
		std::ostream & genCPPAccessorDefinition(std::ostream & out);
		std::ostream & genCPPAccessorAddVar(std::ostream & out);
		std::ostream & genCPPAccessorConstrSys(std::ostream& out, int id);
		std::ostream & genCPPAccessorConstrAcc(std::ostream & out);
	private:
		int ghostDepths;
		std::string type;
		std::string memoryModel;
		CMRConstantDimensionsVector dims;
		CMRConstantDimensionsVector dimStart;
		CMRCompiler::StringVector dimNames;
};

}

#endif //CMR_PROJECT_VARIABLE_H
