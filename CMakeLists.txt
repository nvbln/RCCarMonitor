cmake_minimum_required(VERSION 3.15...4.2)

project(RCCarMonitor VERSION 0.1
                     DESCRIPTION "Monitoring program for RC Car"
                     LANGUAGES CXX)

# Define all source files
set(SRCS
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/DBusBluetoothManager.cpp
    ${PROJECT_SOURCE_DIR}/src/DBusBluetoothDevice.cpp
    ${PROJECT_SOURCE_DIR}/src/DBusGattCharacteristic.cpp
)

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

add_executable(monitor ${SRCS})
add_dependencies(monitor generate_dbus_headers)
target_compile_features(monitor PRIVATE cxx_std_20)
target_link_libraries(monitor PRIVATE SDBusCpp::sdbus-c++)
target_include_directories(monitor PRIVATE ${GENERATED_DIR})
