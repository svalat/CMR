######################################################
MACRO(setup_internal_gmock_and_gtest)
	set(GMOCK_SOURCE_DIR ${CMAKE_SOURCE_DIR}/extern-deps/gmock-1.7.0)
	add_subdirectory(${GMOCK_SOURCE_DIR})
	set(GMOCK_INCLUDE_DIR ${GMOCK_SOURCE_DIR}/include)
	set(GMOCK_INCLUDE_DIRS ${GMOCK_SOURCE_DIR}/include)
	set(GMOCK_BOTH_LIBRARIES gmock gmock_main)
	set(GTEST_BOTH_LIBRARIES gtest)
	set(GTEST_INCLUDE_DIR ${GMOCK_SOURCE_DIR}/gtest/include/)
	set(GTEST_INCLUDE_DIRS ${GMOCK_SOURCE_DIR}/gtest/include/)
ENDMACRO(setup_internal_gmock_and_gtest)

######################################################
MACRO(cmr_setup_mpi)
	set(DISABLE_MPI no CACHE BOOL "Ignore MPI even if present on system.")
	
	#search mpi
	find_package(MPI REQUIRED)
	
	#if present and not disable, set paths
	set(CMR_MPI_INCLUDE_DIR ${MPI_CXX_INCLUDE_PATH})
	set(CMR_MPI_CXX_LIBRARIES ${MPI_CXX_LIBRARIES})
	set(CMR_MPI_CXX_COMPILER ${MPI_CXX_COMPILER})
ENDMACRO(cmr_setup_mpi)
