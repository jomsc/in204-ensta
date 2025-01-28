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
    std::string player_pseudo = "joseph";
    online_player.setPseudo(player_pseudo);

    int status = 0;
    int previous_status = 0;


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

    sf::Font arial;
    std::string fontPath = exeDir + "/assets/fonts/arial.ttf";
    arial.loadFromFile(fontPath);

    sf::Font akira;
    fontPath = exeDir + "/assets/fonts/akira.otf";
    akira.loadFromFile(fontPath);


    bgVideoSprite.setTexture(bgVideoTexture);
    bgVideoSprite.setScale(sf::Vector2f(scaleX, scaleY));

    GameServer game_server = GameServer("zizi", "cacarthur bouvet", 
                                        25565, 2);

    
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
    
    mainMenuInfos.emplace_back(std::vector<int>{ 50, 50, 1380, 267, 13, 30, -1 });
    mainMenuColorArray.emplace_back(sf::Color::White);
    mainMenuHoverColorArray.emplace_back(sf::Color::White);

    mainMenuInfos.emplace_back(std::vector<int>{ 50, 800, 709, 50, 1380, 30, 1 });
    mainMenuColorArray.emplace_back(sf::Color::White);
    mainMenuHoverColorArray.emplace_back(sf::Color::Yellow);

    mainMenuInfos.emplace_back(std::vector<int>{ 50, 860, 505, 50, 1500, 80, 2 });
    mainMenuColorArray.emplace_back(sf::Color::White);
    mainMenuHoverColorArray.emplace_back(sf::Color::Yellow);

    mainMenuInfos.emplace_back(std::vector<int>{ 50, 920, 470, 50, 1500, 130, 3 });
    mainMenuColorArray.emplace_back(sf::Color::White);
    mainMenuHoverColorArray.emplace_back(sf::Color::Yellow);

    mainMenuInfos.emplace_back(std::vector<int>{ 50, 980, 250, 60, 1500, 180, -2 });
    mainMenuColorArray.emplace_back(sf::Color::White);
    mainMenuHoverColorArray.emplace_back(sf::Color::Red);

    Menu mainMenu = Menu(mainMenuInfos, mainMenuTexture, 
                         mainMenuColorArray, mainMenuHoverColorArray);


    // HOST MENU
    std::vector<std::vector<int>> hostMenuInfos;
    sf::Texture hostMenuTexture;
    std::vector<sf::Color> hostMenuColorArray;
    std::vector <sf::Color> hostMenuHoverColorArray;

    std::string hostMenuPath = exeDir + "/assets/menus/menu_host.png";
    if (!hostMenuTexture.loadFromFile(hostMenuPath)) {
        std::cout << "Error loading host menu texture!" << std::endl;  
    }
    
    hostMenuInfos.emplace_back(std::vector<int>{ 460, 257, 1000, 500, 0, 0, -1 });
    hostMenuColorArray.emplace_back(sf::Color::White);
    hostMenuHoverColorArray.emplace_back(sf::Color::White);

    hostMenuInfos.emplace_back(std::vector<int>{ 525, 807, 871, 65, 0, 510, 5 });
    hostMenuColorArray.emplace_back(sf::Color::Yellow);
    hostMenuHoverColorArray.emplace_back(sf::Color::Red);

    Menu hostMenu = Menu(hostMenuInfos, hostMenuTexture, 
                         hostMenuColorArray, hostMenuHoverColorArray);


    // JOIN MENU
    std::vector<std::vector<int>> joinMenuInfos;
    sf::Texture joinMenuTexture;
    std::vector<sf::Color> joinMenuColorArray;
    std::vector <sf::Color> joinMenuHoverColorArray;

    std::string joinMenuPath = exeDir + "/assets/menus/menu_join.png";
    if (!joinMenuTexture.loadFromFile(joinMenuPath)) {
        std::cout << "Error loading join menu texture!" << std::endl;  
    }
    
    joinMenuInfos.emplace_back(std::vector<int>{ 460, 257, 1000, 500, 0, 0, -1 });
    joinMenuColorArray.emplace_back(sf::Color::White);
    joinMenuHoverColorArray.emplace_back(sf::Color::White);

    Menu joinMenu = Menu(joinMenuInfos, joinMenuTexture, 
                         joinMenuColorArray, joinMenuHoverColorArray);


    // PAUSE MENU
    std::vector<std::vector<int>> pauseMenuInfos;
    sf::Texture pauseMenuTexture;
    std::vector<sf::Color> pauseMenuColorArray;
    std::vector <sf::Color> pauseMenuHoverColorArray;

    std::string pauseMenuPath = exeDir + "/assets/menus/menu_pause.png";
    if (!pauseMenuTexture.loadFromFile(pauseMenuPath)) {
        std::cout << "Error loading pause menu texture!" << std::endl;  
    }
    
    pauseMenuInfos.emplace_back(std::vector<int>{ 670, 450, 581, 68, 0, 0, 7 });
    pauseMenuColorArray.emplace_back(sf::Color::White);
    pauseMenuHoverColorArray.emplace_back(sf::Color::Blue);

    pauseMenuInfos.emplace_back(std::vector<int>{ 801, 548, 317, 81, 0, 88 });
    pauseMenuColorArray.emplace_back(sf::Color::White);
    pauseMenuHoverColorArray.emplace_back(sf::Color::Red);

    Menu pauseMenu = Menu(pauseMenuInfos, pauseMenuTexture, 
                         pauseMenuColorArray, pauseMenuHoverColorArray);        

    std::vector<GameInfo> games;


    int mouseX = 0;
    int mouseY = 0;
    bool isClicking = 0;

    int dest = -1;
    int game_chosen = -1;


    bool host = false; 
    bool discovery = false;

    bool isHostInit = false; // is the host initialized? allows to call create game the first time
    bool success = false;


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

        switch (status) {

            case 0: // main menu 
                bgVideoSprite.setTextureRect(srcRect);
                window.draw(bgVideoSprite);
                mainMenu.display(&window, mouseX, mouseY);
                dest = mainMenu.dest(mouseX, mouseY, isClicking);
                if (dest != -1) {
                    if (dest == -2) { window.close(); }
                    else { 
                        previous_status = status;
                        status = dest; 
                    }
                }
                break;

            case 1: // in-game classic
                player.update();
                bgVideoSprite.setTextureRect(srcRect);
                window.draw(bgVideoSprite);
                player.display(&window);
                break;

            case 2: // host LAN 
                if (!isHostInit) {
                    game_server.create_game();
                    game_server.handle_received_packets();

                    std::this_thread::sleep_for(std::chrono::milliseconds(500));

                    if (games.empty()) {
                        games = online_player.game_discovery.discoverGames(100, 1, 1);
                    }

                    success = online_player.connect_to_server(games[0]);
                    if (!success) {
                        std::cout << "Failed to connect to own game oups" << std::endl;
                        status = 0; 
                        GameServer game_server = GameServer("zizi", "cacarthur bouvet", 
                                            25565, 2);
                        isHostInit = false;
                    } else {
                        isHostInit = true;
                    }
                }

                if (online_player.status == 2) {
                    int xOffset = 525;
                    int yOffset = 317;
                    bgVideoSprite.setTextureRect(srcRect);
                    window.draw(bgVideoSprite);
                    hostMenu.display(&window, mouseX, mouseY);

                    // display number of connected players :
                    std::string players_text = "Number of Players : " +
                    std::to_string(game_server.gameDiscovery.numberOfPlayers)
                    + "/" + std::to_string(game_server.maxPlayers());

                    sf::Text players_txt(players_text, arial);
                    players_txt.setCharacterSize(40);
                    players_txt.setStyle(sf::Text::Bold);
                    players_txt.setFillColor(sf::Color::White);
                    players_txt.setPosition(xOffset, yOffset);
                    window.draw(players_txt);

                    dest = hostMenu.dest(mouseX, mouseY, isClicking);
                    if (dest != -1) {
                        if (dest == -2) { window.close(); }
                        else { 
                            if (dest == 5) {
                                if (!game_server.start_game()) {
                                    std::cout << "failed to start game" << std::endl;
                                } else {
                                    std::cout << "GAME STAZRT" << std::endl;
                                }
                            }
                            previous_status = status;
                            status = dest; 
                        }
                    }
                }
                break;
            
            case 3: // join LAN 
                bgVideoSprite.setTextureRect(srcRect);
                window.draw(bgVideoSprite);
                joinMenu.display(&window, mouseX, mouseY);
                
                if (games.empty()) {
                    games = online_player.game_discovery.discoverGames(100, 1, 0);
                }
                
                // display games
                for (int i=0;i<games.size();i++) {
                    int xOffset = 525;
                    int yOffset = 317+120*i;

                    // display game name
                    sf::Text game_name_text(games[i].gameName, arial);
                    game_name_text.setCharacterSize(40);
                    game_name_text.setStyle(sf::Text::Bold);
                    game_name_text.setFillColor(sf::Color::White);
                    game_name_text.setPosition(xOffset, yOffset);
                    window.draw(game_name_text);

                    // display game MOTD
                    sf::Text motd_text(games[i].motd, arial);
                    motd_text.setCharacterSize(25);
                    motd_text.setStyle(sf::Text::Regular);
                    motd_text.setFillColor(sf::Color::White);
                    motd_text.setPosition(xOffset, yOffset+50);
                    window.draw(motd_text);

                    // display number of players
                    std::string players = std::to_string(games[i].currentPlayers)
                                        + "/" + std::to_string(games[i].maxPlayers);
                    sf::Text players_text(players, arial);
                    players_text.setCharacterSize(65);
                    players_text.setStyle(sf::Text::Regular);
                    players_text.setFillColor(sf::Color::Yellow);
                    players_text.setPosition(xOffset+400, yOffset+5);
                    window.draw(players_text);    

                    joinMenu.add_button(std::vector<int>{ xOffset+660, yOffset+20, 218, 56, 0, 510, 4 }, 
                                        sf::Color::Green, sf::Color::Blue, joinMenuTexture);
                }

                // choose a game with mouse
                for (int i=0;i<games.size();i++) {
                    if (joinMenu.buttons[i+1].isInside(mouseX, mouseY) && isClicking) {
                        game_chosen = i;
                    }
                }
                    
                if (game_chosen!=-1 && game_chosen <= games.size()) {
                    //std::cout << "pseudo chosen : " << online_player.getPseudo() << std::endl;
                    std::cout << "address : " << games[game_chosen].serverIP << std::endl;
                    
                    success = online_player.connect_to_server(games[game_chosen]);
                    if (success) {
                        status = 4;
                    }
                }
                break;

            case 4: // waiting LAN
                if (online_player.status == 2) {
                    bgVideoSprite.setTextureRect(srcRect);
                    window.draw(bgVideoSprite);


                    sf::Text name_text(games[game_chosen].gameName, akira);
                    name_text.setCharacterSize(100);
                    name_text.setStyle(sf::Text::Bold);
                    name_text.setFillColor(sf::Color::White);
                    name_text.setPosition(50, 50);
                    window.draw(name_text);
                    
                    std::string waiting_string = "Waiting for game start !";
                    sf::Text waiting_text(waiting_string, akira);
                    waiting_text.setCharacterSize(70);
                    waiting_text.setStyle(sf::Text::Bold);
                    waiting_text.setFillColor(sf::Color::Yellow);
                    waiting_text.setPosition(50, 160);
                    window.draw(waiting_text);
                    break;
                } else {
                    if (online_player.status == 3) { status = 5; }
                }
                
            case 5: // in-game LAN
                break;

            case 6: // pause
                bgVideoSprite.setTextureRect(srcRect);
                window.draw(bgVideoSprite);
                pauseMenu.display(&window, mouseX, mouseY);
                dest = pauseMenu.dest(mouseX, mouseY, isClicking);
                if (dest != -1) {
                    if (dest == 7) { 
                        previous_status = status;
                        status = 1;    
                    }
                    else { 
                        previous_status = 0;
                        status = 0;
                    }
                }
                break; 

            case 8:
                // pause for in-game, we still have to update the online_player.
                break;

            default:
                break;

        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == event.KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (status == 1) {
                        status = 6;
                        previous_status = 1;
                    } else if (status == 5) {
                        status = 8;
                        previous_status = 5;
                    } else {
                        status = previous_status;
                    }
                }
            }
        }
    
        window.display();
        
        sf::Time elapsed = clock.getElapsedTime();
        //std::cout << "Render time : " << elapsed.asMilliseconds() << " ms" << std::endl;
    }

    return 0;
}