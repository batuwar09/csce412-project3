#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <queue>
#include "Request.h"

/**
 * @class RequestQueue
 * @brief Manages a queue of network requests.
 */
class RequestQueue {
public:
    /**
     * @brief Adds a request to the queue.
     * @param req The request to add.
     */
    void enqueue(const Request& req);

    /**
     * @brief Removes and returns the next request from the queue.
     * @return The next request.
     * @throws std::out_of_range if the queue is empty.
     */
    Request dequeue();

    /**
     * @brief Checks if the queue is empty.
     * @return True if the queue is empty, false otherwise.
     */
    bool isEmpty() const;

    /**
     * @brief Gets the number of requests in the queue.
     * @return The number of requests.
     */
    size_t size() const;

private:
    std::queue<Request> queue; ///< The queue of requests.
};

#endif // REQUESTQUEUE_H
