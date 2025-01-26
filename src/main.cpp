#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "server.hpp"
#include <iostream>
#include <enet/enet.h>  
#include <chrono>

#define VIDEO_WIDTH 1600
#define VIDEO_HEIGHT 900


int main()
{
    sf::RenderWindow window(sf::VideoMode(VIDEO_WIDTH, VIDEO_HEIGHT), "Tetris!");
    srand(time(0));

    sf::Clock clock;

    Player player = Player();
    
    sf::Texture bgVideoTexture;
    sf::Sprite bgVideoSprite;
    sf::Clock bgClock;
    bgClock.restart();
    int vidX[107];
    int vidY[107];
    float scaleX = VIDEO_WIDTH/1920.0;
    float scaleY = VIDEO_HEIGHT/1080.0;
    std::cout << scaleX<<std::endl;

    for (int i=0;i<13;i++){
        for (int j=0;j<8;j++) {
            vidX[8*i+j] = 1080*i;
            vidY[8*i+j] = 1920*j;
        }
    }

    for (int j=0;j<3;j++) {
        vidX[8*13+j] = 1080*13;
        vidY[8*13+j] = 1920*j;
    }

    if (!bgVideoTexture.loadFromFile("../src/assets/background_vid/out.jpeg")) {
        std::cout << "Error loading background video!" << std::endl;  
    }
    bgVideoSprite.setTexture(bgVideoTexture);
    bgVideoSprite.setScale(sf::Vector2f(scaleX, scaleY));

    OnlinePlayer online_player = OnlinePlayer();
    GameServer game_server = GameServer("zizi", "cacarthur bouvet", 
                                        25565, 2);
    bool host = true; 
    bool discovery = false;

    if (host) {
        game_server.create_game();
        std::cout << "Game created" << std::endl;
    }


    while (window.isOpen()) {
        clock.restart();

        if (discovery) {
            
            std::vector<GameInfo> games = online_player.game_discovery.discoverGames();
            for (const auto& game : games) {
                std::cout << "Game found : " << std::endl;
                std::cout << "Name : " << game.gameName << std::endl;
                std::cout << "Port : " << game.gamePort << std::endl;
                std::cout << "MOTD : " << game.motd << std::endl;
                std::cout << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        player.update();

        int bgFrameCounter = (int)((bgClock.getElapsedTime().asMilliseconds()%3566)/34);
        sf::IntRect srcRect = sf::IntRect(vidY[bgFrameCounter+1], 
                                          vidX[bgFrameCounter+1],
                                          1920,
                                          1080);
        bgVideoSprite.setTextureRect(srcRect);
        
        window.clear();
        window.draw(bgVideoSprite);
        player.display(&window);
        window.display();
        
        sf::Time elapsed = clock.getElapsedTime();
        //std::cout << "Render time : " << elapsed.asMilliseconds() << " ms" << std::endl;
    }

    return 0;
}