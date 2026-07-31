/*	-------------------------------------------------------------------------------------------------------
	City-State Unique Ability System for Super Power V11 (Pillars of Sovereignty)
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreDLLUtil.h"
#include "CvCityStateUAClasses.h"
#include "CvPlayer.h"

#include "LintFree.h"

//======================================================================================================
// CvCityStateUAEffectEntry
//======================================================================================================
CvCityStateUAEffectEntry::CvCityStateUAEffectEntry(void)
	: m_iFaithPurchaseGreatPeopleCostRiseModifier(0)
	, m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW(0)
	, m_piGreatPersonPoints(nullptr)
	, m_bFaithPurchaseAllGreatPeople(false)
	, m_bGPNoDeathAfterGreatWork(false)
	, m_iGPConcertTourismRetentionPercent(0)
	, m_iGreatMusicianConcertTourismModifier(0)
	, m_iGreatMusicianConcertGoldPercent(0)
	, m_iCapitalAndSecondCapitalCultureModifier(0)
	, m_iCapitalCultureModifierPerTurn(0)
	, m_iCapitalFaithModifierPerTurn(0)
	, m_iCapitalPerTurnYieldModifierMax(0)
	, m_iImmigrationRatePerImmigrant(0)
	, m_iImmigrationRateMax(0)
	, m_iEmigrationRatePerImmigrant(0)
	, m_iEmigrationRateMax(0)
	, m_bPuppetNoTechCostPenalty(false)
	, m_iPuppetTechCostPartial(0)
	, m_bCanPillageNeutralTradeRoute(false)
	, m_iGarrisonCityDefenseModifier(0)
	, m_bLandUnitsImmuneRiverCrossing(false)
	, m_iEnemyFixedDamageModifierInBorders(0)
	, m_iCulturePerWarPeace(0)
	, m_iEnemyCombatModifierInBordersPerBeenDoW(0)
	, m_iUnitProductionModifierPerCity(0)
	, m_iManpowerPerCity(0)
	, m_iCombatBonusPerTechDifference(0)
	, m_iNavalAttackIgnoreBuildingDefense(0)
	, m_iForeignRegenPercent(0)
	, m_iHillsCityDamageReduction(0)
	, m_iHillsMovementModifier(0)
	, m_iHillsCityRangeBonus(0)
	, m_iReligionSpreadSpeedModifier(0)
	, m_iLandTradeRouteDistancePerTradeSlot(0)
	, m_iHappinessPerGoldDonated(0)
	, m_iGoldDonationInterval(0)
	, m_iWonderProductionPerDonationHappiness(0)
	, m_iLuxuryHappinessModifier(0)
{
}

CvCityStateUAEffectEntry::~CvCityStateUAEffectEntry(void)
{
	SAFE_DELETE_ARRAY(m_piGreatPersonPoints);
}

bool CvCityStateUAEffectEntry::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	m_iFaithPurchaseGreatPeopleCostRiseModifier			= kResults.GetInt("FaithPurchaseGreatPeopleCostRiseModifier");
	m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW		= kResults.GetInt("FaithPurchaseGreatPeopleCostRiseModifierPerGW");
	kUtility.PopulateArrayByValue(m_piGreatPersonPoints, "Specialists", "CityStateUAEffect_GreatPersonPoints", "SpecialistType", "EffectType", GetType(), "Points");
	m_bFaithPurchaseAllGreatPeople					= kResults.GetBool("FaithPurchaseAllGreatPeople");

	m_bGPNoDeathAfterGreatWork						= kResults.GetBool("GPNoDeathAfterGreatWork");
	m_iGPConcertTourismRetentionPercent				= kResults.GetInt("GPConcertTourismRetentionPercent");

	m_iGreatMusicianConcertTourismModifier			= kResults.GetInt("GreatMusicianConcertTourismModifier");
	m_iGreatMusicianConcertGoldPercent				= kResults.GetInt("GreatMusicianConcertGoldPercent");

	m_iCapitalAndSecondCapitalCultureModifier		= kResults.GetInt("CapitalAndSecondCapitalCultureModifier");

	m_iCapitalCultureModifierPerTurn				= kResults.GetInt("CapitalCultureModifierPerTurn");
	m_iCapitalFaithModifierPerTurn					= kResults.GetInt("CapitalFaithModifierPerTurn");
	m_iCapitalPerTurnYieldModifierMax				= kResults.GetInt("CapitalPerTurnYieldModifierMax");

	m_iImmigrationRatePerImmigrant					= kResults.GetInt("ImmigrationRatePerImmigrant");
	m_iImmigrationRateMax							= kResults.GetInt("ImmigrationRateMax");
	m_iEmigrationRatePerImmigrant					= kResults.GetInt("EmigrationRatePerImmigrant");
	m_iEmigrationRateMax							= kResults.GetInt("EmigrationRateMax");

	m_bPuppetNoTechCostPenalty						= kResults.GetBool("PuppetNoTechCostPenalty");
	m_iPuppetTechCostPartial						= kResults.GetInt("PuppetTechCostPartial");

	m_bCanPillageNeutralTradeRoute					= kResults.GetBool("CanPillageNeutralTradeRoute");

	m_iGarrisonCityDefenseModifier					= kResults.GetInt("GarrisonCityDefenseModifier");

	m_bLandUnitsImmuneRiverCrossing				= kResults.GetBool("LandUnitsImmuneRiverCrossing");

	m_iEnemyFixedDamageModifierInBorders			= kResults.GetInt("EnemyFixedDamageModifierInBorders");
	m_iCulturePerWarPeace							= kResults.GetInt("CulturePerWarPeace");
	m_iEnemyCombatModifierInBordersPerBeenDoW		= kResults.GetInt("EnemyCombatModifierInBordersPerBeenDoW");

	m_iUnitProductionModifierPerCity				= kResults.GetInt("UnitProductionModifierPerCity");
	m_iManpowerPerCity								= kResults.GetInt("ManpowerPerCity");
	m_iCombatBonusPerTechDifference				= kResults.GetInt("CombatBonusPerTechDifference");

	m_iNavalAttackIgnoreBuildingDefense				= kResults.GetInt("NavalAttackIgnoreBuildingDefense");
	m_iForeignRegenPercent							= kResults.GetInt("ForeignRegenPercent");

	m_iHillsCityDamageReduction						= kResults.GetInt("HillsCityDamageReduction");
	m_iHillsMovementModifier						= kResults.GetInt("HillsMovementModifier");
	m_iHillsCityRangeBonus							= kResults.GetInt("HillsCityRangeBonus");

	m_iReligionSpreadSpeedModifier					= kResults.GetInt("ReligionSpreadSpeedModifier");

	m_iLandTradeRouteDistancePerTradeSlot			= kResults.GetInt("LandTradeRouteDistancePerTradeSlot");

	m_iHappinessPerGoldDonated						= kResults.GetInt("HappinessPerGoldDonated");
	m_iGoldDonationInterval							= kResults.GetInt("GoldDonationInterval");
	m_iWonderProductionPerDonationHappiness			= kResults.GetInt("WonderProductionPerDonationHappiness");

	m_iLuxuryHappinessModifier						= kResults.GetInt("LuxuryHappinessModifier");
	{
		m_vBornGreatPersonSpecialistYield.clear();
		std::string strKey("CityStateUAEffect_BornGreatPersonSpecialistYield");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Specialists.ID as SpecialistID, UnitClasses.ID as UnitClassID, Yields.ID as YieldID, YieldMod from CityStateUAEffect_BornGreatPersonSpecialistYield inner join Specialists on Specialists.Type = SpecialistType inner join UnitClasses on UnitClasses.Type = UnitClassType inner join Yields on Yields.Type = YieldType where EffectType = ?");
		}
		pResults->Bind(1, GetType());
		while(pResults->Step())
		{
			BornGreatPersonSpecialistYieldEntry entry;
			entry.m_iSpecialistType = pResults->GetInt(0);
			entry.m_iUnitClassType = pResults->GetInt(1);
			entry.m_iYieldType = pResults->GetInt(2);
			entry.m_iYieldMod = pResults->GetInt(3);
			m_vBornGreatPersonSpecialistYield.push_back(entry);
		}
	}

	return true;
}

int CvCityStateUAEffectEntry::GetFaithPurchaseGreatPeopleCostRiseModifier() const { return m_iFaithPurchaseGreatPeopleCostRiseModifier; }
int CvCityStateUAEffectEntry::GetFaithPurchaseGreatPeopleCostRiseModifierPerGW() const { return m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW; }
int CvCityStateUAEffectEntry::GetGreatPersonPoints(int i) const { CvAssertMsg(i < GC.getNumSpecialistInfos(), "Index out of bounds"); CvAssertMsg(i > -1, "Index out of bounds"); return m_piGreatPersonPoints ? m_piGreatPersonPoints[i] : 0; }
bool CvCityStateUAEffectEntry::IsFaithPurchaseAllGreatPeople() const { return m_bFaithPurchaseAllGreatPeople; }

bool CvCityStateUAEffectEntry::IsGPNoDeathAfterGreatWork() const { return m_bGPNoDeathAfterGreatWork; }
int CvCityStateUAEffectEntry::GetGPConcertTourismRetentionPercent() const { return m_iGPConcertTourismRetentionPercent; }

int CvCityStateUAEffectEntry::GetGreatMusicianConcertTourismModifier() const { return m_iGreatMusicianConcertTourismModifier; }
int CvCityStateUAEffectEntry::GetGreatMusicianConcertGoldPercent() const { return m_iGreatMusicianConcertGoldPercent; }

int CvCityStateUAEffectEntry::GetCapitalAndSecondCapitalCultureModifier() const { return m_iCapitalAndSecondCapitalCultureModifier; }

int CvCityStateUAEffectEntry::GetCapitalCultureModifierPerTurn() const { return m_iCapitalCultureModifierPerTurn; }
int CvCityStateUAEffectEntry::GetCapitalFaithModifierPerTurn() const { return m_iCapitalFaithModifierPerTurn; }
int CvCityStateUAEffectEntry::GetCapitalPerTurnYieldModifierMax() const { return m_iCapitalPerTurnYieldModifierMax; }

int CvCityStateUAEffectEntry::GetImmigrationRatePerImmigrant() const { return m_iImmigrationRatePerImmigrant; }
int CvCityStateUAEffectEntry::GetImmigrationRateMax() const { return m_iImmigrationRateMax; }
int CvCityStateUAEffectEntry::GetEmigrationRatePerImmigrant() const { return m_iEmigrationRatePerImmigrant; }
int CvCityStateUAEffectEntry::GetEmigrationRateMax() const { return m_iEmigrationRateMax; }

bool CvCityStateUAEffectEntry::IsPuppetNoTechCostPenalty() const { return m_bPuppetNoTechCostPenalty; }
int CvCityStateUAEffectEntry::GetPuppetTechCostPartial() const { return m_iPuppetTechCostPartial; }

bool CvCityStateUAEffectEntry::IsCanPillageNeutralTradeRoute() const { return m_bCanPillageNeutralTradeRoute; }

int CvCityStateUAEffectEntry::GetGarrisonCityDefenseModifier() const { return m_iGarrisonCityDefenseModifier; }

bool CvCityStateUAEffectEntry::IsLandUnitsImmuneRiverCrossing() const { return m_bLandUnitsImmuneRiverCrossing; }

int CvCityStateUAEffectEntry::GetEnemyFixedDamageModifierInBorders() const { return m_iEnemyFixedDamageModifierInBorders; }
int CvCityStateUAEffectEntry::GetCulturePerWarPeace() const { return m_iCulturePerWarPeace; }
int CvCityStateUAEffectEntry::GetEnemyCombatModifierInBordersPerBeenDoW() const { return m_iEnemyCombatModifierInBordersPerBeenDoW; }

int CvCityStateUAEffectEntry::GetUnitProductionModifierPerCity() const { return m_iUnitProductionModifierPerCity; }
int CvCityStateUAEffectEntry::GetManpowerPerCity() const { return m_iManpowerPerCity; }
int CvCityStateUAEffectEntry::GetCombatBonusPerTechDifference() const { return m_iCombatBonusPerTechDifference; }

int CvCityStateUAEffectEntry::GetNavalAttackIgnoreBuildingDefense() const { return m_iNavalAttackIgnoreBuildingDefense; }
int CvCityStateUAEffectEntry::GetForeignRegenPercent() const { return m_iForeignRegenPercent; }

int CvCityStateUAEffectEntry::GetHillsCityDamageReduction() const { return m_iHillsCityDamageReduction; }
int CvCityStateUAEffectEntry::GetHillsMovementModifier() const { return m_iHillsMovementModifier; }
int CvCityStateUAEffectEntry::GetHillsCityRangeBonus() const { return m_iHillsCityRangeBonus; }

int CvCityStateUAEffectEntry::GetReligionSpreadSpeedModifier() const { return m_iReligionSpreadSpeedModifier; }

int CvCityStateUAEffectEntry::GetLandTradeRouteDistancePerTradeSlot() const { return m_iLandTradeRouteDistancePerTradeSlot; }

int CvCityStateUAEffectEntry::GetHappinessPerGoldDonated() const { return m_iHappinessPerGoldDonated; }
int CvCityStateUAEffectEntry::GetGoldDonationInterval() const { return m_iGoldDonationInterval; }
int CvCityStateUAEffectEntry::GetWonderProductionPerDonationHappiness() const { return m_iWonderProductionPerDonationHappiness; }

int CvCityStateUAEffectEntry::GetLuxuryHappinessModifier() const { return m_iLuxuryHappinessModifier; }
//======================================================================================================
// CvCityStateUAEffectXMLEntries
//======================================================================================================
CvCityStateUAEffectXMLEntries::CvCityStateUAEffectXMLEntries(void)
{
}

CvCityStateUAEffectXMLEntries::~CvCityStateUAEffectXMLEntries(void)
{
	DeleteArray();
}

std::vector<CvCityStateUAEffectEntry*>& CvCityStateUAEffectXMLEntries::GetEffectEntries()
{
	return m_paEffectEntries;
}

int CvCityStateUAEffectXMLEntries::GetNumEffects() const
{
	return (int)m_paEffectEntries.size();
}

CvCityStateUAEffectEntry* CvCityStateUAEffectXMLEntries::GetEntry(int index) const
{
	if (index >= 0 && index < (int)m_paEffectEntries.size())
		return m_paEffectEntries[index];
	return NULL;
}

CvCityStateUAEffectEntry* CvCityStateUAEffectXMLEntries::GetEntryByType(const char* szType) const
{
	if (szType == NULL) return NULL;
	for (size_t i = 0; i < m_paEffectEntries.size(); i++)
	{
		if (m_paEffectEntries[i] && strcmp(m_paEffectEntries[i]->GetType(), szType) == 0)
			return m_paEffectEntries[i];
	}
	return NULL;
}

void CvCityStateUAEffectXMLEntries::DeleteArray()
{
	for (size_t i = 0; i < m_paEffectEntries.size(); i++)
	{
		SAFE_DELETE(m_paEffectEntries[i]);
	}
	m_paEffectEntries.clear();
}

//======================================================================================================
// CvCityStateUAEntry
//======================================================================================================
CvCityStateUAEntry::CvCityStateUAEntry(void)
	: m_iAllyEffectID(-1)
	, m_iFriendEffectID(-1)
{
}

CvCityStateUAEntry::~CvCityStateUAEntry(void)
{
}

bool CvCityStateUAEntry::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	m_iAllyEffectID = GC.getInfoTypeForString(kResults.GetText("AllyEffectType"), true);
	m_iFriendEffectID = GC.getInfoTypeForString(kResults.GetText("FriendEffectType"), true);

	return true;
}

int CvCityStateUAEntry::GetAllyEffectID() const { return m_iAllyEffectID; }
int CvCityStateUAEntry::GetFriendEffectID() const { return m_iFriendEffectID; }

//======================================================================================================
// CvCityStateUAXMLEntries
//======================================================================================================
CvCityStateUAXMLEntries::CvCityStateUAXMLEntries(void)
{
}

CvCityStateUAXMLEntries::~CvCityStateUAXMLEntries(void)
{
	DeleteArray();
}

std::vector<CvCityStateUAEntry*>& CvCityStateUAXMLEntries::GetUAEntries()
{
	return m_paUAEntries;
}

int CvCityStateUAXMLEntries::GetNumUAs() const
{
	return (int)m_paUAEntries.size();
}

CvCityStateUAEntry* CvCityStateUAXMLEntries::GetEntry(int index) const
{
	if (index >= 0 && index < (int)m_paUAEntries.size())
		return m_paUAEntries[index];
	return NULL;
}

CvCityStateUAEntry* CvCityStateUAXMLEntries::GetEntryByType(const char* szType) const
{
	if (szType == NULL) return NULL;
	for (size_t i = 0; i < m_paUAEntries.size(); i++)
	{
		if (m_paUAEntries[i] && strcmp(m_paUAEntries[i]->GetType(), szType) == 0)
			return m_paUAEntries[i];
	}
	return NULL;
}

void CvCityStateUAXMLEntries::DeleteArray()
{
	for (size_t i = 0; i < m_paUAEntries.size(); i++)
	{
		SAFE_DELETE(m_paUAEntries[i]);
	}
	m_paUAEntries.clear();
}

//======================================================================================================
// CvPlayerCityStateUA
//======================================================================================================
CvPlayerCityStateUA::CvPlayerCityStateUA()
	: m_pPlayer(NULL)
	, m_iFaithPurchaseGreatPeopleCostRiseModifier(0)
	, m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW(0)
	, m_iFaithPurchaseAllGreatPeopleCount(0)
	, m_iGPNoDeathAfterGreatWorkCount(0)
	, m_iGPConcertTourismRetentionPercent(0)
	, m_iGreatMusicianConcertTourismModifier(0)
	, m_iGreatMusicianConcertGoldPercent(0)
	, m_iCapitalAndSecondCapitalCultureModifier(0)
	, m_iCapitalCultureModifierPerTurn(0)
	, m_iCapitalFaithModifierPerTurn(0)
	, m_iCapitalPerTurnYieldModifierMax(0)
	, m_iImmigrationRatePerImmigrant(0)
	, m_iImmigrationRateMax(0)
	, m_iEmigrationRatePerImmigrant(0)
	, m_iEmigrationRateMax(0)
	, m_iPuppetNoTechCostPenaltyCount(0)
	, m_iPuppetTechCostPartial(0)
	, m_iCanPillageNeutralTradeRouteCount(0)
	, m_iGarrisonCityDefenseModifier(0)
	, m_iLandUnitsImmuneRiverCrossingCount(0)
	, m_iEnemyFixedDamageModifierInBorders(0)
	, m_iCulturePerWarPeace(0)
	, m_iEnemyCombatModifierInBordersPerBeenDoW(0)
	, m_iUnitProductionModifierPerCity(0)
	, m_iManpowerPerCity(0)
	, m_iCombatBonusPerTechDifference(0)
	, m_iNavalAttackIgnoreBuildingDefense(0)
	, m_iForeignRegenPercent(0)
	, m_iHillsCityDamageReduction(0)
	, m_iHillsMovementModifier(0)
	, m_iHillsCityRangeBonus(0)
	, m_iReligionSpreadSpeedModifier(0)
	, m_iLandTradeRouteDistancePerTradeSlot(0)
	, m_iHappinessPerGoldDonated(0)
	, m_iGoldDonationInterval(0)
	, m_iWonderProductionPerDonationHappiness(0)
	, m_iLuxuryHappinessModifier(0)
{
}

CvPlayerCityStateUA::~CvPlayerCityStateUA()
{
	Uninit();
}

void CvPlayerCityStateUA::Init(CvPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
	Reset();
}

void CvPlayerCityStateUA::Uninit()
{
	m_pPlayer = NULL;
	Reset();
}

void CvPlayerCityStateUA::Reset()
{
	m_iFaithPurchaseGreatPeopleCostRiseModifier = 0;
	m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW = 0;
	m_aiGreatPersonPoints.assign(GC.getNumSpecialistInfos(), 0);
	m_iFaithPurchaseAllGreatPeopleCount = 0;
	m_iGPNoDeathAfterGreatWorkCount = 0;
	m_iGPConcertTourismRetentionPercent = 0;
	m_iGreatMusicianConcertTourismModifier = 0;
	m_iGreatMusicianConcertGoldPercent = 0;
	m_iCapitalAndSecondCapitalCultureModifier = 0;
	m_iCapitalCultureModifierPerTurn = 0;
	m_iCapitalFaithModifierPerTurn = 0;
	m_iCapitalPerTurnYieldModifierMax = 0;
	m_iImmigrationRatePerImmigrant = 0;
	m_iImmigrationRateMax = 0;
	m_iEmigrationRatePerImmigrant = 0;
	m_iEmigrationRateMax = 0;
	m_iPuppetNoTechCostPenaltyCount = 0;
	m_iPuppetTechCostPartial = 0;
	m_iCanPillageNeutralTradeRouteCount = 0;
	m_iGarrisonCityDefenseModifier = 0;
	m_iLandUnitsImmuneRiverCrossingCount = 0;
	m_iEnemyFixedDamageModifierInBorders = 0;
	m_iCulturePerWarPeace = 0;
	m_iEnemyCombatModifierInBordersPerBeenDoW = 0;
	m_iUnitProductionModifierPerCity = 0;
	m_iManpowerPerCity = 0;
	m_iCombatBonusPerTechDifference = 0;
	m_iNavalAttackIgnoreBuildingDefense = 0;
	m_iForeignRegenPercent = 0;
	m_iHillsCityDamageReduction = 0;
	m_iHillsMovementModifier = 0;
	m_iHillsCityRangeBonus = 0;
	m_iReligionSpreadSpeedModifier = 0;
	m_iLandTradeRouteDistancePerTradeSlot = 0;
	m_iHappinessPerGoldDonated = 0;
	m_iGoldDonationInterval = 0;
	m_iWonderProductionPerDonationHappiness = 0;
	m_iLuxuryHappinessModifier = 0;
	m_vBornGreatPersonSpecialistYield.clear();
}

void CvPlayerCityStateUA::ApplyEffect(int iEffectID, int iChange)
{
	if (iEffectID < 0 || iChange == 0) return;

	CvCityStateUAEffectEntry* pEffect = GC.getCityStateUAEffectEntry(iEffectID);
	if (!pEffect) return;

	m_iFaithPurchaseGreatPeopleCostRiseModifier			+= pEffect->GetFaithPurchaseGreatPeopleCostRiseModifier() * iChange;
	m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW		+= pEffect->GetFaithPurchaseGreatPeopleCostRiseModifierPerGW() * iChange;
	for (int iSpec = 0; iSpec < GC.getNumSpecialistInfos(); iSpec++)
	{
		m_aiGreatPersonPoints[iSpec] += pEffect->GetGreatPersonPoints(iSpec) * iChange;
	}
	m_iFaithPurchaseAllGreatPeopleCount += (pEffect->IsFaithPurchaseAllGreatPeople() ? iChange : 0);

	m_iGPNoDeathAfterGreatWorkCount += (pEffect->IsGPNoDeathAfterGreatWork() ? iChange : 0);
	m_iGPConcertTourismRetentionPercent				+= pEffect->GetGPConcertTourismRetentionPercent() * iChange;

	m_iGreatMusicianConcertTourismModifier			+= pEffect->GetGreatMusicianConcertTourismModifier() * iChange;
	m_iGreatMusicianConcertGoldPercent				+= pEffect->GetGreatMusicianConcertGoldPercent() * iChange;

	m_iCapitalAndSecondCapitalCultureModifier		+= pEffect->GetCapitalAndSecondCapitalCultureModifier() * iChange;

	m_iCapitalCultureModifierPerTurn				+= pEffect->GetCapitalCultureModifierPerTurn() * iChange;
	m_iCapitalFaithModifierPerTurn					+= pEffect->GetCapitalFaithModifierPerTurn() * iChange;
	m_iCapitalPerTurnYieldModifierMax				+= pEffect->GetCapitalPerTurnYieldModifierMax() * iChange;

	m_iImmigrationRatePerImmigrant					+= pEffect->GetImmigrationRatePerImmigrant() * iChange;
	m_iImmigrationRateMax							+= pEffect->GetImmigrationRateMax() * iChange;
	m_iEmigrationRatePerImmigrant					+= pEffect->GetEmigrationRatePerImmigrant() * iChange;
	m_iEmigrationRateMax							+= pEffect->GetEmigrationRateMax() * iChange;

	m_iPuppetNoTechCostPenaltyCount += (pEffect->IsPuppetNoTechCostPenalty() ? iChange : 0);
	m_iPuppetTechCostPartial						+= pEffect->GetPuppetTechCostPartial() * iChange;

	m_iCanPillageNeutralTradeRouteCount += (pEffect->IsCanPillageNeutralTradeRoute() ? iChange : 0);

	m_iGarrisonCityDefenseModifier					+= pEffect->GetGarrisonCityDefenseModifier() * iChange;

	m_iLandUnitsImmuneRiverCrossingCount += (pEffect->IsLandUnitsImmuneRiverCrossing() ? iChange : 0);

	m_iEnemyFixedDamageModifierInBorders			+= pEffect->GetEnemyFixedDamageModifierInBorders() * iChange;
	m_iCulturePerWarPeace							+= pEffect->GetCulturePerWarPeace() * iChange;
	m_iEnemyCombatModifierInBordersPerBeenDoW		+= pEffect->GetEnemyCombatModifierInBordersPerBeenDoW() * iChange;

	m_iUnitProductionModifierPerCity				+= pEffect->GetUnitProductionModifierPerCity() * iChange;
	m_iManpowerPerCity								+= pEffect->GetManpowerPerCity() * iChange;
	m_iCombatBonusPerTechDifference				+= pEffect->GetCombatBonusPerTechDifference() * iChange;

	m_iNavalAttackIgnoreBuildingDefense				+= pEffect->GetNavalAttackIgnoreBuildingDefense() * iChange;
	m_iForeignRegenPercent							+= pEffect->GetForeignRegenPercent() * iChange;

	m_iHillsCityDamageReduction						+= pEffect->GetHillsCityDamageReduction() * iChange;
	m_iHillsMovementModifier						+= pEffect->GetHillsMovementModifier() * iChange;
	m_iHillsCityRangeBonus							+= pEffect->GetHillsCityRangeBonus() * iChange;

	m_iReligionSpreadSpeedModifier					+= pEffect->GetReligionSpreadSpeedModifier() * iChange;

	m_iLandTradeRouteDistancePerTradeSlot			+= pEffect->GetLandTradeRouteDistancePerTradeSlot() * iChange;

	m_iHappinessPerGoldDonated						+= pEffect->GetHappinessPerGoldDonated() * iChange;
	m_iGoldDonationInterval							+= pEffect->GetGoldDonationInterval() * iChange;
	m_iWonderProductionPerDonationHappiness			+= pEffect->GetWonderProductionPerDonationHappiness() * iChange;

	m_iLuxuryHappinessModifier						+= pEffect->GetLuxuryHappinessModifier() * iChange;
	{
		const std::vector<BornGreatPersonSpecialistYieldEntry>& vEntries = pEffect->GetBornGreatPersonSpecialistYieldEntries();
		int iEntryCount = (int)vEntries.size();
		for (int iEntryIdx = 0; iEntryIdx < iEntryCount; iEntryIdx++)
		{
			const BornGreatPersonSpecialistYieldEntry& e = vEntries[iEntryIdx];
			BornGreatPersonSpecialistYieldEntry entry = e;
			entry.m_iYieldMod *= iChange;
			m_vBornGreatPersonSpecialistYield.push_back(entry);

			// Apply born yield to all cities with existing specialists
			GreatPersonTypes eGP = GetGreatPersonFromUnitClass((UnitClassTypes)e.m_iUnitClassType);
			if (eGP != NO_GREATPERSON && m_pPlayer)
			{
				int iBornCount = m_pPlayer->GetBornGreatPersonCount(eGP);
				int iYieldPerSpec = iBornCount * e.m_iYieldMod / 100 * iChange;
				if (iYieldPerSpec != 0)
				{
					int iLoop;
					for (CvCity* pCity = m_pPlayer->firstCity(&iLoop); pCity != NULL; pCity = m_pPlayer->nextCity(&iLoop))
					{
						int iSpecCount = pCity->GetCityCitizens()->GetSpecialistCount((SpecialistTypes)e.m_iSpecialistType);
						if (iSpecCount > 0)
						{
							pCity->ChangeBaseYieldRateFromSpecialists((YieldTypes)e.m_iYieldType, iYieldPerSpec * iSpecCount);
						}
					}
				}
			}
		}
	}
}

int CvPlayerCityStateUA::GetFaithPurchaseGreatPeopleCostRiseModifier() const { return m_iFaithPurchaseGreatPeopleCostRiseModifier; }
int CvPlayerCityStateUA::GetFaithPurchaseGreatPeopleCostRiseModifierPerGW() const { return m_iFaithPurchaseGreatPeopleCostRiseModifierPerGW; }
int CvPlayerCityStateUA::GetGreatPersonPoints(int i) const { CvAssertMsg(i < GC.getNumSpecialistInfos(), "Index out of bounds"); CvAssertMsg(i > -1, "Index out of bounds"); return (i >= 0 && i < (int)m_aiGreatPersonPoints.size()) ? m_aiGreatPersonPoints[i] : 0; }
bool CvPlayerCityStateUA::IsFaithPurchaseAllGreatPeople() const { return m_iFaithPurchaseAllGreatPeopleCount > 0; }
bool CvPlayerCityStateUA::IsGPNoDeathAfterGreatWork() const { return m_iGPNoDeathAfterGreatWorkCount > 0; }
int CvPlayerCityStateUA::GetGPConcertTourismRetentionPercent() const { return m_iGPConcertTourismRetentionPercent; }
int CvPlayerCityStateUA::GetGreatMusicianConcertTourismModifier() const { return m_iGreatMusicianConcertTourismModifier; }
int CvPlayerCityStateUA::GetGreatMusicianConcertGoldPercent() const { return m_iGreatMusicianConcertGoldPercent; }
int CvPlayerCityStateUA::GetCapitalAndSecondCapitalCultureModifier() const { return m_iCapitalAndSecondCapitalCultureModifier; }
int CvPlayerCityStateUA::GetCapitalCultureModifierPerTurn() const { return m_iCapitalCultureModifierPerTurn; }
int CvPlayerCityStateUA::GetCapitalFaithModifierPerTurn() const { return m_iCapitalFaithModifierPerTurn; }
int CvPlayerCityStateUA::GetCapitalPerTurnYieldModifierMax() const { return m_iCapitalPerTurnYieldModifierMax; }
int CvPlayerCityStateUA::GetImmigrationRatePerImmigrant() const { return m_iImmigrationRatePerImmigrant; }
int CvPlayerCityStateUA::GetImmigrationRateMax() const { return m_iImmigrationRateMax; }
int CvPlayerCityStateUA::GetEmigrationRatePerImmigrant() const { return m_iEmigrationRatePerImmigrant; }
int CvPlayerCityStateUA::GetEmigrationRateMax() const { return m_iEmigrationRateMax; }
bool CvPlayerCityStateUA::IsPuppetNoTechCostPenalty() const { return m_iPuppetNoTechCostPenaltyCount > 0; }
int CvPlayerCityStateUA::GetPuppetTechCostPartial() const { return m_iPuppetTechCostPartial; }
bool CvPlayerCityStateUA::IsCanPillageNeutralTradeRoute() const { return m_iCanPillageNeutralTradeRouteCount > 0; }
int CvPlayerCityStateUA::GetGarrisonCityDefenseModifier() const { return m_iGarrisonCityDefenseModifier; }
bool CvPlayerCityStateUA::IsLandUnitsImmuneRiverCrossing() const { return m_iLandUnitsImmuneRiverCrossingCount > 0; }
int CvPlayerCityStateUA::GetEnemyFixedDamageModifierInBorders() const { return m_iEnemyFixedDamageModifierInBorders; }
int CvPlayerCityStateUA::GetCulturePerWarPeace() const { return m_iCulturePerWarPeace; }
int CvPlayerCityStateUA::GetEnemyCombatModifierInBordersPerBeenDoW() const { return m_iEnemyCombatModifierInBordersPerBeenDoW; }
int CvPlayerCityStateUA::GetUnitProductionModifierPerCity() const { return m_iUnitProductionModifierPerCity; }
int CvPlayerCityStateUA::GetManpowerPerCity() const { return m_iManpowerPerCity; }
int CvPlayerCityStateUA::GetCombatBonusPerTechDifference() const { return m_iCombatBonusPerTechDifference; }
int CvPlayerCityStateUA::GetNavalAttackIgnoreBuildingDefense() const { return m_iNavalAttackIgnoreBuildingDefense; }
int CvPlayerCityStateUA::GetForeignRegenPercent() const { return m_iForeignRegenPercent; }
int CvPlayerCityStateUA::GetHillsCityDamageReduction() const { return m_iHillsCityDamageReduction; }
int CvPlayerCityStateUA::GetHillsMovementModifier() const { return m_iHillsMovementModifier; }
int CvPlayerCityStateUA::GetHillsCityRangeBonus() const { return m_iHillsCityRangeBonus; }
int CvPlayerCityStateUA::GetReligionSpreadSpeedModifier() const { return m_iReligionSpreadSpeedModifier; }
int CvPlayerCityStateUA::GetLandTradeRouteDistancePerTradeSlot() const { return m_iLandTradeRouteDistancePerTradeSlot; }
int CvPlayerCityStateUA::GetHappinessPerGoldDonated() const { return m_iHappinessPerGoldDonated; }
int CvPlayerCityStateUA::GetGoldDonationInterval() const { return m_iGoldDonationInterval; }
int CvPlayerCityStateUA::GetWonderProductionPerDonationHappiness() const { return m_iWonderProductionPerDonationHappiness; }
int CvPlayerCityStateUA::GetLuxuryHappinessModifier() const { return m_iLuxuryHappinessModifier; }
int CvPlayerCityStateUA::GetSpecialistYieldFromBornGreatPerson(SpecialistTypes eSpecialist, YieldTypes eYield) const
{
	if (!m_pPlayer) return 0;
	int iResult = 0;
	int iVecSize = (int)m_vBornGreatPersonSpecialistYield.size();
	for (int iIdx = 0; iIdx < iVecSize; iIdx++)
	{
		const BornGreatPersonSpecialistYieldEntry& entry = m_vBornGreatPersonSpecialistYield[iIdx];
		if (entry.m_iSpecialistType != (int)eSpecialist || entry.m_iYieldType != (int)eYield) continue;
		GreatPersonTypes eGP = GetGreatPersonFromUnitClass((UnitClassTypes)entry.m_iUnitClassType);
		if (eGP == NO_GREATPERSON) continue;
		int iBornCount = m_pPlayer->GetBornGreatPersonCount(eGP);
		int iAdd = iBornCount * entry.m_iYieldMod / 100;
		if (iAdd > 0) iResult += iAdd;
	}
	return iResult;
}
