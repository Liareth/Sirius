#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

#include <cstddef>

namespace Net::Core
{
    struct ConnectionId // This is temporary - until we set up connection internals.
    {
        int sock;
        sockaddr_in destination;
    };

    void SendToConnectionId(const ConnectionId* connection, const std::byte* buff, std::size_t len);
}
