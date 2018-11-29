#include "simulation.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace Simulation
{
    Result Run(const Args& args)
    {
        using namespace Sirius;

        Result result;

        constexpr static int simAttempts = 2500;

        int totalGrazes = 0;
        int totalHits = 0;
        int totalCrits = 0;

        std::vector<float> damages;

        damages.reserve(simAttempts);

        for (int attempt = 0; attempt < simAttempts; ++attempt)
        {
            float dmg = 0.0f;

            Combat::ResolvedAttackHit resolvedAttackHit = ResolveAttackHit(&args.hitSituation);

            if (resolvedAttackHit.result == Combat::ResolvedAttackHit::Graze) ++totalGrazes;
            else if (resolvedAttackHit.result == Combat::ResolvedAttackHit::Hit) ++totalHits;
            else if (resolvedAttackHit.result == Combat::ResolvedAttackHit::Crit) ++totalCrits;

            if (resolvedAttackHit.result != Combat::ResolvedAttackHit::Miss)
            {
                Combat::DamageSituation damageSituation = args.damageSituation;
                damageSituation.resolvedAttackHit = &resolvedAttackHit;

                for (const auto& damageEntry : ResolveAttackDamage(&damageSituation).damageEntries)
                {
                    dmg += damageEntry.damage;
                }
            }

            damages.emplace_back(dmg);
        }

        result.grazeChance = (static_cast<float>(totalGrazes) / simAttempts) * 100.0f;
        result.hitChance = (static_cast<float>(totalHits) / simAttempts) * 100.0f;
        result.critChance = (static_cast<float>(totalCrits) / simAttempts) * 100.0f;

        std::sort(std::begin(damages), std::end(damages));

        // 25th percentile
        int lowIndex = static_cast<int>(std::ceil(0.25f * simAttempts));
        result.dmgLow = damages[lowIndex];

        // avg
        result.dmgAvg = std::accumulate(std::begin(damages), std::end(damages), 0.0f) / simAttempts;

        // 75th percentile
        int highIndex = static_cast<int>(std::ceil(0.75f * simAttempts));
        result.dmgHigh = damages[highIndex];

        return result;
    }
}
