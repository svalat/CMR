/******************************************************************************/
//import latex parser
var Projectvariable = require('../libs/ProjectVariable.js');
var LatexEntity = require('../libs/LatexEntity.js');
var TemplateFactory = require('../libs/TemplateFactory.js');
var Context = require('../libs/Context.js');
var factory = new TemplateFactory('./templates','cmr_cpp');

/*******************  FUNCTION  *********************/
exports.test_setup_1 = function(test)
{
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_setup_2 = function(test)
{
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	variable.addDimension('k',9,0);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_accessor_definition_1 = function(test)
{
	var expected = "CMRCellAccessor< int , CMRMemoryModelColMajor > longNameA ;";
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	test.equal(expected,variable.render(factory,'accessor_definition'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_accessor_definition_2 = function(test)
{
	var expected = "CMRCellAccessor< int[9] , CMRMemoryModelColMajor > longNameA ;";
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	variable.addDimension('k',9,0);
	test.equal(expected,variable.render(factory,'accessor_definition'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_accessor_add_var_1 = function(test)
{
	var expected = "//define variable A_{i,j}\nthis->addVariable(\"longNameA\",sizeof( int ), 1 );";
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	test.equal(expected,variable.render(factory,'accessor_add_var'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_accessor_add_var_2 = function(test)
{
	var expected = "//define variable A_{i,j}\nthis->addVariable(\"longNameA\",sizeof( int[9] ), 1 );";
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	variable.addDimension('k',9,0);
	test.equal(expected,variable.render(factory,'accessor_add_var'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_accessor_add_constr_sys = function(test)
{
	var expected = "longNameA (*(sys.getDomain(0,tstep)),x,y,absolute)";
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	test.equal(expected,variable.render(factory,'accessor_constr_sys'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_read_access_1 = function(test)
{
	var expected = "(*in.longNameA(i+1,j+1))";
	var context = new Context();
	context.setKey('CMRActionLoopType','CMRMeshOperationSimpleLoop');
	var latexEntity = new LatexEntity("A_{i+1,j+1}");
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	test.equal(expected,variable.render(factory,'read_access',context,latexEntity));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_read_access_2 = function(test)
{
	var expected = "(*cell.longNameA(i+1,j+1))";
	var context = new Context();
	context.setKey('CMRActionLoopType','CMRMeshOperationSimpleLoopInPlace');
	var latexEntity = new LatexEntity("A_{i+1,j+1}");
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	test.equal(expected,variable.render(factory,'read_access',context,latexEntity));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_write_access_1 = function(test)
{
	var expected = "(*out.longNameA(i+1,j+1))";
	var context = new Context();
	context.setKey('CMRActionLoopType','CMRMeshOperationSimpleLoop');
	var latexEntity = new LatexEntity("A_{i+1,j+1}");
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	test.equal(expected,variable.render(factory,'write_access',context,latexEntity));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_write_access_2 = function(test)
{
	var expected = "(*cell.longNameA(i+1,j+1))";
	var context = new Context();
	context.setKey('CMRActionLoopType','CMRMeshOperationSimpleLoopInPlace');
	var latexEntity = new LatexEntity("A_{i+1,j+1}");
	var variable = new Projectvariable("A_{i,j}","longNameA","int",1);
	test.equal(expected,variable.render(factory,'write_access',context,latexEntity));
	test.done();
}
