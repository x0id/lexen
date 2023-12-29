find_package(Boost 1.74 COMPONENTS filesystem system unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories("../src")

# core test
add_executable(lexen_test
    test_main.cpp
    test_parser.cpp
    be_parser.cpp
)

target_compile_definitions(lexen_test PUBLIC BOOST_TEST_DYN_LINK)
target_compile_options(lexen_test PUBLIC -W -Wall -Wextra -pedantic -pedantic-errors)
target_link_libraries(lexen_test ${Boost_LIBRARIES})

# extension demo test
add_executable(lexen_ext_test
    test_main.cpp
    test_extension.cpp
    be_parser_extension.cpp
)

target_compile_definitions(lexen_ext_test PUBLIC BOOST_TEST_DYN_LINK)
target_compile_options(lexen_ext_test PUBLIC -W -Wall -Wextra -pedantic -pedantic-errors)
target_link_libraries(lexen_ext_test ${Boost_LIBRARIES})

enable_testing()
add_test(NAME lexen_test COMMAND lexen_test)
add_test(NAME lexen_ext_test COMMAND lexen_ext_test)
