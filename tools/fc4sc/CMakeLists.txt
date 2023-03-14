set(__LIB_NAME fc4sc)

set(FC4SC_INSTALL_DIR ${GROUP_INSTALL_DIR}/${__LIB_NAME}-${FC4SC_VERSION})
set(FC4SC_SRC_DIR ${PROJECT_BINARY_DIR}/src/${__LIB_NAME})
ExternalProject_Add(${__LIB_NAME}
    GIT_REPOSITORY https://github.com/amiq-consulting/${__LIB_NAME}.git
    GIT_TAG ${FC4SC_VERSION}
    PREFIX ${PROJECT_BINARY_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${FC4SC_SRC_DIR} ${FC4SC_INSTALL_DIR}
    )

add_library(${__LIB_NAME}_lib INTERFACE)
set_property(TARGET ${__LIB_NAME}_lib PROPERTY EXPORT_NAME ${__LIB_NAME})
target_include_directories(${__LIB_NAME}_lib INTERFACE ${FC4SC_INSTALL_DIR}/includes)

install(TARGETS ${__LIB_NAME}_lib
    EXPORT ${__LIB_NAME}Targets
    FILE_SET HEADERS
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${__LIB_NAME}ConfigVersion.cmake"
    VERSION ${FC4SC_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(EXPORT ${__LIB_NAME}Targets
    FILE ${__LIB_NAME}Targets.cmake
    NAMESPACE ${__LIB_NAME}::
    DESTINATION ${FC4SC_INSTALL_DIR}/lib/cmake/${__LIB_NAME})

install(FILES "${__LIB_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${__LIB_NAME}ConfigVersion.cmake"
    DESTINATION ${FC4SC_INSTALL_DIR}/lib/cmake/${__LIB_NAME})
