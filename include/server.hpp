#ifndef SERVER_HPP
#define SERVER_HPP

#include "grid.hpp"
#include "game_discovery.hpp"

#include <vector>

class GameServer {
    private:
        GameInfo gameInfo;
        std::vector<std::string> player_list;
        std::vector<Grid> grids;
        GameDiscovery gameDiscovery = GameDiscovery();

        int simulationRate = 10;

    public:
        GameServer() {
            gameInfo.gameName = "A Tetris Game";
            gameInfo.motd = "Welcome to my game!";
            gameInfo.gamePort = 25565;
            gameInfo.maxPlayers = 2;
            gameInfo.currentPlayers = 0;
            gameInfo.isJoinable = 1;
        }
        GameServer(std::string gameName, std::string motd, 
                    int gamePort, int maxPlayers) {
            gameName.resize(16, ' ');
            motd.resize(64, ' ');
            
            gameInfo.gameName = gameName;
            gameInfo.motd = motd;
            gameInfo.gamePort = gamePort;
            gameInfo.maxPlayers = maxPlayers;
            gameInfo.currentPlayers = 0;
            gameInfo.isJoinable = 1;
        }

        void create_game(); 
        void start_game();
        void update();
        void handle_received_packets();
        void send_packets();
        void declare_victory();
        void delete_game();
};

class RivalsServer : public GameServer {

};


#endif