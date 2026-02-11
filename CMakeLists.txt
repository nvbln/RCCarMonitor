cmake_minimum_required(VERSION 3.15...4.2)

project(RCCarMonitor VERSION 0.1
                     DESCRIPTION "Monitoring program for RC Car"
                     LANGUAGES CXX)

# Allows LSPs to read out compilation settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Enable building the code generator in the sdbuscpp dependency
set(SDBUSCPP_BUILD_CODEGEN ON CACHE BOOL "Enable the C++-bindings code generator" FORCE)

add_subdirectory(external/sdbus-cpp)

# Generate the code based on the XML definitions.
set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Define all the xml files.
set(DBUS_XML_FILES
    ${PROJECT_SOURCE_DIR}/dbus/ObjectManager.xml
    ${PROJECT_SOURCE_DIR}/dbus/Device1.xml
    ${PROJECT_SOURCE_DIR}/dbus/GattCharacteristic1.xml
)

set(GENERATED_HEADERS "")

foreach(xml_file ${DBUS_XML_FILES})
    get_filename_component(name ${xml_file} NAME_WE)
    set(out ${GENERATED_DIR}/${name}.generated.h)

    add_custom_command(
        OUTPUT ${out}
        COMMAND sdbus-c++-xml2cpp ${xml_file} --proxy=${out}
        DEPENDS ${xml_file}
        COMMENT "Generating D-Bus interface for ${name}"
    )

    list(APPEND GENERATED_HEADERS ${out})
endforeach()

add_custom_target(generate_dbus_headers ALL DEPENDS ${GENERATED_HEADERS})

# Define all source files
set(SRCS
    ${PROJECT_SOURCE_DIR}/src/DBusBluetoothManager.cpp
    ${PROJECT_SOURCE_DIR}/src/DBusBluetoothDevice.cpp
#    ${PROJECT_SOURCE_DIR}/src/DBusGattCharacteristic.cpp
)

add_library(application ${SRCS})
add_dependencies(application generate_dbus_headers)
target_link_libraries(application PUBLIC SDBusCpp::sdbus-c++)
target_include_directories(application PUBLIC src ${GENERATED_DIR})

# Create executable for application
add_executable(monitor ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(monitor PRIVATE application SDBusCpp::sdbus-c++)

# Create executable for test
enable_testing()

add_executable(monitor_test
    test/DBusGattCharacteristicTest.cpp
)

target_link_libraries(monitor_test 
    PRIVATE application SDBusCpp::sdbus-c++
    PUBLIC GTest::gtest_main GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(monitor_test)
