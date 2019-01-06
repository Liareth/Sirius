#pragma once

#include <cstdint>
#include <cstring>

namespace BioNet
{
    struct BNES
    {
        char magic[4];
        std::uint16_t port;
        std::uint8_t enumerateType;

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;
    };

    std::size_t BNES::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&port, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&enumerateType, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNES::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &port, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &enumerateType, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNES::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(std::uint16_t)) +
            (sizeof(std::uint8_t));
    }

    struct BNCS
    {
        char magic[4];
        std::uint16_t port;
        std::uint8_t connectionType;
        std::uint32_t versionNumber;
        std::uint16_t expansion;
        std::uint8_t language;
        std::uint32_t appId;
        std::uint8_t playerNameLength; // max 128
        char* playerName; // sized by playerNameLength
        std::uint8_t cdKeyLength; // max 8
        char* cdKey; // sized by cdKeyLength
        std::uint8_t legacyCdKeyLength; // max 8
        char* legacyCdKey; // sized by legacyCdKeyLength

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;

        BNCS();
        ~BNCS();
    };

    std::size_t BNCS::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&port, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&connectionType, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint32_t) > end) return 0;
        std::memcpy(&versionNumber, buff, sizeof(std::uint32_t));
        buff += sizeof(std::uint32_t);

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&expansion, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&language, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint32_t) > end) return 0;
        std::memcpy(&appId, buff, sizeof(std::uint32_t));
        buff += sizeof(std::uint32_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&playerNameLength, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);
        if (playerNameLength > 128) playerNameLength = 128;

        if (playerNameLength)
        {
            if (buff + sizeof(char) * playerNameLength > end) return 0;
            playerName = new char[playerNameLength];
            std::memcpy(playerName, buff, sizeof(char) * playerNameLength);
            buff += sizeof(char) * playerNameLength;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&cdKeyLength, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);
        if (cdKeyLength > 8) cdKeyLength = 8;

        if (cdKeyLength)
        {
            if (buff + sizeof(char) * cdKeyLength > end) return 0;
            cdKey = new char[cdKeyLength];
            std::memcpy(cdKey, buff, sizeof(char) * cdKeyLength);
            buff += sizeof(char) * cdKeyLength;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&legacyCdKeyLength, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);
        if (legacyCdKeyLength > 8) legacyCdKeyLength = 8;

        if (legacyCdKeyLength)
        {
            if (buff + sizeof(char) * legacyCdKeyLength > end) return 0;
            legacyCdKey = new char[legacyCdKeyLength];
            std::memcpy(legacyCdKey, buff, sizeof(char) * legacyCdKeyLength);
            buff += sizeof(char) * legacyCdKeyLength;
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNCS::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &port, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &connectionType, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint32_t) > end) return 0;
        std::memcpy(buff, &versionNumber, sizeof(std::uint32_t));
        buff += sizeof(std::uint32_t);

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &expansion, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &language, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint32_t) > end) return 0;
        std::memcpy(buff, &appId, sizeof(std::uint32_t));
        buff += sizeof(std::uint32_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        if (playerNameLength > 128) playerNameLength = 128;
        std::memcpy(buff, &playerNameLength, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (playerNameLength)
        {
            if (buff + sizeof(char) * playerNameLength > end) return 0;
            std::memcpy(buff, playerName, sizeof(char) * playerNameLength);
            buff += sizeof(char) * playerNameLength;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        if (cdKeyLength > 8) cdKeyLength = 8;
        std::memcpy(buff, &cdKeyLength, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (cdKeyLength)
        {
            if (buff + sizeof(char) * cdKeyLength > end) return 0;
            std::memcpy(buff, cdKey, sizeof(char) * cdKeyLength);
            buff += sizeof(char) * cdKeyLength;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        if (legacyCdKeyLength > 8) legacyCdKeyLength = 8;
        std::memcpy(buff, &legacyCdKeyLength, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (legacyCdKeyLength)
        {
            if (buff + sizeof(char) * legacyCdKeyLength > end) return 0;
            std::memcpy(buff, legacyCdKey, sizeof(char) * legacyCdKeyLength);
            buff += sizeof(char) * legacyCdKeyLength;
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNCS::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(std::uint16_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint32_t)) +
            (sizeof(std::uint16_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint32_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * playerNameLength) +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * cdKeyLength) +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * legacyCdKeyLength);
    }

    BNCS::BNCS() : playerName(nullptr), cdKey(nullptr), legacyCdKey(nullptr) { }

    BNCS::~BNCS()
    {
        delete[] playerName;
        delete[] cdKey;
        delete[] legacyCdKey;
    }

    struct BNDS
    {
        char magic[4];
        std::uint16_t port;

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;
    };

    std::size_t BNDS::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&port, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNDS::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &port, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNDS::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(std::uint16_t));
    }

    struct BNDR
    {
        char magic[4];
        std::uint16_t port;
        std::uint8_t servDetailsLength;
        char* servDetails; // sized by servDetailsLength
        std::uint8_t modDescLength;
        char* modDesc; // sized by modDescLength
        std::uint8_t buildLength;
        char* build; // sized by buildLength
        std::int16_t gameType;

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;

        BNDR();
        ~BNDR();
    };

    std::size_t BNDR::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&port, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&servDetailsLength, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (servDetailsLength)
        {
            if (buff + sizeof(char) * servDetailsLength > end) return 0;
            servDetails = new char[servDetailsLength];
            std::memcpy(servDetails, buff, sizeof(char) * servDetailsLength);
            buff += sizeof(char) * servDetailsLength;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&modDescLength, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (modDescLength)
        {
            if (buff + sizeof(char) * modDescLength > end) return 0;
            modDesc = new char[modDescLength];
            std::memcpy(modDesc, buff, sizeof(char) * modDescLength);
            buff += sizeof(char) * modDescLength;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&buildLength, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buildLength)
        {
            if (buff + sizeof(char) * buildLength > end) return 0;
            build = new char[buildLength];
            std::memcpy(build, buff, sizeof(char) * buildLength);
            buff += sizeof(char) * buildLength;
        }

        if (buff + sizeof(std::int16_t) > end) return 0;
        std::memcpy(&gameType, buff, sizeof(std::int16_t));
        buff += sizeof(std::int16_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNDR::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &port, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &servDetailsLength, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (servDetailsLength)
        {
            if (buff + sizeof(char) * servDetailsLength > end) return 0;
            std::memcpy(buff, servDetails, sizeof(char) * servDetailsLength);
            buff += sizeof(char) * servDetailsLength;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &modDescLength, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (modDescLength)
        {
            if (buff + sizeof(char) * modDescLength > end) return 0;
            std::memcpy(buff, modDesc, sizeof(char) * modDescLength);
            buff += sizeof(char) * modDescLength;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &buildLength, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buildLength)
        {
            if (buff + sizeof(char) * buildLength > end) return 0;
            std::memcpy(buff, build, sizeof(char) * buildLength);
            buff += sizeof(char) * buildLength;
        }

        if (buff + sizeof(std::int16_t) > end) return 0;
        std::memcpy(buff, &gameType, sizeof(std::int16_t));
        buff += sizeof(std::int16_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNDR::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(std::uint16_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * servDetailsLength) +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * modDescLength) +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * buildLength) +
            (sizeof(std::int16_t));
    }

    BNDR::BNDR() : servDetails(nullptr), modDesc(nullptr), build(nullptr) { }

    BNDR::~BNDR()
    {
        delete[] servDetails;
        delete[] modDesc;
        delete[] build;
    }

    struct BNVS_CdKey
    {
        std::uint8_t cdKeyLength;
        char* cdKey; // sized by cdKeyLength

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;

        BNVS_CdKey();
        ~BNVS_CdKey();
    };

    std::size_t BNVS_CdKey::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&cdKeyLength, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (cdKeyLength)
        {
            if (buff + sizeof(char) * cdKeyLength > end) return 0;
            cdKey = new char[cdKeyLength];
            std::memcpy(cdKey, buff, sizeof(char) * cdKeyLength);
            buff += sizeof(char) * cdKeyLength;
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNVS_CdKey::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &cdKeyLength, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (cdKeyLength)
        {
            if (buff + sizeof(char) * cdKeyLength > end) return 0;
            std::memcpy(buff, cdKey, sizeof(char) * cdKeyLength);
            buff += sizeof(char) * cdKeyLength;
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNVS_CdKey::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * cdKeyLength);
    }

    BNVS_CdKey::BNVS_CdKey() : cdKey(nullptr) { }

    BNVS_CdKey::~BNVS_CdKey()
    {
        delete[] cdKey;
    }

    struct BNVS
    {
        char magic[4];
        char status;
        std::uint8_t numCdKeys;
        BNVS_CdKey* cdKeys; // sized by numCdKeys
        std::uint8_t msPasswordLength;
        char* msPassword; // sized by msPasswordLength
        std::uint8_t passwordLength; // only if (status=='P')
        char* password; // only if (status=='P'), sized by passwordLength

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;

        BNVS();
        ~BNVS();
    };

    std::size_t BNVS::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(char) > end) return 0;
        std::memcpy(&status, buff, sizeof(char));
        buff += sizeof(char);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&numCdKeys, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (numCdKeys)
        {
            cdKeys = new BNVS_CdKey[numCdKeys];
            for (std::size_t i = 0; i < numCdKeys; ++i)
            {
                if (std::size_t bytes = cdKeys[i].Read(buff, static_cast<std::size_t>(end - buff)); bytes) buff += bytes;
                else return 0;
            }
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&msPasswordLength, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (msPasswordLength)
        {
            if (buff + sizeof(char) * msPasswordLength > end) return 0;
            msPassword = new char[msPasswordLength];
            std::memcpy(msPassword, buff, sizeof(char) * msPasswordLength);
            buff += sizeof(char) * msPasswordLength;
        }

        if (status=='P')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(&passwordLength, buff, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (status=='P')
        {
            if (passwordLength)
            {
                if (buff + sizeof(char) * passwordLength > end) return 0;
                password = new char[passwordLength];
                std::memcpy(password, buff, sizeof(char) * passwordLength);
                buff += sizeof(char) * passwordLength;
            }
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNVS::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(char) > end) return 0;
        std::memcpy(buff, &status, sizeof(char));
        buff += sizeof(char);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &numCdKeys, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (numCdKeys)
        {
            cdKeys = new BNVS_CdKey[numCdKeys];
            for (std::size_t i = 0; i < numCdKeys; ++i)
            {
                if (std::size_t bytes = cdKeys[i].Write(buff, static_cast<std::size_t>(end - buff)); bytes) buff += bytes;
                else return 0;
            }
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &msPasswordLength, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (msPasswordLength)
        {
            if (buff + sizeof(char) * msPasswordLength > end) return 0;
            std::memcpy(buff, msPassword, sizeof(char) * msPasswordLength);
            buff += sizeof(char) * msPasswordLength;
        }

        if (status=='P')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(buff, &passwordLength, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (status=='P')
        {
            if (passwordLength)
            {
                if (buff + sizeof(char) * passwordLength > end) return 0;
                std::memcpy(buff, password, sizeof(char) * passwordLength);
                buff += sizeof(char) * passwordLength;
            }
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNVS::Size() const
    {
        std::size_t structSize = 0;
        for (std::size_t i = 0; i < numCdKeys; ++i) { structSize += cdKeys[i].Size(); }
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(char)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * msPasswordLength) +
            ((status=='P') ? (sizeof(std::uint8_t)) : (0)) +
            ((status=='P') ? (sizeof(char) * passwordLength) : (0));
    }

    BNVS::BNVS() : cdKeys(nullptr), msPassword(nullptr), password(nullptr) { }

    BNVS::~BNVS()
    {
        delete[] cdKeys;
        delete[] msPassword;
        delete[] password;
    }

    struct BNCR
    {
        char magic[4];
        std::uint16_t port;
        char response;
        std::uint8_t rejectReason; // only if (response=='R')
        std::uint8_t pwChallengeLength; // only if (response=='P')
        char* pwChallenge; // only if (response=='P'), sized by pwChallengeLength
        std::uint8_t keyChallengeLength; // only if (response!='R')
        char* keyChallenge; // only if (response!='R'), sized by keyChallengeLength
        std::uint8_t msChallengeLength; // only if (response!='R')
        char* msChallenge; // only if (response!='R'), sized by msChallengeLength

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;

        BNCR();
        ~BNCR();
    };

    std::size_t BNCR::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&port, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(char) > end) return 0;
        std::memcpy(&response, buff, sizeof(char));
        buff += sizeof(char);

        if (response=='R')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(&rejectReason, buff, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (response=='P')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(&pwChallengeLength, buff, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (response=='P')
        {
            if (pwChallengeLength)
            {
                if (buff + sizeof(char) * pwChallengeLength > end) return 0;
                pwChallenge = new char[pwChallengeLength];
                std::memcpy(pwChallenge, buff, sizeof(char) * pwChallengeLength);
                buff += sizeof(char) * pwChallengeLength;
            }
        }

        if (response!='R')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(&keyChallengeLength, buff, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (response!='R')
        {
            if (keyChallengeLength)
            {
                if (buff + sizeof(char) * keyChallengeLength > end) return 0;
                keyChallenge = new char[keyChallengeLength];
                std::memcpy(keyChallenge, buff, sizeof(char) * keyChallengeLength);
                buff += sizeof(char) * keyChallengeLength;
            }
        }

        if (response!='R')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(&msChallengeLength, buff, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (response!='R')
        {
            if (msChallengeLength)
            {
                if (buff + sizeof(char) * msChallengeLength > end) return 0;
                msChallenge = new char[msChallengeLength];
                std::memcpy(msChallenge, buff, sizeof(char) * msChallengeLength);
                buff += sizeof(char) * msChallengeLength;
            }
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNCR::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &port, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(char) > end) return 0;
        std::memcpy(buff, &response, sizeof(char));
        buff += sizeof(char);

        if (response=='R')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(buff, &rejectReason, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (response=='P')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(buff, &pwChallengeLength, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (response=='P')
        {
            if (pwChallengeLength)
            {
                if (buff + sizeof(char) * pwChallengeLength > end) return 0;
                std::memcpy(buff, pwChallenge, sizeof(char) * pwChallengeLength);
                buff += sizeof(char) * pwChallengeLength;
            }
        }

        if (response!='R')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(buff, &keyChallengeLength, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (response!='R')
        {
            if (keyChallengeLength)
            {
                if (buff + sizeof(char) * keyChallengeLength > end) return 0;
                std::memcpy(buff, keyChallenge, sizeof(char) * keyChallengeLength);
                buff += sizeof(char) * keyChallengeLength;
            }
        }

        if (response!='R')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(buff, &msChallengeLength, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (response!='R')
        {
            if (msChallengeLength)
            {
                if (buff + sizeof(char) * msChallengeLength > end) return 0;
                std::memcpy(buff, msChallenge, sizeof(char) * msChallengeLength);
                buff += sizeof(char) * msChallengeLength;
            }
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNCR::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(std::uint16_t)) +
            (sizeof(char)) +
            ((response=='R') ? (sizeof(std::uint8_t)) : (0)) +
            ((response=='P') ? (sizeof(std::uint8_t)) : (0)) +
            ((response=='P') ? (sizeof(char) * pwChallengeLength) : (0)) +
            ((response!='R') ? (sizeof(std::uint8_t)) : (0)) +
            ((response!='R') ? (sizeof(char) * keyChallengeLength) : (0)) +
            ((response!='R') ? (sizeof(std::uint8_t)) : (0)) +
            ((response!='R') ? (sizeof(char) * msChallengeLength) : (0));
    }

    BNCR::BNCR() : pwChallenge(nullptr), keyChallenge(nullptr), msChallenge(nullptr) { }

    BNCR::~BNCR()
    {
        delete[] pwChallenge;
        delete[] keyChallenge;
        delete[] msChallenge;
    }

    struct BNLR
    {
        char magic[4];
        std::uint16_t port;
        std::uint8_t messageId;
        std::uint32_t sessionId;

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;
    };

    std::size_t BNLR::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&port, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&messageId, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint32_t) > end) return 0;
        std::memcpy(&sessionId, buff, sizeof(std::uint32_t));
        buff += sizeof(std::uint32_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNLR::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &port, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &messageId, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint32_t) > end) return 0;
        std::memcpy(buff, &sessionId, sizeof(std::uint32_t));
        buff += sizeof(std::uint32_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNLR::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(std::uint16_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint32_t));
    }

    struct BNXR
    {
        char magic[4];
        std::uint16_t port;
        std::uint8_t protocolVersion;
        std::uint8_t passworded;
        std::uint8_t minLevel;
        std::uint8_t maxLevel;
        std::uint8_t players;
        std::uint8_t maxPlayers;
        std::uint8_t localVault;
        std::uint8_t pvpSetting;
        std::uint8_t pauseAndPlay;
        std::uint8_t onePartyOnly;
        std::uint8_t enforceLegalChar;
        std::uint8_t itemLevelRestrict;
        std::uint8_t expansion;
        std::uint8_t moduleNameLen;
        char* moduleName; // sized by moduleNameLen
        std::uint8_t nwsyncVersion;
        std::uint8_t nwsyncUrlLength;
        char* nwsyncUrl; // sized by nwsyncUrlLength
        std::uint8_t nwsyncHashCount;
        std::uint8_t nwsyncHashFlags; // only if (nwsyncHashCount>0)
        std::uint8_t nwsyncHashLength; // only if (nwsyncHashCount>0)
        char* nwsyncHash; // only if (nwsyncHashCount>0), sized by nwsyncHashLength

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;

        BNXR();
        ~BNXR();
    };

    std::size_t BNXR::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&port, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&protocolVersion, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&passworded, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&minLevel, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&maxLevel, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&players, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&maxPlayers, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&localVault, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&pvpSetting, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&pauseAndPlay, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&onePartyOnly, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&enforceLegalChar, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&itemLevelRestrict, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&expansion, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&moduleNameLen, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (moduleNameLen)
        {
            if (buff + sizeof(char) * moduleNameLen > end) return 0;
            moduleName = new char[moduleNameLen];
            std::memcpy(moduleName, buff, sizeof(char) * moduleNameLen);
            buff += sizeof(char) * moduleNameLen;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&nwsyncVersion, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&nwsyncUrlLength, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (nwsyncUrlLength)
        {
            if (buff + sizeof(char) * nwsyncUrlLength > end) return 0;
            nwsyncUrl = new char[nwsyncUrlLength];
            std::memcpy(nwsyncUrl, buff, sizeof(char) * nwsyncUrlLength);
            buff += sizeof(char) * nwsyncUrlLength;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&nwsyncHashCount, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (nwsyncHashCount>0)
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(&nwsyncHashFlags, buff, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (nwsyncHashCount>0)
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(&nwsyncHashLength, buff, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (nwsyncHashCount>0)
        {
            if (nwsyncHashLength)
            {
                if (buff + sizeof(char) * nwsyncHashLength > end) return 0;
                nwsyncHash = new char[nwsyncHashLength];
                std::memcpy(nwsyncHash, buff, sizeof(char) * nwsyncHashLength);
                buff += sizeof(char) * nwsyncHashLength;
            }
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNXR::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &port, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &protocolVersion, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &passworded, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &minLevel, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &maxLevel, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &players, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &maxPlayers, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &localVault, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &pvpSetting, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &pauseAndPlay, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &onePartyOnly, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &enforceLegalChar, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &itemLevelRestrict, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &expansion, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &moduleNameLen, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (moduleNameLen)
        {
            if (buff + sizeof(char) * moduleNameLen > end) return 0;
            std::memcpy(buff, moduleName, sizeof(char) * moduleNameLen);
            buff += sizeof(char) * moduleNameLen;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &nwsyncVersion, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &nwsyncUrlLength, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (nwsyncUrlLength)
        {
            if (buff + sizeof(char) * nwsyncUrlLength > end) return 0;
            std::memcpy(buff, nwsyncUrl, sizeof(char) * nwsyncUrlLength);
            buff += sizeof(char) * nwsyncUrlLength;
        }

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &nwsyncHashCount, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (nwsyncHashCount>0)
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(buff, &nwsyncHashFlags, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (nwsyncHashCount>0)
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(buff, &nwsyncHashLength, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (nwsyncHashCount>0)
        {
            if (nwsyncHashLength)
            {
                if (buff + sizeof(char) * nwsyncHashLength > end) return 0;
                std::memcpy(buff, nwsyncHash, sizeof(char) * nwsyncHashLength);
                buff += sizeof(char) * nwsyncHashLength;
            }
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNXR::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(std::uint16_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * moduleNameLen) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * nwsyncUrlLength) +
            (sizeof(std::uint8_t)) +
            ((nwsyncHashCount>0) ? (sizeof(std::uint8_t)) : (0)) +
            ((nwsyncHashCount>0) ? (sizeof(std::uint8_t)) : (0)) +
            ((nwsyncHashCount>0) ? (sizeof(char) * nwsyncHashLength) : (0));
    }

    BNXR::BNXR() : moduleName(nullptr), nwsyncUrl(nullptr), nwsyncHash(nullptr) { }

    BNXR::~BNXR()
    {
        delete[] moduleName;
        delete[] nwsyncUrl;
        delete[] nwsyncHash;
    }

    struct BNXI
    {
        char magic[4];
        std::uint16_t port;

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;
    };

    std::size_t BNXI::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&port, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNXI::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &port, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNXI::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(std::uint16_t));
    }

    struct BNLM
    {
        char magic[4];
        std::uint16_t port;
        std::uint8_t messageId;
        std::uint32_t sessionId;

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;
    };

    std::size_t BNLM::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&port, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&messageId, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint32_t) > end) return 0;
        std::memcpy(&sessionId, buff, sizeof(std::uint32_t));
        buff += sizeof(std::uint32_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNLM::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &port, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &messageId, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint32_t) > end) return 0;
        std::memcpy(buff, &sessionId, sizeof(std::uint32_t));
        buff += sizeof(std::uint32_t);

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNLM::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(std::uint16_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint32_t));
    }

    struct BNVR
    {
        char magic[4];
        char response;
        std::uint8_t rejectReason; // only if (response=='R')
        std::uint32_t applicationId; // only if (response=='A')

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;
    };

    std::size_t BNVR::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(char) > end) return 0;
        std::memcpy(&response, buff, sizeof(char));
        buff += sizeof(char);

        if (response=='R')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(&rejectReason, buff, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (response=='A')
        {
            if (buff + sizeof(std::uint32_t) > end) return 0;
            std::memcpy(&applicationId, buff, sizeof(std::uint32_t));
            buff += sizeof(std::uint32_t);
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNVR::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(char) > end) return 0;
        std::memcpy(buff, &response, sizeof(char));
        buff += sizeof(char);

        if (response=='R')
        {
            if (buff + sizeof(std::uint8_t) > end) return 0;
            std::memcpy(buff, &rejectReason, sizeof(std::uint8_t));
            buff += sizeof(std::uint8_t);
        }

        if (response=='A')
        {
            if (buff + sizeof(std::uint32_t) > end) return 0;
            std::memcpy(buff, &applicationId, sizeof(std::uint32_t));
            buff += sizeof(std::uint32_t);
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNVR::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(char)) +
            ((response=='R') ? (sizeof(std::uint8_t)) : (0)) +
            ((response=='A') ? (sizeof(std::uint32_t)) : (0));
    }

    struct BNER
    {
        char magic[4];
        char protocol;
        std::uint16_t port;
        std::uint8_t enumerateType;
        std::uint8_t serverNameLen;
        char* serverName; // sized by serverNameLen

        std::size_t Read(const std::byte* buff, std::size_t maxLen);
        std::size_t Write(std::byte* buff, std::size_t maxLen);
        std::size_t Size() const;

        BNER();
        ~BNER();
    };

    std::size_t BNER::Read(const std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(magic, buff, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(char) > end) return 0;
        std::memcpy(&protocol, buff, sizeof(char));
        buff += sizeof(char);

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(&port, buff, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&enumerateType, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(&serverNameLen, buff, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (serverNameLen)
        {
            if (buff + sizeof(char) * serverNameLen > end) return 0;
            serverName = new char[serverNameLen];
            std::memcpy(serverName, buff, sizeof(char) * serverNameLen);
            buff += sizeof(char) * serverNameLen;
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNER::Write(std::byte* buff, std::size_t maxLen)
    {
        const std::byte* start = buff;
        const std::byte* end = buff + maxLen;

        if (buff + sizeof(char) * 4 > end) return 0;
        std::memcpy(buff, magic, sizeof(char) * 4);
        buff += sizeof(char) * 4;

        if (buff + sizeof(char) > end) return 0;
        std::memcpy(buff, &protocol, sizeof(char));
        buff += sizeof(char);

        if (buff + sizeof(std::uint16_t) > end) return 0;
        std::memcpy(buff, &port, sizeof(std::uint16_t));
        buff += sizeof(std::uint16_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &enumerateType, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (buff + sizeof(std::uint8_t) > end) return 0;
        std::memcpy(buff, &serverNameLen, sizeof(std::uint8_t));
        buff += sizeof(std::uint8_t);

        if (serverNameLen)
        {
            if (buff + sizeof(char) * serverNameLen > end) return 0;
            std::memcpy(buff, serverName, sizeof(char) * serverNameLen);
            buff += sizeof(char) * serverNameLen;
        }

        return static_cast<std::size_t>(buff - start);
    }

    std::size_t BNER::Size() const
    {
        std::size_t structSize = 0;
        return structSize +
            (sizeof(char) * 4) +
            (sizeof(char)) +
            (sizeof(std::uint16_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(std::uint8_t)) +
            (sizeof(char) * serverNameLen);
    }

    BNER::BNER() : serverName(nullptr) { }

    BNER::~BNER()
    {
        delete[] serverName;
    }
}
