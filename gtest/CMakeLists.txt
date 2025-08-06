cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(
  sp_tests
  hello_test.cc
  vector_test.cc
  latch_test.cc
  bounded_channel_test.cc
  option_test.cc
  mem_pool_test.cc
  server_test.cc
  mmf_test.cc
)

target_link_libraries(
  sp_tests
  GTest::gtest_main
)

set(SANITIZE_FLAGS
  #-fsanitize=address,leak,undefined
  #-fsanitize-address-use-after-scope
  #-fsanitize-recover=all
  #-static-libasan

)

target_compile_options(
  sp_tests PUBLIC -Wall -Wextra -Werror -g3 -O3 ${SANITIZE_FLAGS}
)

target_link_options(
  sp_tests PUBLIC -g3 -O3 ${SANITIZE_FLAGS}
)

#set(MY_COMPIL_FLAGS ${MY_COMPIL_FLAGS} /fsanitize=address)
#set(CMAKE_EXE_LINKER_FLAGS "/fsanitize=address")
#target_compile_options(${TARGET_NAME} PRIVATE ${MY_COMPIL_FLAGS})

target_include_directories( 
  sp_tests PUBLIC "${PROJECT_SOURCE_DIR}/../sp/"
)

include(GoogleTest)
gtest_discover_tests(sp_tests)


