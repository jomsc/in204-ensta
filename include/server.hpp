#ifndef SERVER_HPP
#define SERVER_HPP

#include "grid.hpp"
#include "game_discovery.hpp"

#include <vector>
#include <atomic>

class GameServer {
    private:
        GameInfo gameInfo;
        std::vector<std::string> player_list;
        std::vector<Grid> grids;
        std::vector<uint32_t> scores;
        std::vector<uint8_t> levels;
        std::vector<int> loss_list;
        std::vector<int> piece_list; // liste des pieces generees
        std::vector<int> rand_index; // index pour chaque joueur, ou il en est


        uint32_t seed;
        uint32_t sequence_number;

        int status = 0; // 0 : broadcasting, 1 : in-game, 2 : game is over, 3 : game is deleted

        int simulationRate = 10;
        
        int gamesock_fd;
        struct sockaddr_in servaddr;
        struct sockaddr_in cliaddr;
        std::vector<struct sockaddr_in> cliaddr_list;
        uint8_t buffer[1024];

        std::thread receiveThread;



    public:
        GameDiscovery gameDiscovery = GameDiscovery();
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
        void handle_join_requests(std::string pseudo, 
                                 struct sockaddr_in cliaddr);
        bool start_game();
        void update();
        void handle_received_packets();
        void send_game_packets();
        void send_line_packet(int playerSource, int playerDest, int n);
        uint8_t* generate_game_packet(int playerIndex);
        bool declare_victory();
        void delete_game();

        int maxPlayers() { return this->gameInfo.maxPlayers; }
};

class RivalsServer : public GameServer {

};


#endif