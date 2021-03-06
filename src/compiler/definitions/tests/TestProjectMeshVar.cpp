/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "ProjectMeshVar.h"
#include "../parsor/LatexFormula.h"
#include <sstream>
#include "MockProjectEntity.h"
#include "ProjectContext.h"

/**********************  USING  *********************/
using namespace testing;
using namespace std;
using namespace CMRCompiler;

/*********************  CONSTS  *********************/
static const char TEST_CST_1[] = "			CMRCellAccessor<int[5],CMRMemoryModelColMajor> testA;\n";
static const char TEST_CST_2[] = "	//define variable A_{i,j,k}\n\
	this->addVariable(\"testA\",sizeof(int[5]),0);\n";
static const char TEST_CST_3[] = "testA(*(sys.getDomain(1,tstep)),x,y,absolute)\n";
static const char TEST_CST_4[] = "testA(acc.testA,x,y,absolute)\n";

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testConstructor)
{
	ProjectMeshVar variable("A_{i,j}","testA","int",0);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testAddDim_1)
{
	ProjectMeshVar variable("A_{i,j,k}","testA","int",0);
	EXPECT_EQ(2,variable.getCapturedIndices().size());
	variable.addDim("k",5);
	EXPECT_EQ("i",variable.getCapturedIndices()[0]);
	EXPECT_EQ("j",variable.getCapturedIndices()[1]);
	EXPECT_EQ("k",variable.getCapturedIndices()[2]);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testAddDim_2)
{
	ProjectMeshVar variable("A_{i,j}","testA","int",0);
	EXPECT_EQ(2,variable.getCapturedIndices().size());
	EXPECT_THROW(variable.addDim("k",5),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenUsageCCode_1)
{
	ProjectMeshVar variable("A_{i,j,k}","testA","int",0);
	variable.addDim("k",5);

	LatexEntity entity("A_{2+1,2+1,55}");
	ProjectContext context;
	
	stringstream outr;
	variable.genUsageCCode(outr,context,entity);
	EXPECT_EQ("(*in.testA( 2 + 1 , 2 + 1 ))[ 55 ]",outr.str());

	stringstream outw;
	variable.genUsageCCode(outw,context,entity,true);
	EXPECT_EQ("(*out.testA( 2 + 1 , 2 + 1 ))[ 55 ]",outw.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenUsageCCode_2)
{
	ProjectMeshVar variable("A_{i,j,k}","testA","int",0);
	variable.addDim("k",5);

	LatexEntity entity("A_{m+1,m+1,55}");
	MockProjectEntity entity2("m","testM");
	ProjectContext context;
	context.addEntry(&entity2);

	stringstream outr;
	variable.genUsageCCode(outr,context,entity);
	EXPECT_EQ("(*in.testA( testM + 1 , testM + 1 ))[ 55 ]",outr.str());

	stringstream outw;
	variable.genUsageCCode(outw,context,entity,true);
	EXPECT_EQ("(*out.testA( testM + 1 , testM + 1 ))[ 55 ]",outw.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenDefinitionCCode)
{
	ProjectMeshVar variable("A_{i,j,k}","testA","int",0);
	variable.addDim("k",5);
	
	ProjectContext context;
	
	stringstream out;
	EXPECT_THROW(variable.genDefinitionCCode(out,context),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGetTypeWithDims_0)
{
	ProjectMeshVar variable("A_{i,j}","testA","int",0);;
	EXPECT_EQ("int",variable.getTypeWithDims());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGetTypeWithDims_1)
{
	ProjectMeshVar variable("A_{i,j,k}","testA","int",0);
	variable.addDim("k",5);
	EXPECT_EQ("int[5]",variable.getTypeWithDims());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenCPPAccessorDefinition)
{
	ProjectMeshVar variable("A_{i,j,k}","testA","int",0);
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorDefinition(out);
	EXPECT_EQ(TEST_CST_1,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenCPPAccessorAddVar)
{
	ProjectMeshVar variable("A_{i,j,k}","testA","int",0);
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorAddVar(out);
	EXPECT_EQ(TEST_CST_2,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenCPPAccessorConstrSys)
{
	ProjectMeshVar variable("A_{i,j,k}","testA","int",0);
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorConstrSys(out,1);
	EXPECT_EQ(TEST_CST_3,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenCPPAccessorConstrAcc)
{
	ProjectMeshVar variable("A_{i,j,k}","testA","int",0);
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorConstrAcc(out);
	EXPECT_EQ(TEST_CST_4,out.str());
}
