// This tool scans the 1st argument folder and opens every .packet file.
// It outputs the results to stdout.
// Check out the packet files in Server/Packets/Source for format.

#include <algorithm>
#include <cstdio>
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef _WIN32
    #include "Windows.h"
#else
    #include <dirent.h>
    #include <unistd.h>
    #include <sys/stat.h>
#endif

struct FileData
{
    uint32_t m_modified;
    uint32_t m_size;
};

using FileName = std::string;
using FileMap = std::unordered_map<FileName, FileData>;

struct File
{
    FileName m_name;
    FileData m_data;
};

std::string CombinePaths(const std::string& pathOne, const std::string& pathTwo)
{
#ifdef _WIN32
    const std::string separator = "\\";
#else
    const std::string separator = "/";
#endif

    const bool hasSeparator = std::equal(separator.begin(), separator.end(), pathOne.rbegin());
    return hasSeparator ? pathOne + pathTwo : pathOne + separator + pathTwo;
}

FileMap GetAllFilesInDirectory(std::string directory)
{
    FileMap files;

#ifdef _WIN32
    // Windows needs \* appended to the search.
    directory.append("\\*");

    HANDLE findHandle;
    WIN32_FIND_DATA findData;

    findHandle = FindFirstFileA(directory.c_str(), &findData);

    if (findHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                FileName name = findData.cFileName;

                FileData data
                {
                    findData.ftLastWriteTime.dwLowDateTime, findData.nFileSizeLow
                };

                files.insert(std::make_pair(std::move(name), std::move(data)));
            }
        } while (FindNextFileA(findHandle, &findData));
    }

    FindClose(findHandle);
#else
    DIR* dir = opendir(directory.c_str());

    if (dir != nullptr)
    {
        dirent* directoryEntry = readdir(dir);

        while (directoryEntry != nullptr)
        {
            if (directoryEntry->d_type == DT_UNKNOWN || directoryEntry->d_type == DT_REG)
            {
                FileName name = directoryEntry->d_name;

                const std::string fullFilePath = CombinePaths(directory, name);
                struct stat fileStatistics;
                stat(fullFilePath.c_str(), &fileStatistics);

                FileData data
                {
                    static_cast<uint32_t>(fileStatistics.st_mtime),
                    static_cast<uint32_t>(fileStatistics.st_size)
                };

                files.insert(std::make_pair(std::move(name), std::move(data)));
            }

            directoryEntry = readdir(dir);
        }

        closedir(dir);
    }
#endif

    return files;
}

FileMap GetAllPacketsInDirectory(std::string directory)
{
    FileMap allFiles = GetAllFilesInDirectory(std::move(directory));
    FileMap packets;

    for (auto& file : allFiles)
    {
        const std::string& name = file.first;
        const std::string extension = name.substr(name.find_last_of(".") + 1);

        if (extension == "packet")
        {
            packets.insert(std::move(file));
        }
    }

    return packets;
}

std::vector<std::string> StringSplit(const std::string& s, char delimiter)
{
    // from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/ cus I was too lazy to write it myself
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }
    return tokens;
}

struct PacketData
{
    bool isDyn;
    bool isArr;
    bool isStruct;
    bool hasCondition;

    std::string name;
    std::string type;
    std::string condition;
    std::string optional;
};

struct PacketStruct
{
    std::string name;
    std::vector<PacketData> data;
};

struct PacketDesc
{
    std::string namespace_;
    std::string name;
    std::vector<PacketData> data;
    std::vector<PacketStruct> structs;
};

void PrintStruct(PacketStruct& packet)
{
    // Header
    std::printf("    struct %s\n", packet.name.c_str());
    std::printf("    {\n");

    for (auto& packetData : packet.data)
    {
        std::printf("        %s", packetData.type.c_str());

        if (packetData.isDyn)
        {
            std::printf("*");
        }

        std::printf(" %s", packetData.name.c_str());

        if (packetData.isArr)
        {
            std::printf("[%s]", packetData.optional.c_str());
        }

        std::printf(";");

        std::vector<std::string> comments;

        if (packetData.hasCondition)
        {
            char buff[256];
            std::sprintf(buff, "only if (%s)", packetData.condition.c_str());
            comments.emplace_back(buff);
        }

        if (packetData.isDyn)
        {
            char buff[128];
            std::sprintf(buff, "sized by %s", packetData.optional.c_str());
            comments.emplace_back(buff);
        }

        if (!packetData.isArr && !packetData.isDyn && !packetData.isStruct && !packetData.optional.empty())
        {
            char buff[128];
            std::sprintf(buff, "max %s", packetData.optional.c_str());
            comments.emplace_back(buff);
        }

        if (!comments.empty())
        {
            for (std::size_t j = 0; j < comments.size(); ++j)
            {
                if (j == 0)
                {
                    std::printf(" // ");
                }

                std::printf("%s", comments[j].c_str());

                if (j != comments.size() - 1)
                {
                    std::printf(", ");
                }
            }
        }

        std::printf("\n");
    }

    std::vector<std::string> dynNames;

    for (auto& packetData : packet.data)
    {
        if (packetData.isDyn)
        {
            dynNames.emplace_back(packetData.name);
        }
    }

    std::printf("\n");
    std::printf("        inline std::size_t Read(const std::byte* buff, std::size_t maxLen);\n");
    std::printf("        inline std::size_t Write(std::byte* buff, std::size_t maxLen);\n");
    std::printf("        inline std::size_t Size() const;\n");

    if (!dynNames.empty())
    {
        std::printf("\n");
        std::printf("        inline %s();\n", packet.name.c_str());
        std::printf("        inline ~%s();\n", packet.name.c_str());
    }

    std::printf("    };\n\n");

    // Serialize

    auto sizeOfStatement = [](PacketData& packetData) -> char*
    {
        static char sizeOfStatement[256];

        if (packetData.isDyn || packetData.isArr)
        {
            std::sprintf(sizeOfStatement, "sizeof(%s) * %s", packetData.type.c_str(), packetData.optional.c_str());
        }
        else
        {
            std::sprintf(sizeOfStatement, "sizeof(%s)", packetData.type.c_str());
        }

        return sizeOfStatement;
    };

    auto printBody = [&](bool write)
    {
        std::printf("        const std::byte* start = buff;\n");
        std::printf("        const std::byte* end = buff + maxLen;\n\n");

        for (auto& packetData : packet.data)
        {
            std::size_t spaces = 8;

            if (packetData.hasCondition)
            {
                std::printf("%*cif (%s)\n", spaces, ' ', packetData.condition.c_str());
                std::printf("%*c{\n", spaces, ' ');
                spaces += 4;
            }

            if (packetData.isDyn)
            {
                std::printf("%*cif (%s)\n", spaces, ' ', packetData.optional.c_str());
                std::printf("%*c{\n", spaces, ' ');
                spaces += 4;
            }

            if (packetData.isStruct)
            {
                std::printf("%*c%s = new %s[%s];\n", spaces, ' ', packetData.name.c_str(), packetData.type.c_str(), packetData.optional.c_str());
                std::printf("%*cfor (std::size_t i = 0; i < %s; ++i)\n", spaces, ' ', packetData.optional.c_str());
                std::printf("%*c{\n", spaces, ' ');

                const char* operation = write ? "Write" : "Read";
                std::printf("%*c    if (std::size_t bytes = %s[i].%s(buff, static_cast<std::size_t>(end - buff)); bytes) buff += bytes;\n", spaces, ' ', packetData.name.c_str(), operation);
                std::printf("%*c    else return 0;\n", spaces, ' ');
                std::printf("%*c}\n", spaces, ' ');
            }
            else
            {
                char* size = sizeOfStatement(packetData);
                std::printf("%*cif (buff + %s > end) return 0;\n", spaces, ' ', size);

                if (packetData.isDyn && !write)
                {
                    std::printf("%*c%s = new %s[%s];\n", spaces, ' ', packetData.name.c_str(), packetData.type.c_str(), packetData.optional.c_str());
                }

                bool boundsCheck = !packetData.isArr && !packetData.isDyn && !packetData.isStruct && !packetData.optional.empty();

                if (boundsCheck && write)
                {
                    std::printf("%*cif (%s > %s) %s = %s;\n", spaces, ' ', packetData.name.c_str(), packetData.optional.c_str(), packetData.name.c_str(), packetData.optional.c_str());
                }

                const char* addressOf = packetData.isDyn || packetData.isArr ? "" : "&";

                if (write)
                {
                    std::printf("%*cstd::memcpy(buff, %s%s, %s);\n", spaces, ' ', addressOf, packetData.name.c_str(), size);
                }
                else
                {
                    std::printf("%*cstd::memcpy(%s%s, buff, %s);\n", spaces, ' ', addressOf, packetData.name.c_str(), size);
                }

                std::printf("%*cbuff += %s;\n", spaces, ' ', size);

                if (boundsCheck && !write)
                {
                    std::printf("%*cif (%s > %s) %s = %s;\n", spaces, ' ', packetData.name.c_str(), packetData.optional.c_str(), packetData.name.c_str(), packetData.optional.c_str());
                }
            }

            if (packetData.isDyn)
            {
                spaces -= 4;
                std::printf("%*c}\n", spaces, ' ');
            }

            if (packetData.hasCondition)
            {
                spaces -= 4;
                std::printf("%*c}\n", spaces, ' ');
            }

            std::printf("\n");
        }

        std::printf("        return static_cast<std::size_t>(buff - start);\n");
    };

    std::printf("    inline std::size_t %s::Read(const std::byte* buff, std::size_t maxLen)\n", packet.name.c_str());
    std::printf("    {\n");
    printBody(false);
    std::printf("    }\n\n");

    std::printf("    inline std::size_t %s::Write(std::byte* buff, std::size_t maxLen)\n", packet.name.c_str());
    std::printf("    {\n");
    printBody(true);
    std::printf("    }\n\n");

    std::printf("    inline std::size_t %s::Size() const\n", packet.name.c_str());
    std::printf("    {\n");
    std::printf("        std::size_t structSize = 0;\n");

    for (std::size_t j = 0; j < packet.data.size(); ++j)
    {
        auto& packetData = packet.data[j];
        if (packetData.isStruct)
        {
            if (packetData.isDyn)
            {
                std::printf("        for (std::size_t i = 0; i < %s; ++i) { structSize += %s[i].Size(); }", packetData.optional.c_str(), packetData.name.c_str());
            }
            else
            {
                std::printf("        structSize += %s.Size();", packetData.name.c_str());
            }

            std::printf("\n");
        }
    }

    std::printf("        return structSize +");

    for (std::size_t j = 0; j < packet.data.size(); ++j)
    {
        auto& packetData = packet.data[j];

        if (packetData.isStruct)
        {
            // Struct size accounted for above
            continue;
        }

        std::printf("\n            ");

        if (packetData.hasCondition)
        {
            std::printf("((%s) ? ", packetData.condition.c_str());
        }

        std::printf("(%s)", sizeOfStatement(packetData));

        if (packetData.hasCondition)
        {
            std::printf(" : (0))");
        }

        if (j != packet.data.size() - 1)
        {
            std::printf(" +");
        }
    }

    std::printf(";\n");
    std::printf("    }\n");

    if (!dynNames.empty())
    {
        std::printf("\n");
        std::printf("    %s::%s() : ", packet.name.c_str(), packet.name.c_str());
        for (std::size_t j = 0; j < dynNames.size(); ++j)
        {
            std::printf( "%s(nullptr)", dynNames[j].c_str());

            if (j != dynNames.size() - 1)
            {
                std::printf(", ");
            }
        }

        std::printf(" { }\n\n");

        std::printf("    %s::~%s()\n", packet.name.c_str(), packet.name.c_str());
        std::printf("    {\n");
        for (auto& dynName : dynNames)
        {
            std::printf("        delete[] %s;\n", dynName.c_str());
        }
        std::printf("    }\n");
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::printf("Please provide a folder containing .packet files.\n");
        return 1;
    }

    std::unordered_map<std::string, std::vector<PacketDesc>> out;

    for (auto& packet : GetAllPacketsInDirectory(argv[1]))
    {
        std::string path = std::string(argv[1]) + "/" + packet.first;
        std::vector<char> data;

        FILE* file = std::fopen(path.c_str(), "r");
        if (file)
        {
            std::fseek(file, 0, SEEK_END);
            std::size_t fileLen = ftell(file);
            std::fseek(file, 0, SEEK_SET);

            data.resize(fileLen);
            std::fread(data.data(), 1, fileLen, file);

            std::fclose(file);
        }
        else
        {
            std::printf("Couldn't open file %s.\n", path.c_str());
            return 1;
        }

        // Split into lines

        std::vector<std::string> lines;

        for (const char* head = data.data(),
            *end = data.data() + data.size(),
            *tail = head; head < end;)
        {
            while (*head++ != '\n' && head < end) {}
            lines.emplace_back(std::string(tail, head - tail));
            tail = head++;
        }

        if (lines.size() < 2)
        {
            std::printf("Not enough lines %s!\n", path.c_str());
            continue;
        }

        // Second pass -
        // 1. Convert tabs to spaces
        // 2. Remove /r
        // 3. Remove /n

        for (std::string& line : lines)
        {
            for (auto iter = std::rbegin(line); iter != std::rend(line); ++iter)
            {
                char ch = *iter;

                if (ch == '\n' || ch == '\r')
                {
                    line.erase(std::next(iter).base());
                }
                else if (ch == '\t')
                {
                    *iter = ' ';
                }
            }
        }

        auto packetNameSplit = StringSplit(lines[0], '.');

        PacketDesc desc;

        desc.namespace_ = "Net::Packets::" + std::move(packetNameSplit[0]);
        desc.name = std::move(packetNameSplit[1]);

        for (std::size_t i = 1; i < lines.size(); ++i)
        {
            std::function<PacketData()> parseLine;
            parseLine = [&]() -> PacketData
            {
                std::string& line = lines[i];
                auto packetDataSplit = StringSplit(line, ' ');

                std::string type = packetDataSplit[0];
                auto typeSplit = StringSplit(type, '_');

                PacketData data;
                data.isArr = std::find(std::begin(typeSplit), std::end(typeSplit), "ARR") != std::end(typeSplit);
                data.isDyn = std::find(std::begin(typeSplit), std::end(typeSplit), "DYN") != std::end(typeSplit);
                data.isStruct = std::find(std::begin(typeSplit), std::end(typeSplit), "STRUCT") != std::end(typeSplit);
                data.hasCondition = std::find(std::begin(typeSplit), std::end(typeSplit), "C") != std::end(typeSplit);

                data.name = std::move(packetDataSplit[1]);
                data.type = std::move(packetDataSplit[2]);

                std::size_t optionalIndex = 3;

                if (data.hasCondition)
                {
                    data.condition = packetDataSplit[3];
                    optionalIndex += 1;
                }

                if (packetDataSplit.size() == optionalIndex + 1)
                {
                    data.optional = std::move(packetDataSplit[optionalIndex]);
                }

                if (data.isStruct)
                {
                    PacketStruct struct_;
                    struct_.name = desc.name + "_" + data.type;
                    data.type = struct_.name;

                    // We must now parse the proceeding lines (until we hit a }) in order
                    // to determine the member layout so we can later generate it.

                    if (lines[i + 1].find("{") != std::string::npos)
                    {
                        // Could be a struct declaration (since it was already defined earlier).
                        while (true)
                        {
                            ++i;

                            if (lines[i].find("{") != std::string::npos)
                            {
                                continue; //start of struct
                            }

                            if (lines[i].find("}") != std::string::npos)
                            {
                                break; //end of struct
                            }

                            struct_.data.emplace_back(parseLine());
                        }
                    }

                    bool add = true;

                    for (PacketStruct& existingStruct : desc.structs)
                    {
                        if (existingStruct.name == struct_.name)
                        {
                            add = false;
                            break;
                        }
                    }

                    if (add)
                    {
                        desc.structs.emplace_back(std::move(struct_));
                    }
                }

                return data;
            };

            desc.data.emplace_back(parseLine());
        }

        out[desc.namespace_].emplace_back(std::move(desc));
    }

    std::printf("#pragma once\n");
    std::printf("\n");
    std::printf("#include <cstdint>\n");
    std::printf("#include <cstring>\n");
    std::printf("\n");

    for (auto& kvp : out)
    {
        std::printf("namespace %s\n", kvp.first.c_str());
        std::printf("{\n");

        for (std::size_t i = 0; i < kvp.second.size(); ++i)
        {
            auto& packet = kvp.second[i];
            for (PacketStruct& struct_ : packet.structs)
            {
                PrintStruct(struct_);
                std::printf("\n");
            }

            PacketStruct mainStruct;
            mainStruct.name = packet.name;
            mainStruct.data = packet.data;
            PrintStruct(mainStruct);

            if (i != kvp.second.size() - 1)
            {
                std::printf("\n");
            }
        }

        std::printf("}\n");
    }
}
