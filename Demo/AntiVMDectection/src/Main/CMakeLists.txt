CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(${PROJECT_SOURCE_DIR}/pincfg.txt)

AUX_SOURCE_DIRECTORY(. SRC_LIST)

SET(OUTPUT_DIR "IA32")

IF(CMAKE_CL_64 EQUAL 1)

	INCLUDE_DIRECTORIES(${PIN_BASE}source/include/pin)
	INCLUDE_DIRECTORIES(${PIN_BASE}source/include/pin/gen)
	INCLUDE_DIRECTORIES(${PIN_BASE}source/tools/InstLib)
	INCLUDE_DIRECTORIES(${PIN_BASE}extras/xed2-intel64/include)
	INCLUDE_DIRECTORIES(${PIN_BASE}extras/components/include)

	LINK_DIRECTORIES(${PIN_BASE}intel64/lib)
	LINK_DIRECTORIES(${PIN_BASE}intel64/lib-ext)
	LINK_DIRECTORIES(${PIN_BASE}extras/xed2-intel64/lib)

	################################
	# Add your third part libs here
	################################

	SET(OUTPUT_BIN ${OUTPUT_BIN_64})
	SET(OUTPUT_DIR ${OUTPUT_DIR_64})

ELSEIF(CMAKE_CL_64 EQUAL 0)

	INCLUDE_DIRECTORIES(${PIN_BASE}source/include/pin)
	INCLUDE_DIRECTORIES(${PIN_BASE}source/include/pin/gen)
	INCLUDE_DIRECTORIES(${PIN_BASE}source/tools/InstLib)
	INCLUDE_DIRECTORIES(${PIN_BASE}extras/xed2-ia32/include)
	INCLUDE_DIRECTORIES(${PIN_BASE}extras/components/include)

	LINK_DIRECTORIES(${PIN_BASE}ia32/lib)
	LINK_DIRECTORIES(${PIN_BASE}ia32/lib-ext)
	LINK_DIRECTORIES(${PIN_BASE}extras/xed2-ia32/lib)

	################################
	# Add your third part libs here
	################################

	SET(OUTPUT_BIN ${OUTPUT_BIN_32})
	SET(OUTPUT_DIR ${OUTPUT_DIR_32})

ENDIF()

################################
ADD_LIBRARY(PIN_TARGET SHARED ${SRC_LIST})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${OUTPUT_DIR})
SET_TARGET_PROPERTIES(PIN_TARGET PROPERTIES OUTPUT_NAME ${OUTPUT_BIN})

################################
# Default to Release build
SET(CMAKE_BUILD_TYPE "Release")

################################
#
# PIN_COMMON_CXXFLAGS = \
#    /MT /EHs- /EHa- /wd4530 /DTARGET_WINDOWS /DBIGARRAY_MULTIPLIER=1 /DUSING_XED \
#    /D_CRT_SECURE_NO_DEPRECATE /D_SECURE_SCL=0
#
# PIN_IA32_CXXFLAGS           = /DTARGET_IA32 /DHOST_IA32
# PIN_IA32E_CXXFLAGS          = /DTARGET_IA32E /DHOST_IA32E
# 
################################

SET(CMAKE_CXX_FLAGS "")
SET(CMAKE_CXX_FLAGS_RELEASE "")

SET(PIN_COMMON_CXXFLAGS /MT /EHs- /EHa- /wd4530)  
FOREACH(TEMP_VAR ${PIN_COMMON_CXXFLAGS})
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TEMP_VAR}")
ENDFOREACH()

SET(PIN_COMMON_DEFINITIONS /DTARGET_WINDOWS /DBIGARRAY_MULTIPLIER=1 /DUSING_XED /D_CRT_SECURE_NO_DEPRECATE /D_SECURE_SCL=0)
FOREACH(TEMP_VAR ${PIN_COMMON_DEFINITIONS})
	ADD_DEFINITIONS(${TEMP_VAR})
ENDFOREACH()

IF(CMAKE_CL_64 EQUAL 1)
	ADD_DEFINITIONS("/DTARGET_IA32E")
	ADD_DEFINITIONS("/DHOST_IA32E") 
ELSEIF(CMAKE_CL_64 EQUAL 0)
	ADD_DEFINITIONS("/DTARGET_IA32")
	ADD_DEFINITIONS("/DHOST_IA32") 
ENDIF()

################################
#
# Setup PIN linker parameters.
#
# PIN_COMMON_LIBS         = pin.lib libxed.lib libcpmt.lib libcmt.lib pinvm.lib kernel32.lib
# PIN_IA32_LIBS           = ntdll-32.lib
# PIN_IA32E_LIBS          = ntdll-64.lib
# 
################################

SET(PIN_COMMON_LIBS pin.lib libxed.lib libcpmt.lib libcmt.lib pinvm.lib kernel32.lib)
FOREACH(TEMP_VAR ${PIN_COMMON_LIBS})
	TARGET_LINK_LIBRARIES(PIN_TARGET ${TEMP_VAR})
ENDFOREACH()

IF(CMAKE_CL_64 EQUAL 1)
	TARGET_LINK_LIBRARIES(PIN_TARGET ntdll-64.lib) 
ELSEIF(CMAKE_CL_64 EQUAL 0)
	TARGET_LINK_LIBRARIES(PIN_TARGET ntdll-32.lib) 
ENDIF()

################################
#
# PIN_COMMON_LDFLAGS      = /DLL /EXPORT:main /NODEFAULTLIB
# PIN_IA32_LDFLAGS        = /MACHINE:x86 /ENTRY:Ptrace_DllMainCRTStartup@12 /BASE:0x55000000
# PIN_IA32E_LDFLAGS       = /MACHINE:x64 /ENTRY:Ptrace_DllMainCRTStartup /BASE:0xC5000000
#
################################

SET(CMAKE_SHARED_LINKER_FLAGS "")

SET(PIN_COMMON_LDFLAGS /DLL /EXPORT:main /NODEFAULTLIB)
FOREACH(TEMP_VAR ${PIN_COMMON_LDFLAGS})
	SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${TEMP_VAR}") 
ENDFOREACH()

IF(CMAKE_CL_64 EQUAL 1)
	SET(PIN_IA32E_LDFLAGS /MACHINE:x64 /ENTRY:Ptrace_DllMainCRTStartup /BASE:0xC5000000)
	FOREACH(TEMP_VAR ${PIN_IA32E_LDFLAGS})
		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${TEMP_VAR}") 
	ENDFOREACH()
ELSEIF(CMAKE_CL_64 EQUAL 0)
	SET(PIN_IA32_LDFLAGS /MACHINE:x86 /ENTRY:Ptrace_DllMainCRTStartup@12 /BASE:0x55000000)
	FOREACH(TEMP_VAR ${PIN_IA32_LDFLAGS})
		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${TEMP_VAR}") 
	ENDFOREACH()
ENDIF()
