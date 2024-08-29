cmake_minimum_required(VERSION 3.30)

# Project details
project(CaroGame)

# Set directories
set(SOURCE_DIR src)
set(BUILD_DIR build)
set(CLIENT_SOURCE_DIR ${SOURCE_DIR}/client)
set(SERVER_SOURCE_DIR ${SOURCE_DIR}/server)
set(COMMON_SOURCE_DIR ${SOURCE_DIR}/common)
set(MODEL_SOURCE_DIR ${SOURCE_DIR}/model)
set(VIEW_SOURCE_DIR ${SOURCE_DIR}/view)
set(CONTROL_SOURCE_DIR ${SOURCE_DIR}/control)

# Include directories
include_directories(${COMMON_SOURCE_DIR} ${MODEL_SOURCE_DIR} ${VIEW_SOURCE_DIR})

# Find all source files
file(GLOB_RECURSE CLIENT_SOURCE_FILES ${COMMON_SOURCE_DIR}/*.c ${VIEW_SOURCE_DIR}/*.c ${CLIENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE SERVER_SOURCE_FILES ${COMMON_SOURCE_DIR}/*.c ${MODEL_SOURCE_DIR}/*.c ${CONTROL_SOURCE_DIR}/*.c ${SERVER_SOURCE_DIR}/*.c)

# Add executable targets
add_executable(client_app ${CLIENT_SOURCE_FILES})
add_executable(server_app ${SERVER_SOURCE_FILES})

# Specify compile options
if(MSVC)
    # Link Winsock library
    target_link_libraries(client_app ws2_32)
    target_link_libraries(server_app ws2_32)
    target_compile_options(client_app PRIVATE -Wall -W1)
    target_compile_options(server_app PRIVATE -Wall -W1)
else()
    target_compile_options(client_app PRIVATE -Wall -Wextra -pthread)
    target_compile_options(server_app PRIVATE -Wall -Wextra -pthread)
endif()