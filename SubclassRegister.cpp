//
// Created by Gerwa on 2017/9/6.
//

#include "SubclassRegister.h"

#include "GameController.h"
#include "CardMonitors/Geralt_Igni.h"
#include "CardMonitors/Arachas.h"
#include "CardMonitors/Roach.h"
#include "CardMonitors/Arachas_Hatchling.h"
#include "CardMonitors/Vran_Warrior.h"
#include "CardMonitors/Arachas_Behemoth.h"
#include "CardMonitors/Thunderbolt_Potion.h"
#include "CardMonitors/Commanders_Horn.h"
#include "CardMonitors/Lacerate.h"
#include "CardMonitors/Harpy_Egg.h"
#include "CardMonitors/Celaeno_Harpy.h"
#include "CardMonitors/Earth_Elemental.h"
#include "CardMonitors/Crone_Brewess.h"
#include "CardMonitors/Crone_Weavess.h"
#include "CardMonitors/Crone_Whispess.h"
#include "CardMonitors/Unseen_Elder.h"
#include "CardMonitors/Frightener.h"
#include "CardMonitors/Geels.h"
#include "CardMonitors/Bekkers_Twisted_Mirror.h"
#include "CardMonitors/Rally.h"
#include "CardMonitors/Biting_Frost.h"
#include "CardMonitors/Impenetrable_Fog.h"
#include "CardMonitors/Torrential_Rain.h"
#include "CardMonitors/Clear_Skies.h"
#include "CardMonitors/First_Light.h"
#include "CardMonitors/Dagon.h"
#include "CardMonitors/Foglet.h"
#include "CardMonitors/Woodland_Spirit.h"
#include "CardMonitors/Archgriffin.h"
#include "CardMonitors/Wild_Hunt_Rider.h"
#include "CardMonitors/Caranthir.h"

void registerSubclasses() {
    //the earlier registered here, the earlier to process
    GameController::controller()->registerProcessor(new Geralt_Igni);
    GameController::controller()->registerProcessor(new Arachas);
    GameController::controller()->registerProcessor(new Roach);
    GameController::controller()->registerProcessor(new Arachas_Hatching);
    GameController::controller()->registerProcessor(new Vran_Warrior);
    GameController::controller()->registerProcessor(new Arachas_Behemoth);
    GameController::controller()->registerProcessor(new Thunderbolt_Potion);
    GameController::controller()->registerProcessor(new Commanders_Horn);
    GameController::controller()->registerProcessor(new Lacerate);
    GameController::controller()->registerProcessor(new Harpy_Egg);
    GameController::controller()->registerProcessor(new Celaeno_Harpy);
    GameController::controller()->registerProcessor(new Earth_Elemental);
    GameController::controller()->registerProcessor(new Crone_Brewess);
    GameController::controller()->registerProcessor(new Crone_Weavess);
    GameController::controller()->registerProcessor(new Crone_Whispess);
    GameController::controller()->registerProcessor(new Unseen_Elder);
    GameController::controller()->registerProcessor(new Frightener);
    GameController::controller()->registerProcessor(new Geels);
    GameController::controller()->registerProcessor(new Bekkers_Twisted_Mirror);
    GameController::controller()->registerProcessor(new Rally);
    GameController::controller()->registerProcessor(new Biting_Frost);
    GameController::controller()->registerProcessor(new Impenetrable_Fog);
    GameController::controller()->registerProcessor(new Torrential_Rain);
    GameController::controller()->registerProcessor(new Clear_Skies);
    GameController::controller()->registerProcessor(new First_Light);
    GameController::controller()->registerProcessor(new Dagon);
    GameController::controller()->registerProcessor(new Foglet);
    GameController::controller()->registerProcessor(new Woodland_Spirit);
    GameController::controller()->registerProcessor(new Archgriffin);
    GameController::controller()->registerProcessor(new Wild_Hunt_Rider);
    GameController::controller()->registerProcessor(new Caranthir);
}