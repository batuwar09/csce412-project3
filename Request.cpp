#include "Request.h"

/**
 * @brief Constructs a new Request object.
 * @param ipIn Incoming IP address.
 * @param ipOut Outgoing IP address.
 * @param processingTime Time required to process the request in milliseconds.
 */
Request::Request(const std::string& ipIn, const std::string& ipOut, int processingTime)
    : ipIn(ipIn), ipOut(ipOut), processingTime(processingTime) {}

/**
 * @brief Gets the incoming IP address.
 * @return Incoming IP address as a string.
 */
std::string Request::getIpIn() const {
    return ipIn;
}

/**
 * @brief Gets the outgoing IP address.
 * @return Outgoing IP address as a string.
 */
std::string Request::getIpOut() const {
    return ipOut;
}

/**
 * @brief Gets the processing time.
 * @return Processing time in milliseconds.
 */
int Request::getProcessingTime() const {
    return processingTime;
}
