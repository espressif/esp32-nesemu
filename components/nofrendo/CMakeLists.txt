idf_component_register(
    SRC_DIRS "cpu" "libsnss" "nes" "sndhrdw" "mappers" "."
    INCLUDE_DIRS "cpu" "libsnss" "nes" "sndhrdw" "."
)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=char-subscripts -Wno-error=attributes -DNOFRENDO_DEBUG)
