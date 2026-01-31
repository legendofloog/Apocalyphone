#include "gbafe.h"
#include "magic.h"
struct PromotionEntry
{
  u8 charID;
  u8 hpBonus;
  u8 strBonus;
  u8 magBonus;
  u8 sklBonus;
  u8 spdBonus;
  u8 defBonus;
  u8 resBonus;
  u8 lukBonus;
  u8 conBonus;
  u8 swdWexp;
  u8 lncWexp;
  u8 axeWexp;
  u8 bowWexp;
  u8 aniWexp;
  u8 lgtWexp;
  u8 drkWexp;
  u8 stfWexp;
  u8 _pad[2];
};

extern struct PromotionEntry UnitPromotionEntries[];


//not sure what if anything needs to change here?
void ExecUnitPromotion(struct Unit* unit, u8 classId, int itemIdx, s8 unk) {

    if (itemIdx != -1) {
        gBattleActor.weaponBefore = gBattleTarget.weaponBefore = unit->items[itemIdx];
    }

    gBattleActor.weapon = gBattleTarget.weapon = GetUnitEquippedWeapon(unit);

    InitBattleUnitWithoutBonuses(&gBattleTarget, unit);

    ApplyUnitPromotion(unit, classId);

    InitBattleUnitWithoutBonuses(&gBattleActor, unit);

    GenerateBattleUnitStatGainsComparatively(&gBattleActor, &gBattleTarget.unit);

    SetBattleUnitTerrainBonusesAuto(&gBattleActor);
    SetBattleUnitTerrainBonusesAuto(&gBattleTarget);

    if (unk) {
        unit->state |= US_HAS_MOVED;
    }

    if (itemIdx != -1) {
        UnitUpdateUsedItem(unit, itemIdx);
    }

    gBattleHitArray[0].attributes = 0;
    gBattleHitArray[0].info = BATTLE_HIT_INFO_END;
    gBattleHitArray[0].hpChange = 0;

    gBattleStats.config = BATTLE_CONFIG_PROMOTION;

    return;
}

//Planned changes
//void ApplyUnitPromotion(struct Unit* unit, u8 classId){
//    const struct ClassData* promotedClass = GetClassData(classId);
//    //get the unit name
//    //use name to for loop table of promo bonuses including HP, Str, Mag, Skl, Spd, Lck, Def, Res, Wexp
//    //Con and mov seem to be handled by just changing the class, need to find a way to do con manually
//    //save promo bonuses as variables and then apply
//    //reset level and maybe exp
//    //keep ewan thing i guess 
//}


void ApplyUnitPromotion(struct Unit* unit, u8 classId) {
    const struct ClassData* promotedClass = GetClassData(classId);

    int baseClassId = unit->pClassData->number;
    int promClassId = promotedClass->number;

    int i;

    // Apply stat ups

    unit->maxHP += promotedClass->promotionHp;

    if (unit->maxHP > promotedClass->maxHP)
        unit->maxHP = promotedClass->maxHP;

    unit->pow += promotedClass->promotionPow;

    if (unit->pow > promotedClass->maxPow)
        unit->pow = promotedClass->maxPow;

    unit->skl += promotedClass->promotionSkl;

    if (unit->skl > promotedClass->maxSkl)
        unit->skl = promotedClass->maxSkl;

    unit->spd += promotedClass->promotionSpd;

    if (unit->spd > promotedClass->maxSpd)
        unit->spd = promotedClass->maxSpd;

    unit->def += promotedClass->promotionDef;

    if (unit->def > promotedClass->maxDef)
        unit->def = promotedClass->maxDef;

    unit->res += promotedClass->promotionRes;

    if (unit->res > promotedClass->maxRes)
        unit->res = promotedClass->maxRes;

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i)
        unit->ranks[i] -= unit->pClassData->baseRanks[i];

    // Update unit class
    unit->pClassData = promotedClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i) {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > WPN_EXP_S)
            wexp = WPN_EXP_S;

        unit->ranks[i] = wexp;
    }

    // If Pupil -> Shaman promotion, set Anima rank to 0
    if (baseClassId == CLASS_PUPIL && promClassId == CLASS_SHAMAN)
        unit->ranks[ITYPE_ANIMA] = 0;

    unit->level = 1;
    //I could comment this out? maybe? that would be awesome
    unit->exp   = 0;

    unit->curHP += promotedClass->promotionHp;

    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);
}


//Wip code, its not good
void ApplyUnitPromotionNew(struct Unit* unit, u8 classId){

    const struct ClassData* promotedClass = GetClassData(classId);

    int baseClassId = unit->pClassData->number;
    int promClassId = promotedClass->number;
    int unitCharId = unit->pCharacterData->number;

    int i = 0;

    //Find unit stat ups
    while(true){
        u8 character = UnitPromotionEntries[i];
        if(unitCharId == character){
            //break
            //grab bonuses from entry
        }
        if (character = 0){
            break;
        }
        i++;
    }

void GetUnitPromotionEntry(struct Unit* unit){
    //code goes here
}
