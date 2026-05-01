add_library(peregrine_core OBJECT
	"../src/clauses/assignment.cpp"
	"../src/parsers/parser.cpp"
	"../src/propagators/propagator.cpp"
	"../src/solvers/dpll.cpp"
)

target_include_directories(peregrine_core PUBLIC
	${CMAKE_SOURCE_DIR}/include
)

include(GoogleTest)

# Helper for adding testcases
function(add_test test_name test_source)
	add_executable(${test_name} ${test_source} $<TARGET_OBJECTS:peregrine_core>)
	target_link_libraries(${test_name} gtest_main gtest)
	target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
	gtest_discover_tests(${test_name} DISCOVERY_MODE POST_BUILD)
endfunction()

add_test(assignment_test unit/assignment_test.cpp)
add_test(clause_test unit/clause_test.cpp)
add_test(propagate_test unit/propagate_test.cpp)
add_test(dpll_test unit/dpll_test.cpp)
