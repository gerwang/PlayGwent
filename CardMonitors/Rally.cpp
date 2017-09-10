//
// Created by Gerwa on 2017/9/9.
//

#include "Rally.h"

void Rally::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Rally") {
        //in fact rally will noe be in hand
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Rally::slotOnDeploy(CardInfo *mover, int fromR, int toR) {
    if (mover->getCardName() == "Rally") {
        GameController::controller()->DestroySpecialCard(mover, fromR);//effect card, delete
        GameAssets *assets = GameController::controller()->getAssets();
        int fromPlayer = assets->whosePlayerRow(fromR);
        int deckIndex = assets->getDeckIndex(fromPlayer);
        QList<CardInfo *> bronzeUnits;
        for (auto card:assets->getCardArray(deckIndex)) {
            if (card->getType() == CardInfo::Bronze
                && card->getInitialStrength() > 0) {//is a unit
                bronzeUnits.append(card);
            }
        }
        if (!bronzeUnits.empty()) {
            int randomIndex = qrand() % bronzeUnits.size();
            CardInfo *randomCard = bronzeUnits[randomIndex];

            int targetPlayer = fromPlayer;
            if (randomCard->getLoyalty() == CardInfo::Disloyal) {
                targetPlayer = fromPlayer ^ 1;
            }

            int targetRow = -1;
            switch (randomCard->getRow()) {
                case CardInfo::Event: {
                    int Begin = assets->playerBattlefieldBegin(targetPlayer);
                    int End = assets->playerBattlefieldEnd(targetPlayer);
                    targetRow = Begin + qrand() % (End - Begin + 1);
                    break;
                }
                case CardInfo::Melee:
                    targetRow = assets->getPlayerMelee(targetPlayer);
                    break;
                case CardInfo::Ranged:
                    targetRow = assets->getPlayerRanged(targetPlayer);
                    break;
                case CardInfo::Siege:
                    targetRow = assets->getPlayerSiege(targetPlayer);
                    break;
                default:
                    qWarning() << "Invalid row!1236786";
            }

            GameController::controller()->moveCardToRightTop(randomCard, targetRow);
        }
    }
}
