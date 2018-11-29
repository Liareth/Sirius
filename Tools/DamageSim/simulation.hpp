#pragma once

#include <cstdint>
#include <vector>

#include "Combat/Combat.hpp"

namespace Simulation
{
    struct Result
    {
        float grazeChance;
        float hitChance;
        float critChance;

        float dmgLow; // 25th percentile
        float dmgAvg; // average
        float dmgHigh; // 75th percentile
    };

    struct Args
    {
        Sirius::Combat::HitSituation hitSituation;
        Sirius::Combat::DamageSituation damageSituation;
    };

    Result Run(const Args& args);
}
