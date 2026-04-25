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

# Assignment Class test 
gtest_discover_tests(assignment_test
	DISCOVERY_MODE POST_BUILD
)