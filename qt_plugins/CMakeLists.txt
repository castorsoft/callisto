cmake_minimum_required(VERSION 3.19)
project(castor_designer_plugins LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Designer QUIET)

if(NOT Qt6Designer_FOUND)
    message(WARNING "Qt6 Designer component not found. Skipping castor_designer_plugins.")
    return()
endif()

set(PLUGIN_SOURCES
    CastorWidgetsPlugin.h
    CastorWidgetsPlugin.cpp
    InputTextPlugin.h
    InputTextPlugin.cpp
    InputMaskedPlugin.h
    InputMaskedPlugin.cpp
    InputComboBoxPlugin.h
    InputComboBoxPlugin.cpp
    InputCheckBoxPlugin.h
    InputCheckBoxPlugin.cpp
    InputRadioGroupPlugin.h
    InputRadioGroupPlugin.cpp
    InputLookupPlugin.h
    InputLookupPlugin.cpp
    InputButtonPlugin.h
    InputButtonPlugin.cpp
    ../src/presentation/components/inputs/InputBase.h
    ../src/presentation/components/inputs/InputBase.cpp
    ../src/presentation/components/inputs/InputText.h
    ../src/presentation/components/inputs/InputText.cpp
    ../src/presentation/components/inputs/InputMasked.h
    ../src/presentation/components/inputs/InputMasked.cpp
    ../src/presentation/components/inputs/InputComboBox.h
    ../src/presentation/components/inputs/InputComboBox.cpp
    ../src/presentation/components/inputs/InputCheckBox.h
    ../src/presentation/components/inputs/InputCheckBox.cpp
    ../src/presentation/components/inputs/InputRadioGroup.h
    ../src/presentation/components/inputs/InputRadioGroup.cpp
    ../src/presentation/components/inputs/InputLookup.h
    ../src/presentation/components/inputs/InputLookup.cpp
    ../src/presentation/components/inputs/InputButton.h
    ../src/presentation/components/inputs/InputButton.cpp
)

add_library(castor_designer_plugins MODULE ${PLUGIN_SOURCES})

target_include_directories(castor_designer_plugins PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/presentation/components/inputs
)

target_link_libraries(castor_designer_plugins PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Designer
)

set_target_properties(castor_designer_plugins PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins/designer"
)

# Auto-copy to Qt SDK designer plugins folder for Qt Creator / Qt Designer
set(QT_SDK_DESIGNER_DIR "/home/lamb/Qt/6.11.1/gcc_64/plugins/designer")
if(EXISTS "${QT_SDK_DESIGNER_DIR}")
    add_custom_command(TARGET castor_designer_plugins POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:castor_designer_plugins>"
            "${QT_SDK_DESIGNER_DIR}/$<TARGET_FILE_NAME:castor_designer_plugins>"
        COMMENT "Auto-installing castor_designer_plugins to Qt SDK (${QT_SDK_DESIGNER_DIR})"
    )
endif()

# Also auto-copy to Qt Creator's designer plugins folder
set(QT_CREATOR_DESIGNER_DIR "/home/lamb/Qt/Tools/QtCreator/lib/Qt/plugins/designer")
if(EXISTS "${QT_CREATOR_DESIGNER_DIR}")
    add_custom_command(TARGET castor_designer_plugins POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:castor_designer_plugins>"
            "${QT_CREATOR_DESIGNER_DIR}/$<TARGET_FILE_NAME:castor_designer_plugins>"
        COMMENT "Auto-installing castor_designer_plugins to Qt Creator (${QT_CREATOR_DESIGNER_DIR})"
    )
endif()
