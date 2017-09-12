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
find_package(Qt5Network REQUIRED)
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
        ConsoleIOBuffer.h
        logindialog.cpp
        roomdialog.cpp
        CardMonitors/Geralt_Igni.cpp CardMonitors/Geralt_Igni.h CardMonitors/CardMonitorBase.cpp CardMonitors/CardMonitorBase.h CardMonitors/Arachas.cpp CardMonitors/Arachas.h CardMonitors/Roach.cpp CardMonitors/Roach.h CardMonitors/Arachas_Hatchling.cpp CardMonitors/Arachas_Hatchling.h CardMonitors/Vran_Warrior.cpp CardMonitors/Vran_Warrior.h CardMonitors/Arachas_Behemoth.cpp CardMonitors/Arachas_Behemoth.h CardMonitors/Thunderbolt_Potion.cpp CardMonitors/Thunderbolt_Potion.h CardMonitors/Commanders_Horn.cpp CardMonitors/Commanders_Horn.h CardMonitors/Lacerate.cpp CardMonitors/Lacerate.h CardMonitors/Harpy_Egg.cpp CardMonitors/Harpy_Egg.h CardMonitors/Celaeno_Harpy.cpp CardMonitors/Celaeno_Harpy.h CardMonitors/Earth_Elemental.cpp CardMonitors/Earth_Elemental.h CardMonitors/Crone_Brewess.cpp CardMonitors/Crone_Brewess.h CardMonitors/Crone_Weavess.cpp CardMonitors/Crone_Weavess.h CardMonitors/Crone_Whispess.cpp CardMonitors/Crone_Whispess.h CardMonitors/Unseen_Elder.cpp CardMonitors/Unseen_Elder.h CardMonitors/Frightener.cpp CardMonitors/Frightener.h CardMonitors/Geels.cpp CardMonitors/Geels.h CardMonitors/Bekkers_Twisted_Mirror.cpp CardMonitors/Bekkers_Twisted_Mirror.h CardMonitors/Rally.cpp CardMonitors/Rally.h CardMonitors/Biting_Frost.cpp CardMonitors/Biting_Frost.h CardMonitors/Impenetrable_Fog.cpp CardMonitors/Impenetrable_Fog.h CardMonitors/Torrential_Rain.cpp CardMonitors/Torrential_Rain.h CardMonitors/Clear_Skies.cpp CardMonitors/Clear_Skies.h CardMonitors/First_Light.cpp CardMonitors/First_Light.h CardMonitors/Dagon.cpp CardMonitors/Dagon.h CardMonitors/Foglet.cpp CardMonitors/Foglet.h CardMonitors/Woodland_Spirit.cpp CardMonitors/Woodland_Spirit.h CardMonitors/Archgriffin.cpp CardMonitors/Archgriffin.h CardMonitors/Wild_Hunt_Rider.cpp CardMonitors/Wild_Hunt_Rider.h CardMonitors/Caranthir.cpp CardMonitors/Caranthir.h Deck.cpp Deck.h NetworkManager.cpp NetworkManager.h ScreenInputBuffer.cpp ScreenInputBuffer.h)
add_executable(PlayGwent ${SOURCE_FILES})

target_link_libraries(PlayGwent Qt5::Widgets Qt5::Core Qt5::Network)