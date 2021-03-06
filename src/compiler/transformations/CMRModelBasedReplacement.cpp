/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include "CMRModelBasedReplacement.h"
#include "CMRTransformationReplace.h"
#include "../parsor/LatexFormula.h"
#include "../common/Debug.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRModelBasedReplacement::CMRModelBasedReplacement(const std::string& model, const std::string result, bool captureAll)
	:ProjectEntity(model,"model")
{
	//errors
	assert(model.empty() == false);
	assert(result.empty() == false);

	//setup
	this->result = result;
	
	//mark all as capture
	if (captureAll)
		this->markAllCaptureAs(CAPTURE_REQUIRED);
}

/*******************  FUNCTION  *********************/
bool CMRModelBasedReplacement::apply( LatexEntity& entity ) const
{
	//try to match
	if (this->match(entity) == false)
		return false;
	
	//capture fields
	ProjectCaptureMap capture;
	this->capture(entity,capture);
	
	//create the new instance
	LatexFormulas * newEntity = new LatexFormulas(result);
	
	//replace values
	CMRTransformationReplace::replaceAll(capture,*newEntity);
	
	//replace
	if (newEntity->size() == 1)
	{
		entity = *(*newEntity)[0];
	} else {
		entity.clear();
		entity.parameters.push_back(newEntity);
		entity.name = "()";
	}
	
	return true;
}

/*******************  FUNCTION  *********************/
void CMRModelBasedReplacement::genDefinitionCCode ( std::ostream& out, const ProjectContext& context, int indent ) const
{
	cmrFatal("Caution, CMRModelBasedReplacement objects cannot be translated to C++ code.");
}

/*******************  FUNCTION  *********************/
void CMRModelBasedReplacement::genUsageCCode ( std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write ) const
{
	cmrFatal("Caution, CMRModelBasedReplacement objects cannot be translated to C++ code.");
}

}
