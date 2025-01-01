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
        void receive_packet();
        void send_packet();
};

class RivalsPlayer : public OnlinePlayer {
    private:
        int character;
        int ulti_charge; 
};

#endif