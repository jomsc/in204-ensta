#include "../include/game_discovery.hpp"

bool GameDiscovery::initializeHost(const std::string& gameName, 
    const std::string& motd, uint16_t gamePort, int maxPlayers) {
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = DISCOVERY_PORT;
    
    discoveryHost = enet_host_create(&address, 1, 1, 0, 0);
    if (discoveryHost == nullptr) {
        return false;
    }
    
    gameInfo.gameName = gameName;
    gameInfo.gameName.resize(32, ' ');
    gameInfo.motd = motd;
    gameInfo.motd.resize(64, ' ');
    gameInfo.gamePort = gamePort;
    gameInfo.maxPlayers = maxPlayers;
    gameInfo.currentPlayers = 0;
    
    return true;
}

void GameDiscovery::startBroadcasting() {
    isRunning = true;
    broadcastThread = std::thread([this]() {
        ENetAddress broadcastAddr;
        broadcastAddr.host = ENET_HOST_BROADCAST;
        broadcastAddr.port = DISCOVERY_PORT;
        
        ENetPacket* packet;
        while (isRunning) {
            // creation du message de broadcast
            uint8_t buffer_data[105]; // voir netcode.md pour les specs

            // header
            buffer_data[0] = 0xD4; // tete
            buffer_data[1] = 0x01; // type : broadcast
            buffer_data[2] = 0x01; // version : 1
            buffer_data[3] = 0x69; // taille : 105 octets
            
            // game name :
            for (int i=0;i<32;i++) {
                buffer_data[4+i] = static_cast<uint8_t>(gameInfo.gameName[i]);
            }

            // game port
            *reinterpret_cast<uint16_t*>(buffer_data+36) = htons(gameInfo.gamePort);

            buffer_data[38] = gameInfo.currentPlayers; // nb of players
            buffer_data[39] = gameInfo.maxPlayers; // max nb of players
            buffer_data[40] = 0x01; // is joinable
            
            // game motd :
            for (int i=0;i<64;i++) {
                buffer_data[41+i] = static_cast<uint8_t>(gameInfo.motd[i]);
            }
       
            ENetBuffer buffer;
            buffer.data = buffer_data;
            buffer.dataLength = 105;

            // Broadcast the packet
            enet_socket_send(discoveryHost->socket, 
                            &broadcastAddr, 
                            &buffer, 
                            1);
            
            enet_packet_destroy(packet);

            std::cout << "sent packet for broadcast" << std::endl;
            
            std::this_thread::sleep_for(
                std::chrono::milliseconds(BROADCAST_INTERVAL_MS)
            );
        }
    });
}

void GameDiscovery::stopBroadcasting() {
    isRunning = false;
    if (broadcastThread.joinable()) {
        broadcastThread.join();
    }
    if (discoveryHost) {
        enet_host_destroy(discoveryHost);
        discoveryHost = nullptr;
    }
}

void GameDiscovery::updatePlayerCount(int currentPlayers) {
    gameInfo.currentPlayers = currentPlayers;
}


