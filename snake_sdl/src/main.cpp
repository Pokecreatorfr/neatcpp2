#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <SDL2/SDL.h>

#pragma comment(lib, "Ws2_32.lib")

int grid_size = 10;

struct position {
    int x;
    int y;
};

bool is_in_snake(const std::vector<position>& snake, position pos) {
    for (const auto& segment : snake) {
        if (segment.x == pos.x && segment.y == pos.y) {
            return true;
        }
    }
    return false;
}

bool is_snake_dead(const std::vector<position>& snake) {
    for (int i = 1; i < snake.size(); i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return true;
        }
    }


    if (snake[0].x < 0 || snake[0].x >= grid_size || snake[0].y < 0 || snake[0].y >= grid_size) {
        return true;
    }
    return false;
}

void initialize_game(std::vector<position>& snake, position& food) {

    //srand(666);

    snake = { {grid_size / 2, grid_size /2}, {grid_size / 2, grid_size/2 +1} };

    float random = (float)rand() / RAND_MAX;

    if(random < 0.33)
    {
        food = { 4,3};
    }
    else if(random < 0.66)
    {
        food = { 6,3};
    }
    else
    {
        food = { 5,2};
    }

    while (is_in_snake(snake, food)) {
        food = { rand() % grid_size, rand() % grid_size };
    }
}

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

direction turn_left(direction dir) {
    switch (dir) {
    case UP: return LEFT;
    case LEFT: return DOWN;
    case DOWN: return RIGHT;
    case RIGHT: return UP;
    }
    return UP;
}

direction turn_right(direction dir) {
    switch (dir) {
    case UP: return RIGHT;
    case RIGHT: return DOWN;
    case DOWN: return LEFT;
    case LEFT: return UP;
    }
    return UP;
}

std::pair<float, float> calculate_vision(const std::vector<position>& snake, position food, direction dir, position head) {
    float distance = 1.0f;
    float obstacle_type = 0.0f;

    int dx = 0;
    int dy = 0;

    switch (dir) {
    case UP: dy = -1; break;
    case DOWN: dy = 1; break;
    case LEFT: dx = -1; break;
    case RIGHT: dx = 1; break;
    }

    for (int i = 1; i < grid_size; i++) {
        position check_pos = { head.x + i * dx, head.y + i * dy };


        if (check_pos.x < 0 || check_pos.x >= grid_size || check_pos.y < 0 || check_pos.y >= grid_size) {
            distance = (float)i / grid_size ;
            obstacle_type = 0.0f; 
            break;
        }


        if (is_in_snake(snake, check_pos)) {
            distance = (float)i / grid_size;
            obstacle_type = 0.5f;  
            break;
        }

  
        if (check_pos.x == food.x && check_pos.y == food.y) {
            distance = (float)i / grid_size;
            obstacle_type = 1.0f;  // Food
            break;
        }
    }

    return { distance, obstacle_type };
}

int main(int argc, char* args[]) {

    

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "Échec de l'initialisation de Winsock. Erreur : " << iResult << std::endl;
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Erreur de création du socket : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(54000);  
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Erreur lors du bind : " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }


    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Erreur lors de l'écoute : " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Serveur en attente de connexion..." << std::endl;


    SOCKET client_socket = accept(server_socket, nullptr, nullptr);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Erreur lors de l'acceptation de la connexion : " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connecté!" << std::endl;


    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    std::vector<position> snake;
    position food;
    initialize_game(snake, food);
    direction dir = UP;  // Commence par aller vers le haut

    u_int time_divider = 1;

    while (running) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            // Gestion des touches
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    time_divider++;
                    break;
                case SDLK_DOWN:
                    if (time_divider > 1)
                    {
                        time_divider--;
                    }
                    break;
                }
            }
        }

 
        auto [distance_front, obstacle_front] = calculate_vision(snake, food, dir, snake[0]);
        auto [distance_left, obstacle_left] = calculate_vision(snake, food, turn_left(dir), snake[0]);
        auto [distance_right, obstacle_right] = calculate_vision(snake, food, turn_right(dir), snake[0]);


        float vision_data[6] = { distance_front, obstacle_front, distance_left, obstacle_left, distance_right, obstacle_right };
        send(client_socket, (char*)&vision_data, sizeof(vision_data), 0);


        float client_input;
        int recv_result = recv(client_socket, (char*)&client_input, sizeof(client_input), 0);

        if (recv_result == SOCKET_ERROR) {
            std::cerr << "Erreur de réception : " << WSAGetLastError() << std::endl;
            break;
        }
        if (recv_result == 0) {
            std::cerr << "Le client s'est déconnecté." << std::endl;
            break;
        }

        // Adapter la direction en fonction de la zone de l'entrée du client
        if (client_input < 0.33f) {
            dir = turn_left(dir);
        } else if (client_input > 0.66f) {
            dir = turn_right(dir);
        }


        position new_head = snake[0];
        if (dir == UP)
            new_head.y--;
        else if (dir == DOWN)
            new_head.y++;
        else if (dir == LEFT)
            new_head.x--;
        else if (dir == RIGHT)
            new_head.x++;

        snake.insert(snake.begin(), new_head);
        if (snake[0].x == food.x && snake[0].y == food.y) {
            while (is_in_snake(snake, food)) {
                food = { rand() % grid_size, rand() % grid_size };
            }
        } else {
            snake.pop_back();
        }

        int score = snake.size() - 2;  

        if (is_snake_dead(snake)) {

            score = -1;  
            send(client_socket, (char*)&score, sizeof(score), 0);

            initialize_game(snake, food);
        }
        else
        {
            send(client_socket, (char*)&score, sizeof(score), 0);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int x, y;
        SDL_GetWindowSize(window, &x, &y);

        for (const auto& seg : snake) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect rect = { seg.x * (x / grid_size), seg.y * (y / grid_size), (x / grid_size) - 1, (y / grid_size) - 1 };
            SDL_RenderFillRect(renderer, &rect);
        }

        bool reset;
        recv(client_socket, (char*)&reset, sizeof(reset), 0);

        if (reset)
        {
            initialize_game(snake, food);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect food_rect = { food.x * (x / grid_size), food.y *  (y / grid_size), (x / grid_size) - 1, (y / grid_size) - 1 };
        SDL_RenderFillRect(renderer, &food_rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(2 * time_divider);
    }

    closesocket(client_socket);
    closesocket(server_socket);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    WSACleanup();
    return 0;
}