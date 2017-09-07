cmake_minimum_required(VERSION 3.8)
project(PlayGwent)

set(CMAKE_CXX_STANDARD 11)


# set local Qt location
set(CMAKE_PREFIX_PATH "D:\\Qt\\Qt5.9.0\\5.9\\mingw53_32")

# set moc uic rcc
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_AUTORCC on)

# set modules
find_package(Qt5Widgets REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

#qrc file put here
set(SOURCE_FILES
        main.cpp
        ConsoleWindow.cpp
        AbstractUI.h
        cardinfo.cpp
        ConsoleView.cpp
        ConsoleView.h
        GameController.cpp
        GameController.h
        GameAssets.cpp
        GameAssets.h
        GameConstant.h
        SubclassRegister.cpp
        SubclassRegister.h
        AbstractIOBuffer.h
        ConsoleIOBuffer.cpp
        ConsoleIOBuffer.h)
add_executable(PlayGwent ${SOURCE_FILES})

target_link_libraries(PlayGwent Qt5::Widgets Qt5::Core)