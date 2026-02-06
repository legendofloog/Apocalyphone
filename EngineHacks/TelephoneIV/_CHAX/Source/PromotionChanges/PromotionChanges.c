#include "gbafe.h"
#include "magic.h"

struct PromotionBonuses
{
  u8 charID;
  u8 hpBonus;
  u8 strBonus;
  u8 magBonus;
  u8 sklBonus;
  u8 spdBonus;
  u8 defBonus;
  u8 resBonus;
  u8 lckBonus;
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

extern const struct PromotionBonuses UnitPromotionBonusTable[];


const struct PromotionBonuses* GetUnitPromotionBonuses(struct Unit* unit)
{
    for (int i = 0; UnitPromotionBonusTable[i].charID != 0; i++)
    {
        if (UnitPromotionBonusTable[i].charID == unit->pCharacterData->number) { return &UnitPromotionBonusTable[i]; }
    }

    return NULL;
}

void ApplyUnitPromotionSpecific(struct Unit* unit, const struct ClassData* promotedClass, const struct PromotionBonuses* bonuses)
{
    unit->maxHP += bonuses->hpBonus;
    if (unit->maxHP > promotedClass->maxHP) { unit->maxHP = promotedClass->maxHP; }

    unit->pow += bonuses->strBonus;
    if (unit->pow > promotedClass->maxPow) { unit->pow = promotedClass->maxPow; }

    unit->mag += bonuses->magBonus;
    if (unit->mag > MagClassTable[promotedClass->number].maxMag) {unit->mag = MagClassTable[promotedClass->number].maxMag; }

    unit->skl += bonuses->sklBonus;
    if (unit->skl > promotedClass->maxSkl) { unit->skl = promotedClass->maxSkl; }

    unit->spd += bonuses->spdBonus;
    if (unit->spd > promotedClass->maxSpd) { unit->spd = promotedClass->maxSpd; }

    unit->def += bonuses->defBonus;
    if (unit->def > promotedClass->maxDef) { unit->def = promotedClass->maxDef; }

    unit->res += bonuses->resBonus;
    if (unit->res > promotedClass->maxRes) { unit->res = promotedClass->maxRes; }

    unit->lck += bonuses->lckBonus;
    if (unit->lck > 30) { unit->lck = 30; }

    //unit->conBonus += bonuses->conBonus; 
    // uh idk how you are capping con in your project so add that here

    unit->ranks[ITYPE_SWORD] += bonuses->swdWexp;
    if (unit->ranks[ITYPE_SWORD] > WPN_EXP_S) { unit->ranks[ITYPE_SWORD] = WPN_EXP_S; }

    unit->ranks[ITYPE_LANCE] += bonuses->lncWexp;
    if (unit->ranks[ITYPE_LANCE] > WPN_EXP_S) { unit->ranks[ITYPE_LANCE] = WPN_EXP_S; }

    unit->ranks[ITYPE_AXE] += bonuses->axeWexp;
    if (unit->ranks[ITYPE_AXE] > WPN_EXP_S) { unit->ranks[ITYPE_AXE] = WPN_EXP_S; }

    unit->ranks[ITYPE_BOW] += bonuses->swdWexp;
    if (unit->ranks[ITYPE_BOW] > WPN_EXP_S) { unit->ranks[ITYPE_BOW] = WPN_EXP_S; }

    unit->ranks[ITYPE_ANIMA] += bonuses->aniWexp;
    if (unit->ranks[ITYPE_ANIMA] > WPN_EXP_S) { unit->ranks[ITYPE_ANIMA] = WPN_EXP_S; }

    unit->ranks[ITYPE_LIGHT] += bonuses->lgtWexp;
    if (unit->ranks[ITYPE_LIGHT] > WPN_EXP_S) { unit->ranks[ITYPE_LIGHT] = WPN_EXP_S; }

    unit->ranks[ITYPE_DARK] += bonuses->drkWexp;
    if (unit->ranks[ITYPE_DARK] > WPN_EXP_S) { unit->ranks[ITYPE_DARK] = WPN_EXP_S; }

    unit->ranks[ITYPE_STAFF] += bonuses->stfWexp;
    if (unit->ranks[ITYPE_STAFF] > WPN_EXP_S) { unit->ranks[ITYPE_STAFF] = WPN_EXP_S; }

    unit->pClassData = promotedClass;

    unit->level = 1;
    unit->exp = 0;

    unit->curHP += bonuses->hpBonus;
    if (unit->curHP > unit->maxHP) { unit->curHP = unit->maxHP; }

}

void ApplyUnitPromotion(struct Unit* unit, u8 classId) {
    const struct ClassData* promotedClass = GetClassData(classId);

    int baseClassId = unit->pClassData->number;
    int promClassId = promotedClass->number;

    int i;

    const struct PromotionBonuses* bonuses = GetUnitPromotionBonuses(unit);
    if (bonuses) { 
        ApplyUnitPromotionSpecific(unit, promotedClass, bonuses); 
        return; 
    }

    // Apply stat ups

    unit->maxHP += promotedClass->promotionHp;

    if (unit->maxHP > promotedClass->maxHP)
        unit->maxHP = promotedClass->maxHP;

    unit->pow += promotedClass->promotionPow;

    if (unit->pow > promotedClass->maxPow)
        unit->pow = promotedClass->maxPow;

    unit->mag += MagClassTable[promotedClass->number].promotionMag;

    if (unit->mag > MagClassTable[promotedClass->number].maxMag)
        unit->mag = MagClassTable[promotedClass->number].maxMag;

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
    unit->exp   = 0;

    unit->curHP += promotedClass->promotionHp;

    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);
}
