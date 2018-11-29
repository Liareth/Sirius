add_library(SiriusEngine STATIC
    Combat/Combat.cpp Combat/Combat.hpp
    Combat/DamageType.hpp
    Random/Dice.cpp Random/Dice.hpp
    Skill/Combat.cpp Skill/Combat.hpp
    Skill/Crafting.cpp Skill/Crafting.hpp
    Skill/General.cpp Skill/General.hpp
    )

target_include_directories(SiriusEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
