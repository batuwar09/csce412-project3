#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @class Request
 * @brief Represents a network request with IP addresses and processing time.
 */
class Request {
public:
    /**
     * @brief Constructs a new Request object.
     * @param ipIn Incoming IP address.
     * @param ipOut Outgoing IP address.
     * @param processingTime Time required to process the request in milliseconds.
     */
    Request(const std::string& ipIn, const std::string& ipOut, int processingTime);

    /**
     * @brief Gets the incoming IP address.
     * @return Incoming IP address as a string.
     */
    std::string getIpIn() const;

    /**
     * @brief Gets the outgoing IP address.
     * @return Outgoing IP address as a string.
     */
    std::string getIpOut() const;

    /**
     * @brief Gets the processing time.
     * @return Processing time in milliseconds.
     */
    int getProcessingTime() const;

private:
    std::string ipIn; ///< Incoming IP address.
    std::string ipOut; ///< Outgoing IP address.
    int processingTime; ///< Time required to process the request in milliseconds.
};

#endif // REQUEST_H
