#include "ConsoleWindow.h"
#include <QApplication>
#include "ConsoleView.h"
#include "GameController.h"
#include "ConsoleView.h"
#include "LoginDialog.h"
#include "RoomDialog.h"
#include "GraphicsUI.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    /*
//    ConsoleWindow w;
//    w.show();
    QList<QString> completed;
    completed
//            << "Geralt_Igni"
//            << "Arachas"
            //            << "Roach"
//            << "Vran_Warrior"
//            << "Arachas_Behemoth"
            //            << "Thunderbolt_Potion"
            //            << "Commanders_Horn"
//            << "Lacerate"
            //            << "Harpy_Egg"
            //            << "Celaeno_Harpy"
            //            << "Earth_Elemental"
            //            << "Crone_Brewess"
            //            << "Crone_Weavess"
            //            << "Crone_Whispess"
            //            << "Unseen_Elder"
            //            << "Frightener"
            //            << "Geels"
            //            << "Bekkers_Twisted_Mirror"
            //            << "Rally"
            << "Biting_Frost"
            //            << "Impenetrable_Fog"
            //            << "Torrential_Rain"
            //            << "Clear_Skies"
            //            << "First_Light"
            << "Dagon"
            << "Foglet"
            << "Woodland_Spirit"
            << "Archgriffin"
            << "Wild_Hunt_Rider"
            << "Caranthir";

    QList<QString> zeroNames, oneNames;
    for (int i = 0; i < 15; i++) {
        zeroNames << completed[qrand() % completed.size()];
        oneNames << completed[qrand() % completed.size()];
    }
    GameController::controller()->getAssets()->setRandomSeed(234);
    GameController::controller()->getAssets()->loadPlayerDeck(zeroNames, oneNames);
    GameController::controller()->setPlayerName(0, QString("Player0"));
    GameController::controller()->setPlayerName(1, QString("Player1"));
    GameController::controller()->getGameUI()->setLocalPlayer(0);
    GameController::controller()->startGame();
     */
//    GameController::controller()->setAssets(new GameAssets);
//    GameController::controller()->setGameUI(new GraphicsUI);
//    GameController::controller()->startDeckBuilder();
//    LoginDialog dialog;
//    qDebug() << dialog.exec();
    GraphicsUI ui;
    return a.exec();
}