if(DEFINED ENV{VCS_HOME})
    message("Using enviroment variable VCS_HOME  instead of installed VCS_HOME path: ${VCS_HOME}")
    set(VCS_HOME $ENV{VCS_HOME})
endif()
