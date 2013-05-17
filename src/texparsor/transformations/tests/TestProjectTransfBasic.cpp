/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include "../CMRProjectTransf.h"
#include <../definitions/CMRProjectCode.h>
#include <../definitions/CMRProjectIterator.h>
#include <../definitions/CMRProjectConstant.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*********************  CONSTS  *********************/

/*********************  CLASS  **********************/
class MockTransfBasic : public CMRProjectTransformationBasic
{
	public:
		MockTransfBasic(void);
		virtual void transform(CMRProjectCodeNode& node);
		virtual void transform(CMRProjectCodeIteratorLoop& loop);
		virtual void transform(CMRProjectCodeEquation& equation, CMRLatexFormulas2& formula);
		virtual void transform(CMRProjectCodeEquation& equation, CMRLatexEntity2& entity);
		virtual void transform(CMRProjectCodeEquation& equation);
		std::stringstream out;
};

/*******************  FUNCTION  *********************/
MockTransfBasic::MockTransfBasic(void )
	: CMRProjectTransformationBasic("mock")
{

}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRProjectCodeNode& node)
{
	out << "NODE[ ";
	CMRProjectTransformationBasic::transform(node);
	out << " ]"; 
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRProjectCodeEquation& equation, CMRLatexFormulas2& formula)
{
	out << "FORMULA[ ";
	CMRProjectTransformationBasic::transform(equation,formula);
	out << " ]"; 
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRProjectCodeIteratorLoop& loop)
{
	out << "LOOP[ " << loop.getIterator().getLatexName() << " , ";
	CMRProjectTransformationBasic::transform(loop);
	out << " ]";
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRProjectCodeEquation& equation, CMRLatexEntity2& entity)
{
	out << "ENTITY [ " << entity.getName() << " , ";
	CMRProjectTransformationBasic::transform(equation, entity);
	out << " ]";
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRProjectCodeEquation& equation)
{
	out << "EQUATION [ " << equation.getOutput() << " , ";
	CMRProjectTransformationBasic::transform(equation);
	out << " ]";
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectTransfBasic,testConstructor)
{
	MockTransfBasic transf;
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectTransfBasic,testTransformLoop)
{
	MockTransfBasic transf;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addIteratorLoop("k");
	
	transf.run(root);
	SVUT_ASSERT_EQUAL("NODE[ LOOP[ k , NODE[  ] ] ]",transf.out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectTransfBasic,testTransformEquation_1)
{
	MockTransfBasic transf;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("j","testJ","int","0");
	root.addEquation("j","k * 2");
	
	transf.run(root);
	SVUT_ASSERT_EQUAL("NODE[ EQUATION [ j , FORMULA[ ENTITY [ k ,  ]ENTITY [ * ,  ]ENTITY [ 2 ,  ] ] ] ]",transf.out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectTransfBasic,testTransformEquation_2)
{
	MockTransfBasic transf;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("j","testJ","int","0");
	root.addEquation("j","k^{4 + 5}_{9 + 10} * \\frac{1}{2}");
	
	transf.run(root);
	SVUT_ASSERT_EQUAL("NODE[ EQUATION [ j , FORMULA[ ENTITY [ k , FORMULA[ ENTITY [ 9 ,  ]ENTITY [ + ,  ]ENTITY [ 10 ,  ] ]FORMULA[ ENTITY [ 4 ,  ]ENTITY [ + ,  ]ENTITY [ 5 ,  ] ] ]ENTITY [ * ,  ]ENTITY [ \\frac , FORMULA[ ENTITY [ 1 ,  ] ]FORMULA[ ENTITY [ 2 ,  ] ] ] ] ] ]",transf.out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectTransfBasic,testTransformEquation_3)
{
	MockTransfBasic transf;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("j","testJ","int","0");
	root.addEquation("j","1 + \\frac{1}{2}");
	
	transf.run(root);
	SVUT_ASSERT_EQUAL("NODE[ EQUATION [ j , FORMULA[ ENTITY [ 1 ,  ]ENTITY [ + ,  ]ENTITY [ \\frac , FORMULA[ ENTITY [ 1 ,  ] ]FORMULA[ ENTITY [ 2 ,  ] ] ] ] ] ]",transf.out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectTransfBasic,testTransformEquation_4)
{
	MockTransfBasic transf;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("j","testJ","int","0");
	root.addEquation("j","1 +(2 * 3)");
	
	transf.run(root);
	SVUT_ASSERT_EQUAL("NODE[ EQUATION [ j , FORMULA[ ENTITY [ 1 ,  ]ENTITY [ + ,  ]ENTITY [ () , FORMULA[ ENTITY [ 2 ,  ]ENTITY [ * ,  ]ENTITY [ 3 ,  ] ] ] ] ] ]",transf.out.str());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
