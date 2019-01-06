#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <byteswap.h>

namespace Endian
{
    template <typename T>
    T Swap(T val)
    {
        return val;
    }

#define LITTLE_ENDIAN_HOST 1
#define BIG_ENDIAN_HOST 0 // TODO - I think NWN uses little endian network order?

#if BIG_ENDIAN_HOST

    template <>
    std::int16_t Swap(std::int16_t val)
    {
        return bswap_16(val);
    }

    template <>
    std::uint16_t Swap(std::uint16_t val)
    {
        return bswap_16(val);
    }

    template <>
    std::int32_t Swap(std::int32_t val)
    {
        return bswap_32(val);
    }

    template <>
    std::uint32_t Swap(std::uint32_t val)
    {
        return bswap_32(val);
    }

    template <>
    std::int64_t Swap(std::int64_t val)
    {
        return bswap_64(val);
    }

    template <>
    std::uint64_t Swap(std::uint64_t val)
    {
        return bswap_64(val);
    }
#endif

}

#include "Packets/Packets.hpp"

struct ConnectionId
{
    int sock;
    sockaddr_in destination;
};

void PrintPacketChar(char ch)
{
    if (ch < 32 || ch > 126)
    {
        ch = '?';
    }

    std::printf("%c", ch);
}

void Send(const ConnectionId* connection, const std::byte* buff, std::size_t len)
{
    sendto(connection->sock, buff, len, 0,
        reinterpret_cast<const sockaddr*>(&connection->destination),
        sizeof(connection->destination));

    std::printf("<-- ");

    for (std::size_t i = 0; i < len; ++i)
    {
        PrintPacketChar(static_cast<char>(buff[i]));
    }

    std::printf("\n    ");

    for (std::size_t i = 0; i < len; ++i)
    {
        std::printf("%x ", static_cast<unsigned char>(buff[i]));
    }

    std::printf("\n");
}

void ReceiveClientToServerBNxx(const ConnectionId* connection, const std::byte* buff, std::uint16_t len);

void SendServerToClientBNER(const ConnectionId* connection, const BioNet::BNES* bnes);
void SendServerToClientBNXR(const ConnectionId* connection);
void SendServerToClientBNLR(const ConnectionId* connection, const BioNet::BNLM* bnlm);
void SendServerToClientBNDR(const ConnectionId* connection);
void SendServerToClientBNCR(const ConnectionId* connection);
void SendServerToClientBNVR(const ConnectionId* connection);

void ReceiveClientToServerBNxx(const ConnectionId* connection, const std::byte* buff, std::uint16_t len)
{
    char third = static_cast<char>(buff[2]);
    char fourth = static_cast<char>(buff[3]);

    if (third == 'K')
    {
        std::printf("    UNSUPPORTED (Crypto): Dropping.\n");
    }
    else if (third == 'E' && fourth == 'S')
    {
        BioNet::BNES packet;

        if (packet.Read(buff, len))
        {
            SendServerToClientBNER(connection, &packet);
        }
        else
        {
            std::printf("    MALFORMED: Dropping.\n");
        }
    }
    else if (third == 'X' && fourth == 'I')
    {
        BioNet::BNXI packet;

        if (packet.Read(buff, len))
        {
            SendServerToClientBNXR(connection);
        }
        else
        {
            std::printf("    MALFORMED: Dropping.\n");
        }
    }
    else if (third == 'L' && fourth == 'M')
    {
        BioNet::BNLM packet;

        if (packet.Read(buff, len))
        {
            SendServerToClientBNLR(connection, &packet);
        }
        else
        {
            std::printf("    MALFORMED: Dropping.\n");
        }
    }
    else if (third == 'D' && fourth == 'S')
    {
        BioNet::BNDS packet;

        if (packet.Read(buff, len))
        {
            SendServerToClientBNDR(connection);
        }
        else
        {
            std::printf("    MALFORMED: Dropping.\n");
        }
    }
    else if (third == 'C' && fourth == 'S')
    {
        BioNet::BNCS packet;

        if (packet.Read(buff, len))
        {
            // This is where we would establish a connection.
            SendServerToClientBNCR(connection);
        }
        else
        {
            std::printf("    MALFORMED: Dropping.\n");
        }
    }
    else if (third == 'V' && fourth == 'S')
    {
        BioNet::BNVS packet;

        if (packet.Read(buff, len))
        {
            // This is where we would coordinate with the master server.
            SendServerToClientBNVR(connection);
        }
        else
        {
            std::printf("    MALFORMED: Dropping.\n");
        }
    }
    else if (third == 'D' && fourth == 'M')
    {
        std::printf("    UNSUPPORTED (Disconnect Me): Dropping.\n");
    }
    else
    {
        std::printf("    MALFORMED: Dropping.\n");
    }
}

void SendServerToClientBNER(const ConnectionId* connection, const BioNet::BNES* bnes)
{
    static constexpr const char* s_ServerName = "My Little Server";

    BioNet::BNER packet;
    packet.magic[0] = 'B';
    packet.magic[1] = 'N';
    packet.magic[2] = 'E';
    packet.magic[3] = 'R';
    packet.protocol = 'U';
    packet.port = 1234;
    packet.enumerateType = bnes->enumerateType;
    packet.serverNameLen = std::strlen(s_ServerName);
    packet.serverName = strdup(s_ServerName);

    std::vector<std::byte> buff;
    buff.resize(packet.Size());

    if (std::size_t bytes = packet.Write(buff.data(), buff.size()); bytes)
    {
        Send(connection, buff.data(), bytes);
    }
};

void SendServerToClientBNXR(const ConnectionId* connection)
{
    static constexpr const char* s_ModuleName = "My Little Module";

    BioNet::BNXR packet;
    packet.magic[0] = 'B';
    packet.magic[1] = 'N';
    packet.magic[2] = 'X';
    packet.magic[3] = 'R';
    packet.port = 1234;
    packet.protocolVersion = 0xFD;
    packet.passworded = 0;
    packet.minLevel = 1;
    packet.maxLevel = 80;
    packet.players = 0;
    packet.maxPlayers = 69;
    packet.localVault = 0;
    packet.pvpSetting = 0;
    packet.pauseAndPlay = 0;
    packet.onePartyOnly = 0;
    packet.enforceLegalChar = 0;
    packet.itemLevelRestrict = 0;
    packet.expansion = 0;
    packet.moduleNameLen = std::strlen(s_ModuleName);
    packet.moduleName = strdup(s_ModuleName);
    packet.nwsyncVersion = 1;
    packet.nwsyncUrlLength = 0;
    packet.nwsyncHashCount = 0;
    packet.nwsyncHashFlags = 0;
    packet.nwsyncHashLength = 0;

    std::vector<std::byte> buff;
    buff.resize(packet.Size());

    if (std::size_t bytes = packet.Write(buff.data(), buff.size()); bytes)
    {
        Send(connection, buff.data(), bytes);
    }
}

void SendServerToClientBNLR(const ConnectionId* connection, const BioNet::BNLM* bnlm)
{
    BioNet::BNLR packet;
    packet.magic[0] = 'B';
    packet.magic[1] = 'N';
    packet.magic[2] = 'L';
    packet.magic[3] = 'R';
    packet.port = 1234;
    packet.messageId = bnlm->messageId;
    packet.sessionId = bnlm->sessionId;

    std::vector<std::byte> buff;
    buff.resize(packet.Size());

    if (std::size_t bytes = packet.Write(buff.data(), buff.size()); bytes)
    {
        Send(connection, buff.data(), bytes);
    }
}

void SendServerToClientBNDR(const ConnectionId* connection)
{
    static constexpr const char* s_ServerDescription = "This is an open source implementation of nwserver.";
    static constexpr const char* s_ModuleDescription = "My Little Module is a terrible interesting module!";
    static constexpr const char* s_Build = "0";

    BioNet::BNDR packet;
    packet.magic[0] = 'B';
    packet.magic[1] = 'N';
    packet.magic[2] = 'D';
    packet.magic[3] = 'R';
    packet.port = 1234;
    packet.servDetailsLength = std::strlen(s_ServerDescription);
    packet.servDetails = strdup(s_ServerDescription);
    packet.modDescLength = std::strlen(s_ModuleDescription);
    packet.modDesc = strdup(s_ModuleDescription);
    packet.buildLength = std::strlen(s_Build);
    packet.build = strdup(s_Build);
    packet.gameType = 2;

    std::vector<std::byte> buff;
    buff.resize(packet.Size());

    if (std::size_t bytes = packet.Write(buff.data(), buff.size()); bytes)
    {
        Send(connection, buff.data(), bytes);
    }
}

void SendServerToClientBNCR(const ConnectionId* connection)
{
    BioNet::BNCR packet;
    packet.magic[0] = 'B';
    packet.magic[1] = 'N';
    packet.magic[2] = 'C';
    packet.magic[3] = 'R';
    packet.port = 1234;

    static constexpr bool s_Reject = false;

    if constexpr (s_Reject)
    {
        packet.response = 'R';
        packet.rejectReason = 11;
    }
    else
    {
        static constexpr const char* s_KeyChallenge = "KEY_CHALLENGE";
        static constexpr const char* s_MsChallenge = "MS_CHALLENGE";

        packet.response = 'V';
        packet.keyChallengeLength = std::strlen(s_KeyChallenge);
        packet.keyChallenge = strdup(s_KeyChallenge);
        packet.msChallengeLength = std::strlen(s_MsChallenge);
        packet.msChallenge = strdup(s_MsChallenge);
    }

    std::vector<std::byte> buff;
    buff.resize(packet.Size());

    if (std::size_t bytes = packet.Write(buff.data(), buff.size()); bytes)
    {
        Send(connection, buff.data(), bytes);
    }
}

void SendServerToClientBNVR(const ConnectionId* connection)
{
    BioNet::BNVR packet;
    packet.magic[0] = 'B';
    packet.magic[1] = 'N';
    packet.magic[2] = 'V';
    packet.magic[3] = 'R';

    static constexpr bool s_Reject = false;

    if constexpr (s_Reject)
    {
        packet.response = 'R';
        packet.rejectReason = 11;
    }
    else
    {
        packet.response = 'A';
        packet.applicationId = 52;
    }

    std::vector<std::byte> buff;
    buff.resize(packet.Size());

    if (std::size_t bytes = packet.Write(buff.data(), buff.size()); bytes)
    {
        Send(connection, buff.data(), bytes);
    }
}

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

        ConnectionId id;
        id.sock = sock;
        id.destination = clientAddr;

        if (recvLen > 0)
        {
            std::printf(" [%d]\n", recvLen);
            std::printf("--> ");

            for (int i = 0; i < recvLen; ++i)
            {
                PrintPacketChar(static_cast<char>(buff[i]));
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
                    ReceiveClientToServerBNxx(&id, buff, recvLen);
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
