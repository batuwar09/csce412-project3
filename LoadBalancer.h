#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include "RequestQueue.h"
#include "WebServer.h"

/**
 * @class LoadBalancer
 * @brief Manages the distribution of requests to multiple web servers.
 */
class LoadBalancer {
public:
    /**
     * @brief Constructor for LoadBalancer.
     * @param min_servers Minimum number of web servers.
     * @param max_servers Maximum number of web servers.
     * @param threshold_high Threshold for adding a new server.
     * @param threshold_low Threshold for removing a server.
     */
    LoadBalancer(int min_servers, int max_servers, int threshold_high, int threshold_low);

    /**
     * @brief Destructor for LoadBalancer.
     */
    ~LoadBalancer();

    /**
     * @brief Adds a batch of requests to the request queue.
     * @param reqs Vector of requests to be added.
     */
    void addRequests(const std::vector<Request>& reqs);

    /**
     * @brief Distributes requests from the queue to the web servers.
     */
    void distributeRequests();

    /**
     * @brief Generates requests dynamically.
     */
    void dynamicRequestGeneration();

    /**
     * @brief Simulates the load balancer operation for a given number of cycles.
     * @param cycles Number of cycles to simulate.
     */
    void simulate(int cycles);

    /**
     * @brief Stops the load balancer and all web servers.
     */
    void stop();

    /**
     * @brief Indicates whether the load balancer is running.
     */
    bool running;

    /**
     * @brief Vector of web servers managed by the load balancer.
     */
    std::vector<WebServer*> web_servers;

private:
    RequestQueue request_queue; ///< Queue to store incoming requests.
    std::mutex mtx; ///< Mutex for synchronizing access to the request queue.
    std::condition_variable cv; ///< Condition variable for synchronizing request processing.
    int min_servers; ///< Minimum number of web servers.
    int max_servers; ///< Maximum number of web servers.
    int threshold_high; ///< Threshold for adding a new server.
    int threshold_low; ///< Threshold for removing a server.
    int min_processing_time; ///< Minimum processing time observed.
    int max_processing_time; ///< Maximum processing time observed.
    bool stopping; ///< Flag to indicate if stopping is in progress.

    /**
     * @brief Allocates new web servers if needed.
     */
    void allocateServers();

    /**
     * @brief Deallocates web servers if not needed.
     */
    void deallocateServers();

    /**
     * @brief Logs activity messages.
     * @param message Message to log.
     */
    void logActivity(const std::string& message);

    /**
     * @brief Generates a random IP address.
     * @return Random IP address as a string.
     */
    std::string generateRandomIP();
};

#endif // LOADBALANCER_H
