//
// Created by Gerwa on 2017/9/10.
//

#include "First_Light.h"

void First_Light::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "First_Light") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void First_Light::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "First_Light") {
        GameController::controller()->DestroySpecialCard(mover, fromR);
        //spawn rally or clear_skies

        GameAssets *assets = GameController::controller()->getAssets();
        AbstractUI *gameUI = GameController::controller()->getGameUI();

        int fromPlayer = assets->whosePlayerRow(fromR);
        const QList<CardInfo *> &candidates = assets->getCardArray(Player_Candidate);
        GameController::controller()->spawnCardToPosByPlayer(CardInfo::createByName("Clear_Skies"), Player_Candidate,
                                                             candidates.size(), fromPlayer);
        GameController::controller()->spawnCardToPosByPlayer(CardInfo::createByName("Rally"), Player_Candidate,
                                                             candidates.size(), fromPlayer);

        gameUI->switchToScene(AbstractUI::PlayerChooserScene);
        int deckIndex = assets->getDeckIndex(fromPlayer);
        GameController::controller()->performChooseCard(Player_Candidate, Player_Seleted, deckIndex, 1, fromPlayer,
                                                        false, false, true, tr("%1 is choosing %2 card(s) to play").arg(
                        assets->getPlayerName(fromPlayer)).arg(1));
        gameUI->switchToScene(AbstractUI::GameScene);

        CardInfo *seletedCard = assets->getCardArray(Player_Seleted).first();
        int handIndex = assets->getHandIndex(fromPlayer);
        GameController::controller()->performMoveAllCardsFromAToB(Player_Seleted, handIndex);
        while (!candidates.empty()) {
            GameController::controller()->removeCardFromGame(candidates.first());
        }

        GameController::controller()->moveCardToRightTop(seletedCard, assets->getPlayerMelee(
                fromPlayer));//place to an arbitrary row of that player

    }
}
