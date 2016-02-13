FIND_PACKAGE(Doxygen)

if(DOXYGEN_FOUND STREQUAL "YES")
  set(blooto_DOC_INPUT_DIR ${blooto_SOURCE_DIR}/doc)
  set(blooto_DOC_OUTPUT_DIR ${blooto_BINARY_DIR}/doc)

  set(blooto_DOC_MAINPAGE ${blooto_DOC_INPUT_DIR}/blooto.txt)
  set(blooto_DOXYGEN_OUTPUT ${blooto_DOC_OUTPUT_DIR}/html/index.html)

  set(blooto_DOXYGEN_CONFIG_IN ${blooto_DOC_INPUT_DIR}/Doxyfile.in)
  set(blooto_DOXYGEN_CONFIG ${blooto_DOC_OUTPUT_DIR}/Doxyfile)

  configure_file(${blooto_DOXYGEN_CONFIG_IN} ${blooto_DOXYGEN_CONFIG} @ONLY)

  file (GLOB_RECURSE blooto_DOC_HEADERS ${blooto_SOURCE_DIR}/blooto/*.hpp)

  add_custom_command (
    OUTPUT ${blooto_DOXYGEN_OUTPUT}
    COMMAND ${DOXYGEN_EXECUTABLE} ${blooto_DOXYGEN_CONFIG}
    WORKING_DIRECTORY ${blooto_DOC_OUTPUT_DIR}
    DEPENDS ${blooto_DOXYGEN_CONFIG} ${blooto_DOC_MAINPAGE} ${blooto_DOC_HEADERS}
  )

  add_custom_target (apidoc ALL DEPENDS ${blooto_DOXYGEN_OUTPUT})
else(DOXYGEN_FOUND STREQUAL "YES")
  message (WARNING "Doxygen not found! API docs won't be built.")
endif(DOXYGEN_FOUND STREQUAL "YES")
