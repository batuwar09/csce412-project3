#include "RequestQueue.h"
#include <stdexcept>

/**
 * @brief Adds a request to the queue.
 * @param req The request to add.
 */
void RequestQueue::enqueue(const Request& req) {
    queue.push(req);
}

/**
 * @brief Removes and returns the next request from the queue.
 * @return The next request.
 * @throws std::out_of_range if the queue is empty.
 */
Request RequestQueue::dequeue() {
    if (queue.empty()) {
        throw std::out_of_range("Queue is empty");
    }
    Request req = queue.front();
    queue.pop();
    return req;
}

/**
 * @brief Checks if the queue is empty.
 * @return True if the queue is empty, false otherwise.
 */
bool RequestQueue::isEmpty() const {
    return queue.empty();
}

/**
 * @brief Gets the number of requests in the queue.
 * @return The number of requests.
 */
size_t RequestQueue::size() const {
    return queue.size();
}
