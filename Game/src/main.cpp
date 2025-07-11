#include "skelpch.h"
#include "Core/Engine.h"
#include "VoxelSpaceGame.h"

int main()
{

    VoxelSpaceGame mainGame;
    skel::Engine::GetInstance().Run(mainGame);

    return 0;
}
