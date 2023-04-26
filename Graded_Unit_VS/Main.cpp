#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "AssetManager.h"

int main()
{
    // Initialise the RNG
    srand(time(nullptr));

    // Create the game
    Game gameInstance; // default constructor

    // This will not end until the game is over
    gameInstance.RunGameLoop();


    // If the loop exits, game is over
    // Clean up assets
    AssetManager::DestroyAllAssets();

    //End the program
    return 0;
}