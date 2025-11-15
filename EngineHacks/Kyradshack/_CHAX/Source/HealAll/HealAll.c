#include "gbafe.h"
//Menu Stuff
//Heal All

//Heal Individual

//After starting fight have a popup 'do you want to heal your units?' if any units below max

//Functionality
//Heal All
int HealAllCheck(){
	for(int i = 0; i < 62; i++){
		struct Unit unit = gUnitArrayBlue[i];
		if(unit.index == 0){
			continue;
		}
		if(unit.state & US_DEAD){
			continue;
		}
		if(unit.curHP < unit.maxHP){
			return true;
		}
	}
	return false;
}

void HealAll(){
	for(int i = 0; i < 62; i++){
		struct Unit unit = gUnitArrayBlue[i];
		if(unit.index == 0){
			continue;
		}
		if(unit.state & US_DEAD){
			continue;
		}
		if(unit.curHP < unit.maxHP){
			struct Unit* unitPointer = GetUnit(unit.index);
			unitPointer->curHP = unit.maxHP;
		}
	}
}


//Find units who need healing, for loop or something? Store all their missing HP
//Calculate all missing HP, multiply for gold value
//When button pressed check gold
//if gold is too low play buzzer and dont do anything
//if gold enough play a chaching sound and then a heal noise, stop cursor temporarily
//For loop(?) again to find each unit missing hp and set it to max
//Take the money

//Heal Individual