set(__LIB_NAME uvm-systemc)

set(UVM_INSTALL_DIR ${GROUP_INSTALL_DIR}/${__LIB_NAME}-${UVM-SYSTEMC_VERSION})
set(UVM_SRC_DIR ${PROJECT_BINARY_DIR}/src/${__LIB_NAME})

if(NOT DEFINED UVM-SYSTEMC_HOME)
    ExternalProject_Add(${__LIB_NAME}
        URL https://www.accellera.org/images/downloads/standards/systemc/${__LIB_NAME}-${UVM-SYSTEMC_VERSION}.tar.gz
        PREFIX ${PROJECT_BINARY_DIR}
        CONFIGURE_COMMAND autoreconf -fiv ${UVM_SRC_DIR} && ${UVM_SRC_DIR}/configure CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} CXXFLAGS=-std=c++${CMAKE_CXX_STANDARD} --with-systemc=${SYSTEMC_INSTALL_DIR} --with-arch-suffix= --prefix=${UVM_INSTALL_DIR}
        DEPENDS systemc
        )
else()
    add_library(${__LIB_NAME} INTERFACE)
endif()

# Should not do all of this if the config file is there already

add_library(${__LIB_NAME}_shared INTERFACE)
set_property(TARGET ${__LIB_NAME}_shared PROPERTY EXPORT_NAME shared)
target_link_libraries(${__LIB_NAME}_shared INTERFACE ${UVM_INSTALL_DIR}/lib/lib${__LIB_NAME}.so)
set_property(TARGET ${__LIB_NAME}_shared PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${UVM_INSTALL_DIR}/include/)

add_library(${__LIB_NAME}_static INTERFACE)
set_property(TARGET ${__LIB_NAME}_static PROPERTY EXPORT_NAME static)
target_link_libraries(${__LIB_NAME}_static INTERFACE ${UVM_INSTALL_DIR}/lib/lib${__LIB_NAME}.a)
target_include_directories(${__LIB_NAME}_static INTERFACE ${UVM_INSTALL_DIR}/include/)

install(TARGETS ${__LIB_NAME}_shared ${__LIB_NAME}_static
    EXPORT ${__LIB_NAME}Targets
    FILE_SET HEADERS
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${__LIB_NAME}ConfigVersion.cmake"
    VERSION ${UVM-SYSTEMC_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(EXPORT ${__LIB_NAME}Targets
    FILE ${__LIB_NAME}Targets.cmake
    NAMESPACE ${__LIB_NAME}::
    DESTINATION ${UVM_INSTALL_DIR}/lib/cmake/${__LIB_NAME})

install(FILES "${__LIB_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${__LIB_NAME}ConfigVersion.cmake"
    DESTINATION ${UVM_INSTALL_DIR}/lib/cmake/${__LIB_NAME})
