add_executable(Server
    main.cpp
    Net/Core/Connection.cpp Net/Core/Connection.hpp
    Net/Core/Endian.hpp
    Net/Messages/BioNet.cpp Net/Messages/BioNet.hpp
    Net/Messages/BioMaster.cpp Net/Messages/BioMaster.hpp
    Net/Packets/Packets.hpp)

target_link_libraries(Server Game)
