#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "grid.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <enet/enet.h>


class Player {
    private:
        Grid grid;
        int score;
        int level;
        int speed;

    public:
        void handle_input();
        void display();
        void update();
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