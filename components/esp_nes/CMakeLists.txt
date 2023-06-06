file(GLOB_RECURSE T_SOURCES *.c)
file(GLOB_RECURSE IMAGES images/*.c)

idf_component_register(
    SRCS ${T_SOURCES} ${IMAGES}
    INCLUDE_DIRS "."
    PRIV_REQUIRES driver nofrendo hid nvs_flash
)
