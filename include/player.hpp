#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "grid.hpp"

#include "game_discovery.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

class Player {
    private:
        uint32_t score;
        int level;
        int buffer[3]; /*pour les 3 prochaines pièces*/
        _Float32 speed;

        int level_up_buffer;
        int num_lines_cleared;

        sf::Clock gravity_clock;
        sf::Clock rotate_clock;
        sf::Clock movement_clock;

        uint32_t sequence_number;

        float speeds[15] = { // cells per frame at 60fps (official guidelines)
            0.01667, 
            0.021017, 
            0.026977, 
            0.035256,
            0.04693,
            0.06361,
            0.0879,
            0.1236,
            0.1775,
            0.2598,
            0.388,
            0.59,
            0.92,
            1.46,
            2.36
        };

    public:
        Grid grid;
        uint32_t seed;

        void display(sf::RenderWindow *window);
        void update();
        void update_next_pieces();
        void update_score(int num_lines_cleared,int level);
        void update_level();
        uint8_t get_pieces_size() { return this->grid.pieces.size(); }
        uint32_t get_sequence_number() { return sequence_number; }
        uint32_t get_score() { return score; }
        uint8_t get_level() { return level; }
        void set_score(uint32_t theScore) { this->score = theScore; }
        void set_level(int theLevel) { this->level = theLevel; }


        Player() {
            grid = Grid();
            score = 0;
            level = 1;
            for(int i=0;i<=2;i++){
                buffer[i]=rand()%7;
            }
            speed= 1/(60*speeds[1]);

            level_up_buffer=0;
            num_lines_cleared=0;
            gravity_clock.restart();
            rotate_clock.restart();
            movement_clock.restart();

            grid.spawn(rand()%7);
        }

        float get_speed(int level) {
            int i = (level > 15) ? 15 : level; // vitesse max = niveau 15
            return 1/(60*speeds[i]);
        }
};

class OnlinePlayer : public Player {
    private:
        int network_mode;
        bool connected;
        std::string pseudo;
        int status = 0; // 0 : menu, 1 : discovery, 2 : connected, 3 : ingame
        // 4 : lost, 5 : won

        int gamesock_fd;
        struct sockaddr_in servaddr;
        uint8_t buffer[1024];

        std::thread receiveThread;

    public:
        
        void send_packet(int input, int malus);
        uint8_t* generate_game_packet(int input, int malus);
        bool connect_to_server(GameInfo gameInfo);
        
        GameDiscovery game_discovery = GameDiscovery();

        void handle_received_packets();
        void handle_line_packet(uint8_t *buffer);
        void handle_game_packet(uint8_t *buffer);
        void handle_start_packet(uint8_t *buffer);
        void handle_end_packet(uint8_t *buffer);

        void setPseudo (std::string thePseudo) { this->pseudo = thePseudo; }

        OnlinePlayer() {
            if ( (gamesock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
                perror("game server socket creation failed");
                exit(EXIT_FAILURE);
            } 
            
            memset(&servaddr, 0, sizeof(servaddr));
        }

        ~OnlinePlayer() {
            close(gamesock_fd);
        }
};

class RivalsPlayer : public OnlinePlayer {
    private:
        int character;
        int ulti_charge; 
};

#endif