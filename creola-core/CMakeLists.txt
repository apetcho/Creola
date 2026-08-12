set(CREOLA_CORE_HEADERS
    creola-common.hpp
    creola-parser.hpp
    creola-pprint.hpp
    creola-expr.hpp
    creola-engine.hpp
    creola.hpp
)


set(CREOLA_CORE_SOURCES
    creola-common.cpp
    creola-parser.cpp
    creola-pprint.cpp
    creola-expr.cpp
    creola-engine.cpp
)

add_library(creola ${CREAOLA_CRORE_SOUCRES} ${CREOLA_CORE_HEADERS})