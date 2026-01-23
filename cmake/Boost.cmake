include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)      
# --- Fetch Boost ---
set(BOOST_INCLUDE_LIBRARIES bimap describe dynamic_bitset flyweight math multi_array multi_index stacktrace static_string graph)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_Declare(
        Boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.gz
        USES_TERMINAL_DOWNLOAD TRUE
        DOWNLOAD_NO_EXTRACT FALSE
      )
FetchContent_MakeAvailable(Boost)