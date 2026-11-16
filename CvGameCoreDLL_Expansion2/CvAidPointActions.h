#pragma once

#include <CvEnums.h>
#include <string.h>

// Maps the Type strings of the AidPointActions table to the enum values. The table is the single
// source of truth for cost, discountability and availability: an action that has no row there is
// not available at all (see CvGame::GetAidPointBaseCost), so this list only names the actions.
struct AidPointActionTypeName
{
	const char* szName;
	AidPointActionTypes eType;
};

inline const AidPointActionTypeName* GetAidPointActionTypes(int& iCount)
{
	static const AidPointActionTypeName kActions[] = {
		{ "AID_POINT_ACTION_REROLL_QUESTS",      AID_POINT_ACTION_REROLL_QUESTS },
		{ "AID_POINT_ACTION_NEXT_GREAT_PERSON",  AID_POINT_ACTION_NEXT_GREAT_PERSON },
		{ "AID_POINT_ACTION_PENDING_GIFT_UNIT",  AID_POINT_ACTION_PENDING_GIFT_UNIT },
		{ "AID_POINT_ACTION_HASTEN_CONGRESS",    AID_POINT_ACTION_HASTEN_CONGRESS },
		{ "AID_POINT_ACTION_ANNEX_CITY_STATE",   AID_POINT_ACTION_ANNEX_CITY_STATE },
		{ "AID_POINT_ACTION_DENOUNCE",           AID_POINT_ACTION_DENOUNCE },
		{ "AID_POINT_ACTION_PEACE",              AID_POINT_ACTION_PEACE },
		{ "AID_POINT_ACTION_PERMANENT_VOTE",     AID_POINT_ACTION_PERMANENT_VOTE },
		{ "AID_POINT_ACTION_EMERGENCY_PROPOSAL", AID_POINT_ACTION_EMERGENCY_PROPOSAL },
	};
	iCount = (int)(sizeof(kActions) / sizeof(kActions[0]));
	return kActions;
}

inline AidPointActionTypes GetAidPointActionTypeFromString(const char* szType)
{
	if(szType == NULL)
		return NO_AID_POINT_ACTION;

	int iCount = 0;
	const AidPointActionTypeName* pActions = GetAidPointActionTypes(iCount);
	for(int i = 0; i < iCount; i++)
	{
		if(strcmp(szType, pActions[i].szName) == 0)
			return pActions[i].eType;
	}
	return NO_AID_POINT_ACTION;
}
