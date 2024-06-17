#include <iostream>
#include <thread>
#include "Request.h"
#include "WebServer.h"
#include "LoadBalancer.h"

/**
 * @brief Main function to run the load balancer simulation.
 * @return int 
 */
int main() {
    int min_servers;
    int max_servers;
    int threshold_high;
    int threshold_low;
    int simulation_time;

    std::cout << "Enter minimum number of web servers: ";
    std::cin >> min_servers;

    std::cout << "Enter maximum number of web servers: ";
    std::cin >> max_servers;

    std::cout << "Enter high threshold for adding a server: ";
    std::cin >> threshold_high;

    std::cout << "Enter low threshold for removing a server: ";
    std::cin >> threshold_low;

    std::cout << "Enter simulation time in clock cycles: ";
    std::cin >> simulation_time;

    LoadBalancer loadBalancer(min_servers, max_servers, threshold_high, threshold_low);
    loadBalancer.simulate(simulation_time);
    loadBalancer.stop();

    return 0;
}
