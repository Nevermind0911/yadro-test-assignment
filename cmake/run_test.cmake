execute_process(COMMAND ${EXEC} ${INPUT})

if(NOT EXISTS result.txt)
    message(FATAL_ERROR "result.txt was not produced")
endif()

file(READ result.txt ACTUAL)
file(READ ${EXPECTED} EXPECTED_CONTENT)

if(NOT "${ACTUAL}" STREQUAL "${EXPECTED_CONTENT}")
    message(FATAL_ERROR
        "Output mismatch\n--- expected ---\n${EXPECTED_CONTENT}--- actual ---\n${ACTUAL}")
endif()
