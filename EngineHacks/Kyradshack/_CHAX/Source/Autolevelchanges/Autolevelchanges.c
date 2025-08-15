#include "gbafe.h"
#include "magic.h"
#include "autolevel.h"
#define MAX(aValue, bValue) ((aValue) > bValue ? (aValue) : (bValue))
#define MIN(aValue, bValue) ((aValue) > bValue ? (bValue) : (aValue))

//Changing autolevel penalty to subtract stats, allowing them to go below class bases
//Also makes bosses use character growths for autolevel penalties
void UnitAutolevelPenalty(struct Unit* unit, u8 classId, int levelCount) {
    if (levelCount) {
    	if (UNIT_CATTRIBUTES(unit)&CA_BOSS){
        unit->maxHP -= GetAutoleveledStatIncrease(unit->pCharacterData->growthHP,  levelCount);
        unit->pow   -= GetAutoleveledStatIncrease(unit->pCharacterData->growthPow, levelCount);
        unit->mag   -= GetAutoleveledStatIncrease(MagCharTable[unit->pCharacterData->number].growthMag, levelCount);
        unit->skl   -= GetAutoleveledStatIncrease(unit->pCharacterData->growthSkl, levelCount);
        unit->spd   -= GetAutoleveledStatIncrease(unit->pCharacterData->growthSpd, levelCount);
        unit->def   -= GetAutoleveledStatIncrease(unit->pCharacterData->growthDef, levelCount);
        unit->res   -= GetAutoleveledStatIncrease(unit->pCharacterData->growthRes, levelCount);
        unit->lck   -= GetAutoleveledStatIncrease(unit->pCharacterData->growthLck, levelCount);    		
    	}
    	else {
        unit->maxHP -= GetAutoleveledStatIncrease(unit->pClassData->growthHP,  levelCount);
        unit->pow   -= GetAutoleveledStatIncrease(unit->pClassData->growthPow, levelCount);
        unit->mag   -= GetAutoleveledStatIncrease(MagClassTable[unit->pClassData->number].growthMag, levelCount);
        unit->skl   -= GetAutoleveledStatIncrease(unit->pClassData->growthSkl, levelCount);
        unit->spd   -= GetAutoleveledStatIncrease(unit->pClassData->growthSpd, levelCount);
        unit->def   -= GetAutoleveledStatIncrease(unit->pClassData->growthDef, levelCount);
        unit->res   -= GetAutoleveledStatIncrease(unit->pClassData->growthRes, levelCount);
        unit->lck   -= GetAutoleveledStatIncrease(unit->pClassData->growthLck, levelCount);
    	}
       //Had an issue with the mag shorthand 
       /*(unit->maxHP < 1) && (unit->maxHP = 1);
       (unit->pow < 0) && (unit->pow = 0);
       (unit->mag < 0) && (unit->mag = 0);
       (unit->skl < 0) && (unit->skl = 0);
       (unit->spd < 0) && (unit->spd = 0);
       (unit->def < 0) && (unit->def = 0);
       (unit->res < 0) && (unit->res = 0);
       (unit->lck < 0) && (unit->lck = 0);*/

       if(unit->maxHP < 1){
       	unit->maxHP = 1;
       }
         if(unit->pow < 0){
       	unit->pow = 0;
       }
       if(unit->mag < 0){
       	unit->mag = 0;
       }
       if(unit->skl < 0){
       	unit->skl = 0;
       }
       if(unit->spd < 0){
       	unit->spd = 0;
       }
       if(unit->def < 0){
       	unit->def = 0;
       }
       if(unit->res < 0){
       	unit->res = 0;
       }
       if(unit->lck < 0){
       	unit->lck = 0;
       }

    }
}
//Makes bosses use character growths for autolevel bonuses
void UnitAutolevelCore(struct Unit* unit, u8 classId, int levelCount) {
    if (levelCount) {
    	//levelCount += 28; //for adjusting level for testing purposes
    	if (UNIT_CATTRIBUTES(unit)&CA_BOSS){
        unit->maxHP += GetAutoleveledStatIncrease(unit->pCharacterData->growthHP,  levelCount);
        unit->pow   += GetAutoleveledStatIncrease(unit->pCharacterData->growthPow, levelCount);
        unit->mag   += GetAutoleveledStatIncrease(MagCharTable[unit->pCharacterData->number].growthMag, levelCount);
        unit->skl   += GetAutoleveledStatIncrease(unit->pCharacterData->growthSkl, levelCount);
        unit->spd   += GetAutoleveledStatIncrease(unit->pCharacterData->growthSpd, levelCount);
        unit->def   += GetAutoleveledStatIncrease(unit->pCharacterData->growthDef, levelCount);
        unit->res   += GetAutoleveledStatIncrease(unit->pCharacterData->growthRes, levelCount);
        unit->lck   += GetAutoleveledStatIncrease(unit->pCharacterData->growthLck, levelCount);    		
    	}
    	else {
        unit->maxHP += GetAutoleveledStatIncrease(unit->pClassData->growthHP,  levelCount);
        unit->pow   += GetAutoleveledStatIncrease(unit->pClassData->growthPow, levelCount);
        unit->mag   += GetAutoleveledStatIncrease(MagClassTable[unit->pClassData->number].growthMag, levelCount);
        unit->skl   += GetAutoleveledStatIncrease(unit->pClassData->growthSkl, levelCount);
        unit->spd   += GetAutoleveledStatIncrease(unit->pClassData->growthSpd, levelCount);
        unit->def   += GetAutoleveledStatIncrease(unit->pClassData->growthDef, levelCount);
        unit->res   += GetAutoleveledStatIncrease(unit->pClassData->growthRes, levelCount);
        unit->lck   += GetAutoleveledStatIncrease(unit->pClassData->growthLck, levelCount);
    	}
    }
}

//Allows for units to roll below their averages as well as above them.
//Additionally, designed to increase the likelihood of a stat being bang on average. Slightly hacky solution though.
//Commented out but a system in place for random chances for highly deviant stats
int GetAutoleveledStatIncrease(int growth, int levelCount) {
  return GetStatIncreaseNPC((growth * levelCount) + (NextRN_N((growth * levelCount) / 4) - (growth * levelCount) / 8));

}

//Designed to remove randomness from the getstatincrease, instead all randomness is a product of the growth totals
int GetStatIncreaseNPC(int growth) {
    int result = 0;

    while (growth > 100) {
        result++;
        growth -= 100;
    }

    if (growth >= 50) //modifying this
        result++;

    return result;
}

//Adds luck as a class base stat, though no promotion gain
void UnitLoadStatsFromChracter(struct Unit* unit, const struct CharacterData* character) {
    int i;
    
    unit->maxHP = character->baseHP + unit->pClassData->baseHP;
    unit->pow   = character->basePow + unit->pClassData->basePow;
  unit->mag   = MagCharTable[character->number].baseMag + MagClassTable[unit->pClassData->number].baseMag;
    unit->skl   = character->baseSkl + unit->pClassData->baseSkl;
    unit->spd   = character->baseSpd + unit->pClassData->baseSpd;
    unit->def   = character->baseDef + unit->pClassData->baseDef;
    unit->res   = character->baseRes + unit->pClassData->baseRes;
    unit->lck   = character->baseLck + unit->pClassData->baseLck;

    unit->conBonus = 0;

    for (i = 0; i < 8; ++i) {
        unit->ranks[i] = unit->pClassData->baseRanks[i];

        if (unit->pCharacterData->baseRanks[i])
            unit->ranks[i] = unit->pCharacterData->baseRanks[i];
    }

    if (UNIT_FACTION(unit) == FACTION_BLUE && (unit->level != UNIT_LEVEL_MAX))
        unit->exp = 0;
    else
        unit->exp = UNIT_EXP_DISABLED;
}