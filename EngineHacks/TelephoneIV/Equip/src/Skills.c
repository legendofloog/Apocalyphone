int GetEquipmentSkill(Unit* unit){
	Item item = GetUnitEquippedItem(unit);

	if (GetItemIndex(item) == 0){
		return 0;
	}

	return GetItemData(GetItemIndex(item))->skill;
}
