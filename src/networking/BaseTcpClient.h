/**
 * @file BaseTcpClient.h
 *
 * @date Apr 13, 2017
 * @author edward
 */

#pragma once

#include <map>
#include <vector>

#include "TcpAddress.h"

namespace pddm {
namespace networking {

/**
 * Contains the common implementation details of TcpNetworkClient and
 * TcpUtilityClient, such as the epoll monitoring loop for receiving incoming
 * messages. This class should not be used polymorphically and uses the CRTP to
 * avoid virtual function dispatch when handing off to the subclass's
 * implementation of receive_message().
 */
template<typename Impl>
class BaseTcpClient {
    private:
        Impl* impl_this;
        void require_receive_message(const std::vector<char>& message_bytes) { impl_this->receive_message(message_bytes); }
    protected:
        /** Maps Meter IDs to IP address/port pairs. */
        std::map<int, TcpAddress> id_to_ip_map;
    private:
        /** File descriptor for monitoring incoming connections */
        int epoll_fd;
        int server_socket_fd;
    protected:
        BaseTcpClient(Impl* subclass_this, const TcpAddress& my_address, const std::map<int, TcpAddress>& meter_ips_by_id);
        virtual ~BaseTcpClient();
    public:
        void monitor_incoming_messages();
};

} /* namespace networking */
} /* namespace pddm */

#include "BaseTcpClient_impl.h"
