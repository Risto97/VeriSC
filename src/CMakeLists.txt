# Macro to make a list of all subdirectories in given directory
macro(SUBDIRLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

# Get a list of directories in this directory
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})

set(LIB_INCLUDE_DIRS "")

foreach(subdir ${SUBDIRS})
  add_subdirectory(${subdir})
  list(APPEND LIB_INCLUDE_DIRS ${LIB_INCLUDE_DIR})
endforeach()

set(LIB_INCLUDE_DIRS ${LIB_INCLUDE_DIRS} PARENT_SCOPE)
