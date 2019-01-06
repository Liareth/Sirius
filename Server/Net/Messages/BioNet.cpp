#include "Server/Net/Messages/BioNet.hpp"

#include <cstring>
#include <cstdio>
#include <vector>

namespace Net::Messages::BioNet
{
    void ReceiveClientToServerBNxx(const Core::ConnectionId* connection, const std::byte* buff, std::uint16_t len)
    {
        char third = static_cast<char>(buff[2]);
        char fourth = static_cast<char>(buff[3]);

        if (third == 'K')
        {
            std::printf("    UNSUPPORTED (Crypto): Dropping.\n");
        }
        else if (third == 'E' && fourth == 'S')
        {
            Packets::BioNet::BNES packet;

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
            Packets::BioNet::BNXI packet;

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
            Packets::BioNet::BNLM packet;

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
            Packets::BioNet::BNDS packet;

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
            Packets::BioNet::BNCS packet;

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
            Packets::BioNet::BNVS packet;

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

    void SendServerToClientBNER(const Core::ConnectionId* connection, const Packets::BioNet::BNES* bnes)
    {
        static constexpr const char* s_ServerName = "My Little Server";

        Packets::BioNet::BNER packet;
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
            Core::SendToConnectionId(connection, buff.data(), bytes);
        }
    };

    void SendServerToClientBNXR(const Core::ConnectionId* connection)
    {
        static constexpr const char* s_ModuleName = "My Little Module";

        Packets::BioNet::BNXR packet;
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
        packet.pvpSetting = 0;\
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
            Core::SendToConnectionId(connection, buff.data(), bytes);
        }
    }

    void SendServerToClientBNLR(const Core::ConnectionId* connection, const Packets::BioNet::BNLM* bnlm)
    {
        Packets::BioNet::BNLR packet;
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
            Core::SendToConnectionId(connection, buff.data(), bytes);
        }
    }

    void SendServerToClientBNDR(const Core::ConnectionId* connection)
    {
        static constexpr const char* s_ServerDescription = "This is an open source implementation of nwserver.";
        static constexpr const char* s_ModuleDescription = "My Little Module is a terrible interesting module!";
        static constexpr const char* s_Build = "0";

        Packets::BioNet::BNDR packet;
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
            Core::SendToConnectionId(connection, buff.data(), bytes);
        }
    }

    void SendServerToClientBNCR(const Core::ConnectionId* connection)
    {
        Packets::BioNet::BNCR packet;
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
            Core::SendToConnectionId(connection, buff.data(), bytes);
        }
    }

    void SendServerToClientBNVR(const Core::ConnectionId* connection)
    {
        Packets::BioNet::BNVR packet;
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
            Core::SendToConnectionId(connection, buff.data(), bytes);
        }
    }
}