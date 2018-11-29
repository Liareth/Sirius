#pragma once

namespace Sirius::Combat
{
    struct DamageType
    {
        enum Enum
        {
            Melee,
            Primitive,
            Ballistic,
            Explosive,
            Energy,
            Special
        };
    };
}
