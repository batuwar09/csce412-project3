#include "WebServer.h"
#include <iostream>
#include <chrono>

/**
 * @brief Constructs a new WebServer object.
 * @param id The ID of the web server.
 */
WebServer::WebServer(int id) : id(id), running(true), serverThread(&WebServer::processRequests, this) {}

/**
 * @brief Destroys the WebServer object.
 */
WebServer::~WebServer() {
    stop();
}

/**
 * @brief Processes requests in the queue.
 */
void WebServer::processRequests() {
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !requests.empty() || !running; });

        while (!requests.empty() && running) {
            Request req = requests.front();
            requests.pop();
            lock.unlock();

            std::cout << "WebServer " << id << " processing request from " << req.getIpIn()
                      << " to " << req.getIpOut() << " with time " << req.getProcessingTime() << " ms\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(req.getProcessingTime()));

            lock.lock();
        }
    }
    std::cout << "WebServer " << id << " stopped processing requests." << std::endl;
}

/**
 * @brief Adds a request to the server's queue.
 * @param req The request to add.
 */
void WebServer::addRequest(const Request& req) {
    std::unique_lock<std::mutex> lock(mtx);
    requests.push(req);
    cv.notify_one();
}

/**
 * @brief Stops the server from processing requests.
 */
void WebServer::stop() {
    running = false;
    cv.notify_all();
    if (serverThread.joinable()) {
        serverThread.join();
    }
}
