#ifndef GAME_DISCOVERY_HPP
#define GAME_DISCOVERY_HPP

#include <enet/enet.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

class GameDiscovery {
private:
    static const uint16_t DISCOVERY_PORT = 8888;
    static const int BROADCAST_INTERVAL_MS = 1000;
    
    ENetHost* discoveryHost;
    std::atomic<bool> isRunning;
    std::thread broadcastThread;
    
    struct GameInfo {
        std::string gameName;
        std::string motd;
        uint16_t gamePort;
        u_int8_t maxPlayers;
        u_int8_t currentPlayers;
    };
    
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
            char data[1024];
            buffer.data = data;
            buffer.dataLength = sizeof(data);
            
            if (enet_socket_receive(client->socket, &sender, &buffer, 1) > 0) {
                std::string message(static_cast<char*>(buffer.data));
                
                // Parse received game info
                size_t pos1 = message.find(':');
                size_t pos2 = message.find(':', pos1 + 1);
                
                if (pos1 != std::string::npos && pos2 != std::string::npos) {
                    GameInfo game;
                    game.gameName = message.substr(0, pos1);
                    game.gamePort = std::stoi(message.substr(pos1 + 1, pos2 - pos1 - 1));
                    // Parse player counts...
                    discoveredGames.push_back(game);
                }
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        enet_host_destroy(client);
        return discoveredGames;
    }
};























#endif