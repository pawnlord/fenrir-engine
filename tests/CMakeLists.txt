include(FetchContent)


FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable (geometry geometry.cpp)
add_executable (input input.cpp)


target_link_libraries (geometry LINK_PUBLIC fenrir GTest::gtest_main)
target_link_libraries (input LINK_PUBLIC fenrir GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(geometry)
gtest_discover_tests(input)