/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRProjectCode.h"
#include "CMRGenCode.h"
#include "CMRProjectIterator.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectLocalVariable::CMRProjectLocalVariable ( const std::string& latexName, const std::string& longName , const std::string & type,const std::string & defaultValue) 
	: CMRProjectEntity ( latexName, longName )
	, defaultValue(defaultValue)
{
	this->type = type;
}

/*******************  FUNCTION  *********************/
void CMRProjectLocalVariable::genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context ) const
{
	out << type << " " << getLongName() << " = ";
	cmrGenEqCCode(out,context,defaultValue);
	out << ";" << endl;
}

/*******************  FUNCTION  *********************/
void CMRProjectLocalVariable::genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write ) const
{
	out << getLongName();
}

/*******************  FUNCTION  *********************/
CMRProjectContext& CMRProjectCodeEntry::getContext ( void )
{
	return context;
}

/*******************  FUNCTION  *********************/
void CMRProjectCodeEntry::onParentChange ( CMRProjectCodeEntry* newParent )
{
	if (newParent == NULL)
		context.setParent(NULL);
	else
		context.setParent(&newParent->context);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeType CMRProjectCodeBlockNode::getType ( void ) const
{
	return CMR_PROJECT_CODE_NODE;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation::CMRProjectCodeEquation ( const string& latexName, const string& compute )
	:output(latexName)
	,formula(compute)
{
	
}

/*******************  FUNCTION  *********************/
CMRProjectCodeType CMRProjectCodeEquation::getType ( void ) const
{
	return CMR_PROJECT_CODE_EQUATION;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeIteratorLoop::CMRProjectCodeIteratorLoop ( const std::string & iterator )
{
	//build iterator loop
	const CMRProjectEntity * entity = context.find(iterator);
	const CMRProjectIterator * it = dynamic_cast<const CMRProjectIterator*>(entity);
	if (it == NULL)
		throw CMRLatexException("Invalid iterator type : "+string(iterator));
	
	//create local variable
	string name = context.genTempName("tmp_it");
	this->addLocalVariable(iterator,name,"int","0");
	
	this->it = it;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeBlockNode& CMRProjectCodeNode::addSubBlock ( CMRProjectCodeTreeInsert location )
{
	CMRProjectCodeBlockNode * res = new CMRProjectCodeBlockNode();
	this->insert(res,location);
	return *res;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation& CMRProjectCodeNode::addEquation ( const string& latexName, const string& longName, const string& compute, CMRProjectCodeTreeInsert location )
{
	CMRProjectCodeEquation * eq = new CMRProjectCodeEquation(latexName,compute);
	this->insert(eq,location);
	return *eq;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeIteratorLoop & CMRProjectCodeNode::addIteratorLoop ( const string& iterator, CMRProjectCodeTreeInsert location )
{
	CMRProjectCodeIteratorLoop * loop = new CMRProjectCodeIteratorLoop(iterator);
	this->insert(loop,location);
	return * loop;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeType CMRProjectCodeIteratorLoop::getType ( void ) const
{
	return CMR_PROJECT_CODE_ITERATOR;
}

/*******************  FUNCTION  *********************/
CMRProjectLocalVariable& CMRProjectCodeNode::addLocalVariable ( const string& latexName, const string& longName, const string& type, const string& defaultValue, CMRProjectCodeTreeInsert location )
{
	CMRProjectLocalVariable * var = new CMRProjectLocalVariable(latexName,longName,type,defaultValue);
	this->context.addEntry(var);
	return *var;
}