#pragma once

#include "Game/Combat/DamageType.hpp"
#include "Game/Skill/Combat.hpp"
#include <vector>

namespace Sirius::Combat
{
    struct ResolvedAttackHit
    {
        enum Result
        {
            Miss,
            Graze,
            Hit,
            Crit
        };

        int attackerRoll;
        int defenderRoll;

        Result result;
    };

    struct ResolvedAttackDamage
    {
        struct DamageEntry
        {
            DamageType::Enum damageType;
            float damage;
        };

        std::vector<DamageEntry> damageEntries;
    };

    struct HitSituation
    {
        struct SkillSet
        {
            Skill::Combat::Enum skillType;

            int skillLevel;
            int skillPrimaryStat;
            int skillSecondaryStat;

            float attackRollSkillMultiplier;
            float attackRollPrimaryStatMultiplier;
            float attackRollSecondaryStatMultiplier;
        };

        SkillSet attackerSkillSet;
        float attackerHitRange; // 0 -> attackerHitRange = graze
        float attackerCritRange; // attackerHitRange -> attackerCritRange = hit

        SkillSet defenderSkillSet;
        float defenderConcealment;
    };

    struct DamageSituation
    {
        HitSituation* hitSituation;
        ResolvedAttackHit* resolvedAttackHit;

        struct OffensiveData
        {
            struct DamageDice
            {
                DamageType::Enum damageType;
                int damageDice;
                int damageSides;
                float damageCritMultiplier;
            };

            std::vector<DamageDice> baseDamageDice;

            DamageType::Enum baseDamageSkillType;
            float baseDamageSkillMultiplier;
            float baseDamagePrimaryStatMultiplier;
            float baseDamageSecondaryStatMultiplier;
        };

        struct DefensiveData
        {
            struct FlatDR
            {
                DamageType::Enum damageType;
                float value;
            };

            struct PercentageDR
            {
                DamageType::Enum damageType;
                float value; // 0 -> 100
            };

            std::vector<FlatDR> flatDrs;
            std::vector<PercentageDR> percentageDrs;
        };

        OffensiveData offensiveData;
        DefensiveData defensiveData;
    };

    ResolvedAttackHit ResolveAttackHit(const HitSituation* situation);
    ResolvedAttackDamage ResolveAttackDamage(const DamageSituation* situation);
}
