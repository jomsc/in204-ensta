#include "player.hpp"
#include "server.hpp"
#include "menu.hpp"
#include "filesystem_resolve.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>


int main(int argc, char **argv) 
{

    sf::VideoMode ecran = sf::VideoMode::getDesktopMode();
    int VIDEO_WIDTH= ecran.width;
    int VIDEO_HEIGHT= ecran.height;

    //sf::RenderWindow window(sf::VideoMode(VIDEO_WIDTH, VIDEO_HEIGHT), "Tetris!");
    sf::RenderWindow window(sf::VideoMode(), "TETRIS RIVALS", sf::Style::Fullscreen);
    srand(time(0));

    sf::Clock clock;

    Player player = Player();
    OnlinePlayer online_player = OnlinePlayer();

    int status = 0; // 0 : menu, 1 : classic game, 2 : LAN screen, 3 : LAN game

    player.grid.size_cell=(VIDEO_HEIGHT-2*player.grid.y_offset
        -player.grid.line_thickness*(player.grid.numrows+1))/(player.grid.numrows+1);
    player.grid.x_offset= (VIDEO_WIDTH-player.grid.numcols*player.grid.size_cell
        -(player.grid.numcols+1)*player.grid.line_thickness)/2;
    player.grid.size_waiting_cell = 0.625*player.grid.size_cell;
    
    sf::Texture bgVideoTexture;
    sf::Sprite bgVideoSprite;
    sf::Clock bgClock;
    bgClock.restart();
    int vidX[107];
    int vidY[107];
    float scaleX = VIDEO_WIDTH/1920.0;
    float scaleY = VIDEO_HEIGHT/1080.0;

    bool debug_headless = false;

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
    

    std::string exeDir = getExecutableDirectory();
    std::string videoPath = exeDir + "/assets/background_vid/out.jpeg";


    if (!bgVideoTexture.loadFromFile(videoPath)) {
        std::cout << "Error loading background video!" << std::endl;  
    }
    bgVideoSprite.setTexture(bgVideoTexture);
    bgVideoSprite.setScale(sf::Vector2f(scaleX, scaleY));

    GameServer game_server = GameServer("zizi", "cacarthur bouvet", 
                                        25565, 0);

    
    // DEFINITIONS DE TOUS LES MENUS 

    // MAIN MENU
    std::vector<std::vector<int>> mainMenuInfos;
    sf::Texture mainMenuTexture;
    std::vector<sf::Color> mainMenuColorArray;
    std::vector <sf::Color> mainMenuHoverColorArray;

    std::string mainMenuPath = exeDir + "/assets/menus/mainmenu.png";
    if (!mainMenuTexture.loadFromFile(mainMenuPath)) {
        std::cout << "Error loading main menu texture!" << std::endl;  
    }
    
    mainMenuInfos.emplace_back(std::vector<int>{ 50, 50, 1404, 267, 0, 0, -1 });
    mainMenuColorArray.emplace_back(sf::Color::White);
    mainMenuHoverColorArray.emplace_back(sf::Color::White);

    mainMenuInfos.emplace_back(std::vector<int>{ 50, 800, 696, 48, 1380, 0, 1 });
    mainMenuColorArray.emplace_back(sf::Color::White);
    mainMenuHoverColorArray.emplace_back(sf::Color::Yellow);

    mainMenuInfos.emplace_back(std::vector<int>{ 50, 860, 484, 48, 1500, 50, 2 });
    mainMenuColorArray.emplace_back(sf::Color::White);
    mainMenuHoverColorArray.emplace_back(sf::Color::Yellow);

    mainMenuInfos.emplace_back(std::vector<int>{ 50, 920, 452, 48, 1500, 100, 3 });
    mainMenuColorArray.emplace_back(sf::Color::White);
    mainMenuHoverColorArray.emplace_back(sf::Color::Yellow);

    mainMenuInfos.emplace_back(std::vector<int>{ 50, 980, 222, 57, 1500, 150, -2 });
    mainMenuColorArray.emplace_back(sf::Color::White);
    mainMenuHoverColorArray.emplace_back(sf::Color::Red);

    Menu mainMenu = Menu(mainMenuInfos, mainMenuTexture, 
                         mainMenuColorArray, mainMenuHoverColorArray);

    int mouseX = 0;
    int mouseY = 0;
    bool isClicking = 0;

    int dest = -1;


    bool host = false; 
    bool discovery = false;

    // parsing the args
    if (argc > 1) {
        if (argc!=2 && argc!=3) {
            std::cout << "Usage : ./TETRIS_RIVALS --<mode>" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (strcmp(argv[1], "--client") == 0) {
            host = false;
            discovery = true;
        }

        if (strcmp(argv[1], "--server") == 0) {
            host = true;
            discovery = false;
            std::cout << "STARTING ON SERVER MODE" << std::endl;
        }
        
        

        if (strcmp(argv[1], "--host") == 0) {
            host = true;
            discovery = true;
        }

        if (strcmp(argv[1], "--classic") == 0) {
            host = false;
            discovery = false;
            status = 1;
        }

        if (argc == 3) {
            if (strcmp(argv[2], "--debug") == 0) { 
                debug_headless = 1;
                window.close(); 
                std::cout << "HEADLESS MODE ACTIVE" << std::endl;
                std::cout << std::endl;
            }
        }
        

    } else { // no args, default behavior
        status = 0;
    }

    if (host) {
        game_server.create_game();
        game_server.handle_received_packets();
    }   


    std::cout << "test : " << status << std::endl;

    while (window.isOpen() || debug_headless) {
        clock.restart();
        window.clear();

        int bgFrameCounter = (int)((bgClock.getElapsedTime().asMilliseconds()%3566)/34);
        sf::IntRect srcRect = sf::IntRect(vidY[bgFrameCounter+1], 
                                                    vidX[bgFrameCounter+1],
                                                    1920,
                                                    1080);

        isClicking = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        mouseX = sf::Mouse::getPosition().x;
        mouseY = sf::Mouse::getPosition().y;

        status = 0;

        switch (status) {

            case 0:
                bgVideoSprite.setTextureRect(srcRect);
                window.draw(bgVideoSprite);
                mainMenu.display(&window, mouseX, mouseY);
                dest = mainMenu.dest(mouseX, mouseY, isClicking);
                break;

            case 1:
                player.update();
                bgVideoSprite.setTextureRect(srcRect);
                window.draw(bgVideoSprite);
                player.display(&window);
                break;

            case 2:
                if (discovery) {
                    std::vector<GameInfo> games = online_player.game_discovery.discoverGames(2000, 1);
                    for (const auto& game : games) {
                        std::cout << std::endl;
                        std::cout << "Game found : " << std::endl;
                        std::cout << "Name : " << game.gameName << std::endl;
                        std::cout << "Address : " << game.serverIP << ":" << game.gamePort << std::endl;
                        std::cout << "MOTD : " << game.motd << std::endl;
                        
                        std::cout << "Players : " << 
                        std::to_string(game.currentPlayers) << "/" 
                        << std::to_string(game.maxPlayers) << std::endl;
                        
                        std::cout << std::endl;
                    }
                    std::cout << "choose a game (-1 to refuse)" << std::endl;
                    int game_chosen;
                    std::cin >> game_chosen;
                    
                    if (game_chosen!=-1 && game_chosen <= games.size()) {
                        std::string pseudo;
                        std::cout << "choose your pseudo (16 characters max)" << std::endl;
                        std::cin >> pseudo;
                        pseudo.resize(16, ' ');
                        online_player.setPseudo(pseudo);
                        bool success = false;
                        //std::cout << "pseudo chosen : " << online_player.getPseudo() << std::endl;
                        std::cout << "address : " << games[game_chosen].serverIP << std::endl;
                        
                        success = online_player.connect_to_server(games[game_chosen]);
                        if (success) {
                            status = 3;
                            std::cout << "status : " << status << std::endl;
                        }
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                break;
            
            case 3:
                std::cout << "JE VEUX JOUER NSM" << std::endl;
                break;
            
            default:
                break;

        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    
        window.display();
        
        sf::Time elapsed = clock.getElapsedTime();
        //std::cout << "Render time : " << elapsed.asMilliseconds() << " ms" << std::endl;
    }

    return 0;
}