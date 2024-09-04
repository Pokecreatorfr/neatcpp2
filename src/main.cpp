#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <network.hpp>
#include <time.h>
#include <algorithm> // Pour std::min, std::max

#pragma comment(lib, "Ws2_32.lib")

struct entity {
    Network* network;
    float fitness;
    int id;
};

int main(int argv, char** args) {
    srand(static_cast<unsigned>(time(nullptr)));

    // Initialisation de WinSock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "Échec de l'initialisation de Winsock. Erreur : " << iResult << std::endl;
        return 1;
    }

    // Créer un socket pour le client
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Erreur de création du socket : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Spécifier l'adresse du serveur
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(54000);  // Le même port que le serveur
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Adresse IP locale

    // Connecter au serveur
    iResult = connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "Échec de la connexion au serveur : " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connecté au serveur!" << std::endl;

    // Initialisation de la population d'entités (réseaux de neurones)
    int population_size = 500; 
    std::vector<entity> population;

    std::vector<int> topology = { 6,6,12,16,32, 1}; 

    for (int i = 0; i < population_size; i++) {
        Network* network = new Network(topology);
        entity e = {network, 0.0f, i};
        population.push_back(e);
    }

    // Boucle principale
    bool running = true;

    int generation = 0;

    while(true)
    {
        generation++;
        for(int i = 0 ; i < population.size(); i++)
        {
            int turn_without_food = 0;
            int turn_alive = 0;
            bool game_over = false;
            while(!game_over)
            {
                float vision_data[6];
                int recv_result = recv(client_socket, (char*)&vision_data, sizeof(vision_data), 0);
                if (recv_result == SOCKET_ERROR) {
                    std::cerr << "Erreur de réception : " << WSAGetLastError() << std::endl;
                    break;
                }
                if (recv_result == 0) {
                    std::cerr << "Le client s'est déconnecté." << std::endl;
                    break;
                }

                matrix input(6, 1);
                for (int j = 0; j < 6; j++) {
                    input.data[j][0] = vision_data[j];
                }

                matrix output = population[i].network->feed_forward(input);

                float client_input = output.data[0][0];

                send(client_socket, (char*)&client_input, sizeof(client_input), 0);

                int score;
                recv(client_socket, (char*)&score, sizeof(score), 0);

                if(score ==  population[i].fitness)
                {
                    turn_without_food++;
                }

                population[i].fitness = score;


                if(score == -1 || turn_without_food > 100)
                {
                    game_over = true;
                    // envoyer au server qu'il faut réinitialiser le jeu
                    bool reset = true;  
                    send(client_socket, (char*)&reset, sizeof(reset), 0);

                    if(score == -1)
                    {
                        population[i].fitness += turn_alive * 0.05;
                    }
                }
                else
                {
                    bool reset = false;
                    send(client_socket, (char*)&reset, sizeof(reset), 0);
                }
                turn_alive++;
            }
        }

        // Trier la population par fitness
        std::sort(population.begin(), population.end(), [](entity a, entity b) {
            return a.fitness > b.fitness;
        });

        std::cout << "Generation: " << generation << std::endl;
        std::cout << "Best score: " << std::endl;
        std::cout << "ID: " << population[0].id << " Fitness: " << population[0].fitness << std::endl;
        population[0].network->print_info();

        if(population[0].fitness == 0)
        {
            std::cout << "Génération d'abrutis , on les tue tous !" << std::endl;
            for (int i = 0; i < population.size(); i++) {
                delete population.back().network;
                population.pop_back();
            }

            for (int i = 0; i < population_size; i++) {
                Network* network = new Network(topology);
                entity e = {network, 0.0f, i};
                population.push_back(e);
            }
        }
        else
        {
            float death_rate = 0.75;

            for(int i = 0; i < population.size() * death_rate; i++)
            {
                delete population.back().network;
                population.pop_back();
            }

            int population_left = population.size();

            for(int i = 0 ; population_size > population.size(); i++)
            {
                Network* new_network = population[i % population_left].network->clone();

                new_network->mutate(0.08, 0.08, 0.05, 0.03);

                int id = 0;

                for (int j = 0; j < population.size(); j++) {
                    if (population[j].id == id) {
                        id++;
                        j = 0;
                    }
                }

                entity e = {new_network, 0.0f, id};

                population.push_back(e);
            }
        }

    }

    

    

    return 0;
}