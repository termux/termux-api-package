cmake_minimum_required(VERSION 3.10.0)
project(termux-usb)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libusb REQUIRED libusb-1.0)
# For test building on Linux below needs to be changed to
# find_package(Protobuf REQUIRED CONFIG) for some reason..
find_package(Protobuf REQUIRED)
protobuf_generate(
    LANGUAGE C
    GENERATE_EXTENSIONS .pb-c.c
    OUT_VAR PROTO_SRCS
    PROTOS proto/UsbAPI.proto
)
add_library(termux-usb SHARED termux-usb.c termux-usb.h ${PROTO_SRCS} ${PROTO_HDRS})

target_include_directories(
  termux-usb
  PUBLIC ${Protobuf_INCLUDE_DIRS} ${libusb_INCLUDE_DIRS}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/proto ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}
)
target_link_libraries(termux-usb PUBLIC ${Protobuf_LIBRARIES} ${CMAKE_BINARY_DIR}/libtermux-api.so -lprotobuf-c ${libusb_LIBRARIES})

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libtermux-usb.so
  TYPE LIB
)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/termux-usb.h
  TYPE INCLUDE
)
