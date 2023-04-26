#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include "Game.h"
#include "Piece.h"
#include "Menu.h"

#include <iostream>
#include <vector>
#include <time.h>
#include <atlconv.h>
#include <thread>
#include <semaphore>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

int wind_x = 1000, wind_y = 667;

int main()
{
    Menu menu(wind_x, wind_y);

    return 0;
}