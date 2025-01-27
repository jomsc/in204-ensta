#include "server.hpp"

#include <enet/enet.h>
#include <cstdlib>


void GameServer::create_game() {
    gameDiscovery.initializeHost(gameInfo.gameName, gameInfo.motd,
                                gameInfo.gamePort, gameInfo.maxPlayers);
    gameDiscovery.startBroadcasting();

    if ( (gamesock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("game server socket creation failed");
        exit(EXIT_FAILURE);
    } 
    
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(gameInfo.gamePort);

    if ( bind(gamesock_fd, (const struct sockaddr *)&servaddr, 
    sizeof(servaddr)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }  
    
    seed = time(NULL);
}

void GameServer::handle_join_requests(std::string pseudo,
                                     struct sockaddr_in cliaddr) {

    if (gameInfo.currentPlayers < gameInfo.maxPlayers) {
        // check if pseudo is taken, if not accept player in the game
        if (std::find(player_list.begin(), player_list.end(), pseudo) 
        == player_list.end()) { // if pseudo isn't already taken :
            
            // add player to the server
            // on rajoute une case dans tous les tableaux
            player_list.push_back(pseudo);
            cliaddr_list.push_back(cliaddr);

            Grid grid = Grid();
            grids.push_back(grid);
            scores.push_back(0);
            levels.push_back(1);
            loss_list.push_back(0);
            rand_index.push_back(0);
            gameInfo.currentPlayers += 1;
            this->gameDiscovery.numberOfPlayers = gameInfo.currentPlayers;

            // send back the join request accepted packet to them
            uint8_t buffer_data[25]; 
            buffer_data[0] = 0xD4; 
            buffer_data[1] = 0x02;
            buffer_data[2] = 0x01; 
            buffer_data[3] = 25;
            
            buffer_data[4] = 0x02; 
            pseudo.resize(16, ' ');
            for (int i=0;i<16;i++) {
                buffer_data[5+i] = static_cast<uint8_t>(pseudo[i]);
            }
            
            buffer_data[21] = this->seed & 0xFF;
            buffer_data[22] = (this->seed >> 8) & 0xFF;
            buffer_data[23] = (this->seed >> 16) & 0xFF;
            buffer_data[24] = (this->seed >> 24) & 0xFF;

            // envoyer buffer_data a cliaddr
            int len = sizeof(cliaddr);
            sendto(gamesock_fd, buffer_data, buffer_data[3], MSG_CONFIRM,
                  (const struct sockaddr *) &cliaddr, len);

            std::cout << "Player accepted ! Pseudo : " << player_list.back() << std::endl;
            std::cout << "Players : " << std::to_string(gameInfo.currentPlayers) 
            << "/" << std::to_string(gameInfo.maxPlayers) << std::endl;

        } else {
            uint8_t buffer_data[25]; 
            buffer_data[0] = 0xD4; 
            buffer_data[1] = 0x02;
            buffer_data[2] = 0x01; 
            buffer_data[3] = 21;
            
            buffer_data[4] = 0x04; 
            pseudo.resize(16, ' ');
            for (int i=0;i<16;i++) {
                buffer_data[5+i] = static_cast<uint8_t>(pseudo[i]);
            }

            int len = sizeof(cliaddr);
            sendto(gamesock_fd, buffer_data, buffer_data[3], MSG_CONFIRM,
                  (const struct sockaddr *) &cliaddr, len);
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
        for (int i=0;i<16;i++) {
            buffer_data[5+i] = static_cast<uint8_t>(pseudo[i]);
        }
        
        int len = sizeof(cliaddr);
        sendto(gamesock_fd, buffer_data, buffer_data[3], MSG_CONFIRM,
              (const struct sockaddr *) &cliaddr, len);
    }
    std::cout << "test3" << std::endl;
}

bool GameServer::start_game() {
    if (gameInfo.currentPlayers > 1) {
        status = 1;
        gameDiscovery.stopBroadcasting();
        // send packet game start by broadcasting the packet
        uint8_t buffer_data[5];
        buffer_data[0] = 0xD4;
        buffer_data[1] = 0x06;
        buffer_data[2] = 0x01;
        buffer_data[3] = 5;
        buffer_data[4] = 1;

        for (auto cliaddr_t : cliaddr_list) {
            int len = sizeof(cliaddr_t);
            sendto(gamesock_fd, buffer_data, buffer_data[3], MSG_CONFIRM,
                (const struct sockaddr *) &cliaddr_t, len);
        }

        return true;
    } else {
        std::cout << "Not enough players." << std::endl;
        return false;
    }
}

void GameServer::update() {} 

void GameServer::handle_received_packets() {
    std::cout << "handling received packets" << std::endl;
    receiveThread = std::thread([this]() {
        while (status < 2) {
            socklen_t len;
            int n;
            len = sizeof(cliaddr);
            n = recvfrom(gamesock_fd, (uint8_t*)buffer, 1024, MSG_WAITALL,
                        (struct sockaddr *) &cliaddr, &len);

            if (buffer[0] != 0xD4) {
                std::cout << "incorrect head: " << buffer[0] << std::endl;
                return;
            }

            if (buffer[2] != 0x01) {
                std::cout << "incorrect version: " << buffer[2] << std::endl;
                return;
            }

            if (buffer[3] != n) {
                std::cout << "incorrect size: " 
                << n << " with expected length: " << buffer[3] << std::endl;
                return;
            }

            std::string pseudo;
            switch (buffer[1]) { 
                // on appelle la fonction correcte en fonction du type de paquet 

                case 1:
                    // broadcast packet, ignore
                    std::cout << "Other server on LAN, ignoring." << std::endl;
                    break;

                case 2:
                    // join request
                    std::cout << "Join request recieved" << std::endl;
                    if (buffer[4]==1) {
                        pseudo.resize(16, ' ');
                        for (int i=0;i<16;i++) {
                            pseudo[i] = static_cast<char>(buffer[5+i]);
                        }
                        handle_join_requests(pseudo, cliaddr);
                    } else { // received join accepted/refused
                        std::cout << "Unexpected packet received" << std::endl;
                    }

                    std::cout << "test4" << std::endl;
                    break;
                

                default:
                    std::cout << "Unexpected packet received" << std::endl;
            }
        }        
    });
}



void GameServer::send_game_packets() {
    for (int i=0;i<this->gameInfo.currentPlayers;i++) {
        uint8_t* data = generate_game_packet(i);

        int len = sizeof(cliaddr_list[i]);
        sendto(gamesock_fd, data, data[3], MSG_CONFIRM,
              (const struct sockaddr *) &cliaddr_list[i], len);

        std::cout << "Packet sent to player " << i 
        << " successfully." << std::endl;
    }

    return;
}

void GameServer::send_line_packet(int playerSource, int playerDest, int n) {
    uint8_t buffer_data[25];
    buffer_data[0] = 0xD4;
    buffer_data[1] = 0x05;
    buffer_data[2] = 0x01;
    buffer_data[3] = 25;

    *reinterpret_cast<uint32_t*>(buffer_data[4]) = this->sequence_number;
    buffer_data[8] = n; // quelle case sera libre de la ligne envoyée

    for (int i=0;i<64;i++) {
        buffer_data[9+i] = static_cast<uint8_t>(player_list[playerSource][i]);
    }

    int len = sizeof(cliaddr_list[playerDest]);
    sendto(gamesock_fd, buffer_data, buffer_data[3], MSG_CONFIRM,
            (const struct sockaddr *) &cliaddr_list[playerDest], len);

    std::cout << "Line packet with index " << n << " sent to "
              << player_list[playerDest] << " from "
              << player_list[playerSource] << " successfully." << std::endl;
    return;
}

uint8_t* GameServer::generate_game_packet(int playerIndex) {
    uint8_t buffer_data[233];
    buffer_data[0] = 0xD4;
    buffer_data[1] = 0x04;
    buffer_data[2] = 0x01;
    buffer_data[3] = 233;
    
    *reinterpret_cast<uint32_t*>(buffer_data[4]) = this->sequence_number;
    *reinterpret_cast<uint32_t*>(buffer_data[8]) = this->scores[playerIndex];
    buffer_data[12] = this->levels[playerIndex];
    
    for (int i=0;i<this->grids[playerIndex].get_dimensions();i++) {
        buffer_data[13+i] = this->grids[playerIndex].cells[i];
    }

    return buffer_data;
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

            int len = sizeof(cliaddr_list[i]);
            sendto(gamesock_fd, buffer_data, buffer_data[3], MSG_CONFIRM,
                    (const struct sockaddr *) &cliaddr_list[i], len);


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

                        int len = sizeof(cliaddr_list[j]);
                        sendto(gamesock_fd, buffer_data, buffer_data[3], MSG_CONFIRM,
                                (const struct sockaddr *) &cliaddr_list[j], len);

                        status = 2;
                                
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
    if (receiveThread.joinable()) {
        receiveThread.join();
        status = 3;
    }
    close(gamesock_fd);
}