add_library(peregrine_core OBJECT
	"../src/clauses/assignment.cpp"
	"../src/parsers/parser.cpp"
	"../src/propagators/propagator.cpp"
)

target_include_directories(peregrine_core PUBLIC
	${CMAKE_SOURCE_DIR}/include
)

include(GoogleTest)

add_executable(assignment_test
	unit/assignment_test.cpp
	$<TARGET_OBJECTS:peregrine_core>
)

target_link_libraries(assignment_test
	gtest_main
	gtest
)

target_include_directories(assignment_test PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

add_executable(clause_test
	unit/clause_test.cpp
	$<TARGET_OBJECTS:peregrine_core>
)

target_link_libraries(clause_test
	gtest_main
	gtest
)

target_include_directories(clause_test PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

gtest_discover_tests(
	assignment_test
	DISCOVERY_MODE POST_BUILD
)

gtest_discover_tests(
	clause_test
	DISCOVER_MODE POST_BUILD
)
