cmake_minimum_required(VERSION 3.8)
project(PlayGwent)

set(CMAKE_CXX_STANDARD 11)


# set local Qt location
#set(CMAKE_PREFIX_PATH "D:\\Qt\\Qt5.9.0\\5.9\\mingw53_32")

# set moc uic rcc
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_AUTORCC on)

# set modules
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5OpenGL REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

aux_source_directory(CardMonitors/ MONITOR_SRC)

#qrc file put here
set(SOURCE_FILES
        main.cpp
        ConsoleWindow.cpp
        AbstractUI.h
        CardInfo.cpp
        ConsoleView.cpp
        ConsoleView.h
        GameController.cpp
        GameController.h
        GameAssets.cpp
        GameAssets.h
        GameConstant.h
        SubclassRegister.cpp
        SubclassRegister.h
        AbstractInputBuffer.h
        ConsoleIOBuffer.cpp
        ConsoleIOBuffer.h
        LoginDialog.cpp
        RoomDialog.cpp
        ZoomGraphicsView.cpp
        CardWidget.cpp
        ${MONITOR_SRC}
        Deck.cpp Deck.h
        NetworkManager.cpp NetworkManager.h
        ScreenInputBuffer.cpp ScreenInputBuffer.h
        GraphicsUI.cpp GraphicsUI.h
        CardArrayWidget.cpp CardArrayWidget.h
        AnimatedEllipse.cpp AnimatedEllipse.h
        AnimatedRect.cpp AnimatedRect.h
        AbstractOutputBuffer.h ScreenIOBuffer.cpp ScreenIOBuffer.h
        TimeWidget.cpp TimeWidget.h resource.qrc)
add_executable(PlayGwent ${SOURCE_FILES})

if(WIN32 AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set_property(TARGET PlayGwent PROPERTY WIN32_EXECUTABLE true)
endif()

target_link_libraries(PlayGwent Qt5::Widgets Qt5::Core Qt5::Network Qt5::OpenGL)