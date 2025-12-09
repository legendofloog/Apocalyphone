int GetEquipmentStatBonus(Unit* unit, int stat){
	Item item = GetUnitEquippedItem(unit);

	if (GetItemIndex(item) == 0){
		return 0;
	}

	const ItemData* itemData = GetItemData(GetItemIndex(item));

	const struct ItemStatBonuses* itemStatBonuses = itemData -> pStatBonuses;

	if (itemStatBonuses == 0){
		return 0;
	}

	return ((s8*)itemStatBonuses)[stat];
}
