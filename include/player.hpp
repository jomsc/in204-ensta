#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "grid.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <enet/enet.h>

#include <cstdlib>


class Player {
    private:
        Grid grid;
        int score;
        int level;
        int speed;

        sf::Clock update_clock;
        sf::Clock rotate_clock;

    public:
        void display(sf::RenderWindow *window);
        void update();

        Player() {
            grid = Grid();
            score = 0;
            level = 1;
            speed = 1;

            update_clock.restart();
            rotate_clock.restart();

            grid.spawn(rand()%7);
        }
};

class OnlinePlayer : public Player {
    private:
        int network_mode;
        ENetHost *client;
        ENetPeer *peer;
        ENetAddress address;
        ENetEvent event;
        bool connected;

    public:
        void handle_received_packets();
        void send_packet();
        void generate_packet();
        void connect_to_server(ENetAddress address);

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