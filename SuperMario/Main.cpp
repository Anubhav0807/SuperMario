#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <Windows.h>
#include "MapManager.h"

static void ResizeWindow(const sf::RenderWindow& window, sf::View& view, sf::Event& action)
{
    return;
    sf::Vector2u winSize = window.getSize();
    float aspectRatio = float(winSize.x) / float(winSize.y);
    view.setSize(SCREEN_HEIGHT * aspectRatio, SCREEN_WIDTH);
}

static int randint(int start, int stop) {
    int randomNum;
    while (true) {
        randomNum = rand()%(stop+1);
        if (start <= randomNum && randomNum <= stop) {
            return randomNum;
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Super Mario");
    window.setFramerateLimit(60);
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

    srand(time(nullptr)); // Set seed
    MapManager mapManager;

    float deltaTime = 0.0f;
    sf::Clock clock;

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds(); // Restarts the clock and return the time since last restart
        if (deltaTime > 0.05f)
            deltaTime = 0.05f;

        sf::Event action;
        while (window.pollEvent(action))
        {
            switch (action.type)
            {
            case sf::Event::Closed:
                window.close();
                return 0;
            case sf::Event::Resized:
                ResizeWindow(window, view, action);
                break;
            case sf::Event::KeyReleased:
                if (action.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    return 0;
                }
                else if (action.key.code == sf::Keyboard::H) {
                    mapManager.toggleHitbox();
                }
                else if (action.key.code == sf::Keyboard::R)
                {
                    mapManager.restart();
                }
                break;
            }
            mapManager.player->concludeJump(action);
        }
        
        float center_x, center_y;
        if (CAM_FOLLOW) {
            center_x = std::min(std::max(mapManager.player->getPosition().x, SCREEN_WIDTH / 2.0f), SCREEN_WIDTH * 7.0f); // Clamping
            if (mapManager.player->getPosition().y > mapManager.player->getSize().y / 2.0f) {
                center_y = +SCREEN_HEIGHT / 2.0f;
            }
            else {
                center_y = -SCREEN_HEIGHT / 2.0f + 16.0f * SIZE_RATIO; // block size = 16.0f
            }
            view.setCenter(center_x, center_y);
            window.setView(view);
        }
        mapManager.update(deltaTime, center_x);

        window.clear();
        mapManager.draw(window);
        window.display();
    }

    return 0;
}

int __stdcall wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    return main();
}