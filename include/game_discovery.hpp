#ifndef GAME_DISCOVERY_HPP
#define GAME_DISCOVERY_HPP

#include <enet/enet.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

struct GameInfo {
    std::string gameName;
    std::string motd;
    uint16_t gamePort;
    u_int8_t maxPlayers;
    u_int8_t currentPlayers;
    ENetAddress serverAddress;
    uint8_t isJoinable;
};

class GameDiscovery {
private:
    static const uint16_t DISCOVERY_PORT = 8888;
    static const int BROADCAST_INTERVAL_MS = 1000;
    
    ENetHost* discoveryHost;
    std::atomic<bool> isRunning;
    std::thread broadcastThread;
    
    GameInfo gameInfo;

public:
    
    GameDiscovery() : discoveryHost(nullptr), isRunning(false) {}
    
    bool initializeHost(const std::string& gameName, const std::string& motd,
        uint16_t gamePort, int maxPlayers);
    
    void startBroadcasting();
    void stopBroadcasting();
    void updatePlayerCount(int currentPlayers);
    
    // Client-side discovery
    static std::vector<GameInfo> discoverGames(int timeoutMs = 2000) {
        std::vector<GameInfo> discoveredGames;
        
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = DISCOVERY_PORT;
        
        ENetHost* client = enet_host_create(&address, 1, 1, 0, 0);
        if (client == nullptr) {
            return discoveredGames;
        }
        
        auto startTime = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - startTime).count() < timeoutMs) {
            
            ENetAddress sender;
            ENetBuffer buffer;
            
            if (enet_socket_receive(client->socket, &sender, &buffer, 1) > 0) {
                uint8_t* data = static_cast<uint8_t*>(buffer.data);
                
                if (data[0] != 0xD4) {
                    std::cout << "incorrect head: " << data[0] << std::endl;
                    break;
                }
                
                if (data[1] != 0x01) {
                    std::cout << "incorrect type: " << data[1] << std::endl;
                    break;
                }

                if (data[2] != 0x01) {
                    std::cout << "incorrect version: " << data[2] << std::endl;
                    break;
                }

                if (data[3] != buffer.dataLength) {
                    std::cout << "incorrect length: " << buffer.dataLength 
                    << " with expected length: " << data[3] << std::endl;
                    break;
                }
                
                GameInfo game;

                game.gameName.resize(32, ' ');
                for (int i=0;i<32;i++) {
                    game.gameName[i] = static_cast<char>(data[4+i]);
                }

                game.gamePort = ntohs(static_cast<uint16_t>(data[36]) << 8 + data[37]);
                game.currentPlayers = data[38];
                game.maxPlayers = data[39];
                game.isJoinable = data[40];

                game.motd.resize(64, ' ');
                for (int i=0;i<64;i++) {
                    game.motd[i] = static_cast<char>(data[41+i]);
                }

                game.serverAddress = sender;
                discoveredGames.push_back(game);
                
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        enet_host_destroy(client);
        return discoveredGames;
    }
};























#endif