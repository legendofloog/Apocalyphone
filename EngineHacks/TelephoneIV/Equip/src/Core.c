#define US_UNEQUIPMENT (1 << 31)

bool IsItemEquipment (Item item){
	extern u8 EquipmentList[];

	if (GetItemIndex(item) == 0) {
		return false;
	}

	int cnt = 0;

	while(EquipmentList[cnt] != 0){
		if (GetItemIndex(item) == EquipmentList[cnt]){
			return true;
		}
		++cnt;
	}

	return false;
}

bool CanUnitEquipItem (Unit* unit, Item item){
	if (IsItemEquipment(item) != true){
		return false;
	}

	// Can add extra conditionals here

	if (GetItemIndex(item) == 0xE2){
		if (GetItemData(GetItemIndex(GetUnitEquippedWeapon(unit)))->weaponType != ITYPE_BOW){
			return false;
		}
	}

	return true;
}

Item GetUnitEquippedItem (Unit* unit){
	Item item = 0;

	if (unit->state & US_UNEQUIPMENT){
		return item;
	}

	for (int i = 0; i < UNIT_ITEM_COUNT; ++i){
		if (CanUnitEquipItem(unit, unit->items[i])){
			return unit->items[i];
		}
	}

	return item;
}

int GetUnitEquippedItemSlot (Unit* unit){
	if (unit->state & US_UNEQUIPMENT){
		return 0xFF;
	}

	for (int i = 0; i < UNIT_ITEM_COUNT; ++i){
		if (CanUnitEquipItem(unit, unit->items[i])){
			return i;
		}
	}

	return 0xFF;
}
