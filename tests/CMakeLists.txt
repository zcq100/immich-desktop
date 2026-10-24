find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Network Test)

qt_add_executable(TlsConfigTest
    TlsConfigTest.cpp
    ${CMAKE_SOURCE_DIR}/src/core/TlsConfig.cpp
    ${CMAKE_SOURCE_DIR}/src/core/TlsConfig.h
    ${CMAKE_SOURCE_DIR}/src/core/VideoStreamServer.cpp
    ${CMAKE_SOURCE_DIR}/src/core/VideoStreamServer.h
)

target_include_directories(TlsConfigTest PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(TlsConfigTest PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Test
)

add_test(NAME TlsConfigTest COMMAND TlsConfigTest)

qt_add_executable(I18nTest
    I18nTest.cpp
    ${CMAKE_SOURCE_DIR}/src/core/I18n.cpp
    ${CMAKE_SOURCE_DIR}/src/core/I18n.h
)

target_include_directories(I18nTest PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(I18nTest PRIVATE
    Qt6::Core
    Qt6::Test
)

add_test(NAME I18nTest COMMAND I18nTest)

qt_add_executable(AppSettingsTest
    AppSettingsTest.cpp
    ${CMAKE_SOURCE_DIR}/src/core/AppSettings.cpp
    ${CMAKE_SOURCE_DIR}/src/core/AppSettings.h
    ${CMAKE_SOURCE_DIR}/src/core/I18n.cpp
    ${CMAKE_SOURCE_DIR}/src/core/I18n.h
    ${CMAKE_SOURCE_DIR}/src/core/Theme.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Theme.h
)

target_include_directories(AppSettingsTest PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(AppSettingsTest PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Test
)

if(WIN32)
    target_link_libraries(AppSettingsTest PRIVATE crypt32)
endif()

add_test(NAME AppSettingsTest COMMAND AppSettingsTest)
