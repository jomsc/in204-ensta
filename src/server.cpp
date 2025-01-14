#include "../include/server.hpp"

#include <enet/enet.h>
#include <cstdlib>

void GameServer::create_game() {
    gameDiscovery.initializeHost(gameInfo.gameName, gameInfo.motd,
                                gameInfo.gamePort, gameInfo.maxPlayers);
    gameDiscovery.startBroadcasting();

    server_host = enet_host_create(&gameInfo.serverAddress, 
                                    gameInfo.maxPlayers,
                                    32,
                                    0,
                                    0);
    
    seed = time(NULL);
    if (server_host == NULL) {
    fprintf (stderr, 
             "An error occurred while trying to create an ENet server host.\n");
    exit (EXIT_FAILURE);
    }
}

void GameServer::handle_join_requests() {
    ENetEvent event;
    while (enet_host_service(server_host, &event, 1000) > 0) {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            uint8_t* data = static_cast<uint8_t*>(event.packet->data);
            if (data[0] != 0xD4) {
                std::cout << "incorrect head: " << data[0] << std::endl;
                return;
            }

            if (data[1] != 0x02) {
                std::cout << "incorrect type: " << data[1] << std::endl;
                return;
            }

            if (data[2] != 0x01) {
                std::cout << "incorrect version: " << data[2] << std::endl;
                return;
            }

            if (data[3] != event.packet->dataLength-1) {
                std::cout << "incorrect size: " 
                << event.packet->dataLength-1 
                << " with expected length: " << data[3] << std::endl;
                return;
            }

            if (data[4] == 1) { // join request from a peer
                if (gameInfo.currentPlayers < gameInfo.maxPlayers) {
                    // get the pseudo 
                    std::string pseudo;
                    pseudo.resize(16, ' ');
                    for (int i=0;i<16;i++) {
                        pseudo[i] = static_cast<char>(data[5+i]);
                    }
                    // check if pseudo is taken, if not accept player in the game
                    if (std::find(player_list.begin(), player_list.end(), pseudo) 
                    == player_list.end()) { // if pseudo isn't already taken :
                        
                        // add player to the server
                        player_list.push_back(pseudo);
                        player_peers.push_back(event.peer);
                        Grid grid = Grid();
                        grids.push_back(grid);
                        loss_list.push_back(0);
                        gameInfo.currentPlayers++;

                        // send back the join request accepted packet to them
                        uint8_t buffer_data[25]; 
                        buffer_data[0] = 0xD4; 
                        buffer_data[1] = 0x02;
                        buffer_data[2] = 0x01; 
                        buffer_data[3] = 25;
                        
                        buffer_data[4] = 0x02; 
                        pseudo.resize(16, ' ');
                        for (int i=0;i<64;i++) {
                            buffer_data[5+i] = static_cast<uint8_t>(pseudo[i]);
                        }

                        *reinterpret_cast<uint32_t*>(data[21]) = this->seed;

                        ENetPacket* packet = enet_packet_create(buffer_data, 
                                                buffer_data[3]+1,
                                                ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(event.peer, 0, packet);
                    } else {
                        uint8_t buffer_data[25]; 
                        buffer_data[0] = 0xD4; 
                        buffer_data[1] = 0x02;
                        buffer_data[2] = 0x01; 
                        buffer_data[3] = 21;
                        
                        buffer_data[4] = 0x04; 
                        pseudo.resize(16, ' ');
                        for (int i=0;i<64;i++) {
                            buffer_data[5+i] = static_cast<uint8_t>(pseudo[i]);
                        }

                        *reinterpret_cast<uint32_t*>(data[21]) = this->seed;

                        ENetPacket* packet = enet_packet_create(buffer_data, 
                                                buffer_data[3]+1,
                                                ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(event.peer, 0, packet);
                    }
                } else {
                    uint8_t buffer_data[25]; 
                    buffer_data[0] = 0xD4; 
                    buffer_data[1] = 0x02;
                    buffer_data[2] = 0x01; 
                    buffer_data[3] = 21;
                    
                    buffer_data[4] = 0x03; 
                    std::string pseudo;
                    pseudo.resize(16, ' ');
                    for (int i=0;i<64;i++) {
                        buffer_data[5+i] = static_cast<uint8_t>(pseudo[i]);
                    }

                    *reinterpret_cast<uint32_t*>(data[21]) = this->seed;

                    ENetPacket* packet = enet_packet_create(buffer_data, 
                                            buffer_data[3]+1,
                                            ENET_PACKET_FLAG_RELIABLE);
                    enet_peer_send(event.peer, 0, packet);
                }
            } else {
                std::cout << "Unexpected packet received." << std::endl;
            }
        }
    }
}

bool GameServer::start_game() {
    if (gameInfo.currentPlayers > 1) {
        gameDiscovery.stopBroadcasting();
        // send packet game start by broadcasting the packet
        uint8_t buffer_data[5];
        buffer_data[0] = 0xD4;
        buffer_data[1] = 0x06;
        buffer_data[2] = 0x01;
        buffer_data[3] = 5;
        buffer_data[4] = 1;

        ENetPacket* packet = enet_packet_create(buffer_data, 
                                                buffer_data[3]+1,
                                                ENET_PACKET_FLAG_RELIABLE);

        enet_host_broadcast(server_host, 0, packet);

        return true;
    } else {
        return false;
    }
}

void GameServer::update() {} 

void GameServer::handle_received_packets() {}

void GameServer::send_packets() {}

uint8_t* GameServer::generate_game_packet(int playerIndex) {
    
}

bool GameServer::declare_victory() {
    for (int i=0;i<grids.size();i++) {
        if (grids[i].hasLost()) {
            std::cout << "Player " << player_list[i] << " lost." << std::endl;
            loss_list[i] = 1;
            gameInfo.currentPlayers--;

            // send loss packet
            uint8_t buffer_data[25]; 
            buffer_data[0] = 0xD4; 
            buffer_data[1] = 0x02;
            buffer_data[2] = 0x01; 
            buffer_data[3] = 5;
            
            buffer_data[4] = 1; 

            ENetPacket* packet = enet_packet_create(buffer_data, 
                                    buffer_data[3]+1,
                                    ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(player_peers[i], 0, packet);

            if (gameInfo.currentPlayers == 1) {
                // someone won, send them game won packet
                for (int j=0;j<grids.size();j++) {
                    if (!loss_list[j]) {
                        uint8_t buffer_data[25]; 
                        buffer_data[0] = 0xD4; 
                        buffer_data[1] = 0x02;
                        buffer_data[2] = 0x01; 
                        buffer_data[3] = 5;
                        
                        buffer_data[4] = 3; 

                        ENetPacket* packet = enet_packet_create(buffer_data, 
                                                buffer_data[3]+1,
                                                ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(player_peers[j], 0, packet);
                        return true;
                    }
                }
            } else {
                return false;
            }
        }
    }

    return false;
}

void GameServer::delete_game() {
    enet_host_destroy(server_host);
}