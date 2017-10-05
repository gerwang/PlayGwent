//
// Created by Gerwa on 2017/9/10.
//

#include "Dagon.h"

void Dagon::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Dagon") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Dagon::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Dagon") {
        GameAssets *assets = GameController::controller()->getAssets();
        AbstractUI *gameUI = GameController::controller()->getGameUI();
        int fromPlayer = assets->whosePlayerRow(fromR);
        const QList<CardInfo *> &candidates = assets->getCardArray(Player_Candidate);
        QList<QString> cardNames = {"Biting_Frost", "Impenetrable_Fog", "Torrential_Rain"};
        for (const auto &cardName:cardNames) {
            GameController::controller()->spawnCardToPosByPlayer(CardInfo::createByName(cardName), Player_Candidate,
                                                                 candidates.size(), fromPlayer);
        }

        gameUI->switchToScene(AbstractUI::PlayerChooserScene);
        int deckIndex = assets->getDeckIndex(fromPlayer);
        GameController::controller()->performChooseCard(Player_Candidate, Player_Seleted, deckIndex, 1, fromPlayer,
                                                        false, false, true,
                                                        tr("%1 is choosing %2 card(s) to deploy").arg(
                                                                assets->getPlayerName(fromPlayer)).arg(1));
        gameUI->switchToScene(AbstractUI::GameScene);

        CardInfo *seletedCard = assets->getCardArray(Player_Seleted).first();
        int handIndex = assets->getHandIndex(fromPlayer);
        GameController::controller()->performMoveAllCardsFromAToB(Player_Seleted, handIndex);

        while (!candidates.empty()) {
            GameController::controller()->removeCardFromGame(candidates.first());
        }

        GameController::controller()->requestPlayerToPlayFromHand(seletedCard, false);
    }
}
