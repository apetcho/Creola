set(READLINE_LIB_DIR /opt/homebrew/Cellar/readline/8.3.3/lib)
set(READLINE_INC_DIR /opt/homebrew/Cellar/readline/8.3.3/include)

find_path(
    CREOLA_READLINE_INCLUDE_DIR
    NAMES readline/readline.h
    HINTS "${READLINE_INC_DIR}"
)

find_path(
    CREOLA_HISTORY_INCLUDE_DIR
    NAMES readline/history.h
    HINTS "${READLINE_INC_DIR}"
)

find_library(CREOLA_READLINE_LIB
    NAMES readline
    HINTS "${READLINE_LIB_DIR}"
)
find_library(CREOLA_HISTORY_LIB
    NAMES history
    HINTS HINTS "${READLINE_LIB_DIR}"
)

set(CREOLA_READLINE_INCLUDE_DIRS 
    ${CREOLA_READLINE_INCLUDE_DIR} ${CREOLA_HISTORY_INCLUDE_DIR}
)
set(CREOLA_READLINE_LIBRARIES ${CREOLA_READLINE_LIB} ${CREOLA_HISTORY_LIB})

message("CREOLA_READLINE_LIBRARIES: ${CREOLA_READLINE_LIBRARIES}")

if(CREOLA_READLINE_INCLUDE_DIR AND NOT TARGET Creola::Readline)
    add_library(Creola::Readline INTERFACE IMPORTED)
    set_target_properties(
        Creola::Readline
        PROPERTIES
        INTERFACE_LINK_LIBRARIES "${CREOLA_READLINE_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${CREOLA_READLINE_INCLUDE_DIR}"
    )
endif()

add_executable(creola-cli main.cpp cli.cpp)
target_link_libraries(creola-cli PRIVATE creola Creola::Readline)