//
// Created by Gerwa on 2017/9/10.
//

#include "Archgriffin.h"

void Archgriffin::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Archgriffin") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Archgriffin::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Archgriffin") {
        GameController::controller()->changeRowWeather(toR, Weather::Sunny);//remove weather effect
        GameAssets *assets = GameController::controller()->getAssets();
        QList<CardInfo *> graveyardBackup[2];
        graveyardBackup[0] = assets->getCardArray(Player0_Graveyard);
        graveyardBackup[1] = assets->getCardArray(Player1_Graveyard);
        if (!graveyardBackup[0].empty() || !graveyardBackup[1].empty()) {//really have a card to move

            GameController::controller()->setAnimation(false);
            for (int player = 0; player < 2; player++) {
                int graveyardId = (player == 0 ? Player0_Graveyard : Player1_Graveyard);
                const QList<CardInfo *> &graveyard = assets->getCardArray(graveyardId);
                for (int column = graveyard.size() - 1; column >= 0; column--) {
                    if (graveyard[column]->getType() == CardInfo::Bronze) {//only bronze card
                        GameController::controller()->performMoveCardToRightTop(graveyardId, column, Player_Candidate);
                    }
                }
            }
            GameController::controller()->setAnimation(true);

            AbstractUI *gameUI = GameController::controller()->getGameUI();
            gameUI->switchToScene(AbstractUI::PlayerChooserScene);

            int fromPlayer = assets->whosePlayerRow(fromR);
            GameController::controller()->performChooseCard(Player_Candidate, Player_Seleted, Player0_Graveyard, 1,
                                                            fromPlayer, false, false, true,
                                                            tr("%1 is choosing %2 card(s) to move to another graveyard").arg(
                                                                    assets->getPlayerName(fromPlayer)).arg(1));

            CardInfo *seletedCard = assets->getCardArray(Player_Seleted).first();

            GameController::controller()->setAnimation(false);
            GameController::controller()->performMoveAllCardsFromAToB(Player_Seleted, Player_Candidate);
            const QList<CardInfo *> &candidates = assets->getCardArray(Player_Candidate);
            while (!candidates.empty()) {//resume the candidate
                if (graveyardBackup[0].contains(candidates.first())) {
                    GameController::controller()->performMoveCardToRightTop(Player_Candidate, 0, Player0_Graveyard);
                } else if (graveyardBackup[1].contains(candidates.first())) {
                    GameController::controller()->performMoveCardToRightTop(Player_Candidate, 0, Player1_Graveyard);
                } else {
                    qWarning() << "invalid graveyard belong23874169075";
                }
            }
            GameController::controller()->setAnimation(true);

            gameUI->switchToScene(AbstractUI::GameScene);

            int targetGraveyard = graveyardBackup[0].contains(seletedCard) ? Player1_Graveyard : Player0_Graveyard;
            GameController::controller()->moveCardToRightTop(seletedCard, targetGraveyard);
        }
    }
}
