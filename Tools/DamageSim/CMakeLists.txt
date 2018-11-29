find_package(PythonLibs 2.7)

add_executable(DamageSim 
    main.cpp
    simulation.cpp simulation.hpp
    matplotlibcpp.h
    concurrentqueue.h)

target_include_directories(DamageSim PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(DamageSim ${PYTHON_LIBRARIES})

target_link_libraries(DamageSim SiriusEngine)