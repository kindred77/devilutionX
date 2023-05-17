include(functions/FetchContent_MakeAvailableExcludeFromAll)

include(FetchContent)
FetchContent_Declare(boost
    URL https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz
    URL_HASH SHA256=66a469b6e608a51f8347236f4912e27dc5c60c60d7d53ae9bfe4683316c6f04c
)
FetchContent_MakeAvailableExcludeFromAll(boost)

add_library(boost INTERFACE)
target_include_directories(boost INTERFACE ${boost_SOURCE_DIR})
add_library(boost::boost ALIAS boost)
