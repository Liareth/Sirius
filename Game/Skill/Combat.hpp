#pragma once

namespace Sirius::Skill
{
    struct Combat
    {
        enum Enum
        {
            Melee_OneHanded,
            Melee_TwoHanded,
            Melee_DoubleSided,
            Melee_MartialArts,
            Melee_Alientech,

            Ranged_Primitive,
            Ranged_Ballistic,
            Ranged_Alientech,

            Psionics_Physical,
            Psionics_Mental,
            Psionics_Augment,

            Defensive_Dodge,
            Defensive_LightArmour,
            Defensive_MediumArmour,
            Defensive_HeavyArmour,
            Defensive_Shields,
            Defensive_Alientech,

            Utility_FirstAid,
            Utility_Stealth,
            Utility_Scouting
        };
    };
}
