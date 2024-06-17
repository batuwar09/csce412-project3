#include "LoadBalancer.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <sstream>
#include <climits>

/**
 * @brief Constructor for LoadBalancer.
 */
LoadBalancer::LoadBalancer(int min_servers, int max_servers, int threshold_high, int threshold_low)
    : running(true), min_servers(min_servers), max_servers(max_servers),
      threshold_high(threshold_high), threshold_low(threshold_low),
      min_processing_time(INT_MAX), max_processing_time(0), stopping(false) { // Initialize with extreme values and stopping flag
    for (int i = 0; i < min_servers; ++i) {
        web_servers.push_back(new WebServer(i));
    }
    std::cout << "LoadBalancer initialized with " << min_servers << " servers." << std::endl;
}

/**
 * @brief Destructor for LoadBalancer.
 */
LoadBalancer::~LoadBalancer() {
    for (WebServer* server : web_servers) {
        delete server;
    }
}

/**
 * @brief Logs activity messages.
 * @param message Message to log.
 */
void LoadBalancer::logActivity(const std::string& message) {
    std::cout << message << std::endl;
}

/**
 * @brief Adds a batch of requests to the request queue.
 * @param reqs Vector of requests to be added.
 */
void LoadBalancer::addRequests(const std::vector<Request>& reqs) {
    std::unique_lock<std::mutex> lock(mtx);
    for (const Request& req : reqs) {
        if (!running) break; // Prevent adding new requests after stopping
        request_queue.enqueue(req);
        
        // Update the min and max processing times
        if (req.getProcessingTime() < min_processing_time) {
            min_processing_time = req.getProcessingTime();
        }
        if (req.getProcessingTime() > max_processing_time) {
            max_processing_time = req.getProcessingTime();
        }

        std::cout << "Request added: " << req.getIpIn() << " -> " << req.getIpOut()
                  << " with processing time " << req.getProcessingTime() << " ms" << std::endl;
    }
    cv.notify_one();
}

/**
 * @brief Distributes requests from the queue to the web servers.
 */
void LoadBalancer::distributeRequests() {
    std::cout << "LoadBalancer started distributing requests." << std::endl;
    while (running || !request_queue.isEmpty()) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !request_queue.isEmpty() || !running; });

        while (!request_queue.isEmpty()) {
            allocateServers();
            for (WebServer* server : web_servers) {
                if (!request_queue.isEmpty()) {
                    server->addRequest(request_queue.dequeue());
                }
            }
            deallocateServers();
        }
    }
    std::cout << "Queue size after processing: " << request_queue.size() << std::endl;
    std::cout << "LoadBalancer stopped distributing requests." << std::endl;
}

/**
 * @brief Generates requests dynamically.
 */
void LoadBalancer::dynamicRequestGeneration() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::vector<Request> new_requests;
        int num_requests = rand() % 5 + 1;
        for (int i = 0; i < num_requests; ++i) {
            std::string ipIn = generateRandomIP();
            std::string ipOut = generateRandomIP();
            Request req(ipIn, ipOut, rand() % 3000 + 2000);
            new_requests.push_back(req);
        }

        addRequests(new_requests);
    }
    std::cout << "Dynamic request generation stopped." << std::endl;
}

/**
 * @brief Allocates new web servers if needed.
 */
void LoadBalancer::allocateServers() {
    std::cout << "Checking if new server allocation is needed..." << std::endl;
    if (request_queue.size() > threshold_high && web_servers.size() < max_servers) {
        int new_server_id = web_servers.size();
        web_servers.push_back(new WebServer(new_server_id));
        std::cout << "Allocated new WebServer with ID " << new_server_id << std::endl;
    }
}

/**
 * @brief Deallocates web servers if not needed.
 */
void LoadBalancer::deallocateServers() {
    std::cout << "Checking if server deallocation is needed..." << std::endl;
    if (request_queue.size() < threshold_low && web_servers.size() > min_servers) {
        int remove_server_id = web_servers.size() - 1;
        delete web_servers[remove_server_id];
        web_servers.pop_back();
        std::cout << "Deallocated WebServer with ID " << remove_server_id << std::endl;
    }
}

/**
 * @brief Simulates the load balancer operation for a given number of cycles.
 * @param cycles Number of cycles to simulate.
 */
void LoadBalancer::simulate(int cycles) {
    std::thread genThread(&LoadBalancer::dynamicRequestGeneration, this);
    std::thread distThread(&LoadBalancer::distributeRequests, this);

    std::this_thread::sleep_for(std::chrono::seconds(cycles / 1000)); // Convert cycles to seconds
    
    {
        std::unique_lock<std::mutex> lock(mtx);
        running = false;
    }
    genThread.join(); // Ensure request generation stops first
    cv.notify_all(); // Wake up any waiting threads
    distThread.join();

    // Log the range of task times
    std::cout << "Final queue size: " << request_queue.size() << std::endl;
    std::cout << "All requests processed, LoadBalancer stopped." << std::endl;
    logActivity("Range of task times: [" + std::to_string(min_processing_time) + " ms, " + std::to_string(max_processing_time) + " ms]");
}

/**
 * @brief Stops the load balancer and all web servers.
 */
void LoadBalancer::stop() {
    std::unique_lock<std::mutex> lock(mtx);
    if (stopping) return; // Ensure stop is only called once
    stopping = true;

    running = false;

    while (!request_queue.isEmpty()) {
        if (!request_queue.isEmpty()) {
            allocateServers();
            for (WebServer* server : web_servers) {
                if (!request_queue.isEmpty()) {
                    server->addRequest(request_queue.dequeue());
                }
            }
            deallocateServers();
        }
    }

    // Give some time for servers to finish processing
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    cv.notify_all();
    for (WebServer* server : web_servers) {
        server->stop();
    }

    // Log the range of task times
    std::cout << "Final queue size: " << request_queue.size() << std::endl;
    std::cout << "All requests processed, LoadBalancer stopped." << std::endl;
    logActivity("Range of task times: [" + std::to_string(min_processing_time) + " ms, " + std::to_string(max_processing_time) + " ms]");
}

/**
 * @brief Generates a random IP address.
 * @return Random IP address as a string.
 */
std::string LoadBalancer::generateRandomIP() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    std::stringstream ss;
    ss << dis(gen) << '.' << dis(gen) << '.' << dis(gen) << '.' << dis(gen);
    return ss.str();
}
