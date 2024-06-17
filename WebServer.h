#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "Request.h"

/**
 * @class WebServer
 * @brief Simulates a web server processing network requests.
 */
class WebServer {
public:
    /**
     * @brief Constructs a new WebServer object.
     * @param id The ID of the web server.
     */
    WebServer(int id);

    /**
     * @brief Destroys the WebServer object.
     */
    ~WebServer();

    /**
     * @brief Processes requests in the queue.
     */
    void processRequests();

    /**
     * @brief Adds a request to the server's queue.
     * @param req The request to add.
     */
    void addRequest(const Request& req);

    /**
     * @brief Stops the server from processing requests.
     */
    void stop();

private:
    int id; ///< The ID of the web server.
    std::queue<Request> requests; ///< The queue of requests.
    std::mutex mtx; ///< Mutex for synchronizing access to the queue.
    std::condition_variable cv; ///< Condition variable for managing request processing.
    bool running; ///< Flag indicating if the server is running.
    std::thread serverThread; ///< Thread for processing requests.
};

#endif // WEBSERVER_H
