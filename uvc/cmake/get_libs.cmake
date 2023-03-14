find_package(SystemCLanguage CONFIG REQUIRED
    PATHS ${LIBS_CFG_DIR}/*
    NO_DEFAULT_PATH
    )
# find_package(scv CONFIG REQUIRED
#     PATHS ${LIBS_CFG_DIR}/*
#     NO_DEFAULT_PATH
#     )
find_package(uvm-systemc CONFIG REQUIRED
    PATHS ${LIBS_CFG_DIR}/*
    NO_DEFAULT_PATH
    )

list(APPEND LIBS SystemC::systemc uvm-systemc::shared)
