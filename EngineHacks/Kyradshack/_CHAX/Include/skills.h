extern int SkillTester(struct Unit* unit, int id);
//extern void SetBit(u32* address, u8 bitOffset);
//extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(struct Unit* unit);

extern int BreakBitOffset_Link;
extern const u16 GiveItemEvent;

#include "../../../SkillSystem/skill_definitions.event"