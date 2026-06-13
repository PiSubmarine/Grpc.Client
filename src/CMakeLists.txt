include("${_build_cmake_local}")

find_package(gRPC CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

message("Adding library: ${PISUBMARINE_MODULE_NAME}")
add_library(${PISUBMARINE_MODULE_NAME}
        PiSubmarine/Grpc/Client/Channel.cpp
        "../public/PiSubmarine/Grpc/Client/Channel.h")
target_include_directories(${PISUBMARINE_MODULE_NAME} PUBLIC "../public")
PiSubmarineInitTarget(${PISUBMARINE_MODULE_NAME})

PiSubmarineAddDependency("https://github.com/PiSubmarine/Logging.Api.git" "")
target_link_libraries(${PISUBMARINE_MODULE_NAME} PUBLIC PiSubmarine.Logging.Api)

target_link_libraries(${PISUBMARINE_MODULE_NAME} PRIVATE gRPC::grpc++)
target_link_libraries(${PISUBMARINE_MODULE_NAME} PRIVATE spdlog::spdlog)
