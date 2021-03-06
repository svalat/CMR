######################################################
#            PROJECT  : CartesianMeshRuntime         #
#            VERSION  : 0.0.0                        #
#            DATE     : 02/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
cmr_setup_mpi()

######################################################
include_directories(${CMR_MPI_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/extern-deps/iniparser/src)

######################################################
set(cmr_SRCS common/CMRSpaceSplitter.cpp 
             common/CMRGeometry.cpp 
             common/CMRDebug.cpp 
             common/CMRCommon.cpp
             domain/CMRAbstractDomain.cpp
             domain/CMRComposedDomain.cpp
             domain/CMRDomainStorage.cpp
             domain/CMRVarSystem.cpp
             communication/CMRBufferedComm.cpp
             communication/CMRComm.cpp
             communication/CMRCommSchem.cpp
             communication/CMRCommFactory.cpp 
             mpi/CMRMPIDomainBuilder.cpp
             mpi/CMRMPIComm.cpp
             mpi/CMRMPICommFactory.cpp
             mpi/CMRMPIReduction.cpp
             runner/CMRRunner.cpp
             runner/CMRRunnerSeq.cpp
             runner/CMRRunnerOMPForRect.cpp
             runner/CMRRunnerOMPTask.cpp
             runner/CMRRunnerOMPLoops.cpp
             outputer/CMRBasicOutputer.cpp
             application/CMRApplication.cpp
             application/CMRApplicationSeq.cpp
             application/svutArgp.cpp
             application/CMRCmdOptions.cpp)
set(cmr_OPTIONAL_SRCS "")
set(iniparser_SRCS ${CMAKE_SOURCE_DIR}/extern-deps/iniparser/src/iniparser.c 
             ${CMAKE_SOURCE_DIR}/extern-deps/iniparser/src/dictionary.c)

######################################################
find_package(OpenMP QUIET)
if(OPENMP_FOUND)
	message("OPENMP FOUND")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
	add_definitions(-DHAVE_OPENMP)
endif()

######################################################
add_library(cmr SHARED ${cmr_SRCS} ${iniparser_SRCS})
target_link_libraries(cmr ${CMR_MPI_CXX_LIBRARIES})

######################################################
#install procedure
INSTALL(TARGETS cmr
        ARCHIVE DESTINATION lib${LIB_SUFFIX}
        LIBRARY DESTINATION lib${LIB_SUFFIX})
install(DIRECTORY "common" DESTINATION "include/cmr" FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "domain" DESTINATION "include/cmr" FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "mpi" DESTINATION "include/cmr" FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "runner" DESTINATION "include/cmr" FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "communication" DESTINATION "include/cmr" FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "operations" DESTINATION "include/cmr" FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "outputer" DESTINATION "include/cmr" FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "application" DESTINATION "include/cmr" FILES_MATCHING PATTERN "*.h")
INSTALL(FILES CMR.h DESTINATION include/cmr)

######################################################
if (NOT DISABLE_TESTS)
        add_subdirectory(tests)
		add_subdirectory(common/tests)
		add_subdirectory(domain/tests)
endif (NOT DISABLE_TESTS)
