#include "Random/Dice.hpp"

namespace Sirius::Random
{
    inline int fast_rand()
    {
        static int s_Seed = 1052;
        s_Seed = (214013 * s_Seed + 2531011);
        return (s_Seed >> 16) & 0x7FFF;
    }

    // TODO - replace with NWN impl?
    int (*g_randFunc)() = &fast_rand;

    std::uint32_t Roll(std::uint8_t dice, std::uint8_t sides)
    {
        std::uint32_t value = 0;

        while (dice-- > 0)
        {
            value += g_randFunc() % sides + 1;
        }

        return value;
    }
}
