######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#For integration of tests in jenkins, but only in self-test mode
macro (cmr_add_test test_name)
	#steup wrapper
	if (ENABLE_VALGRIND)
			set(tmp_test_wrapper valgrind --xml=yes --xml-file=${CMAKE_CURRENT_BINARY_DIR}/${test_name}.memcheck.xml --tool=memcheck --leak-check=full --show-reachable=yes)
	else(ENABLE_VALGRIND)
			set(tmp_test_wrapper "")
	endif(ENABLE_VALGRIND)

	#setup output options
	if (ENABLE_JUNIT_OUTPUT)
		set(tmp_test_run_option -o ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.junit.xml -m junit_xml)
	endif (ENABLE_JUNIT_OUTPUT)

	#gen text command and register
	add_test(${test_name} ${tmp_test_wrapper} ${CMAKE_CURRENT_BINARY_DIR}/${test_name} ${tmp_test_run_option})
endmacro (cmr_add_test)


######################################################
#For integration of tests in jenkins, but only in self-test mode
macro (cmr_add_gtest test_name)
	#steup wrapper
	if (ENABLE_VALGRIND)
			set(tmp_test_wrapper valgrind --xml=yes --xml-file=${CMAKE_CURRENT_BINARY_DIR}/${test_name}.memcheck.xml --tool=memcheck --leak-check=full --show-reachable=yes)
	else(ENABLE_VALGRIND)
			set(tmp_test_wrapper "")
	endif(ENABLE_VALGRIND)

	#setup output options
	if (ENABLE_JUNIT_OUTPUT)
		set(tmp_test_run_option --gtest_output=xml:${CMAKE_CURRENT_BINARY_DIR}/${test_name}.gtest.xml)
	endif (ENABLE_JUNIT_OUTPUT)

	#gen text command and register
	add_test(${test_name} ${tmp_test_wrapper} ${CMAKE_CURRENT_BINARY_DIR}/${test_name} ${tmp_test_run_option})
endmacro (cmr_add_gtest)
