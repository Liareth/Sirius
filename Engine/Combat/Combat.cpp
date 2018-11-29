#include "Combat/Combat.hpp"
#include "Random/Dice.hpp"

#include <unordered_map>

namespace Sirius::Combat
{
    ResolvedAttackHit ResolveAttackHit(const HitSituation* situation)
    {
        ResolvedAttackHit resolvedAttackHit;
        resolvedAttackHit.result = ResolvedAttackHit::Miss;

        float concealment = situation->defenderConcealment;

        if (concealment == 0.0f || Random::Roll(1, 100) > concealment)
        {
            int attackerRoll = Random::Roll(1, 100);
            int defenderRoll = Random::Roll(1, 100);

            resolvedAttackHit.attackerRoll = attackerRoll;
            resolvedAttackHit.defenderRoll = defenderRoll;

            float attackerSkillContrib =
                situation->attackerSkillSet.skillLevel * 
                situation->attackerSkillSet.attackRollSkillMultiplier;

            float attackerPrimaryStatContrib =
                situation->attackerSkillSet.skillPrimaryStat *
                situation->attackerSkillSet.attackRollPrimaryStatMultiplier;

            float attackerSecondaryStatContrib =
                situation->attackerSkillSet.skillSecondaryStat *
                situation->attackerSkillSet.attackRollSecondaryStatMultiplier;

            float defenderSkillContrib =
                situation->defenderSkillSet.skillLevel * 
                situation->defenderSkillSet.attackRollSkillMultiplier;

            float defenderPrimaryStatContrib =
                situation->defenderSkillSet.skillPrimaryStat *
                situation->defenderSkillSet.attackRollPrimaryStatMultiplier;

            float defenderSecondaryStatContrib =
                situation->defenderSkillSet.skillSecondaryStat *
                situation->defenderSkillSet.attackRollSecondaryStatMultiplier;

            float delta = 
                (attackerRoll + attackerSkillContrib + attackerPrimaryStatContrib + attackerSecondaryStatContrib) -
                (defenderRoll + defenderSkillContrib + defenderPrimaryStatContrib + defenderSecondaryStatContrib);

            if (delta >= situation->attackerCritRange)
            {
                resolvedAttackHit.result = ResolvedAttackHit::Crit;
            }
            else if (delta >= situation->attackerHitRange)
            {
                resolvedAttackHit.result = ResolvedAttackHit::Hit;
            }
            else if (delta >= 0.0f)
            {
                resolvedAttackHit.result = ResolvedAttackHit::Graze;
            }
        }

        return resolvedAttackHit;
    }

    ResolvedAttackDamage ResolveAttackDamage(const DamageSituation* situation)
    {
        float baseDamageSkillContrib =
            situation->hitSituation->attackerSkillSet.skillLevel *
            situation->offensiveData.baseDamageSkillMultiplier;

        float baseDamagePrimaryStatContrib =
            situation->hitSituation->attackerSkillSet.skillPrimaryStat *
            situation->offensiveData.baseDamagePrimaryStatMultiplier;

        float baseDamageSecondaryStatContrib =
            situation->hitSituation->attackerSkillSet.skillSecondaryStat *
            situation->offensiveData.baseDamageSecondaryStatMultiplier;

        std::unordered_map<DamageType::Enum, float> totalDamageMap;

        for (const DamageSituation::OffensiveData::DamageDice& damageDice : situation->offensiveData.baseDamageDice)
        {
            float damageMultiplier = 1.0f;
            if (situation->resolvedAttackHit->result == ResolvedAttackHit::Graze)
            {
                damageMultiplier = 0.5f;
            }
            else if (situation->resolvedAttackHit->result == ResolvedAttackHit::Crit)
            {
                damageMultiplier = damageDice.damageCritMultiplier;
            }

            totalDamageMap[damageDice.damageType] += Random::Roll(damageDice.damageDice, damageDice.damageSides) * damageMultiplier;
        }

        // Skill-based damage is excluded from crits.
        totalDamageMap[situation->offensiveData.baseDamageSkillType] += 
            baseDamageSkillContrib + baseDamagePrimaryStatContrib +baseDamageSecondaryStatContrib;

        for (auto& totalDamage : totalDamageMap)
        {
            for (const DamageSituation::DefensiveData::FlatDR& flatDr : situation->defensiveData.flatDrs)
            {
                if (flatDr.damageType == totalDamage.first && totalDamage.second > 0.0f)
                {
                    totalDamage.second = totalDamage.second - flatDr.value;
                }
            }

            for (const DamageSituation::DefensiveData::PercentageDR& percentageDr : situation->defensiveData.percentageDrs)
            {
                if (percentageDr.damageType == totalDamage.first  && totalDamage.second > 0.0f)
                {
                    totalDamage.second -= totalDamage.second / 100 * percentageDr.value;
                }
            }
        }

        ResolvedAttackDamage resolvedAttackDamage;

        for (auto& totalDamage : totalDamageMap)
        {
            if (totalDamage.second > 0.0f)
            {
                ResolvedAttackDamage::DamageEntry entry;
                entry.damageType = totalDamage.first;
                entry.damage = totalDamage.second;
                resolvedAttackDamage.damageEntries.emplace_back(std::move(entry));
            }
        }

        return resolvedAttackDamage;
    }
}
