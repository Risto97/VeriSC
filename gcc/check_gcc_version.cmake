set(CMAKE_SYSTEM_MIC_CLUSTER "Linux-3.10.0-1160.71.1.el7.x86_64") # TODO IS this enough?
message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")

message(STATUS "G++: ${CMAKE_CXX_COMPILER}")
set(GCC_VERSION ${CMAKE_CXX_COMPILER_VERSION})

set(CXX_COMPILER ${CMAKE_CXX_COMPILER}) # Default path
set(C_COMPILER ${CMAKE_C_COMPILER}) # Default path
set(COMPILE_GCC TRUE)           #Defaults
set(DOWNLOAD_GCC_BINARY FALSE)  #Defaults
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    if (NOT (GCC_VERSION VERSION_GREATER 7.0 OR GCC_VERSION VERSION_EQUAL 7.0))
        message(STATUS "${PROJECT_NAME} requires g++ 7.0 or greater.")

        set(GCC_VERSION 12.2.0)
        set(GCC_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/gcc-${GCC_VERSION})
        set(GCC_SRC_DIR ${PROJECT_BINARY_DIR}/src/gcc-${GCC_VERSION})
        set(CXX_COMPILER ${GCC_INSTALL_DIR}/bin/g++)
        set(C_COMPILER ${GCC_INSTALL_DIR}/bin/gcc)

        if(CMAKE_SYSTEM MATCHES ${CMAKE_SYSTEM_MIC_CLUSTER}) # IF precompiled binary exists
            set(DOWNLOAD_GCC_BINARY TRUE)
            set(COMPILE_GCC FALSE)
        else() # If no precompiled binary exists
            set(DOWNLOAD_GCC_BINARY FALSE)
            set(COMPILE_GCC TRUE)
        endif()
    else()                          # Instead use system GCC
        set(COMPILE_GCC FALSE)
        set(DOWNLOAD_GCC_BINARY FALSE)
        set(GCC_VERSION ${CMAKE_CXX_COMPILER_VERSION})
        set(CXX_COMPILER ${CMAKE_CXX_COMPILER}) # Default path
        set(C_COMPILER ${CMAKE_C_COMPILER}) # Default path
    endif ()

elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    message(FATAL_ERROR "Clang is not supported at the moment") # TODO still recompile GCC, comment this message and try
else ()
    message(FATAL_ERROR "Other compilers than GNU are not supported at the moment") # TODO still recompile GCC, comment this message and try
endif ()
