#pragma once

namespace Sirius::Skill
{
    struct Crafting
    {
        enum Enum
        {
            BaseBuilding_Construction,
            BaseBuilding_Engineering,

            ItemCreation_WeaponSmithing,
            ItemCreation_ArmourSmithing,
            ItemCreation_Augments,
            ItemCreation_Cooking,

            Medical_Surgery,
            Medical_Alientech
        };
    };
}
