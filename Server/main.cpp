#include "Server/Net/Messages/BioNet.hpp"

#include <cstdio>
#include <arpa/inet.h>

int main()
{
    int sock = socket(PF_INET, SOCK_DGRAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(1234);

    socklen_t sockAddrInLen = sizeof(sockaddr_in);
    bind(sock, reinterpret_cast<sockaddr*>(&serverAddr), sockAddrInLen);

    while (true)
    {
        static constexpr std::uint16_t s_MaxPacketSize = 960;
        std::byte buff[s_MaxPacketSize];

        sockaddr_in clientAddr;
        int recvLen = recvfrom(sock, buff, s_MaxPacketSize, 0, reinterpret_cast<sockaddr*>(&clientAddr), &sockAddrInLen);
        std::printf("%s:%d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        Net::Core::ConnectionId id;
        id.sock = sock;
        id.destination = clientAddr;

        if (recvLen > 0)
        {
            std::printf(" [%d]\n", recvLen);
            std::printf("--> ");

            for (int i = 0; i < recvLen; ++i)
            {
                char ch = static_cast<char>(buff[i]);
                if (ch < 32 || ch > 126)
                {
                    ch = '?';
                }
                std::printf("%c", ch);
            }

            std::printf("\n    ");

            for (int i = 0; i < recvLen; ++i)
            {
                std::printf("%x ", static_cast<unsigned char>(buff[i]));
            }

            std::printf("\n");

            if (recvLen >= 4)
            {
                if (static_cast<char>(buff[0]) == 'B' &&
                    static_cast<char>(buff[1]) == 'N')
                {
                    Net::Messages::BioNet::ReceiveClientToServerBNxx(&id, buff, recvLen);
                }
                else
                {
                    std::printf("    MALFORMED: Dropping.\n");
                }
            }
            else
            {
                std::printf("    MALFORMED: Dropping.\n");
            }

            std::printf("\n");
        }
        else
        {
            std::printf(" [Empty]\n\n");
        }
    }
}
