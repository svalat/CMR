/********************  HEADERS  *********************/
#include "CMRProject.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRProject2::CMRProject2(void)
{
	rootContext.addEntry(new CMRProjectLocalVariable("i","x","int"));
	rootContext.addEntry(new CMRProjectLocalVariable("j","y","int"));
	rootContext.addEntry(new CMRProjectLocalVariable("x","pos.getAbsX(x)","int"));
	rootContext.addEntry(new CMRProjectLocalVariable("y","pos.getAbsY(y)","int"));
}

/*******************  FUNCTION  *********************/
ProjectConstant& CMRProject2::addConstant ( const std::string& latexName, const std::string& longName, const std::string & type )
{
	ProjectConstant * tmp = new ProjectConstant(latexName,longName,type);
	constants.push_back(tmp);
	rootContext.addEntry(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
ProjectAction& CMRProject2::addAction ( std::string name, std::string descr, const std::string & loopType )
{
	ProjectAction * action = new ProjectAction(name,descr,loopType,&rootContext);
	actions.push_back(action);
	return *action;
}

/*******************  FUNCTION  *********************/
ProjectDefinition& CMRProject2::addDefinition ( const std::string& latexName, const std::string& longName )
{
	ProjectDefinition * def = new ProjectDefinition(latexName,longName,&rootContext);
	definitions.push_back(def);
	rootContext.addEntry(def);
	return *def;
}

/*******************  FUNCTION  *********************/
ProjectDefinition& CMRProject2::addDefinition ( const std::string& latexName, const std::string& longName, const std::string& compute )
{
	ProjectDefinition & def = addDefinition(latexName,longName);
	def.addEquation(latexName,compute);
	return def;
}

/*******************  FUNCTION  *********************/
ProjectIterator& CMRProject2::addIterator ( const std::string& latexName, const std::string& longName, int start, int end )
{
	ProjectIterator * it = new ProjectIterator(latexName,longName,start,end);
	iterators.push_back(it);
	rootContext.addEntry(it);
	return *it;
}

/*******************  FUNCTION  *********************/
ProjectMeshVar& CMRProject2::addvariable ( const std::string& latexName, const std::string& longName, const std::string& type , int ghost)
{
	ProjectMeshVar * tmp = new ProjectMeshVar(latexName,longName,type, ghost);
	variables.push_back(tmp);
	rootContext.addEntry(tmp);
	//TODO create alias
	//rootContext.addEntry(new CMRProjectAlias(tmp,tmp->shortName + "'",tmp->longName + "Prim"));
	return *tmp;
}

/*******************  FUNCTION  *********************/
void CMRProject2::printDebug ( void )
{
	//TODO
}

/*******************  FUNCTION  *********************/
void CMRProject2::runTransformation ( CMRTransformation& transf )
{
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
		(*it)->runTransformation(transf);
	for (CMRProjectDefinitionVector::iterator it = definitions.begin() ; it != definitions.end() ; ++it)
		(*it)->runTransformation(transf);
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCode ( std::ostream& out )
{
	out << "#include <application/CMRApplicationSeq.h>" << endl;
	out << "#include <CMR.h>" << endl;
	out << "//user headers" << endl;
	for (StringVector::const_iterator it = userHeaders.begin() ; it != userHeaders.end() ; ++it)
		out << "#include <" << *it << ">" << endl;
	out << endl;
	out << "#define WIDTH 800" <<endl;
	out << "#define HEIGHT 100" << endl;
	out << "#define WRITE_STEP_INTERVAL 50" << endl;
	out << "#define ITERATIONS 8000" << endl;
		
	genCCodeOfConsts(out);
	output.genCode(out,rootContext,"DeclStruct");
	genCCodeOfVariables(out);
	genCCodeOfDefinitions(out);
	genCCodeOfActions(out,lang);
	output.genCode(out,rootContext,"ImplAction");
	genCCodeOfMain(out,lang);
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfConsts ( std::ostream& out )
{
	//nothing to do
	if (constants.empty())
			return;

	//gen consts
	out << "/************************ CONSTS **********************/" << endl;
	for (CMRProjectConstantVector::iterator it = constants.begin() ; it != constants.end() ; ++it)
		(*it)->genDefinitionCCode(out,rootContext);
	out << endl;
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfVariables ( ostream& out )
{
	//nothing to do
	if (variables.empty())
		return;

	//build the variable system
	out << "/********************** VARIABLES *********************/" << endl;
	out << "class VarSystem : public CMRVarSystem" << endl;
	out << "{" << endl;
	out << "\tpublic:" << endl;
	out << "\t\tstruct CellAccessor" << endl;
	out << "\t\t{" << endl;
	out << "\t\t\tCellAccessor(CMRVarSystem & sys,int tstep,int x,int y,bool absolute = true);" << endl;
	out << "\t\t\tCellAccessor(CellAccessor & acc,int x,int y,bool absolute = false);" << endl;
	
	for (CMRProjectVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
		(*it)->genCPPAccessorDefinition(out);

	out << "\t\t};" << endl;
	out << "\tpublic:" << endl;
	out << "\t\tVarSystem(CMRDomainBuilder * builder = NULL);" << endl;
	out << "};" << endl;
	out << endl;

	out << "VarSystem::VarSystem ( CMRDomainBuilder * builder)" << endl;
	out << "\t:CMRVarSystem(builder)" << endl;
	out << "{" << endl;
	
	for (CMRProjectVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
		(*it)->genCPPAccessorAddVar(out);

	out << "}" << endl;
	out << endl;
	out << "VarSystem::CellAccessor::CellAccessor (CMRVarSystem & sys,int tstep,int x,int y,bool absolute)" << endl;
	
	int cnt = 0;
	for (CMRProjectVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
	{
		if (cnt == 0)
			out << "\t:";
		else
			out << "\t,";
		(*it)->genCPPAccessorConstrSys(out,cnt++);
		out << endl;
	}

	out << "{}" << endl;
	out << endl;
	out << "VarSystem::CellAccessor::CellAccessor ( CellAccessor& acc, int x, int y,bool absolute)" << endl;
	
	cnt = 0;
	for (CMRProjectVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
	{
		if (cnt == 0)
			out << "\t:";
		else
			out << "\t,";
		(*it)->genCPPAccessorConstrAcc(out);
		out << endl;
		cnt++;
	}
	
	out << "{}" << endl;
	out << endl;	
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfDefinitions ( ostream& out )
{
	if (definitions.empty())
		return;
	out << "/********************* DEFINITIONS ********************/" << endl;
	for (CMRProjectDefinitionVector::iterator it = definitions.begin() ; it != definitions.end() ; ++it)
		(*it)->genDefinitionCCode(out,rootContext);
	out << endl;
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfActions ( ostream& out, CMRCompiler::LangDef & lang )
{
	if (actions.empty())
		return;

	out << "/*********************** ACTIONS *********************/" << endl;
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
		(*it)->genDefinitionCCode(out,lang,rootContext);
	out << endl;
}

/*******************  FUNCTION  *********************/
ProjectCallAction& CMRProject2::addInitCallAction ( const string& actionName )
{
	ProjectCallAction * action = new ProjectCallAction(actionName);
	this->initActions.push_back(action);
	return *action;
}

/*******************  FUNCTION  *********************/
ProjectCallAction& CMRProject2::addMainLoopCallAction ( const string& actionName )
{
	ProjectCallAction * action = new ProjectCallAction(actionName);
	this->loopActions.push_back(action);
	return *action;
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfMain ( ostream& out, LangDef& lang )
{
	out << "int main(int argc, char ** argv)" << endl;
	out << "{" << endl;
	out << "\tCMRApplicationSeq app(argc,argv,new VarSystem,WIDTH,HEIGHT,WRITE_STEP_INTERVAL);" << endl;
	out << endl;
	out << "\tCMRRect local = app.getLocalRect();" << endl;
	out << "\tCMRRect global = app.getGlobalRect();" << endl;
	out << endl;
	out << "\t//setup write system" << endl;
	out << "\tapp.addPrepareWriteAction(new ActionUpdateFileout::LoopType(),app.getLocalRect());" << endl;
	out << endl;

	out << "\t//setup init actions" << endl;
	int cnt = 0;
	for (CMRProjectCallActionVector::const_iterator it = initActions.begin() ; it != initActions.end() ; ++it)
		(*it)->genCode(out,lang,"addInitAction",cnt++,1);
	out << endl;
	
	out << "\t//setup compute actions" << endl;
	cnt = 0;
	for (CMRProjectCallActionVector::const_iterator it = loopActions.begin() ; it != loopActions.end() ; ++it)
		(*it)->genCode(out,lang,"addLoopAction",cnt++,1);
	out << endl;

	//runner
	out << "\t//run" << endl;
	out << "\tapp.run();" << endl;
	out << endl;
	out << "\treturn EXIT_SUCCESS;" << endl;
	out << "}" << endl << endl;
}

/*******************  FUNCTION  *********************/
void CMRProject2::addUserHeader ( const string& value )
{
	this->userHeaders.push_back(value);
}

/*******************  FUNCTION  *********************/
void CMRProject2::addOutputEntry ( const string& name, const string& type, const string& value )
{
	output.addEntry(name,type,value);
}

}
