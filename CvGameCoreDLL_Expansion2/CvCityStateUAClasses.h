/*	-------------------------------------------------------------------------------------------------------
	City-State Unique Ability System for Super Power V11 (Pillars of Sovereignty)
	------------------------------------------------------------------------------------------------------- */
#ifndef CVCITYSTATEUACLASSES_H
#define CVCITYSTATEUACLASSES_H

#pragma once

#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreDLLUtil.h"

//======================================================================================================
// CvCityStateUAEffectEntry - mirrors CityStateUAEffects database table
//======================================================================================================
class CvCityStateUAEffectEntry : public CvBaseInfo
{
public:
	CvCityStateUAEffectEntry(void);
	~CvCityStateUAEffectEntry(void);

	bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

	// Florence (Fulolunsi)
	int GetFaithPurchaseGreatPeopleCostRiseModifier() const;
	int GetFaithPurchaseGreatPeopleCostRiseModifierPerGW() const;
	bool IsFaithPurchaseAllGreatPeople() const;
	// Buenos Aires (BuYiNuoSiAiLiSi)
	bool IsGPNoDeathAfterGreatWork() const;
	int GetGPConcertTourismRetentionPercent() const;
	// Brussels (BuLuSaiEr)
	int GetGreatMusicianConcertTourismModifier() const;
	int GetGreatMusicianConcertGoldPercent() const;
	// Bratislava (BuLaDiSiLaFa)
	int GetCapitalAndSecondCapitalCultureModifier() const;
	// Kiev (JiFu)
	int GetCapitalCultureModifierPerTurn() const;
	int GetCapitalFaithModifierPerTurn() const;
	int GetCapitalPerTurnYieldModifierMax() const;
	// Bucharest (BuJiaLeSiTe)
	int GetImmigrationRatePerImmigrant() const;
	int GetImmigrationRateMax() const;
	int GetEmigrationRatePerImmigrant() const;
	int GetEmigrationRateMax() const;
	// Kuala Lumpur (JiLongPo)
	bool IsPuppetNoTechCostPenalty() const;
	int GetPuppetTechCostPartial() const;
	// Almaty (ALaMuTu)
	bool IsCanPillageNeutralTradeRoute() const;
	// Belgrade (BeiErGeLaiDe)
	int GetGarrisonCityDefenseModifier() const;
	// Budapest (BuDaPeiSi)
	bool IsLandUnitsImmuneRiverCrossing() const;
	// Ha Noi (HeNei)
	int GetEnemyFixedDamageModifierInBorders() const;
	int GetCulturePerWarPeace() const;
	int GetEnemyCombatModifierInBordersPerBeenDoW() const;
	// Mbanza Kongo (MuBanZhaGangGuo)
	int GetUnitProductionModifierPerCity() const;
	int GetManpowerPerCity() const;
	int GetCombatBonusPerTechDifference() const;
	// Sidon (XiDun)
	int GetNavalAttackIgnoreBuildingDefense() const;
	int GetForeignRegenPercent() const;
	// Sofia (SuoFeiYa)
	int GetHillsCityDamageReduction() const;
	int GetHillsMovementModifier() const;
	int GetHillsCityRangeBonus() const;
	// Vatican (FanDiGang)
	int GetReligionSpreadSpeedModifier() const;
	// Kyzyl (KeZiLe)
	int GetLandTradeRouteDistancePerTradeSlot() const;
	// Dubai (DiBai)
	int GetHappinessPerGoldDonated() const;
	int GetGoldDonationInterval() const;
	int GetWonderProductionPerDonationHappiness() const;
	// Malacca (MaLiuJia)
	int GetLuxuryHappinessModifier() const;

private:
	// Florence
	int m_iFaithPurchaseGreatPeopleCostRiseModifier;
	int m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW;
	bool m_bFaithPurchaseAllGreatPeople;
	// Buenos Aires
	bool m_bGPNoDeathAfterGreatWork;
	int m_iGPConcertTourismRetentionPercent;
	// Brussels
	int m_iGreatMusicianConcertTourismModifier;
	int m_iGreatMusicianConcertGoldPercent;
	// Bratislava
	int m_iCapitalAndSecondCapitalCultureModifier;
	// Kiev
	int m_iCapitalCultureModifierPerTurn;
	int m_iCapitalFaithModifierPerTurn;
	int m_iCapitalPerTurnYieldModifierMax;
	// Bucharest
	int m_iImmigrationRatePerImmigrant;
	int m_iImmigrationRateMax;
	int m_iEmigrationRatePerImmigrant;
	int m_iEmigrationRateMax;
	// Kuala Lumpur
	bool m_bPuppetNoTechCostPenalty;
	int m_iPuppetTechCostPartial;
	// Almaty
	bool m_bCanPillageNeutralTradeRoute;
	// Belgrade
	int m_iGarrisonCityDefenseModifier;
	// Budapest
	bool m_bLandUnitsImmuneRiverCrossing;
	// Ha Noi
	int m_iEnemyFixedDamageModifierInBorders;
	int m_iCulturePerWarPeace;
	int m_iEnemyCombatModifierInBordersPerBeenDoW;
	// Mbanza Kongo
	int m_iUnitProductionModifierPerCity;
	int m_iManpowerPerCity;
	int m_iCombatBonusPerTechDifference;
	// Sidon
	int m_iNavalAttackIgnoreBuildingDefense;
	int m_iForeignRegenPercent;
	// Sofia
	int m_iHillsCityDamageReduction;
	int m_iHillsMovementModifier;
	int m_iHillsCityRangeBonus;
	// Vatican
	int m_iReligionSpreadSpeedModifier;
	// Kyzyl
	int m_iLandTradeRouteDistancePerTradeSlot;
	// Dubai
	int m_iHappinessPerGoldDonated;
	int m_iGoldDonationInterval;
	int m_iWonderProductionPerDonationHappiness;
	// Malacca
	int m_iLuxuryHappinessModifier;
};

//======================================================================================================
// CvCityStateUAEffectXMLEntries - container for all CityStateUAEffects entries
//======================================================================================================
class CvCityStateUAEffectXMLEntries
{
public:
	CvCityStateUAEffectXMLEntries(void);
	~CvCityStateUAEffectXMLEntries(void);

	std::vector<CvCityStateUAEffectEntry*>& GetEffectEntries();
	int GetNumEffects() const;
	CvCityStateUAEffectEntry* GetEntry(int index) const;
	CvCityStateUAEffectEntry* GetEntryByType(const char* szType) const;
	void DeleteArray();

private:
	std::vector<CvCityStateUAEffectEntry*> m_paEffectEntries;
};

//======================================================================================================
// CvCityStateUAEntry - mirrors CityStateUAs database table
//======================================================================================================
class CvCityStateUAEntry : public CvBaseInfo
{
public:
	CvCityStateUAEntry(void);
	~CvCityStateUAEntry(void);

	bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

	int GetAllyEffectID() const;
	int GetFriendEffectID() const;

private:
	int m_iAllyEffectID;
	int m_iFriendEffectID;
};

//======================================================================================================
// CvCityStateUAXMLEntries - container for all CityStateUAs entries
//======================================================================================================
class CvCityStateUAXMLEntries
{
public:
	CvCityStateUAXMLEntries(void);
	~CvCityStateUAXMLEntries(void);

	std::vector<CvCityStateUAEntry*>& GetUAEntries();
	int GetNumUAs() const;
	CvCityStateUAEntry* GetEntry(int index) const;
	CvCityStateUAEntry* GetEntryByType(const char* szType) const;
	void DeleteArray();

private:
	std::vector<CvCityStateUAEntry*> m_paUAEntries;
};

//======================================================================================================
// CvPlayerCityStateUA - per-player cache of active city-state UA effects
//======================================================================================================
class CvPlayer;
class CvPlayerCityStateUA
{
public:
	CvPlayerCityStateUA();
	~CvPlayerCityStateUA();

	void Init(CvPlayer* pPlayer);
	void Uninit();

	// Add / remove an effect when friendship status changes
	void ApplyEffect(int iEffectID, int iChange);  // iChange = +1 (apply) or -1 (remove)

	// Query accumulated modifier values for each effect type
	// Florence
	int GetFaithPurchaseGreatPeopleCostRiseModifier() const;
	int GetFaithPurchaseGreatPeopleCostRiseModifierPerGW() const;
	bool IsFaithPurchaseAllGreatPeople() const;
	// Buenos Aires
	bool IsGPNoDeathAfterGreatWork() const;
	int GetGPConcertTourismRetentionPercent() const;
	// Brussels
	int GetGreatMusicianConcertTourismModifier() const;
	int GetGreatMusicianConcertGoldPercent() const;
	// Bratislava
	int GetCapitalAndSecondCapitalCultureModifier() const;
	// Kiev
	int GetCapitalCultureModifierPerTurn() const;
	int GetCapitalFaithModifierPerTurn() const;
	int GetCapitalPerTurnYieldModifierMax() const;
	// Bucharest
	int GetImmigrationRatePerImmigrant() const;
	int GetImmigrationRateMax() const;
	int GetEmigrationRatePerImmigrant() const;
	int GetEmigrationRateMax() const;
	// Kuala Lumpur
	bool IsPuppetNoTechCostPenalty() const;
	int GetPuppetTechCostPartial() const;
	// Almaty
	bool IsCanPillageNeutralTradeRoute() const;
	// Belgrade
	int GetGarrisonCityDefenseModifier() const;
	// Budapest
	bool IsLandUnitsImmuneRiverCrossing() const;
	// Ha Noi
	int GetEnemyFixedDamageModifierInBorders() const;
	int GetCulturePerWarPeace() const;
	int GetEnemyCombatModifierInBordersPerBeenDoW() const;
	// Mbanza Kongo
	int GetUnitProductionModifierPerCity() const;
	int GetManpowerPerCity() const;
	int GetCombatBonusPerTechDifference() const;
	// Sidon
	int GetNavalAttackIgnoreBuildingDefense() const;
	int GetForeignRegenPercent() const;
	// Sofia
	int GetHillsCityDamageReduction() const;
	int GetHillsMovementModifier() const;
	int GetHillsCityRangeBonus() const;
	// Vatican
	int GetReligionSpreadSpeedModifier() const;
	// Kyzyl
	int GetLandTradeRouteDistancePerTradeSlot() const;
	// Dubai
	int GetHappinessPerGoldDonated() const;
	int GetGoldDonationInterval() const;
	int GetWonderProductionPerDonationHappiness() const;
	// Malacca
	int GetLuxuryHappinessModifier() const;

	void Reset();

protected:
	CvPlayer* m_pPlayer;

	// Accumulated modifier values
	int m_iFaithPurchaseGreatPeopleCostRiseModifier;
	int m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW;
	int m_iFaithPurchaseAllGreatPeopleCount;
	int m_iGPNoDeathAfterGreatWorkCount;
	int m_iGPConcertTourismRetentionPercent;
	int m_iGreatMusicianConcertTourismModifier;
	int m_iGreatMusicianConcertGoldPercent;
	int m_iCapitalAndSecondCapitalCultureModifier;
	int m_iCapitalCultureModifierPerTurn;
	int m_iCapitalFaithModifierPerTurn;
	int m_iCapitalPerTurnYieldModifierMax;
	int m_iImmigrationRatePerImmigrant;
	int m_iImmigrationRateMax;
	int m_iEmigrationRatePerImmigrant;
	int m_iEmigrationRateMax;
	int m_iPuppetNoTechCostPenaltyCount;
	int m_iPuppetTechCostPartial;
	int m_iCanPillageNeutralTradeRouteCount;
	int m_iGarrisonCityDefenseModifier;
	int m_iLandUnitsImmuneRiverCrossingCount;
	int m_iEnemyFixedDamageModifierInBorders;
	int m_iCulturePerWarPeace;
	int m_iEnemyCombatModifierInBordersPerBeenDoW;
	int m_iUnitProductionModifierPerCity;
	int m_iManpowerPerCity;
	int m_iCombatBonusPerTechDifference;
	int m_iNavalAttackIgnoreBuildingDefense;
	int m_iForeignRegenPercent;
	int m_iHillsCityDamageReduction;
	int m_iHillsMovementModifier;
	int m_iHillsCityRangeBonus;
	int m_iReligionSpreadSpeedModifier;
	int m_iLandTradeRouteDistancePerTradeSlot;
	int m_iHappinessPerGoldDonated;
	int m_iGoldDonationInterval;
	int m_iWonderProductionPerDonationHappiness;
	int m_iLuxuryHappinessModifier;
};

#endif // CVCITYSTATEUACLASSES_H
