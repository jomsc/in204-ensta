#ifndef GAME_DISCOVERY_HPP
#define GAME_DISCOVERY_HPP

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

struct GameInfo {
    std::string gameName;
    std::string motd;
    uint16_t gamePort;
    u_int8_t maxPlayers;
    u_int8_t currentPlayers;
    char* serverIP; 
    uint8_t isJoinable;
};

class GameDiscovery {
private:
    static const uint16_t DISCOVERY_PORT = 8888;
    
    std::atomic<bool> isRunning;
    std::thread broadcastThread;
    
    GameInfo gameInfo;

    int discosock_fd;

public:
    const int BROADCAST_INTERVAL_MS = 1000;
    GameDiscovery() : isRunning(false) {}
    
    bool initializeHost(const std::string& gameName, const std::string& motd,
        uint16_t gamePort, int maxPlayers);
    
    void startBroadcasting();
    void stopBroadcasting();
    void updatePlayerCount(int currentPlayers);
    
    std::vector<GameInfo> discoverGames(int timeoutMs = 2000, 
                                        uint8_t gameType = 1);    
};

#endif