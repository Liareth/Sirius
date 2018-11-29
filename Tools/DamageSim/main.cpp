#include "simulation.hpp"
#include "matplotlibcpp.h"

#include <vector>
#include <string>

static int s_ColourIndex = 0;

std::string GetColourString()
{
    constexpr static const char* colours[] =
    {
        "#000000", "#FFFF00", "#1CE6FF", "#FF34FF", "#FF4A46", "#008941", "#006FA6", "#A30059",
        "#FFDBE5", "#7A4900", "#0000A6", "#63FFAC", "#B79762", "#004D43", "#8FB0FF", "#997D87",
        "#5A0007", "#809693", "#71CAB2", "#1B4400", "#4FC601", "#3B5DFF", "#4A3B53", "#FF2F80",
        "#61615A", "#BA0900", "#6B7900", "#00C2A0", "#FFAA92", "#FF90C9", "#B903AA", "#D16100",
        "#DDEFFF", "#000035", "#7B4F4B", "#A1C299", "#300018", "#0AA6D8", "#013349", "#00846F",
        "#372101", "#FFB500", "#C2FFED", "#A079BF", "#CC0744", "#C0B9B2", "#C2FF99", "#001E09",
        "#00489C", "#6F0062", "#0CBD66", "#EEC3FF", "#456D75", "#B77B68", "#7A87A1", "#788D66",
        "#885578", "#FAD09F", "#FF8A9A", "#D157A0", "#BEC459", "#456648", "#0086ED", "#886F4C",

        "#34362D", "#B4A8BD", "#00A6AA", "#452C2C", "#636375", "#A3C8C9", "#FF913F", "#938A81",
        "#575329", "#00FECF", "#B05B6F", "#8CD0FF", "#3B9700", "#04F757", "#C8A1A1", "#1E6E00",
        "#7900D7", "#A77500", "#6367A9", "#A05837", "#6B002C", "#772600", "#D790FF", "#9B9700",
        "#549E79", "#FFF69F", "#201625", "#72418F", "#BC23FF", "#99ADC0", "#3A2465", "#922329",
        "#5B4534", "#FDE8DC", "#404E55", "#0089A3", "#CB7E98", "#A4E804", "#324E72", "#6A3A4C",
        "#83AB58", "#001C1E", "#D1F7CE", "#004B28", "#C8D0F6", "#A3A489", "#806C66", "#222800",
        "#BF5650", "#E83000", "#66796D", "#DA007C", "#FF1A59", "#8ADBB4", "#1E0200", "#5B4E51",
        "#C895C5", "#320033", "#FF6832", "#66E1D3", "#CFCDAC", "#D0AC94", "#7ED379", "#012C58"
    };

    return colours[s_ColourIndex++];
};

struct GenericSimResult
{
    int defenderSkill;
    Simulation::Result result;
};

std::vector<std::pair<int, std::vector<GenericSimResult>>> RunGenericSim()
{
    using namespace Sirius;

    Simulation::Args args;

    { // Hit situation
        static constexpr float skillMulti = 0.75f;
        static constexpr float priStatMulti = 0.75f;
        static constexpr float secStatMulti = 0.385f;

        Combat::HitSituation::SkillSet attackerSkillSet;
        attackerSkillSet.skillLevel = 0;
        attackerSkillSet.skillPrimaryStat = 0;
        attackerSkillSet.skillSecondaryStat = 0;
        attackerSkillSet.attackRollSkillMultiplier = skillMulti;
        attackerSkillSet.attackRollPrimaryStatMultiplier = priStatMulti;
        attackerSkillSet.attackRollSecondaryStatMultiplier = secStatMulti;

        Combat::HitSituation::SkillSet defenderSkillSet;
        defenderSkillSet.skillLevel = attackerSkillSet.skillLevel;
        defenderSkillSet.skillPrimaryStat = attackerSkillSet.skillPrimaryStat;
        defenderSkillSet.skillSecondaryStat = attackerSkillSet.skillSecondaryStat;
        defenderSkillSet.attackRollSkillMultiplier = attackerSkillSet.attackRollSkillMultiplier;
        defenderSkillSet.attackRollPrimaryStatMultiplier = attackerSkillSet.attackRollPrimaryStatMultiplier;
        defenderSkillSet.attackRollSecondaryStatMultiplier = attackerSkillSet.attackRollSecondaryStatMultiplier;

        args.hitSituation.attackerSkillSet = std::move(attackerSkillSet);
        args.hitSituation.attackerHitRange = 25;
        args.hitSituation.attackerCritRange = 75;
        args.hitSituation.defenderSkillSet = std::move(defenderSkillSet);
        args.hitSituation.defenderConcealment = 0.0f;
    }

    { // Damage situation
        static constexpr float skillMulti = 0.25f;
        static constexpr float priStatMulti = 0.25f;
        static constexpr float secStatMulti = 0.125f;

        Combat::DamageSituation::OffensiveData offensiveData;

        Combat::DamageSituation::OffensiveData::DamageDice damageDice;
        damageDice.damageType = Combat::DamageType::Ballistic;
        damageDice.damageDice = 2;
        damageDice.damageSides = 8;
        damageDice.damageCritMultiplier = 2.0f;
        offensiveData.baseDamageDice.emplace_back(std::move(damageDice));

        offensiveData.baseDamageSkillType = damageDice.damageType;
        offensiveData.baseDamageSkillMultiplier = skillMulti;
        offensiveData.baseDamagePrimaryStatMultiplier = priStatMulti;
        offensiveData.baseDamageSecondaryStatMultiplier = secStatMulti;

        Combat::DamageSituation::DefensiveData defensiveData;

        Combat::DamageSituation::DefensiveData::FlatDR flatDr;
        flatDr.damageType = damageDice.damageType;
        flatDr.value = 15;
        defensiveData.flatDrs.emplace_back(std::move(flatDr));

        Combat::DamageSituation::DefensiveData::PercentageDR percentageDr;
        percentageDr.damageType = damageDice.damageType;
        percentageDr.value = 50;
        defensiveData.percentageDrs.emplace_back(std::move(percentageDr));

        args.damageSituation.hitSituation = &args.hitSituation;
        args.damageSituation.offensiveData = std::move(offensiveData);
        args.damageSituation.defensiveData = std::move(defensiveData);
    }

    std::vector<std::pair<int, std::vector<GenericSimResult>>> results;

    for (int attackerSkill = 0; attackerSkill <= 200; attackerSkill += 25)
    {
        std::pair<int, std::vector<GenericSimResult>> thisAttackerResults;
        thisAttackerResults.first = attackerSkill;

        for (int defenderSkill = 0; defenderSkill <= 200; ++defenderSkill)
        {
            args.hitSituation.attackerSkillSet.skillLevel = attackerSkill;
            args.hitSituation.attackerSkillSet.skillPrimaryStat = std::max(0, (attackerSkill - 10) / 4);
            args.hitSituation.attackerSkillSet.skillSecondaryStat = args.hitSituation.attackerSkillSet.skillPrimaryStat;

            args.hitSituation.defenderSkillSet.skillLevel = defenderSkill;
            args.hitSituation.defenderSkillSet.skillPrimaryStat = std::max(0, (defenderSkill - 10) / 4);
            args.hitSituation.defenderSkillSet.skillSecondaryStat = args.hitSituation.defenderSkillSet.skillPrimaryStat;

            GenericSimResult result;
            result.defenderSkill = defenderSkill;
            result.result = Simulation::Run(args);
            thisAttackerResults.second.emplace_back(std::move(result));
        }

        results.emplace_back(std::move(thisAttackerResults));
    }

    return results;
}

void OutputHitTable()
{
    s_ColourIndex = 0;
    matplotlibcpp::title("Hit chance");
    matplotlibcpp::figure_size(1920, 1080);

    for (const auto& resultPair : RunGenericSim())
    {
        std::vector<double> defenderSkills;
        for (const auto& result : resultPair.second)
        {
            defenderSkills.emplace_back(result.defenderSkill);
        }

        std::vector<double> overallHitChances;
        for (const auto& result : resultPair.second)
        {
            overallHitChances.emplace_back(result.result.grazeChance + result.result.hitChance + result.result.critChance);
        }

        matplotlibcpp::named_plot("Attacker skill " + std::to_string(resultPair.first), defenderSkills, overallHitChances, GetColourString());
    }

    matplotlibcpp::xlim(0, 200);
    matplotlibcpp::xlabel("Defender skill");
    matplotlibcpp::ylim(0, 100);
    matplotlibcpp::ylabel("Percentage");
    matplotlibcpp::legend();

    char filenameBuff[256];
    std::sprintf(filenameBuff, "overall_hit");
    matplotlibcpp::save(std::string("/root/dev/sirius/") + filenameBuff);
}

void OutputDamageTable()
{
    s_ColourIndex = 0;
    matplotlibcpp::title("Hit chance");
    matplotlibcpp::figure_size(1920, 1080);

    for (const auto& resultPair : RunGenericSim())
    {
        std::vector<double> defenderSkills;
        for (const auto& result : resultPair.second)
        {
            defenderSkills.emplace_back(result.defenderSkill);
        }

        std::vector<double> damageLow;
        std::vector<double> damageAvg;
        std::vector<double> damageHigh;

        for (const auto& result : resultPair.second)
        {
            damageLow.emplace_back(result.result.dmgLow);
            damageAvg.emplace_back(result.result.dmgAvg);
            damageHigh.emplace_back(result.result.dmgHigh);
        }

        matplotlibcpp::named_plot("Attacker skill " + std::to_string(resultPair.first), defenderSkills, damageAvg, GetColourString());
    }

    matplotlibcpp::xlim(0, 200);
    matplotlibcpp::xlabel("Defender skill");
    matplotlibcpp::ylabel("Damage per hit");
    matplotlibcpp::legend();

    char filenameBuff[256];
    std::sprintf(filenameBuff, "damage");
    matplotlibcpp::save(std::string("/root/dev/sirius/") + filenameBuff);
}

int main()
{
    OutputHitTable();
    OutputDamageTable();
}
