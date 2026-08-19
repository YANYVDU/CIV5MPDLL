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
struct BornGreatPersonSpecialistYieldEntry {
	int m_iSpecialistType;
	int m_iUnitClassType;
	int m_iYieldType;
	int m_iYieldMod;
};

struct BuildingGreatPersonPointsEntry {
	int m_iBuildingClass;
	int m_iSpecialist;
	int m_iPoints;
};

struct BornGreatPersonAllyInfluenceModEntry {
	int m_iUnitClassType;
	int m_iModPerBorn;
};

struct GreatWorkGreatPersonPointsEntry {
	int m_iGreatWorkClassType;
	int m_iSpecialistType;
	int m_iRate;
	bool m_bCapitalOnly;
};

struct InternalTRToUCSPerEraYieldEntry {
	int m_iYieldType;
	int m_iYieldValue;
};

struct YieldToYieldViaTRToUCSEntry {
	int m_iInYieldType;
	int m_iOutYieldType;
	int m_iPercent;
};

struct PurchasedBuildingXPEntry {
	int m_iBuildingClass;
	int m_iDomain;
	int m_iXP;
};

struct UnitBornYieldEntry {
	int m_iMinorCivType;
	int m_iUnitClass;
	int m_iYieldType;
	int m_iYieldMod;
};

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
	int GetGreatPersonPoints(int i) const;
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
	int GetMilitaryUnitProductionXP() const;
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
	int GetIdeologyPressurePerDonationHappiness() const;
	// Genoa (ReNaYa): gold-gift influence +% per sea trade route
	int GetGoldDonationInfluenceModifierPerSeaRoute() const;
	// Malacca (MaLiuJia)
	int GetLuxuryHappinessModifier() const;
	int GetFoodKeptModifierPerLuxury() const;
	int GetTradeRouteGoldModifierPerLuxuryType() const;
	// Panama (BaNaMa)
	int GetTradeRouteGoldModifierPerDistance() const;
	int GetUnhappinessReductionPerCrossContinentRoute() const;
	const std::vector<BornGreatPersonSpecialistYieldEntry>& GetBornGreatPersonSpecialistYieldEntries() const { return m_vBornGreatPersonSpecialistYield; }
	const std::vector<BuildingGreatPersonPointsEntry>& GetBuildingGreatPersonPointsEntries() const { return m_vBuildingGPP; }
	const std::vector<BornGreatPersonAllyInfluenceModEntry>& GetBornAllyInfluenceModEntries() const { return m_vBornAllyInfluenceMod; }
	// Prague (BuLaGe) / Yerevan (AiLiWen): building-class yield percentage modifiers
	int GetBuildingClassYieldModifiers(int i, int j) const;
	// Brussels (BuLuSaiEr): specialist great person point accumulation rate (%)
	int GetSpecialistPointRate(int i) const;
	// Brussels: each great work of a class grants great person points to a specialist (Rate=100 => 1 great work = 1 point)
	const std::vector<GreatWorkGreatPersonPointsEntry>& GetGreatWorkGreatPersonPointsEntries() const { return m_vGreatWorkGreatPersonPoints; }
	// Brussels: specified unit class's one-shot great person output modifier (%)
	int GetGreatPersonOneShotModifier(int i) const;
	// Colombo: flat per-era yield on the international trade route (InternalTR) to this city-state (UCS)
	const std::vector<InternalTRToUCSPerEraYieldEntry>& GetInternalTRToUCSPerEraYieldEntries() const { return m_vInternalTRToUCSPerEraYield; }
	int GetInternalTRToUCSPerEraYield(int eYield) const;
	// Colombo: in cities with a trade route to this city-state (UCS), a percentage of the input yield is granted as extra output yield
	int GetYieldToYieldViaTRToUCS(int eInYield, int eOutYield) const;
	// Valletta: enemy city besieged by >= this many of our combat units cannot heal
	int GetEnemyCityNoHealBesiegeCount() const;
	// Valletta: buying the specified building class grants all units of the specified domain XP
	const std::vector<PurchasedBuildingXPEntry>& GetPurchasedBuildingXPEntries() const { return m_vPurchasedBuildingXP; }
	// Valletta: born unit of the specified unit class grants a configurable yield = YieldMod% of influence with MinorCivType
	const std::vector<UnitBornYieldEntry>& GetUnitBornYieldEntries() const { return m_vUnitBornYield; }
	// Prague: a city with our own spy garrisoned in it grants yield percentage modifiers (per YieldType)
	int GetSpyGarrisonYieldModifiers(int i) const;
	// Prague: killing an enemy spy grants spy progress toward a new spy (100 = kill 1 gain 1)
	int GetSpyKillGainSpyProgress() const;
	// Melbourne: a city that owns the specified improved resource grants yield percentage modifiers (per YieldType)
	int GetResourceYieldModifiers(int i, int j) const;
	// Antananarivo
	int GetCoastalCityGrowthThresholdModifier() const;
	int GetDiplomaticPrestigePerCity() const;
	int GetImprovementYieldModifiers(int i, int j) const;
	// Zanzibar: each worked plot holding the specified improvement grants flat local happiness
	int GetImprovementHappiness(int i) const;

private:
	// Florence
	int m_iFaithPurchaseGreatPeopleCostRiseModifier;
	int m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW;
	int* m_piGreatPersonPoints;
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
	int m_iMilitaryUnitProductionXP;
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
	int m_iIdeologyPressurePerDonationHappiness;
	// Genoa
	int m_iGoldDonationInfluenceModifierPerSeaRoute;
	// Malacca
	int m_iLuxuryHappinessModifier;
	int m_iFoodKeptModifierPerLuxury;
	int m_iTradeRouteGoldModifierPerLuxuryType;
	// Panama
	int m_iTradeRouteGoldModifierPerDistance;
	int m_iUnhappinessReductionPerCrossContinentRoute;
	// Prague / Yerevan
	int** m_ppiBuildingClassYieldModifiers;
	// Brussels
	int* m_piSpecialistPointRate;
	std::vector<GreatWorkGreatPersonPointsEntry> m_vGreatWorkGreatPersonPoints;
	int* m_piGreatPersonOneShotModifier;
	std::vector<BornGreatPersonSpecialistYieldEntry> m_vBornGreatPersonSpecialistYield;
	std::vector<BuildingGreatPersonPointsEntry> m_vBuildingGPP;
	std::vector<BornGreatPersonAllyInfluenceModEntry> m_vBornAllyInfluenceMod;
	std::vector<InternalTRToUCSPerEraYieldEntry> m_vInternalTRToUCSPerEraYield;
	std::vector<YieldToYieldViaTRToUCSEntry> m_vYieldToYieldViaTRToUCS;
	// Valletta
	int m_iEnemyCityNoHealBesiegeCount;
	std::vector<PurchasedBuildingXPEntry> m_vPurchasedBuildingXP;
	std::vector<UnitBornYieldEntry> m_vUnitBornYield;
	// Prague
	int* m_piSpyGarrisonYieldModifiers;
	int m_iSpyKillGainSpyProgress;
	// Melbourne
	int** m_ppiResourceYieldModifiers;
	// Antananarivo
	int m_iCoastalCityGrowthThresholdModifier;
	int m_iDiplomaticPrestigePerCity;
	int** m_ppiImprovementYieldModifiers;
	// Zanzibar
	int* m_piImprovementHappiness;
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
class CvCity;
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
	int GetGreatPersonPoints(int i) const;
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
	int GetMilitaryUnitProductionXP() const;
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
	int GetIdeologyPressurePerDonationHappiness() const;
	// Genoa
	int GetGoldDonationInfluenceModifierPerSeaRoute() const;
	// Malacca
	int GetLuxuryHappinessModifier() const;
	int GetFoodKeptModifierPerLuxury() const;
	int GetTradeRouteGoldModifierPerLuxuryType() const;
	// Panama
	int GetTradeRouteGoldModifierPerDistance() const;
	int GetUnhappinessReductionPerCrossContinentRoute() const;
	int GetSpecialistYieldFromBornGreatPerson(SpecialistTypes eSpecialist, YieldTypes eYield) const;
	int GetBuildingGreatPersonPointsForCity(const CvCity* pCity, SpecialistTypes eSpecialist) const;
	int GetAllyInfluenceModFromBornGreatPerson() const;
	// Prague / Yerevan: building-class yield percentage modifiers
	int GetBuildingClassYieldModifier(BuildingClassTypes eBuildingClass, YieldTypes eYieldType) const;
	bool HasBuildingClassYieldModifiers() const;
	// Brussels
	int GetSpecialistPointRate(SpecialistTypes eSpecialist) const;
	int GetGreatWorkGreatPersonPointsForCity(const CvCity* pCity, SpecialistTypes eSpecialist) const;
	bool HasGreatWorkGreatPersonPoints() const;
	int GetGreatPersonOneShotModifier(UnitClassTypes eUnitClass) const;
	// Valletta
	int GetEnemyCityNoHealBesiegeCount() const;
	const std::vector<PurchasedBuildingXPEntry>& GetPurchasedBuildingXPEntries() const;
	const std::vector<UnitBornYieldEntry>& GetUnitBornYieldEntries() const;
	int GetSpyGarrisonYieldModifier(YieldTypes eYieldType) const;
	bool HasSpyGarrisonYieldModifiers() const;
	int GetSpyKillGainSpyProgress() const;
	int GetResourceYieldModifier(ResourceTypes eResource, YieldTypes eYield) const;
	bool HasResourceYieldModifiers() const;
	// Antananarivo
	int GetCoastalCityGrowthThresholdModifier() const;
	int GetDiplomaticPrestigePerCity() const;
	int GetImprovementYieldModifier(ImprovementTypes eImprovement, YieldTypes eYield) const;
	bool HasImprovementYieldModifiers() const;
	// Zanzibar
	int GetImprovementHappiness(ImprovementTypes eImprovement) const;
	bool HasImprovementHappiness() const;

	void Reset();

protected:
	CvPlayer* m_pPlayer;

	// Accumulated modifier values
	int m_iFaithPurchaseGreatPeopleCostRiseModifier;
	int m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW;
	std::vector<int> m_aiGreatPersonPoints;
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
	int m_iMilitaryUnitProductionXP;
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
	int m_iIdeologyPressurePerDonationHappiness;
	// Genoa
	int m_iGoldDonationInfluenceModifierPerSeaRoute;
	int m_iLuxuryHappinessModifier;
	int m_iFoodKeptModifierPerLuxury;
	int m_iTradeRouteGoldModifierPerLuxuryType;
	int m_iTradeRouteGoldModifierPerDistance;
	int m_iUnhappinessReductionPerCrossContinentRoute;
	// Prague / Yerevan
	int** m_ppiBuildingClassYieldModifiers;
	int m_iBuildingClassYieldModifierCount;
	// Brussels
	std::vector<int> m_aiSpecialistPointRate;
	std::vector<GreatWorkGreatPersonPointsEntry> m_vGreatWorkGreatPersonPoints;
	std::vector<int> m_aiGreatPersonOneShotModifier;
	std::vector<BornGreatPersonSpecialistYieldEntry> m_vBornGreatPersonSpecialistYield;
	std::vector<BuildingGreatPersonPointsEntry> m_vBuildingGPP;
	std::vector<BornGreatPersonAllyInfluenceModEntry> m_vBornAllyInfluenceMod;
	// Valletta
	int m_iEnemyCityNoHealBesiegeCount;
	std::vector<PurchasedBuildingXPEntry> m_vPurchasedBuildingXP;
	std::vector<UnitBornYieldEntry> m_vUnitBornYield;
	// Prague
	std::vector<int> m_aiSpyGarrisonYieldModifiers;
	int m_iSpyGarrisonYieldModifierCount;
	int m_iSpyKillGainSpyProgress;
	// Melbourne
	int** m_ppiResourceYieldModifiers;
	int m_iResourceYieldModifierCount;
	// Antananarivo
	int m_iCoastalCityGrowthThresholdModifier;
	int m_iDiplomaticPrestigePerCity;
	int** m_ppiImprovementYieldModifiers;
	int m_iImprovementYieldModifierCount;
	// Zanzibar
	std::vector<int> m_aiImprovementHappiness;
	int m_iImprovementHappinessCount;
};

#endif // CVCITYSTATEUACLASSES_H
