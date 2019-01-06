#include "Server/Net/Core/Connection.hpp"

#include <cstdio>

namespace Net::Core
{
    void SendToConnectionId(const ConnectionId* connection, const std::byte* buff, std::size_t len)
    {
        sendto(connection->sock, buff, len, 0,
            reinterpret_cast<const sockaddr*>(&connection->destination),
            sizeof(connection->destination));

        std::printf("<-- ");

        for (std::size_t i = 0; i < len; ++i)
        {
            char ch = static_cast<char>(buff[i]);
            if (ch < 32 || ch > 126)
            {
                ch = '?';
            }
            std::printf("%c", ch);
        }

        std::printf("\n    ");

        for (std::size_t i = 0; i < len; ++i)
        {
            std::printf("%x ", static_cast<unsigned char>(buff[i]));
        }

        std::printf("\n");
    }
}

