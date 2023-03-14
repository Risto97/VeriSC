find_package(gcc CONFIG REQUIRED
    PATHS ${VERISC_INSTALL_DIR}/*
    NO_DEFAULT_PATH
    )

cmake_minimum_required(VERSION 3.20)
project(uvc VERSION 0.0.1)

set(UVC_LIBS apb nmi i2c common)

foreach(lib ${UVC_LIBS})
    add_subdirectory(${lib})
endforeach()

add_library(${PROJECT_NAME} SHARED)
set_property(TARGET ${PROJECT_NAME} PROPERTY EXPORT_NAME shared)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${UVC_LIBS}
    )

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
    )

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    FILE_SET HEADERS
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME})

install(FILES "${PROJECT_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME})
