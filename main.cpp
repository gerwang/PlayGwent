#include "ConsoleWindow.h"
#include <QApplication>
#include "ConsoleView.h"
#include "GameController.h"
#include "ConsoleView.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    ConsoleWindow w;
//    w.show();
    GameController::getController()->setAssets(new GameAssets);
    GameController::getController()->setGameUI(new ConsoleView);
    QList<QString> zeroNames, oneNames;
    for (int i = 0; i < 15; i++) {
        zeroNames << "Geralt_Igni";
        oneNames << "Geralt_Igni";
    }
    GameController::getController()->getAssets()->setRandomSeed(233);
    GameController::getController()->getAssets()->loadPlayerDeck(zeroNames, oneNames);
    GameController::getController()->getAssets()->setPlayerName(0, QString("Player0"));
    GameController::getController()->getAssets()->setPlayerName(1, QString("Player1"));
    GameController::getController()->getGameUI()->setLocalPlayer(0);
    GameController::getController()->startGame();
    return a.exec();
}