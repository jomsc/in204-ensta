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
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) 
            && rotate_clock.getElapsedTime().asMilliseconds() > 300) {
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

bool OnlinePlayer::connect_to_server(GameInfo gameInfo, std::string pseudo) {
    ENetEvent event;
    ENetAddress address = gameInfo.serverAddress;

    // initialisation de la connection ENet
    server = enet_host_connect (this->client, &address, 2, 0);    
    
    if (server == NULL) {
        fprintf (stderr,  "ENet connection initialization peer error.\n");
        return false;
    }
    
    if (enet_host_service (client, & event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connection to " << gameInfo.gameName <<
        ":" << gameInfo.gamePort << " succeeded." << std::endl;
        
        // echange du paquet de connection défini dans netcode.md
        
        // creation du message de broadcast
        uint8_t buffer_data[21]; // voir netcode.md pour les specs

        // header
        buffer_data[0] = 0xD4; // tete
        buffer_data[1] = 0x02; // type : game_join
        buffer_data[2] = 0x01; // version : 1
        buffer_data[3] = 0x15; // taille : 21 octets
        
        buffer_data[4] = 0x01; // join requested
        pseudo.resize(16, ' ');
        for (int i=0;i<64;i++) {
            buffer_data[5+i] = static_cast<uint8_t>(pseudo[i]);
        }

        ENetPacket* packet = enet_packet_create(buffer_data, 
                                                buffer_data[3]+1,
                                                ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(server, 0, packet);

        if (enet_host_service(client, &event, 1000) > 0) {
            if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                uint8_t* data = static_cast<uint8_t*>(event.packet->data);

                if (data[0] != 0xD4) {
                    std::cout << "incorrect head: " << data[0] << std::endl;
                    return false;
                }

                if (data[1] != 0x02) {
                    std::cout << "incorrect type: " << data[1] << std::endl;
                    return false;
                }

                if (data[2] != 0x01) {
                    std::cout << "incorrect version: " << data[2] << std::endl;
                    return false;
                }

                if (data[3] != event.packet->dataLength-1) {
                    std::cout << "incorrect size: " 
                    << event.packet->dataLength-1 
                    << " with expected length: " << data[3] << std::endl;
                    return false;
                }

                switch (data[4]) {
                    case 1:
                        std::cout << "Unexpected request." << std::endl;
                        return false;
                    case 2:
                        std::cout << "Join request accepted" << std::endl;
                        pseudo.resize(16, ' ');
                        for (int i=0;i<16;i++) {
                            pseudo[i] = static_cast<char>(data[5+i]);
                        }
                        return true;
                    case 3:
                        std::cout << "Join request refused : server full" << std::endl;
                        return false;
                    case 4:
                        std::cout << "Join request refused : pseudo already taken" << std::endl;
                        return false;
                    case 5: 
                        std::cout << "Join request refused : unspecified reasons" << std::endl;
                        return false;
                    default:
                        return false;
                }
            }
        }
    } else {
        enet_peer_reset (server);
        std::cout << "Connection to " << gameInfo.gameName <<
        ":" << gameInfo.gamePort << " failed." << std::endl;
        return false;
    }
}

void OnlinePlayer::handle_received_packets() {}

void OnlinePlayer::send_packet(int input) {
    uint8_t* buffer_data = generate_game_packet(input);

    ENetPacket* packet = enet_packet_create(buffer_data, buffer_data[3]+1, 0);

    enet_peer_send(server, 0, packet);
}

uint8_t* OnlinePlayer::generate_game_packet(int input) {
    uint8_t size = 128+5*this->get_pieces_size();

    uint8_t buffer_data[size];

    // header
    buffer_data[0] = 0xD4;
    buffer_data[1] = 0x03;
    buffer_data[2] = 0x01;
    buffer_data[3] = size;

    // sequence number
    *reinterpret_cast<uint32_t*>(&buffer_data[4]) = this->get_sequence_number();

    // input
    buffer_data[8] = input;

    // score en 32 bits sur 4*8 bits
    *reinterpret_cast<uint32_t*>(&buffer_data[9]) = this->get_score();
    
    // niveau
    buffer_data[13] = this->get_level(); 

    // la grille (ouille ouille ouille)
    for (int i=0;i<grid.get_dimensions()/2;i++) {
        uint8_t data = static_cast<uint8_t>(grid.cells[2*i]) << 4 
                       + static_cast<uint8_t>(grid.cells[2*i+1]);
        // on bitshift la piece en 2*i et on additionne avec celle en 2*i+1
        // pour en mettre deux dans un uint8_t vu que valeur max 16 nous convient

        buffer_data[i+14] = data;
    }

    // pieces
    int offset = 14+grid.get_dimensions()/2;
    for (int i=0;i<grid.pieces.size();i++) {
        buffer_data[offset+5*i] = grid.pieces[i].x;
        buffer_data[offset+5*i+1] = grid.pieces[i].y;
        buffer_data[offset+5*i+2] = grid.pieces[i].orientation;
        buffer_data[offset+5*i+3] = grid.pieces[i].type;
        buffer_data[offset+5*i+4] = grid.pieces[i].value;
    }

    return buffer_data; 
}
