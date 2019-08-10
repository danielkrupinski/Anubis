#include "ItemDefinitionIndex.h"

INT getWeaponIndex(ItemDefinitionIndex itemDefinitionIndex)
{
    switch (itemDefinitionIndex) {
    default: return 0;

    case ItemDefinitionIndex_Glock: return 1;
    case ItemDefinitionIndex_Hkp2000: return 2;
    case ItemDefinitionIndex_Usp_s: return 3;
    case ItemDefinitionIndex_Elite: return 4;
    case ItemDefinitionIndex_P250: return 5;
    case ItemDefinitionIndex_Tec9: return 6;
    case ItemDefinitionIndex_Fiveseven: return 7;
    case ItemDefinitionIndex_Cz75a: return 8;
    case ItemDefinitionIndex_Deagle: return 9;
    case ItemDefinitionIndex_Revolver: return 10;

    case ItemDefinitionIndex_Nova: return 11;
    case ItemDefinitionIndex_Xm1014: return 12;
    case ItemDefinitionIndex_Sawedoff: return 13;
    case ItemDefinitionIndex_Mag7: return 14;
    case ItemDefinitionIndex_M249: return 15;
    case ItemDefinitionIndex_Negev: return 16;

    case ItemDefinitionIndex_Mac10: return 17;
    case ItemDefinitionIndex_Mp9: return 18;
    case ItemDefinitionIndex_Mp7: return 19;
    case ItemDefinitionIndex_Mp5sd: return 20;
    case ItemDefinitionIndex_Ump45: return 21;
    case ItemDefinitionIndex_P90: return 22;
    case ItemDefinitionIndex_Bizon: return 23;

    case ItemDefinitionIndex_GalilAr: return 24;
    case ItemDefinitionIndex_Famas: return 25;
    case ItemDefinitionIndex_Ak47: return 26;
    case ItemDefinitionIndex_M4A1: return 27;
    case ItemDefinitionIndex_M4a1_s: return 28;
    case ItemDefinitionIndex_Ssg08: return 29;
    case ItemDefinitionIndex_Sg553: return 30;
    case ItemDefinitionIndex_Aug: return 31;
    case ItemDefinitionIndex_Awp: return 32;
    case ItemDefinitionIndex_G3SG1: return 33;
    case ItemDefinitionIndex_Scar20: return 34;
    }
}
