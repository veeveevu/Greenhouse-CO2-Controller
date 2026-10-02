file(GLOB_RECURSE APP_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/*.cc"
)

list(FILTER APP_SOURCES EXCLUDE REGEX "http_test\\.cpp$") #cái này trùng main của main chính nè

add_executable(GreenHouse ${APP_SOURCES}
        data_structs.h
        rtos_tasks/TaskCloud.cpp
        rtos_tasks/TaskCloud.h
        cloud/WifiManager.cpp
        cloud/WifiManager.h
        cloud/ThingSpeak.cpp
        cloud/ThingSpeak.h
        cloud/secrets.h
        lib/ipstack/tls_common.h)
#[[add_executable(${PROJECT_NAME}
        main.cpp
        lib/critical_section.cpp
        lib/Fmutex.cpp
        lib/Fmutex.h
        blinker.cpp
        blinker.h

        #        ipstack/IPStack.cpp
        #        ipstack/IPStack.h
        lib/ipstack/lwipopts.h
        lib/ipstack/tls_common.c
        lib/ipstack/picow_tls_client.c
        lib/ipstack/lwipopts_tls.h

        lib/i2c/PicoI2C.cpp
        lib/i2c/PicoI2C.h

        lib/uart/PicoOsUart.cpp
        lib/uart/PicoOsUart.h

        lib/modbus/nanomodbus.h
        lib/modbus/nanomodbus.c
        lib/modbus/ModbusRegister.cpp
        lib/modbus/ModbusRegister.h
        lib/modbus/ModbusClient.cpp
        lib/modbus/ModbusClient.h

        lib/display/framebuf.cpp
        lib/display/framebuf.h
        lib/display/mono_vlsb.cpp
        lib/display/mono_vlsb.h
        #        display/ssd1306.cpp
        #        display/ssd1306.h
        lib/display/ssd1306os.h
        lib/display/ssd1306os.cpp
)]]

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
        lib/modbus
        lib/uart
        lib/i2c
        lib/display
        lib/ipstack
        drivers
        hardware
        rtos_tasks
)

target_compile_definitions(${ProjectName} PRIVATE
        WIFI_SSID=\"$ENV{WIFI_SSID}\"
        WIFI_PASSWORD=\"$ENV{WIFI_PASSWORD}\"
        NO_SYS=0            # don't want NO_SYS (generally this would be in your lwipopts.h)
        PICO_CYW43_ARCH_DEFAULT_COUNTRY_CODE=CYW43_COUNTRY_FINLAND
)

target_link_libraries(${ProjectName} 
        pico_stdlib
        hardware_i2c
        FreeRTOS-Kernel-Heap4
        pico_cyw43_arch_lwip_sys_freertos
        pico_lwip_mbedtls
        pico_mbedtls

)

# Ignore warnings from lwip code
set_source_files_properties(
        ${PICO_LWIP_PATH}/src/apps/altcp_tls/altcp_tls_mbedtls.c
        PROPERTIES
        COMPILE_OPTIONS "-Wno-unused-result"
)

pico_add_extra_outputs(${ProjectName})

# Disable usb output, enable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
