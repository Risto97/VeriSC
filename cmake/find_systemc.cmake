# SystemC dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Find SystemC using SystemC's CMake integration
set(CMAKE_PREFIX_PATH "${SYSTEMC}")
find_package(SystemCLanguage CONFIG REQUIRED)
if (NOT SystemCLanguage_FOUND)
    message(FATAL_ERROR "SystemC not found")
endif()
