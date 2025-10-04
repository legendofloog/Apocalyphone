#include "gbafe.h"
#include "skills.h"

bool HasTerrain(struct BattleUnit* bu){
	if ((bu->terrainAvoid + bu->terrainDefense + bu->terrainResistance) > 0){
		return true;
	}
	return false;
}

void CoverShooter(struct BattleUnit* actor, struct BattleUnit* target){
	if (SkillTester(&actor->unit, CoverShooterID)){
		if (HasTerrain(actor)){
			actor->battleAttack += 4;
			actor->battleCritRate += 15;
		}
	}
}

int IsCapturing(struct Unit* unit){
	return unit->state & (1 << 30);
}

int prHalveIfCapturing(int stat, struct Unit* unit){
	if (IsCapturing(unit)){
		if (!SkillTester(unit, GrapplerID)){
			return stat >> 1;
		}
	}
	return stat;
}

int IsBroken(struct Unit* unit){
	return CheckBit(GetUnitDebuffEntry(unit), BreakBitOffset_Link);
}

void FollowUp(struct BattleUnit* actor, struct BattleUnit* target){
	if (SkillTester(&actor->unit, FollowUpID)){
		if (IsBroken(&target->unit)){
			actor->battleAttack += (actor->battleAttack >> 1);
		}
	}
}

struct WeightedItem {
    u8 number;
    u8 weight;
};

const struct WeightedItem ScavengerLevel1[] = {
	{ITEM_SWORD_IRON, 70},
	{ITEM_SWORD_STEEL, 25},
	{ITEM_VULNERARY, 5}
};

const struct WeightedItem ScavengerLevel5[] = {
	{ITEM_SWORD_IRON, 50},
	{ITEM_SWORD_STEEL, 40},
	{ITEM_SWORD_SILVER, 10}
};

const struct WeightedItem* GetItemList(int level){
	if (level >= 5){
		return ScavengerLevel5;
	}
	else{
		return ScavengerLevel1;
	}
}

void GiveItemFromScavenger(struct Unit* unit){
	int roll = NextRN_100();
	int level = unit->level;
	if (unit->pClassData->attributes & CA_PROMOTED){
		level += 20;
	}
	const struct WeightedItem* list = GetItemList(level);
	int count = 0;
	while(true){
		struct WeightedItem item = list[count];
		count++;
		roll -= item.weight;
		if (roll <= 0){
			gEventSlots[0x3] = item.number;
			break;
		}
	}
	CallEvent(&GiveItemEvent, EV_EXEC_CUTSCENE);
}

void TryDoScavenger(){
	if (gActionData.unitActionType == UNIT_ACTION_WAIT){
		if (SkillTester(gActiveUnit, ScavengerID)){
			if (Roll1RN(GetUnitLuck(gActiveUnit) << 1)){
				GiveItemFromScavenger(gActiveUnit);
			}
		}
	}
}
