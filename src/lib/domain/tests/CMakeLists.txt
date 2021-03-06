######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
include(${CMAKE_SOURCE_DIR}/cmake/unittest-macros.cmake)

######################################################
#add_definitions(-DTEST_DATA="${CMAKE_CURRENT_SOURCE_DIR}/datas")

######################################################
#setup directories
add_definitions(-DUNITTEST)

######################################################
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${UNITTEST_INCLUDE_DIR})
include_directories(../)

######################################################
#test list
set(ALLOC_GTEST_LIST TestMemoryModels TestCellAccessor TestGenericMemoryAccessor TestAbstractType TestAbstractDomain TestDomainStorage TestVarSystem)

######################################################
#Now you can build your tests
foreach(i ${ALLOC_GTEST_LIST})
	add_executable(${i} ${i}.cpp)
	target_link_libraries(${i} ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} cmr cmr_skip_mpi_init)
	cmr_add_gtest(${i})
endforeach(i)
