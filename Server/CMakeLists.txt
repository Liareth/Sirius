add_executable(Server
    main.cpp
    Packets/Packets.hpp)

target_link_libraries(Server Game)
