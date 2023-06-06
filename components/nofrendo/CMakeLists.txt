file(GLOB_RECURSE T_SOURCES *.c)
file(GLOB_RECURSE CPU_SOURCES cpu/*.c)
file(GLOB_RECURSE NSS_SOURCES libsnss/*.c)
file(GLOB_RECURSE NES_SOURCES nes/*.c)
file(GLOB_RECURSE SNDHRDW_SOURCES sndhrdw/*.c)
file(GLOB_RECURSE MAP_SOURCES mappers/*.c)

idf_component_register(SRCS ${T_SOURCES} ${CPU_SOURCES} ${NSS_SOURCES} ${NES_SOURCES} ${SNDHRDW_SOURCES} ${MAP_SOURCES} INCLUDE_DIRS "cpu" "libsnss" "nes" "sndhrdw" ".")
