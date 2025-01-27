#include "../include/game_discovery.hpp"

bool GameDiscovery::initializeHost(const std::string& gameName, 
    const std::string& motd, uint16_t gamePort, int maxPlayers) {
       
    gameInfo.gameName = gameName;
    gameInfo.gameName.resize(32, ' ');
    gameInfo.motd = motd;
    gameInfo.motd.resize(64, ' ');
    gameInfo.gamePort = gamePort;
    gameInfo.maxPlayers = maxPlayers;
    gameInfo.currentPlayers = 0;
    numberOfPlayers = 0;
    
    return true;
}

void GameDiscovery::startBroadcasting() {
    isRunning = true;
    std::cout << "started broadcasting" << std::endl;
    broadcastThread = std::thread([this]() {

        discosock_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (discosock_fd < 0) {
            perror("socket");
            return 1;
        }

        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(DISCOVERY_PORT);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(discosock_fd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("Bind failed");
            close(discosock_fd);
            exit(EXIT_FAILURE);
        }

        uint8_t buffer[1024];
        sockaddr_in fromAddr;
        socklen_t fromAddrLen = sizeof(fromAddr);
        fromAddr.sin_family = AF_INET;
        
        // creation du message de broadcast
        uint8_t buffer_data[105]; // voir netcode.md pour les specs
        buffer_data[0] = 0xD4; // tete
        buffer_data[1] = 0x01; // type : broadcast
        buffer_data[2] = 0x01; // version : 1
        buffer_data[3] = 0x69; // taille : 105 octets
        for (int i=0;i<32;i++) { // game name
            buffer_data[4+i] = static_cast<uint8_t>(gameInfo.gameName[i]);
        } // port
        *reinterpret_cast<uint16_t*>(buffer_data+36) = gameInfo.gamePort;
        buffer_data[38] = numberOfPlayers; // nb of players
        buffer_data[39] = gameInfo.maxPlayers; // max nb of players
        buffer_data[40] = 0x01; // is joinable
        for (int i=0;i<64;i++) { // motd
            buffer_data[41+i] = static_cast<uint8_t>(gameInfo.motd[i]);
        }

        while (isRunning) {
            
             

            ssize_t received = recvfrom(discosock_fd, buffer, sizeof(buffer), 0,
                                  (sockaddr*)&fromAddr, &fromAddrLen);

            gameInfo.currentPlayers = numberOfPlayers;
            buffer_data[38] = numberOfPlayers;
            std::cout << "Number of Players : " << std::to_string(numberOfPlayers) << std::endl;

            if (buffer[0] == 0xD4 &&
                buffer[1] == 0 &&
                buffer[2] == 1 &&
                buffer[3] == received &&
                buffer[4] == 1) {
                
                ssize_t sent = sendto(discosock_fd, buffer_data, 105, 0,
                            (sockaddr*)&fromAddr, sizeof(fromAddr));
                
                if (sent < 0) {
                    perror("sendto");
                    close(discosock_fd);
                    return 1;
                }

                char clientIP[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &fromAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
                std::cout << "Broadcasted server presence to client at " << 
                clientIP << std::endl;
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(100)
            );
        }
    });
}

void GameDiscovery::stopBroadcasting() {
    isRunning = false;
    if (broadcastThread.joinable()) {
        broadcastThread.join();
    }
    close(discosock_fd);
}

void GameDiscovery::updatePlayerCount(u_int8_t currentPlayers) {
    numberOfPlayers = currentPlayers;
}

std::vector<GameInfo> GameDiscovery::discoverGames(int timeoutMs, uint8_t gameType) {
    std::vector<GameInfo> discoveredGames;
    discosock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (discosock_fd < 0 ) {
        perror("socket");
        return discoveredGames;
    }

    int broadcastEnable = 1;
    if (setsockopt(discosock_fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, 
        sizeof(broadcastEnable)) < 0) {
        perror("setsockopt");
        close(discosock_fd);
        return discoveredGames;
    }

    sockaddr_in broadcastAddr;
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(DISCOVERY_PORT);
    //broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255"); // broadcast
    broadcastAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost


    uint8_t buffer[1024];
    sockaddr_in fromAddr;
    socklen_t fromAddrLen = sizeof(fromAddr);

    uint8_t broadcastMsg[5];
    broadcastMsg[0] = 0xD4;
    broadcastMsg[1] = 0x00;
    broadcastMsg[2] = 0x01;
    broadcastMsg[3] = 5;
    broadcastMsg[4] = gameType;
    
    auto startTime = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - startTime).count() < timeoutMs) {
        
        ssize_t sent = sendto(discosock_fd, broadcastMsg, 5, 0,
                            (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));

        std::cout << "Sent game request" << std::endl;

        ssize_t received = recvfrom(discosock_fd, buffer, sizeof(buffer), 0,
                                (sockaddr*)&fromAddr, &fromAddrLen);

        if (received < 0) {
            perror("recvfrom");
            close(discosock_fd);
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

        inet_ntop(AF_INET, &fromAddr.sin_addr, game.serverIP, INET_ADDRSTRLEN);


        game.gamePort = (buffer[37] << 8) + buffer[36];
        game.currentPlayers = buffer[38];
        game.maxPlayers = buffer[39];
        game.isJoinable = buffer[40];

        game.motd.resize(64, ' ');
        for (int i=0;i<64;i++) {
            game.motd[i] = static_cast<char>(buffer[41+i]);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        bool alreadyFound = false;
        for (int i=0;i<discoveredGames.size();i++) {
            if (discoveredGames[i].gameName == game.gameName) {
                alreadyFound = true;
            }
        }

        if (!alreadyFound) { discoveredGames.push_back(game); }
    }
    
    close(discosock_fd);
    return discoveredGames;
}