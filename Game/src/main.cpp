#include "skelpch.h"
#include "Core/Engine.h"
#include "precomp.h"
#include "game.h"


int main()
{

    MetalGearGame mainGame;
    skel::Engine::GetInstance().Run(mainGame);

    return 0;
}
