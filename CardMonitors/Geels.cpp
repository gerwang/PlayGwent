//
// Created by Gerwa on 2017/9/9.
//

#include "Geels.h"

void Geels::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Geels") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Geels::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Geels") {
        GameAssets *assets = GameController::controller()->getAssets();
        int fromPlayer = assets->whosePlayerRow(fromR);
        int deckIndex = assets->getDeckIndex(fromPlayer);
        //remember to include leader! //in fact, leaders won't appear in deck
        const QList<CardInfo *> &deck = assets->getCardArray(deckIndex);
        const CardInfo::Type typeList[2] = {CardInfo::Gold, CardInfo::Silver};
        for (auto type : typeList) {
            for (int column = 0; column < deck.size(); column++) {
                if (deck[column]->getType() == type) {
                    GameController::controller()->performMoveCardToRightTop(deckIndex, column, Player_Candidate);
                    break;
                }
            }
        }
        if (!assets->getCardArray(Player_Candidate).empty()) {//really has one to choose
            AbstractUI *gameUI = GameController::controller()->getGameUI();

            gameUI->switchToScene(AbstractUI::PlayerChooserScene);
            GameController::controller()->performChooseCard(Player_Candidate, Player_Seleted, deckIndex, 1, fromPlayer,
                                                            false, false, true,
                                                            QString("%1 is choosing %2 cards to play").arg(
                                                                    assets->getPlayerName(fromPlayer)).arg(1));
            gameUI->switchToScene(AbstractUI::GameScene);

            GameController::controller()->performMoveAllCardsFromAToB(Player_Candidate,
                                                                      deckIndex);//the card reserved for next choice
            CardInfo *seletedCard = assets->getCardArray(Player_Seleted).first();
            int handIndex = assets->getHandIndex(fromPlayer);
            GameController::controller()->performMoveAllCardsFromAToB(Player_Seleted, handIndex);
            GameController::controller()->requestPlayerToPlayFromHand(seletedCard, false);
        }
    }
}
