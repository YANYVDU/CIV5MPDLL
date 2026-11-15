#pragma once

#include <CvEnums.h>
#include <string.h>

// Fallback definition of the aid point actions, used when the AidPointActions
// table cannot be queried and to map the table's Type strings to the enum.
struct AidPointActionDefaults
{
	const char* szName;
	AidPointActionTypes eType;
	const char* szCostOption;
	int iDefaultCost;
	bool bDiscountable;
};

inline const AidPointActionDefaults* GetAidPointActionDefaults(int& iCount)
{
	static const AidPointActionDefaults kActions[] = {
		{ "AID_POINT_ACTION_REROLL_QUESTS",       AID_POINT_ACTION_REROLL_QUESTS,       "SP_AID_POINT_COST_REROLL_QUESTS",       20, true },
		{ "AID_POINT_ACTION_NEXT_GREAT_PERSON",   AID_POINT_ACTION_NEXT_GREAT_PERSON,   "SP_AID_POINT_COST_NEXT_GREAT_PERSON",   50, true },
		{ "AID_POINT_ACTION_PENDING_GIFT_UNIT",   AID_POINT_ACTION_PENDING_GIFT_UNIT,   "SP_AID_POINT_COST_PENDING_GIFT_UNIT",   10, true },
		{ "AID_POINT_ACTION_HASTEN_CONGRESS",     AID_POINT_ACTION_HASTEN_CONGRESS,     "SP_AID_POINT_COST_HASTEN_CONGRESS",     20, true },
		{ "AID_POINT_ACTION_ANNEX_CITY_STATE",    AID_POINT_ACTION_ANNEX_CITY_STATE,    "SP_AID_POINT_COST_ANNEX_CITY_STATE",    50, true },
		{ "AID_POINT_ACTION_DENOUNCE",            AID_POINT_ACTION_DENOUNCE,            "SP_AID_POINT_COST_DENOUNCE",            20, true },
		{ "AID_POINT_ACTION_PEACE",               AID_POINT_ACTION_PEACE,               "SP_AID_POINT_COST_PEACE",               40, true },
		{ "AID_POINT_ACTION_PERMANENT_VOTE",      AID_POINT_ACTION_PERMANENT_VOTE,      "SP_AID_POINT_COST_PERMANENT_VOTE",      60, true },
		{ "AID_POINT_ACTION_EMERGENCY_PROPOSAL",  AID_POINT_ACTION_EMERGENCY_PROPOSAL,  "SP_AID_POINT_COST_EMERGENCY_PROPOSAL",  40, true },
	};
	iCount = (int)(sizeof(kActions) / sizeof(kActions[0]));
	return kActions;
}

inline AidPointActionTypes GetAidPointActionTypeFromString(const char* szType)
{
	if(szType == NULL)
		return NO_AID_POINT_ACTION;

	int iCount = 0;
	const AidPointActionDefaults* pActions = GetAidPointActionDefaults(iCount);
	for(int i = 0; i < iCount; i++)
	{
		if(strcmp(szType, pActions[i].szName) == 0)
			return pActions[i].eType;
	}
	return NO_AID_POINT_ACTION;
}
