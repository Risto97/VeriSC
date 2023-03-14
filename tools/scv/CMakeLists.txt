set(__LIB_NAME scv)

set(SCV_INSTALL_DIR ${GROUP_INSTALL_DIR}/${__LIB_NAME}-${SCV_VERSION})
set(SCV_SRC_DIR ${PROJECT_BINARY_DIR}/src/${__LIB_NAME})
set(SCV_CXX "${CMAKE_CXX_COMPILER} -std=c++${CMAKE_CXX_STANDARD}")
ExternalProject_Add(${__LIB_NAME}
    URL https://www.accellera.org/images/downloads/standards/systemc/${__LIB_NAME}-${SCV_VERSION}.tar.gz
    PREFIX ${PROJECT_BINARY_DIR}
    PATCH_COMMAND sed -i "s/lib-$SYSC_TARGET/lib/g" ${SCV_SRC_DIR}/configure.ac
    CONFIGURE_COMMAND autoreconf -fiv ${SCV_SRC_DIR} && ${SCV_SRC_DIR}/configure CC=${CMAKE_C_COMPILER} CXX=${SCV_CXX} CXXFLAGS=-std=c++${CMAKE_CXX_STANDARD} --with-systemc=${SYSTEMC_INSTALL_DIR} --prefix=${SCV_INSTALL_DIR}
    DEPENDS systemc
    )

add_library(${__LIB_NAME}_shared INTERFACE)
set_property(TARGET ${__LIB_NAME}_shared PROPERTY EXPORT_NAME shared)
target_link_libraries(${__LIB_NAME}_shared INTERFACE ${SCV_INSTALL_DIR}/lib/lib${__LIB_NAME}.so)
target_include_directories(${__LIB_NAME}_shared INTERFACE ${SCV_INSTALL_DIR}/include/)

add_library(${__LIB_NAME}_static INTERFACE)
set_property(TARGET ${__LIB_NAME}_static PROPERTY EXPORT_NAME static)
target_link_libraries(${__LIB_NAME}_static INTERFACE ${SCV_INSTALL_DIR}/lib/lib${__LIB_NAME}.a)
target_include_directories(${__LIB_NAME}_static INTERFACE ${SCV_INSTALL_DIR}/include/)

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
    VERSION ${SCV_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(EXPORT ${__LIB_NAME}Targets
    FILE ${__LIB_NAME}Targets.cmake
    NAMESPACE ${__LIB_NAME}::
    DESTINATION ${SCV_INSTALL_DIR}/lib/cmake/${__LIB_NAME})

install(FILES "${__LIB_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${__LIB_NAME}ConfigVersion.cmake"
    DESTINATION ${SCV_INSTALL_DIR}/lib/cmake/${__LIB_NAME})
