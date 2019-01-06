#pragma once

#include "Server/Net/Core/Connection.hpp"
#include "Server/Net/Packets/Packets.hpp"

namespace Net::Messages::BioNet
{
    void ReceiveClientToServerBNxx(const Core::ConnectionId* connection, const std::byte* buff, std::uint16_t len);

    void SendServerToClientBNER(const Core::ConnectionId* connection, const Packets::BioNet::BNES* bnes);
    void SendServerToClientBNXR(const Core::ConnectionId* connection);
    void SendServerToClientBNLR(const Core::ConnectionId* connection, const Packets::BioNet::BNLM* bnlm);
    void SendServerToClientBNDR(const Core::ConnectionId* connection);
    void SendServerToClientBNCR(const Core::ConnectionId* connection);
    void SendServerToClientBNVR(const Core::ConnectionId* connection);
}
