#ifndef GAME_DISCOVERY_HPP
#define GAME_DISCOVERY_HPP

#include <enet/enet.h>
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
    ENetAddress serverAddress;
    uint8_t isJoinable;
};

class GameDiscovery {
private:
    static const uint16_t DISCOVERY_PORT = 8888;
    
    std::atomic<bool> isRunning;
    std::thread broadcastThread;
    
    GameInfo gameInfo;

    int sock;

public:
    const int BROADCAST_INTERVAL_MS = 1000;
    GameDiscovery() : isRunning(false) {}
    
    bool initializeHost(const std::string& gameName, const std::string& motd,
        uint16_t gamePort, int maxPlayers);
    
    void startBroadcasting();
    void stopBroadcasting();
    void updatePlayerCount(int currentPlayers);
    
    // Client-side discovery
    std::vector<GameInfo> discoverGames(int timeoutMs = 2000) {
        std::vector<GameInfo> discoveredGames;

        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0 ) {
            perror("socket");
            return discoveredGames;
        }

        int reuse = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
            perror("setsockopt");
            close(sock);
            return discoveredGames;
        }

        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(DISCOVERY_PORT);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("bind");
            close(sock);
            return discoveredGames;
        }

        uint8_t buffer[1024];
        sockaddr_in fromAddr;
        socklen_t fromAddrLen = sizeof(fromAddr);
        
        auto startTime = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - startTime).count() < timeoutMs) {
            
            ssize_t received = recvfrom(sock, buffer, sizeof(buffer), 0,
                                  (sockaddr*)&fromAddr, &fromAddrLen);

            if (received < 0) {
                perror("recvfrom");
                close(sock);
                return discoveredGames;
            }
            
            if (buffer[0] != 0xD4) {
                std::cout << "incorrect head: " << buffer[0] << std::endl;
                break;
            }
            
            if (buffer[1] != 0x01) {
                std::cout << "incorrect type: " << buffer[1] << std::endl;
                break;
            }

            if (buffer[2] != 0x01) {
                std::cout << "incorrect version: " << buffer[2] << std::endl;
                break;
            }

            if (buffer[3] != static_cast<uint8_t>(received)) {
                std::cout << "incorrect length: " << received 
                << " with expected length: " << buffer[3] << std::endl;
                break;
            }
            
            GameInfo game;

            game.gameName.resize(32, ' ');
            for (int i=0;i<32;i++) {
                game.gameName[i] = static_cast<char>(buffer[4+i]);
            }

            char serverIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &fromAddr.sin_addr, serverIP, INET_ADDRSTRLEN);

            game.gamePort = ntohs(static_cast<uint16_t>(buffer[36]) << 8 + buffer[37]);
            game.currentPlayers = buffer[38];
            game.maxPlayers = buffer[39];
            game.isJoinable = buffer[40];

            game.motd.resize(64, ' ');
            for (int i=0;i<64;i++) {
                game.motd[i] = static_cast<char>(buffer[41+i]);
            }

            enet_address_set_host(&game.serverAddress, serverIP);
            discoveredGames.push_back(game);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        close(sock);
        return discoveredGames;
    }
};

#endif