#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "grid.hpp"
#include "game_discovery.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <enet/enet.h>

#include <cstdlib>


class Player {
    private:
        uint32_t score;
        int level;
        _Float32 speed;

        sf::Clock update_clock;
        sf::Clock rotate_clock;

        uint32_t sequence_number;

    public:
        Grid grid;
        uint32_t seed;

        void display(sf::RenderWindow *window);
        void update();
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
            speed = 1;
            sequence_number = 0;

            update_clock.restart();
            rotate_clock.restart();

            grid.spawn(rand()%7);
        }
};

class OnlinePlayer : public Player {
    private:
        int network_mode;
        ENetHost *client;
        ENetPeer *server;
        ENetAddress address;
        ENetEvent event;
        bool connected;
        GameDiscovery game_discovery = GameDiscovery();
        char pseudo[16];

    public:
        void handle_received_packets(ENetPacket* packet);
        void send_packet(int input, int malus);
        uint8_t* generate_game_packet(int input, int malus);
        bool connect_to_server(GameInfo gameInfo, std::string pseudo);
        void handle_start_packet();

        OnlinePlayer() {
            if (enet_initialize () != 0) {
                fprintf (stderr, "ENet initialization error.\n");
            }

            client = enet_host_create(NULL, 1, 2, 0, 0);

            if (client == NULL) {
                fprintf (stderr, "ENet client host creation error.\n");
                exit(EXIT_FAILURE);
            } 
        }

        ~OnlinePlayer() {
            enet_host_destroy(client);
            enet_deinitialize();
        }
};

class RivalsPlayer : public OnlinePlayer {
    private:
        int character;
        int ulti_charge; 
};

#endif