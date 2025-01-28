#include "player.hpp"
#include <iostream>

void Player::display(sf::RenderWindow *window) {
    grid.draw(window, this->score, this->level,this->buffer);
}

void Player::update_next_pieces(){
            for(int i=0;i<2;i++){
            this->buffer[i]=this->buffer[i+1];
            }
            this->buffer[2]= rand()%7;
        }

void Player::update_score(int num_lines_cleared,int level){
    switch (num_lines_cleared)
    {
    case 1:
        this->score += level*40;
        this->num_lines_cleared=0;
        break;
    
    case 2:
        this->score += level*100;
        this->num_lines_cleared=0;
        break;

    case 3:
        this->score += level*300;
        this->num_lines_cleared=0;
        break;

    case 4:
        this->score += level*1200;
        this->num_lines_cleared=0;
        break;
    
    default:
        break;
    }
}

void Player::update_level(){
    if(this->level_up_buffer>=10){
        this->level++;
        this->level_up_buffer=this->level_up_buffer%10;
    }
}

void Player::update() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)&&
            down_clock.getElapsedTime().asMilliseconds() > 155&&
            spawn_wait_clock.getElapsedTime().asMilliseconds() > 400) //déplacement vers le bas
        {
            if(floor && soft_lock_clock.getElapsedTime().asMilliseconds()<=175)
            {
                grid.update(&floor,&left_wall,&right_wall,&lock_in);
                down_clock.restart();
            }
            else if(floor && soft_lock_clock.getElapsedTime().asMilliseconds()>175)
            {
                lock_in=true;
                grid.update(&floor,&left_wall,&right_wall,&lock_in);
                down_clock.restart();
            }
             else
            {
                floor=false;
                left_wall=false;
                right_wall=false;
                grid.pieces[0].move(2);
                grid.update(&floor,&left_wall,&right_wall,&lock_in);
                down_clock.restart();
            }
        }
    else if (gravity_clock.getElapsedTime().asSeconds() > speed&&
        spawn_wait_clock.getElapsedTime().asMilliseconds() > 400) //déplacement automatique vers le bas
    {
        if(floor && soft_lock_clock.getElapsedTime().asMilliseconds()<=175)
        {   
            left_wall=false;
            right_wall=false;
            grid.update(&floor,&left_wall,&right_wall,&lock_in);
            gravity_clock.restart();
            down_clock.restart();
        }
        else if(floor && soft_lock_clock.getElapsedTime().asMilliseconds()>175)
        {
            lock_in=true;
            grid.update(&floor,&left_wall,&right_wall,&lock_in);
            gravity_clock.restart();
            down_clock.restart();
        }    
        else
        {
            floor=false;
            left_wall=false;
            right_wall=false;
            grid.pieces[0].move(2);
            grid.update(&floor,&left_wall,&right_wall,&lock_in);
            gravity_clock.restart();
            down_clock.restart();
        }
        }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)&&
            movement_clock.getElapsedTime().asMilliseconds() > 155)  //déplacement vers la droite
        {
            if(!left_wall)
            {
            grid.pieces[0].move(0);
            floor=false;
            right_wall=false;
            grid.update(&floor,&left_wall,&right_wall,&lock_in);
            movement_clock.restart();
            }
            else
            {
            grid.update(&floor,&left_wall,&right_wall,&lock_in);
            movement_clock.restart();
            }
        } 
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)&&
            movement_clock.getElapsedTime().asMilliseconds() > 155) //déplacement vers la gauche
        {
        if(!right_wall)
            {
            grid.pieces[0].move(1);
            floor=false;
            left_wall=false;
            grid.update(&floor,&left_wall,&right_wall,&lock_in);
            movement_clock.restart();
            }
            else
            {
            grid.update(&floor,&left_wall,&right_wall,&lock_in);
            movement_clock.restart();
            }
        }  



    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)&&
    spawn_wait_clock.getElapsedTime().asMilliseconds() > 400) 
        {
        this->speed = get_speed(14);
        } 
        else 
        {
        this->speed= get_speed(this->level-1);
        }



    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && 
            rotate_clock.getElapsedTime().asMilliseconds() > 200) 
        {
        grid.wall_kick_clockwise(grid.pieces[0].type, grid.pieces[0].orientation);
        left_wall=false;
        right_wall=false;
        floor=false;
        grid.pieces[0].rotate(0);
        grid.update(&floor,&left_wall,&right_wall,&lock_in);
        rotate_clock.restart();
        }   
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && 
            rotate_clock.getElapsedTime().asMilliseconds() > 200)
        {
        grid.wall_kick_counterclockwise(grid.pieces[0].type, grid.pieces[0].orientation);
        left_wall=false;
        right_wall=false;
        floor = false;
        grid.pieces[0].rotate(1);
        grid.update(&floor,&left_wall,&right_wall,&lock_in);
        rotate_clock.restart();
        }



    if (grid.pieces.empty()) {
        left_wall=false;
        right_wall=false;
        floor=false;
        grid.spawn(this->buffer[0], &fin);
        spawn_wait_clock.restart();
        update_next_pieces();
    }


    std::vector<int> lines = grid.check_lines();
    if (!lines.empty()) {
        this->num_lines_cleared =0;
        for (auto line : lines) {
            this->num_lines_cleared++;
            this->level_up_buffer++;
            grid.clear_line(line);
        }
    }

    update_score(this->num_lines_cleared,this->level);

    update_level();
}

bool OnlinePlayer::connect_to_server(GameInfo gameInfo) {
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(gameInfo.gamePort);

    if (inet_pton(AF_INET, gameInfo.serverIP, &servaddr.sin_addr) <= 0) {
        std::cout << "Game IP : " << gameInfo.serverIP << " failed" << std::endl;
        perror("Invalid address / Address not supported");
        close(gamesock_fd);
        return false;
    }
    
    if (connect(gamesock_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection to server failed.");
        close(gamesock_fd);
        return false;
    } else {
        // connection succeeded, sending game packet 
        std::cout << "Connection to " << gameInfo.gameName <<
        ":" << gameInfo.gamePort << " succeeded." << std::endl;


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

        ssize_t bytes_sent = send(gamesock_fd, buffer_data, buffer_data[3], 0);

        if (bytes_sent < 0) { 
            perror("Join request send failed.");
            close(gamesock_fd);
            return false;
        }

        std::cout << "Join request sent, waiting for response." << std::endl;

        uint8_t buffer[1024];
        ssize_t bytes_received = recv(gamesock_fd, buffer, sizeof(buffer)-1,0);

        if (bytes_received < 0) {
            perror("Join request response receive failed.");
            return false;
        }

        if (buffer[0] != 0xD4) {
            std::cout << "incorrect head: " << buffer[0] << std::endl;
            return false;
        }

        if (buffer[1] != 0x02) {
            std::cout << "incorrect type: " << buffer[1] << std::endl;
            return false;
        }

        if (buffer[2] != 0x01) {
            std::cout << "incorrect version: " << buffer[2] << std::endl;
            return false;
        }

        if (buffer[3] != bytes_received) {
            std::cout << "incorrect size: " << bytes_received
            << " with expected length: " << buffer[3] << std::endl;
            return false;
        }

        switch (buffer[4]) {
            case 1:
                std::cout << "Unexpected request." << std::endl;
                return false;
            case 2:
                std::cout << "Join request accepted!" << std::endl;
                pseudo.resize(16, ' ');
                for (int i=0;i<16;i++) {
                    pseudo[i] = static_cast<char>(buffer[5+i]);
                }
                std::cout << "test 1" << std::endl;
                this->seed = 
                        ((uint32_t)buffer[21]      ) |
                        ((uint32_t)buffer[21] << 8 ) |
                        ((uint32_t)buffer[21] << 16) |
                        ((uint32_t)buffer[21] << 24); 
                
                status = 2;
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

void OnlinePlayer::handle_received_packets() {
    receiveThread = std::thread([this]() {
        socklen_t len;
        int n;
        len = sizeof(servaddr);
        n = recv(gamesock_fd, buffer, sizeof(buffer) - 1, 0);

        if (n < 0) {
            std::cerr << "Receive failed\n";
        }

        uint8_t buffer[1024];
        
        if (buffer[0] != 0xD4) {
            std::cout << "incorrect head: " << buffer[0] << std::endl;
            return;
        }

        if (buffer[2] != 0x01) {
            std::cout << "incorrect version: " << buffer[2] << std::endl;
            return;
        }

        if (buffer[3] != n) {
            std::cout << "incorrect size: " << n
            << " with expected length: " << buffer[3] << std::endl;
            return;
        }

        switch (buffer[1]) {
            case 0:
                std::cout << "Received game broadcast request packet, ignoring"
                << std::endl;
                break;

            case 1:
                std::cout << "Received game broadcast packet, ignoring" 
                << std::endl;
                break;
            
            case 2:
                // game join / joined
                break;
            
            case 3:
                std::cout << "Received client->server packet, ignoring" 
                << std::endl;
                break;
            
            case 4:
                // classique server->client
                handle_game_packet(buffer);
                break;

            case 5:
                // server -> client je me prends une ligne
                handle_line_packet(buffer);
                break;

            case 6:
                // game start
                handle_start_packet(buffer);
                break;

            case 7:
                // game end
                handle_end_packet(buffer);
                break;

            default:
                break;
        }
    }); 
}

void OnlinePlayer::send_packet(int input, int malus) {
    uint8_t* buffer_data = generate_game_packet(input, malus);

    ssize_t bytes_sent = send(gamesock_fd, buffer_data, buffer_data[3], 0);

    if (bytes_sent < 0) { 
        perror("sent game packet.");
    }
}

uint8_t* OnlinePlayer::generate_game_packet(int input, int malus) {

    uint8_t buffer_data[10];
    // header
    buffer_data[0] = 0xD4;
    buffer_data[1] = 0x03;
    buffer_data[2] = 0x01;
    buffer_data[3] = 10;

    // sequence number
    *reinterpret_cast<uint32_t*>(&buffer_data[4]) = this->get_sequence_number();

    // input
    buffer_data[8] = input;
    buffer_data[9] = malus;
    return buffer_data; 
}

void OnlinePlayer::handle_start_packet(uint8_t *buffer) { 
    if (buffer[4] == 1) {
        if (status == 2) {
            std::cout << "GAME STARTS !" << std::endl;
            status = 3;
        } else {
            std::cout << "uhh... not connected to server ?" << std::endl;
        }
    } else {
        std::cout << "Unexpected game start packet received." << std::endl;
    }
}

// TODO
void OnlinePlayer::handle_line_packet(uint8_t *buffer) {
    int malus = buffer[8];
    std::string pseudo_adversaire;
    pseudo_adversaire.resize(16, ' ');
    for (int i=0;i<16;i++) {
        pseudo_adversaire[i] = static_cast<char>(buffer[5+i]);
    }

    // TODO : ON AJOUTE LA LIGNE PAR LE BAS DE LA PART DE 
    // PSEUDO_ADVERSAIRE
}

// simple no rollback for now
void OnlinePlayer::handle_game_packet(uint8_t *buffer) {
    this->set_score(*reinterpret_cast<uint32_t*>(buffer[8]));
    this->set_level(buffer[12]);
    for (int i=0;i<this->grid.get_dimensions();i++) {
        this->grid.cells[i] = buffer[13+i];
    }
}

void OnlinePlayer::handle_end_packet(uint8_t *buffer) {
    switch (buffer[4]) {
        case 0:
            std::cout << "staying alive!" << std::endl;
            break;
        case 1:
            std::cout << "game over! you reached the top" << std::endl;
            status = 4;
            break;
        case 2:
            std::cout << "game over! but why ?" << std::endl;
            status = 4;
            break;
        case 3:
            std::cout << "YOU WON!!!" << std::endl;
            status = 5;
            break;
        default:
            break;
    }
}