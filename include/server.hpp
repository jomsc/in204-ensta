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
        std::vector<uint32_t> scores;
        std::vector<uint8_t> levels;
        std::vector<int> loss_list;
        GameDiscovery gameDiscovery = GameDiscovery();
        std::vector<int> piece_list; // liste des pieces generees
        std::vector<int> rand_index; // index pour chaque joueur, ou il en est

        std::vector<ENetPeer*> player_peers;
        ENetHost* server_host;

        uint32_t seed;
        uint32_t sequence_number;

        int simulationRate = 10;
        

    public:
        GameServer() {
            gameInfo.gameName = "A Tetris Game";
            gameInfo.motd = "Welcome to my game!";
            gameInfo.gamePort = 25565;
            gameInfo.maxPlayers = 2;
            gameInfo.currentPlayers = 0;
            gameInfo.isJoinable = 1;
            gameInfo.serverAddress.host = ENET_HOST_ANY;
            gameInfo.serverAddress.port = gameInfo.gamePort;
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
            gameInfo.serverAddress.host = ENET_HOST_ANY;
            gameInfo.serverAddress.port = gameInfo.gamePort;
        }

        void create_game(); 
        void handle_join_requests();
        bool start_game();
        void update();
        void handle_received_packets();
        void send_game_packets();
        void send_line_packet(int playerSource, int playerDest, int n);
        uint8_t* generate_game_packet(int playerIndex);
        bool declare_victory();
        void delete_game();
};

class RivalsServer : public GameServer {

};


#endif