cmake_minimum_required(VERSION 3.16.0)
project(Browsio VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Fetch used libraries
if(WIN32)
    set(CURL_INCLUDE_DIR "${workspaceFolder}/dependencies/curl/include") 
    set(CURL_LIBRARY "${workspaceFolder}/dependencies/curl/lib/libcurl.lib")
endif()

find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
add_subdirectory(dependencies/spdlog)

include_directories("./" ${CURL_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})

add_executable(${PROJECT_NAME} src/main.cpp src/Connection.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        CURL::libcurl
        spdlog::spdlog
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)