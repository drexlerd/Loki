add_executable(loki "loki.cpp")
target_compile_definitions(loki PRIVATE LOKI_VERSION="${PROJECT_VERSION}")
target_link_libraries(loki loki::parsers)

if(LOKI_BUILD_TESTS)
    add_test(NAME loki_cli_version COMMAND loki --version)
    set_tests_properties(loki_cli_version PROPERTIES
        PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")

    set(LOKI_CLI_MISSING_DOMAIN_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/loki_cli_missing_domain.cmake")
    file(GENERATE
        OUTPUT "${LOKI_CLI_MISSING_DOMAIN_SCRIPT}"
        CONTENT [=[
execute_process(
    COMMAND "$<TARGET_FILE:loki>" "missing-domain-fixture/domain.pddl"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)
if(result EQUAL 0)
    message(FATAL_ERROR "missing-domain CLI invocation unexpectedly succeeded")
endif()
set(combined "${output}${error}")
string(FIND "${combined}" "Could not open PDDL file" diagnostic_position)
if(diagnostic_position EQUAL -1)
    message(FATAL_ERROR "missing-domain CLI invocation did not report the expected diagnostic: ${combined}")
endif()
]=])
    add_test(NAME loki_cli_missing_domain COMMAND "${CMAKE_COMMAND}" -P "${LOKI_CLI_MISSING_DOMAIN_SCRIPT}")

    set(LOKI_CLI_OUT_PROBLEM_REQUIRES_PROBLEM_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/loki_cli_out_problem_requires_problem.cmake")
    file(GENERATE
        OUTPUT "${LOKI_CLI_OUT_PROBLEM_REQUIRES_PROBLEM_SCRIPT}"
        CONTENT [=[
set(domain_path "${CMAKE_CURRENT_BINARY_DIR}/loki-cli-out-problem-domain.pddl")
set(problem_output_path "${CMAKE_CURRENT_BINARY_DIR}/loki-cli-out-problem-output.pddl")
file(WRITE "${domain_path}" "
(define (domain cli-out-problem)
  (:predicates (p))
)
")
execute_process(
    COMMAND "$<TARGET_FILE:loki>" "--out-problem" "${problem_output_path}" "${domain_path}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)
if(result EQUAL 0)
    message(FATAL_ERROR "out-problem-without-problem CLI invocation unexpectedly succeeded")
endif()
if(EXISTS "${problem_output_path}")
    message(FATAL_ERROR "out-problem-without-problem CLI invocation unexpectedly wrote an output file")
endif()
set(combined "${output}${error}")
string(FIND "${combined}" "--out-problem requires a problem file" diagnostic_position)
if(diagnostic_position EQUAL -1)
    message(FATAL_ERROR "out-problem-without-problem CLI invocation did not report the expected diagnostic: ${combined}")
endif()
]=])
    add_test(NAME loki_cli_out_problem_requires_problem COMMAND "${CMAKE_COMMAND}" -P "${LOKI_CLI_OUT_PROBLEM_REQUIRES_PROBLEM_SCRIPT}")

    set(LOKI_CLI_OUT_DOMAIN_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/loki_cli_out_domain.cmake")
    file(GENERATE
        OUTPUT "${LOKI_CLI_OUT_DOMAIN_SCRIPT}"
        CONTENT [=[
set(domain_path "${CMAKE_CURRENT_BINARY_DIR}/loki-cli-out-domain-input.pddl")
set(domain_output_path "${CMAKE_CURRENT_BINARY_DIR}/loki-cli-out-domain-output.pddl")
file(REMOVE "${domain_output_path}")
file(WRITE "${domain_path}" "
(define (domain cli-out-domain)
  (:predicates (p))
)
")
execute_process(
    COMMAND "$<TARGET_FILE:loki>" "--out-domain" "${domain_output_path}" "${domain_path}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "out-domain CLI invocation failed unexpectedly:
${output}
${error}")
endif()
if(NOT EXISTS "${domain_output_path}")
    message(FATAL_ERROR "out-domain CLI invocation did not write an output file")
endif()
file(READ "${domain_output_path}" domain_output)
string(FIND "${domain_output}" "(define (domain cli-out-domain)" domain_position)
if(domain_position EQUAL -1)
    message(FATAL_ERROR "out-domain CLI output did not contain the translated domain name: ${domain_output}")
endif()
string(FIND "${domain_output}" "(:predicates" predicates_position)
if(predicates_position EQUAL -1)
    message(FATAL_ERROR "out-domain CLI output did not contain a predicate section: ${domain_output}")
endif()
string(FIND "${domain_output}" "(p)" predicate_position)
if(predicate_position EQUAL -1)
    message(FATAL_ERROR "out-domain CLI output did not contain the user predicate declaration: ${domain_output}")
endif()
]=])
    add_test(NAME loki_cli_out_domain COMMAND "${CMAKE_COMMAND}" -P "${LOKI_CLI_OUT_DOMAIN_SCRIPT}")

    set(LOKI_CLI_PARSE_DIAGNOSTIC_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/loki_cli_parse_diagnostic.cmake")
    file(GENERATE
        OUTPUT "${LOKI_CLI_PARSE_DIAGNOSTIC_SCRIPT}"
        CONTENT [=[
set(domain_path "${CMAKE_CURRENT_BINARY_DIR}/loki-cli-parse-domain.pddl")
file(WRITE "${domain_path}" "
(define (domain cli-parse)
  (:predicates (p)
)
")
execute_process(
    COMMAND "$<TARGET_FILE:loki>" "${domain_path}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)
if(result EQUAL 0)
    message(FATAL_ERROR "parse-error CLI invocation unexpectedly succeeded")
endif()
set(combined "${output}${error}")
string(FIND "${combined}" "Could not parse PDDL domain" message_position)
if(message_position EQUAL -1)
    message(FATAL_ERROR "parse-error CLI invocation did not report parse diagnostic: ${combined}")
endif()
string(FIND "${combined}" "line 2, column" location_position)
if(location_position EQUAL -1)
    message(FATAL_ERROR "parse-error CLI invocation did not report source location: ${combined}")
endif()
]=])
    add_test(NAME loki_cli_parse_diagnostic COMMAND "${CMAKE_COMMAND}" -P "${LOKI_CLI_PARSE_DIAGNOSTIC_SCRIPT}")

    set(LOKI_CLI_STRICT_DIAGNOSTIC_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/loki_cli_strict_diagnostic.cmake")
    file(GENERATE
        OUTPUT "${LOKI_CLI_STRICT_DIAGNOSTIC_SCRIPT}"
        CONTENT [=[
set(domain_path "${CMAKE_CURRENT_BINARY_DIR}/loki-cli-strict-domain.pddl")
file(WRITE "${domain_path}" "
(define (domain cli-strict)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :precondition (or (p) (q))
    :effect (and))
)
")
execute_process(
    COMMAND "$<TARGET_FILE:loki>" "--strict" "${domain_path}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)
if(result EQUAL 0)
    message(FATAL_ERROR "strict CLI invocation unexpectedly succeeded")
endif()
set(combined "${output}${error}")
string(FIND "${combined}" ":disjunctive-preconditions" requirement_position)
if(requirement_position EQUAL -1)
    message(FATAL_ERROR "strict CLI invocation did not report the missing requirement: ${combined}")
endif()
string(FIND "${combined}" "line 6, column 19" location_position)
if(location_position EQUAL -1)
    message(FATAL_ERROR "strict CLI invocation did not report source location: ${combined}")
endif()
]=])
    add_test(NAME loki_cli_strict_diagnostic COMMAND "${CMAKE_COMMAND}" -P "${LOKI_CLI_STRICT_DIAGNOSTIC_SCRIPT}")
endif()
