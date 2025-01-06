#include "../include/player.hpp"
#include <iostream>

void Player::display(sf::RenderWindow *window) {
    grid.draw(window);
    // afficher le score et le level
    // afficher les 3 prochaines pieces
}

void Player::update() {
    int input = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        input = 0;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        input = 1;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        input = 2;
    } else  {
        input = -1;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && rotate_clock.getElapsedTime().asMilliseconds() > 300) {
        grid.pieces[0].rotate();
        rotate_clock.restart();
    } 

    if ((input == 0 || input == 1) 
        && update_clock.getElapsedTime().asMilliseconds() > 500) {
        grid.update(input);
        grid.update(2);
        update_clock.restart();
    } else if (input == 2 && update_clock.getElapsedTime().asMilliseconds() > 150) {
        grid.update(2);
        update_clock.restart();
    } else if (update_clock.getElapsedTime().asMilliseconds() > 500) {
        grid.update(2);
        update_clock.restart();
    }

    if (grid.pieces.empty()) {
        grid.spawn(rand()%7);
    }

    std::vector<int> lines = grid.check_lines();
    if (!lines.empty()) {
        for (auto line : lines) {
            grid.clear_line(line);
        }
    }
}

void OnlinePlayer::connect_to_server(ENetAddress address) {
    ENetEvent event;
    ENetPeer *peer;
    
    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect (this->client, & address, 2, 0);    
    
    if (peer == NULL) {
    fprintf (stderr,  "ENet connection initialization peer error.\n");
    exit (EXIT_FAILURE);
    }
    
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service (client, & event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        puts ("Connection to some.server.net:1234 succeeded.");
    } else {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset (peer);
    
        puts ("Connection to some.server.net:1234 failed.");
    }
}
