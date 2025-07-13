#include "skelpch.h"
#include "Core/Engine.h"
#include "ExampleGame.h"

int main()
{

    ExampleGame mainGame;
    skel::Engine::GetInstance().Run(mainGame);

    return 0;
}
