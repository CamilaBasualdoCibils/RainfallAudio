include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)      
# --- Fetch Boost ---
set(BOOST_INCLUDE_LIBRARIES bimap describe dynamic_bitset flyweight math multi_array multi_index stacktrace static_string)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.90.0
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(Boost)