cmake_minimum_required(VERSION 3.16)
project(TrussViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)

add_executable(TrussViewer
    main.cpp
    TrussWidget.cpp
    TrussRenderer.cpp
)

target_link_libraries(TrussViewer PRIVATE Qt6::Widgets)

add_custom_command(TARGET TrussViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/FT1.json"
        "$<TARGET_FILE_DIR:TrussViewer>/FT1.json"
)

add_executable(TrussGen
    TrussGen.cpp
    TrussRenderer.cpp
)
target_link_libraries(TrussGen PRIVATE Qt6::Gui)
