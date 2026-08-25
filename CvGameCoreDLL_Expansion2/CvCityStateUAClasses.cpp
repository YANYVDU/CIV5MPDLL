/*	-------------------------------------------------------------------------------------------------------
	City-State Unique Ability System for Super Power V11 (Pillars of Sovereignty)
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreDLLUtil.h"
#include "CvCityStateUAClasses.h"
#include "CvPlayer.h"
#include "CvDatabaseUtility.h"

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
	, m_iMilitaryUnitProductionXP(0)
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
	, m_iCoupChanceModifier(0)
	, m_iSpyKillChancePerSpy(0)
	, m_iReligionSpreadSpeedModifier(0)
	, m_iLandTradeRouteDistancePerTradeSlot(0)
	, m_iHappinessPerGoldDonated(0)
	, m_iGoldDonationInterval(0)
	, m_iWonderProductionPerDonationHappiness(0)
	, m_iIdeologyPressurePerDonationHappiness(0)
	, m_iGoldDonationInfluenceModifierPerSeaRoute(0)
	, m_piFriendCityStateYieldModifiers(nullptr)
	, m_piAllyCityStateYieldModifiers(nullptr)
	, m_piPolicyYieldModifiers(nullptr)
	, m_iGoldenAgeThresholdPerPopulation(0)
	, m_iLuxuryHappinessModifier(0)
	, m_iFoodKeptModifierPerLuxury(0)
	, m_iTradeRouteGoldModifierPerLuxuryType(0)
	, m_iTradeRouteGoldModifierPerDistance(0)
	, m_iUnhappinessReductionPerCrossContinentRoute(0)
	, m_iEnemyCityNoHealBesiegeCount(0)
	, m_ppiBuildingClassYieldModifiers(NULL)
	, m_piSpecialistPointRate(nullptr)
	, m_piGreatPersonOneShotModifier(nullptr)
	, m_piSpyGarrisonYieldModifiers(nullptr)
	, m_iSpyKillGainSpyProgress(0)
	, m_ppiResourceYieldModifiers(NULL)
	, m_iCoastalCityGrowthThresholdModifier(0)
	, m_iDiplomaticPrestigePerCity(0)
	, m_ppiImprovementYieldModifiers(NULL)
	, m_piImprovementHappiness(nullptr)
{
}

CvCityStateUAEffectEntry::~CvCityStateUAEffectEntry(void)
{
	SAFE_DELETE_ARRAY(m_piGreatPersonPoints);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingClassYieldModifiers);
	SAFE_DELETE_ARRAY(m_piSpecialistPointRate);

	SAFE_DELETE_ARRAY(m_piGreatPersonOneShotModifier);
	SAFE_DELETE_ARRAY(m_piSpyGarrisonYieldModifiers);
	SAFE_DELETE_ARRAY(m_piFriendCityStateYieldModifiers);
	SAFE_DELETE_ARRAY(m_piAllyCityStateYieldModifiers);
	SAFE_DELETE_ARRAY(m_piPolicyYieldModifiers);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiResourceYieldModifiers);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiImprovementYieldModifiers);
	SAFE_DELETE_ARRAY(m_piImprovementHappiness);
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
	m_iMilitaryUnitProductionXP						= kResults.GetInt("MilitaryUnitProductionXP");

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
	m_iCoupChanceModifier							= kResults.GetInt("CoupChanceModifier");
	m_iSpyKillChancePerSpy							= kResults.GetInt("SpyKillChancePerSpy");

	m_iReligionSpreadSpeedModifier					= kResults.GetInt("ReligionSpreadSpeedModifier");

	m_iLandTradeRouteDistancePerTradeSlot			= kResults.GetInt("LandTradeRouteDistancePerTradeSlot");

	m_iHappinessPerGoldDonated						= kResults.GetInt("HappinessPerGoldDonated");
	m_iGoldDonationInterval							= kResults.GetInt("GoldDonationInterval");
	m_iWonderProductionPerDonationHappiness			= kResults.GetInt("WonderProductionPerDonationHappiness");
	m_iIdeologyPressurePerDonationHappiness			= kResults.GetInt("IdeologyPressurePerDonationHappiness");

	m_iGoldDonationInfluenceModifierPerSeaRoute		= kResults.GetInt("GoldDonationInfluenceModifierPerSeaRoute");
	m_iGoldenAgeThresholdPerPopulation				= kResults.GetInt("GoldenAgeThresholdPerPopulation");

	//CityState UA (Genoa / Vilnius): per-unit yield % modifiers, keyed by YieldType (Rate=100 => +1% per friend/ally CS or unlocked policy)
	kUtility.PopulateArrayByValue(m_piFriendCityStateYieldModifiers, "Yields", "CityStateUAEffect_FriendCityStateYieldModifiers", "YieldType", "EffectType", GetType(), "YieldMod");
	kUtility.PopulateArrayByValue(m_piAllyCityStateYieldModifiers, "Yields", "CityStateUAEffect_AllyCityStateYieldModifiers", "YieldType", "EffectType", GetType(), "YieldMod");
	kUtility.PopulateArrayByValue(m_piPolicyYieldModifiers, "Yields", "CityStateUAEffect_PolicyYieldModifiers", "YieldType", "EffectType", GetType(), "YieldMod");

	m_iLuxuryHappinessModifier						= kResults.GetInt("LuxuryHappinessModifier");
	m_iFoodKeptModifierPerLuxury						= kResults.GetInt("FoodKeptModifierPerLuxury");
	m_iTradeRouteGoldModifierPerLuxuryType			= kResults.GetInt("TradeRouteGoldModifierPerLuxuryType");
	m_iTradeRouteGoldModifierPerDistance				= kResults.GetInt("TradeRouteGoldModifierPerDistance");
	m_iUnhappinessReductionPerCrossContinentRoute	= kResults.GetInt("UnhappinessReductionPerCrossContinentRoute");

	m_iEnemyCityNoHealBesiegeCount					= kResults.GetInt("EnemyCityNoHealBesiegeCount");
	m_iSpyKillGainSpyProgress						= kResults.GetInt("SpyKillGainSpyProgress");

	m_iCoastalCityGrowthThresholdModifier			= kResults.GetInt("CoastalCityGrowthThresholdModifier");
	m_iDiplomaticPrestigePerCity					= kResults.GetInt("DiplomaticPrestigePerCity");

	//CityState UA (Melbourne): city owning the specified improved resource grants yield percentage modifiers
	{
		kUtility.Initialize2DArray(m_ppiResourceYieldModifiers, "Resources", "Yields");

		std::string strKey("CityStateUAEffect_ResourceYieldModifiers");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Resources.ID as ResourceID, Yields.ID as YieldID, YieldMod from CityStateUAEffect_ResourceYieldModifiers inner join Resources on Resources.Type = ResourceType inner join Yields on Yields.Type = YieldType where EffectType = ?");
		}

		pResults->Bind(1, GetType());
		while(pResults->Step())
		{
			const int iResourceID = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYieldMod = pResults->GetInt(2);

			m_ppiResourceYieldModifiers[iResourceID][iYieldID] = iYieldMod;
		}
	}

	//CityState UA (Antananarivo): each worked plot holding the specified improvement grants yield percentage modifiers
	{
		kUtility.Initialize2DArray(m_ppiImprovementYieldModifiers, "Improvements", "Yields");

		std::string strKey("CityStateUAEffect_ImprovementYieldModifiers");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Improvements.ID as ImprovementID, Yields.ID as YieldID, YieldMod from CityStateUAEffect_ImprovementYieldModifiers inner join Improvements on Improvements.Type = ImprovementType inner join Yields on Yields.Type = YieldType where EffectType = ?");
		}

		pResults->Bind(1, GetType());
		while(pResults->Step())
		{
			const int iImprovementID = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYieldMod = pResults->GetInt(2);

			m_ppiImprovementYieldModifiers[iImprovementID][iYieldID] = iYieldMod;
		}
	}

	//CityState UA (Zanzibar): each worked plot holding the specified improvement grants flat local happiness
	kUtility.PopulateArrayByValue(m_piImprovementHappiness, "Improvements", "CityStateUAEffect_ImprovementHappiness", "ImprovementType", "EffectType", GetType(), "Happiness");

	//BuildingClassYieldModifiers (Prague / Yerevan)
	{
		kUtility.Initialize2DArray(m_ppiBuildingClassYieldModifiers, "BuildingClasses", "Yields");

		std::string strKey("CityStateUAEffect_BuildingClassYieldModifiers");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select BuildingClasses.ID as BuildingClassID, Yields.ID as YieldID, YieldMod from CityStateUAEffect_BuildingClassYieldModifiers inner join BuildingClasses on BuildingClasses.Type = BuildingClassType inner join Yields on Yields.Type = YieldType where EffectType = ?");
		}

		pResults->Bind(1, GetType());
		while(pResults->Step())
		{
			const int BuildingClassID = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYieldMod = pResults->GetInt(2);

			m_ppiBuildingClassYieldModifiers[BuildingClassID][iYieldID] = iYieldMod;
		}
	}
	//Brussels: specialist great person point accumulation rate (%)
	kUtility.PopulateArrayByValue(m_piSpecialistPointRate, "Specialists", "CityStateUAEffect_SpecialistPointRate", "SpecialistType", "EffectType", GetType(), "Rate");
	//Prague: city with our own spy garrisoned grants yield percentage modifiers (per YieldType)
	kUtility.PopulateArrayByValue(m_piSpyGarrisonYieldModifiers, "Yields", "CityStateUAEffect_SpyGarrisonYieldModifiers", "YieldType", "EffectType", GetType(), "YieldMod");
	//Brussels: each great work of a class grants great person points to a specialist
	{
		m_vGreatWorkGreatPersonPoints.clear();
		std::string strKey("CityStateUAEffect_GreatWorkGreatPersonPoints");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select GreatWorkClasses.ID as GreatWorkClassID, Specialists.ID as SpecialistID, Rate, CapitalOnly from CityStateUAEffect_GreatWorkGreatPersonPoints inner join GreatWorkClasses on GreatWorkClasses.Type = GreatWorkClassType inner join Specialists on Specialists.Type = SpecialistType where EffectType = ?");
		}

		pResults->Bind(1, GetType());
		while(pResults->Step())
		{
			GreatWorkGreatPersonPointsEntry entry;
			entry.m_iGreatWorkClassType = pResults->GetInt(0);
			entry.m_iSpecialistType = pResults->GetInt(1);
			entry.m_iRate = pResults->GetInt(2);
			entry.m_bCapitalOnly = (pResults->GetInt(3) != 0);
			m_vGreatWorkGreatPersonPoints.push_back(entry);
		}
	}
	//Brussels: specified unit class's one-shot great person output modifier (%)
	kUtility.PopulateArrayByValue(m_piGreatPersonOneShotModifier, "UnitClasses", "CityStateUAEffect_GreatPersonOneShotModifier", "UnitClassType", "EffectType", GetType(), "Modifier");
	{
		m_vBuildingGPP.clear();
		std::string strKey2("CityStateUAEffect_BuildingGreatPersonPoints");
		Database::Results* pResults2 = kUtility.GetResults(strKey2);
		if(pResults2 == NULL)
		{
			pResults2 = kUtility.PrepareResults(strKey2, "select BuildingClasses.ID as BuildingClassID, Specialists.ID as SpecialistID, Points from CityStateUAEffect_BuildingGreatPersonPoints inner join BuildingClasses on BuildingClasses.Type = BuildingClassType inner join Specialists on Specialists.Type = SpecialistType where EffectType = ?");
		}
		pResults2->Bind(1, GetType());
		while(pResults2->Step())
		{
			BuildingGreatPersonPointsEntry entry;
			entry.m_iBuildingClass = pResults2->GetInt(0);
			entry.m_iSpecialist = pResults2->GetInt(1);
			entry.m_iPoints = pResults2->GetInt(2);
			m_vBuildingGPP.push_back(entry);
		}
	}
	{
		m_vBornAllyInfluenceMod.clear();
		std::string strKey3("CityStateUAEffect_BornGreatPersonAllyInfluenceMod");
		Database::Results* pResults3 = kUtility.GetResults(strKey3);
		if(pResults3 == NULL)
		{
			pResults3 = kUtility.PrepareResults(strKey3, "select UnitClasses.ID as UnitClassID, ModPerBorn from CityStateUAEffect_BornGreatPersonAllyInfluenceMod inner join UnitClasses on UnitClasses.Type = UnitClassType where EffectType = ?");
		}
		pResults3->Bind(1, GetType());
		while(pResults3->Step())
		{
			BornGreatPersonAllyInfluenceModEntry entry;
			entry.m_iUnitClassType = pResults3->GetInt(0);
			entry.m_iModPerBorn = pResults3->GetInt(1);
			m_vBornAllyInfluenceMod.push_back(entry);
		}
	}
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
	//Colombo: flat per-era yield on the international trade route (InternalTR) to this city-state (UCS)
	{
		m_vInternalTRToUCSPerEraYield.clear();
		std::string strKey("CityStateUAEffect_InternalTRToUCSPerEraYield");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Yields.ID as YieldID, YieldValue from CityStateUAEffect_InternalTRToUCSPerEraYield inner join Yields on Yields.Type = YieldType where EffectType = ?");
		}
		pResults->Bind(1, GetType());
		while(pResults->Step())
		{
			InternalTRToUCSPerEraYieldEntry entry;
			entry.m_iYieldType = pResults->GetInt(0);
			entry.m_iYieldValue = pResults->GetInt(1);
			m_vInternalTRToUCSPerEraYield.push_back(entry);
		}
	}
	//Colombo: in cities with a trade route to this city-state (UCS), a percentage of the input yield is granted as extra output yield
	{
		m_vYieldToYieldViaTRToUCS.clear();
		std::string strKey("CityStateUAEffect_YieldToYieldViaTRToUCS");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select YieldsIn.ID as InYieldID, YieldsOut.ID as OutYieldID, Percent from CityStateUAEffect_YieldToYieldViaTRToUCS inner join Yields as YieldsIn on YieldsIn.Type = InYieldType inner join Yields as YieldsOut on YieldsOut.Type = OutYieldType where EffectType = ?");
		}
		pResults->Bind(1, GetType());
		while(pResults->Step())
		{
			YieldToYieldViaTRToUCSEntry entry;
			entry.m_iInYieldType = pResults->GetInt(0);
			entry.m_iOutYieldType = pResults->GetInt(1);
			entry.m_iPercent = pResults->GetInt(2);
			m_vYieldToYieldViaTRToUCS.push_back(entry);
		}
	}
	//Valletta: buying the specified building class grants all units of the specified domain XP
	{
		m_vPurchasedBuildingXP.clear();
		std::string strKey("CityStateUAEffect_PurchasedBuildingXP");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select BuildingClasses.ID as BuildingClassID, Domains.ID as DomainID, XP from CityStateUAEffect_PurchasedBuildingXP inner join BuildingClasses on BuildingClasses.Type = BuildingClassType inner join Domains on Domains.Type = DomainType where EffectType = ?");
		}
		pResults->Bind(1, GetType());
		while(pResults->Step())
		{
			PurchasedBuildingXPEntry entry;
			entry.m_iBuildingClass = pResults->GetInt(0);
			entry.m_iDomain = pResults->GetInt(1);
			entry.m_iXP = pResults->GetInt(2);
			m_vPurchasedBuildingXP.push_back(entry);
		}
	}
	//Valletta: born unit of the specified unit class grants a configurable yield equal to YieldMod% of influence with the specified city-state (MinorCivType)
	{
		m_vUnitBornYield.clear();
		std::string strKey("CityStateUAEffect_UnitBornYield");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select MinorCivilizations.ID as MinorCivID, UnitClasses.ID as UnitClassID, Yields.ID as YieldID, YieldMod from CityStateUAEffect_UnitBornYield inner join MinorCivilizations on MinorCivilizations.Type = MinorCivType inner join UnitClasses on UnitClasses.Type = UnitClassType inner join Yields on Yields.Type = YieldType where EffectType = ?");
		}
		pResults->Bind(1, GetType());
		while(pResults->Step())
		{
			UnitBornYieldEntry entry;
			entry.m_iMinorCivType = pResults->GetInt(0);
			entry.m_iUnitClass = pResults->GetInt(1);
			entry.m_iYieldType = pResults->GetInt(2);
			entry.m_iYieldMod = pResults->GetInt(3);
			m_vUnitBornYield.push_back(entry);
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

int CvCityStateUAEffectEntry::GetMilitaryUnitProductionXP() const { return m_iMilitaryUnitProductionXP; }

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
int CvCityStateUAEffectEntry::GetCoupChanceModifier() const { return m_iCoupChanceModifier; }
int CvCityStateUAEffectEntry::GetSpyKillChancePerSpy() const { return m_iSpyKillChancePerSpy; }

int CvCityStateUAEffectEntry::GetReligionSpreadSpeedModifier() const { return m_iReligionSpreadSpeedModifier; }

int CvCityStateUAEffectEntry::GetLandTradeRouteDistancePerTradeSlot() const { return m_iLandTradeRouteDistancePerTradeSlot; }

int CvCityStateUAEffectEntry::GetHappinessPerGoldDonated() const { return m_iHappinessPerGoldDonated; }
int CvCityStateUAEffectEntry::GetGoldDonationInterval() const { return m_iGoldDonationInterval; }
int CvCityStateUAEffectEntry::GetWonderProductionPerDonationHappiness() const { return m_iWonderProductionPerDonationHappiness; }
int CvCityStateUAEffectEntry::GetIdeologyPressurePerDonationHappiness() const { return m_iIdeologyPressurePerDonationHappiness; }
int CvCityStateUAEffectEntry::GetGoldDonationInfluenceModifierPerSeaRoute() const { return m_iGoldDonationInfluenceModifierPerSeaRoute; }
int CvCityStateUAEffectEntry::GetFriendCityStateYieldModifier(YieldTypes eYieldType) const
{
	CvAssertMsg(eYieldType >= 0 && eYieldType < NUM_YIELD_TYPES, "Index out of bounds");
	return m_piFriendCityStateYieldModifiers ? m_piFriendCityStateYieldModifiers[(int)eYieldType] : 0;
}
int CvCityStateUAEffectEntry::GetAllyCityStateYieldModifier(YieldTypes eYieldType) const
{
	CvAssertMsg(eYieldType >= 0 && eYieldType < NUM_YIELD_TYPES, "Index out of bounds");
	return m_piAllyCityStateYieldModifiers ? m_piAllyCityStateYieldModifiers[(int)eYieldType] : 0;
}
int CvCityStateUAEffectEntry::GetPolicyYieldModifier(YieldTypes eYieldType) const
{
	CvAssertMsg(eYieldType >= 0 && eYieldType < NUM_YIELD_TYPES, "Index out of bounds");
	return m_piPolicyYieldModifiers ? m_piPolicyYieldModifiers[(int)eYieldType] : 0;
}
int CvCityStateUAEffectEntry::GetGoldenAgeThresholdPerPopulation() const { return m_iGoldenAgeThresholdPerPopulation; }

int CvCityStateUAEffectEntry::GetLuxuryHappinessModifier() const { return m_iLuxuryHappinessModifier; }
int CvCityStateUAEffectEntry::GetFoodKeptModifierPerLuxury() const { return m_iFoodKeptModifierPerLuxury; }
int CvCityStateUAEffectEntry::GetTradeRouteGoldModifierPerLuxuryType() const { return m_iTradeRouteGoldModifierPerLuxuryType; }
int CvCityStateUAEffectEntry::GetTradeRouteGoldModifierPerDistance() const { return m_iTradeRouteGoldModifierPerDistance; }
int CvCityStateUAEffectEntry::GetUnhappinessReductionPerCrossContinentRoute() const { return m_iUnhappinessReductionPerCrossContinentRoute; }

int CvCityStateUAEffectEntry::GetBuildingClassYieldModifiers(int i, int j) const
{
	CvAssertMsg(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiBuildingClassYieldModifiers ? m_ppiBuildingClassYieldModifiers[i][j] : 0;
}

int CvCityStateUAEffectEntry::GetSpecialistPointRate(int i) const
{
	CvAssertMsg(i < GC.getNumSpecialistInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piSpecialistPointRate ? m_piSpecialistPointRate[i] : 0;
}

int CvCityStateUAEffectEntry::GetSpyGarrisonYieldModifiers(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piSpyGarrisonYieldModifiers ? m_piSpyGarrisonYieldModifiers[i] : 0;
}

int CvCityStateUAEffectEntry::GetSpyKillGainSpyProgress() const
{
	return m_iSpyKillGainSpyProgress;
}

int CvCityStateUAEffectEntry::GetResourceYieldModifiers(int i, int j) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiResourceYieldModifiers ? m_ppiResourceYieldModifiers[i][j] : 0;
}

int CvCityStateUAEffectEntry::GetCoastalCityGrowthThresholdModifier() const
{
	return m_iCoastalCityGrowthThresholdModifier;
}

int CvCityStateUAEffectEntry::GetDiplomaticPrestigePerCity() const
{
	return m_iDiplomaticPrestigePerCity;
}

int CvCityStateUAEffectEntry::GetImprovementYieldModifiers(int i, int j) const
{
	CvAssertMsg(i < GC.getNumImprovementInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiImprovementYieldModifiers ? m_ppiImprovementYieldModifiers[i][j] : 0;
}

int CvCityStateUAEffectEntry::GetImprovementHappiness(int i) const
{
	CvAssertMsg(i < GC.getNumImprovementInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	if (!m_piImprovementHappiness || i < 0 || i >= GC.getNumImprovementInfos())
		return 0;
	return m_piImprovementHappiness[i];
}

int CvCityStateUAEffectEntry::GetGreatPersonOneShotModifier(int i) const
{
	CvAssertMsg(i < GC.getNumUnitClassInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piGreatPersonOneShotModifier ? m_piGreatPersonOneShotModifier[i] : 0;
}

int CvCityStateUAEffectEntry::GetInternalTRToUCSPerEraYield(int eYield) const
{
	int iTotal = 0;
	for (size_t i = 0; i < m_vInternalTRToUCSPerEraYield.size(); i++)
	{
		if (m_vInternalTRToUCSPerEraYield[i].m_iYieldType == eYield)
			iTotal += m_vInternalTRToUCSPerEraYield[i].m_iYieldValue;
	}
	return iTotal;
}

int CvCityStateUAEffectEntry::GetYieldToYieldViaTRToUCS(int eInYield, int eOutYield) const
{
	int iTotal = 0;
	for (size_t i = 0; i < m_vYieldToYieldViaTRToUCS.size(); i++)
	{
		if (m_vYieldToYieldViaTRToUCS[i].m_iInYieldType == eInYield && m_vYieldToYieldViaTRToUCS[i].m_iOutYieldType == eOutYield)
			iTotal += m_vYieldToYieldViaTRToUCS[i].m_iPercent;
	}
	return iTotal;
}

int CvCityStateUAEffectEntry::GetEnemyCityNoHealBesiegeCount() const { return m_iEnemyCityNoHealBesiegeCount; }
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
	, m_iBuildingClassYieldModifierCount(0)
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
	, m_iMilitaryUnitProductionXP(0)
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
	, m_iCoupChanceModifier(0)
	, m_iSpyKillChancePerSpy(0)
	, m_iReligionSpreadSpeedModifier(0)
	, m_iLandTradeRouteDistancePerTradeSlot(0)
	, m_iHappinessPerGoldDonated(0)
	, m_iGoldDonationInterval(0)
	, m_iWonderProductionPerDonationHappiness(0)
	, m_iIdeologyPressurePerDonationHappiness(0)
	, m_iGoldDonationInfluenceModifierPerSeaRoute(0)
	, m_iGoldenAgeThresholdPerPopulation(0)
	, m_iLuxuryHappinessModifier(0)
	, m_iFoodKeptModifierPerLuxury(0)
	, m_iTradeRouteGoldModifierPerLuxuryType(0)
	, m_iTradeRouteGoldModifierPerDistance(0)
	, m_iUnhappinessReductionPerCrossContinentRoute(0)
	, m_iEnemyCityNoHealBesiegeCount(0)
	, m_ppiBuildingClassYieldModifiers(NULL)
	, m_iSpyGarrisonYieldModifierCount(0)
	, m_iSpyKillGainSpyProgress(0)
	, m_ppiResourceYieldModifiers(NULL)
	, m_iResourceYieldModifierCount(0)
	, m_iCoastalCityGrowthThresholdModifier(0)
	, m_iDiplomaticPrestigePerCity(0)
	, m_ppiImprovementYieldModifiers(NULL)
	, m_iImprovementYieldModifierCount(0)
	, m_iImprovementHappinessCount(0)
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
	m_iMilitaryUnitProductionXP = 0;
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
	m_iCoupChanceModifier = 0;
	m_iSpyKillChancePerSpy = 0;
	m_iReligionSpreadSpeedModifier = 0;
	m_iLandTradeRouteDistancePerTradeSlot = 0;
	m_iHappinessPerGoldDonated = 0;
	m_iGoldDonationInterval = 0;
	m_iWonderProductionPerDonationHappiness = 0;
	m_iIdeologyPressurePerDonationHappiness = 0;
	m_iGoldDonationInfluenceModifierPerSeaRoute = 0;
	m_aiFriendCityStateYieldModifiers.assign(NUM_YIELD_TYPES, 0);
	m_aiAllyCityStateYieldModifiers.assign(NUM_YIELD_TYPES, 0);
	m_aiPolicyYieldModifiers.assign(NUM_YIELD_TYPES, 0);
	m_iGoldenAgeThresholdPerPopulation = 0;
	m_iLuxuryHappinessModifier = 0;
	m_iFoodKeptModifierPerLuxury = 0;
	m_iTradeRouteGoldModifierPerLuxuryType = 0;
	m_iTradeRouteGoldModifierPerDistance = 0;
	m_iUnhappinessReductionPerCrossContinentRoute = 0;
	m_iBuildingClassYieldModifierCount = 0;
	m_iSpyKillGainSpyProgress = 0;
	m_iSpyGarrisonYieldModifierCount = 0;
	m_aiSpyGarrisonYieldModifiers.assign(NUM_YIELD_TYPES, 0);
	m_iResourceYieldModifierCount = 0;
	m_iCoastalCityGrowthThresholdModifier = 0;
	m_iDiplomaticPrestigePerCity = 0;
	m_iImprovementYieldModifierCount = 0;
	m_iImprovementHappinessCount = 0;
	m_aiImprovementHappiness.assign(GC.getNumImprovementInfos(), 0);
	m_aiSpecialistPointRate.assign(GC.getNumSpecialistInfos(), 0);
	m_vGreatWorkGreatPersonPoints.clear();
	m_aiGreatPersonOneShotModifier.assign(GC.getNumUnitClassInfos(), 0);
	// Mirrors CvDatabaseUtility::Initialize2DArray (non-static, so allocate manually here)
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingClassYieldModifiers);
	{
		const int iNumBC = GC.getNumBuildingClassInfos();
		if (iNumBC > 0)
		{
			const unsigned int iNumBytes = iNumBC * sizeof(int*) + iNumBC * NUM_YIELD_TYPES * sizeof(int);
			unsigned char* pData = FNEW(unsigned char[iNumBytes], c_eCiv5GameplayDLL, 0);
			m_ppiBuildingClassYieldModifiers = (int**)pData;
			m_ppiBuildingClassYieldModifiers[0] = (int*)(pData + iNumBC * sizeof(int*));
			for (int j = 0; j < NUM_YIELD_TYPES; ++j) m_ppiBuildingClassYieldModifiers[0][j] = 0;
			for (int i = 1; i < iNumBC; i++)
			{
				m_ppiBuildingClassYieldModifiers[i] = m_ppiBuildingClassYieldModifiers[i-1] + NUM_YIELD_TYPES;
				for (int j = 0; j < NUM_YIELD_TYPES; ++j) m_ppiBuildingClassYieldModifiers[i][j] = 0;
			}
		}
	}
	// Mirrors CvDatabaseUtility::Initialize2DArray (non-static, so allocate manually here)
	CvDatabaseUtility::SafeDelete2DArray(m_ppiResourceYieldModifiers);
	{
		const int iNumRes = GC.getNumResourceInfos();
		if (iNumRes > 0)
		{
			const unsigned int iNumBytes = iNumRes * sizeof(int*) + iNumRes * NUM_YIELD_TYPES * sizeof(int);
			unsigned char* pData = FNEW(unsigned char[iNumBytes], c_eCiv5GameplayDLL, 0);
			m_ppiResourceYieldModifiers = (int**)pData;
			m_ppiResourceYieldModifiers[0] = (int*)(pData + iNumRes * sizeof(int*));
			for (int j = 0; j < NUM_YIELD_TYPES; ++j) m_ppiResourceYieldModifiers[0][j] = 0;
			for (int i = 1; i < iNumRes; i++)
			{
				m_ppiResourceYieldModifiers[i] = m_ppiResourceYieldModifiers[i-1] + NUM_YIELD_TYPES;
				for (int j = 0; j < NUM_YIELD_TYPES; ++j) m_ppiResourceYieldModifiers[i][j] = 0;
			}
		}
	}
	// Mirrors CvDatabaseUtility::Initialize2DArray (non-static, so allocate manually here)
	CvDatabaseUtility::SafeDelete2DArray(m_ppiImprovementYieldModifiers);
	{
		const int iNumImp = GC.getNumImprovementInfos();
		if (iNumImp > 0)
		{
			const unsigned int iNumBytes = iNumImp * sizeof(int*) + iNumImp * NUM_YIELD_TYPES * sizeof(int);
			unsigned char* pData = FNEW(unsigned char[iNumBytes], c_eCiv5GameplayDLL, 0);
			m_ppiImprovementYieldModifiers = (int**)pData;
			m_ppiImprovementYieldModifiers[0] = (int*)(pData + iNumImp * sizeof(int*));
			for (int j = 0; j < NUM_YIELD_TYPES; ++j) m_ppiImprovementYieldModifiers[0][j] = 0;
			for (int i = 1; i < iNumImp; i++)
			{
				m_ppiImprovementYieldModifiers[i] = m_ppiImprovementYieldModifiers[i-1] + NUM_YIELD_TYPES;
				for (int j = 0; j < NUM_YIELD_TYPES; ++j) m_ppiImprovementYieldModifiers[i][j] = 0;
			}
		}
	}
	m_vBornGreatPersonSpecialistYield.clear();
	m_vBuildingGPP.clear();
	m_vBornAllyInfluenceMod.clear();
	m_iEnemyCityNoHealBesiegeCount = 0;
	m_vPurchasedBuildingXP.clear();
	m_vUnitBornYield.clear();
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
	m_iMilitaryUnitProductionXP						+= pEffect->GetMilitaryUnitProductionXP() * iChange;

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
	// Sofia (spy/coup UA)
	m_iCoupChanceModifier							+= pEffect->GetCoupChanceModifier() * iChange;
	m_iSpyKillChancePerSpy							+= pEffect->GetSpyKillChancePerSpy() * iChange;

	m_iReligionSpreadSpeedModifier					+= pEffect->GetReligionSpreadSpeedModifier() * iChange;

	m_iLandTradeRouteDistancePerTradeSlot			+= pEffect->GetLandTradeRouteDistancePerTradeSlot() * iChange;

	m_iHappinessPerGoldDonated						+= pEffect->GetHappinessPerGoldDonated() * iChange;
	m_iGoldDonationInterval							+= pEffect->GetGoldDonationInterval() * iChange;
	m_iWonderProductionPerDonationHappiness			+= pEffect->GetWonderProductionPerDonationHappiness() * iChange;
	m_iIdeologyPressurePerDonationHappiness			+= pEffect->GetIdeologyPressurePerDonationHappiness() * iChange;
	m_iGoldDonationInfluenceModifierPerSeaRoute		+= pEffect->GetGoldDonationInfluenceModifierPerSeaRoute() * iChange;
	//Genoa / Vilnius: per-unit yield % modifiers (per friend/ally CS or unlocked policy)
	for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
	{
		int iFriendMod = pEffect->GetFriendCityStateYieldModifier((YieldTypes)iYield);
		if (iFriendMod != 0) m_aiFriendCityStateYieldModifiers[iYield] += iFriendMod * iChange;
		int iAllyMod = pEffect->GetAllyCityStateYieldModifier((YieldTypes)iYield);
		if (iAllyMod != 0) m_aiAllyCityStateYieldModifiers[iYield] += iAllyMod * iChange;
		int iPolicyMod = pEffect->GetPolicyYieldModifier((YieldTypes)iYield);
		if (iPolicyMod != 0) m_aiPolicyYieldModifiers[iYield] += iPolicyMod * iChange;
	}
	m_iGoldenAgeThresholdPerPopulation				+= pEffect->GetGoldenAgeThresholdPerPopulation() * iChange;

	m_iLuxuryHappinessModifier						+= pEffect->GetLuxuryHappinessModifier() * iChange;
	m_iFoodKeptModifierPerLuxury						+= pEffect->GetFoodKeptModifierPerLuxury() * iChange;
	m_iTradeRouteGoldModifierPerLuxuryType			+= pEffect->GetTradeRouteGoldModifierPerLuxuryType() * iChange;
	m_iTradeRouteGoldModifierPerDistance			+= pEffect->GetTradeRouteGoldModifierPerDistance() * iChange;
	m_iUnhappinessReductionPerCrossContinentRoute	+= pEffect->GetUnhappinessReductionPerCrossContinentRoute() * iChange;
	m_iEnemyCityNoHealBesiegeCount					+= pEffect->GetEnemyCityNoHealBesiegeCount() * iChange;
	m_iSpyKillGainSpyProgress						+= pEffect->GetSpyKillGainSpyProgress() * iChange;
	m_iCoastalCityGrowthThresholdModifier			+= pEffect->GetCoastalCityGrowthThresholdModifier() * iChange;
	m_iDiplomaticPrestigePerCity					+= pEffect->GetDiplomaticPrestigePerCity() * iChange;
	{
		if (m_ppiBuildingClassYieldModifiers)
		{
			for (int iBC = 0; iBC < GC.getNumBuildingClassInfos(); iBC++)
			{
				for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
				{
					int iMod = pEffect->GetBuildingClassYieldModifiers(iBC, iYield);
					if (iMod != 0)
					{
						m_ppiBuildingClassYieldModifiers[iBC][iYield] += iMod * iChange;
						m_iBuildingClassYieldModifierCount += iChange;
					}
				}
			}
		}
	}
	//CityState UA (Melbourne): city owning the specified improved resource grants yield percentage modifiers
	{
		if (m_ppiResourceYieldModifiers)
		{
			for (int iRes = 0; iRes < GC.getNumResourceInfos(); iRes++)
			{
				for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
				{
					int iMod = pEffect->GetResourceYieldModifiers(iRes, iYield);
					if (iMod != 0)
					{
						m_ppiResourceYieldModifiers[iRes][iYield] += iMod * iChange;
						m_iResourceYieldModifierCount += iChange;
					}
				}
			}
		}
	}
	//CityState UA (Antananarivo): each worked plot holding the specified improvement grants yield percentage modifiers
	{
		if (m_ppiImprovementYieldModifiers)
		{
			for (int iImp = 0; iImp < GC.getNumImprovementInfos(); iImp++)
			{
				for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
				{
					int iMod = pEffect->GetImprovementYieldModifiers(iImp, iYield);
					if (iMod != 0)
					{
						m_ppiImprovementYieldModifiers[iImp][iYield] += iMod * iChange;
						m_iImprovementYieldModifierCount += iChange;
					}
				}
			}
		}
	}
	//CityState UA (Zanzibar): each worked plot holding the specified improvement grants flat local happiness
	for (int iImp = 0; iImp < GC.getNumImprovementInfos(); iImp++)
	{
		int iHappy = pEffect->GetImprovementHappiness(iImp);
		if (iHappy != 0)
		{
			m_aiImprovementHappiness[iImp] += iHappy * iChange;
			m_iImprovementHappinessCount += iChange;
		}
	}
	//Prague: city with our own spy garrisoned grants yield percentage modifiers
	for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
	{
		int iMod = pEffect->GetSpyGarrisonYieldModifiers(iYield);
		if (iMod != 0)
		{
			m_aiSpyGarrisonYieldModifiers[iYield] += iMod * iChange;
			m_iSpyGarrisonYieldModifierCount += iChange;
		}
	}
	//Brussels: specialist great person point accumulation rate
	for (int iSpec = 0; iSpec < GC.getNumSpecialistInfos(); iSpec++)
	{
		m_aiSpecialistPointRate[iSpec] += pEffect->GetSpecialistPointRate(iSpec) * iChange;
	}
	{
		const std::vector<GreatWorkGreatPersonPointsEntry>& vEntries = pEffect->GetGreatWorkGreatPersonPointsEntries();
		for (size_t i = 0; i < vEntries.size(); i++)
		{
			GreatWorkGreatPersonPointsEntry entry = vEntries[i];
			entry.m_iRate *= iChange;
			m_vGreatWorkGreatPersonPoints.push_back(entry);
		}
	}
	//Brussels: specified unit class's one-shot great person output modifier
	for (int iUC = 0; iUC < GC.getNumUnitClassInfos(); iUC++)
	{
		m_aiGreatPersonOneShotModifier[iUC] += pEffect->GetGreatPersonOneShotModifier(iUC) * iChange;
	}
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
	{
		const std::vector<BuildingGreatPersonPointsEntry>& vEntries = pEffect->GetBuildingGreatPersonPointsEntries();
		for (size_t i = 0; i < vEntries.size(); i++)
		{
			BuildingGreatPersonPointsEntry entry = vEntries[i];
			entry.m_iPoints *= iChange;
			m_vBuildingGPP.push_back(entry);
		}
	}
	{
		const std::vector<BornGreatPersonAllyInfluenceModEntry>& vInfEntries = pEffect->GetBornAllyInfluenceModEntries();
		for (size_t i = 0; i < vInfEntries.size(); i++)
		{
			const BornGreatPersonAllyInfluenceModEntry& e = vInfEntries[i];
			m_vBornAllyInfluenceMod.push_back(e);
		}
	}
	{
		const std::vector<PurchasedBuildingXPEntry>& vEntries = pEffect->GetPurchasedBuildingXPEntries();
		for (size_t i = 0; i < vEntries.size(); i++)
		{
			PurchasedBuildingXPEntry entry = vEntries[i];
			entry.m_iXP *= iChange;
			m_vPurchasedBuildingXP.push_back(entry);
		}
	}
	{
		const std::vector<UnitBornYieldEntry>& vEntries = pEffect->GetUnitBornYieldEntries();
		for (size_t i = 0; i < vEntries.size(); i++)
		{
			UnitBornYieldEntry entry = vEntries[i];
			entry.m_iYieldMod *= iChange;
			m_vUnitBornYield.push_back(entry);
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

int CvPlayerCityStateUA::GetMilitaryUnitProductionXP() const { return m_iMilitaryUnitProductionXP; }
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
int CvPlayerCityStateUA::GetCoupChanceModifier() const { return m_iCoupChanceModifier; }
int CvPlayerCityStateUA::GetSpyKillChancePerSpy() const { return m_iSpyKillChancePerSpy; }
int CvPlayerCityStateUA::GetReligionSpreadSpeedModifier() const { return m_iReligionSpreadSpeedModifier; }
int CvPlayerCityStateUA::GetLandTradeRouteDistancePerTradeSlot() const { return m_iLandTradeRouteDistancePerTradeSlot; }
int CvPlayerCityStateUA::GetHappinessPerGoldDonated() const { return m_iHappinessPerGoldDonated; }
int CvPlayerCityStateUA::GetGoldDonationInterval() const { return m_iGoldDonationInterval; }
int CvPlayerCityStateUA::GetWonderProductionPerDonationHappiness() const { return m_iWonderProductionPerDonationHappiness; }
int CvPlayerCityStateUA::GetIdeologyPressurePerDonationHappiness() const { return m_iIdeologyPressurePerDonationHappiness; }
int CvPlayerCityStateUA::GetGoldDonationInfluenceModifierPerSeaRoute() const { return m_iGoldDonationInfluenceModifierPerSeaRoute; }
int CvPlayerCityStateUA::GetFriendCityStateYieldModifier(YieldTypes eYieldType) const
{
	return (eYieldType >= 0 && (int)eYieldType < (int)m_aiFriendCityStateYieldModifiers.size()) ? m_aiFriendCityStateYieldModifiers[(int)eYieldType] : 0;
}
int CvPlayerCityStateUA::GetAllyCityStateYieldModifier(YieldTypes eYieldType) const
{
	return (eYieldType >= 0 && (int)eYieldType < (int)m_aiAllyCityStateYieldModifiers.size()) ? m_aiAllyCityStateYieldModifiers[(int)eYieldType] : 0;
}
int CvPlayerCityStateUA::GetPolicyYieldModifier(YieldTypes eYieldType) const
{
	return (eYieldType >= 0 && (int)eYieldType < (int)m_aiPolicyYieldModifiers.size()) ? m_aiPolicyYieldModifiers[(int)eYieldType] : 0;
}
int CvPlayerCityStateUA::GetGoldenAgeThresholdPerPopulation() const { return m_iGoldenAgeThresholdPerPopulation; }
int CvPlayerCityStateUA::GetLuxuryHappinessModifier() const { return m_iLuxuryHappinessModifier; }
int CvPlayerCityStateUA::GetFoodKeptModifierPerLuxury() const { return m_iFoodKeptModifierPerLuxury; }
int CvPlayerCityStateUA::GetTradeRouteGoldModifierPerLuxuryType() const { return m_iTradeRouteGoldModifierPerLuxuryType; }
int CvPlayerCityStateUA::GetTradeRouteGoldModifierPerDistance() const { return m_iTradeRouteGoldModifierPerDistance; }
int CvPlayerCityStateUA::GetUnhappinessReductionPerCrossContinentRoute() const { return m_iUnhappinessReductionPerCrossContinentRoute; }
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

int CvPlayerCityStateUA::GetBuildingGreatPersonPointsForCity(const CvCity* pCity, SpecialistTypes eSpecialist) const
{
	if (!pCity) return 0;
	int iTotal = 0;
	for (size_t i = 0; i < m_vBuildingGPP.size(); i++)
	{
		const BuildingGreatPersonPointsEntry& entry = m_vBuildingGPP[i];
		if (entry.m_iSpecialist == (int)eSpecialist && entry.m_iPoints > 0)
		{
			int iCount = pCity->GetNumBuildingClass((BuildingClassTypes)entry.m_iBuildingClass);
			iTotal += entry.m_iPoints * iCount;
		}
	}
	return iTotal;
}

int CvPlayerCityStateUA::GetAllyInfluenceModFromBornGreatPerson() const
{
	if (!m_pPlayer) return 0;
	int iTotal = 0;
	for (size_t i = 0; i < m_vBornAllyInfluenceMod.size(); i++)
	{
		const BornGreatPersonAllyInfluenceModEntry& entry = m_vBornAllyInfluenceMod[i];
		GreatPersonTypes eGP = GetGreatPersonFromUnitClass((UnitClassTypes)entry.m_iUnitClassType);
		if (eGP == NO_GREATPERSON) continue;
		int iBornCount = m_pPlayer->GetBornGreatPersonCount(eGP);
		iTotal += iBornCount * entry.m_iModPerBorn;
	}
	return iTotal;
}

int CvPlayerCityStateUA::GetBuildingClassYieldModifier(BuildingClassTypes eBuildingClass, YieldTypes eYieldType) const
{
	if (!m_ppiBuildingClassYieldModifiers) return 0;
	return m_ppiBuildingClassYieldModifiers[(int)eBuildingClass][(int)eYieldType];
}

bool CvPlayerCityStateUA::HasBuildingClassYieldModifiers() const
{
	return m_iBuildingClassYieldModifierCount > 0;
}

int CvPlayerCityStateUA::GetSpyGarrisonYieldModifier(YieldTypes eYieldType) const
{
	return (eYieldType >= 0 && (int)eYieldType < (int)m_aiSpyGarrisonYieldModifiers.size()) ? m_aiSpyGarrisonYieldModifiers[(int)eYieldType] : 0;
}

bool CvPlayerCityStateUA::HasSpyGarrisonYieldModifiers() const
{
	return m_iSpyGarrisonYieldModifierCount > 0;
}

int CvPlayerCityStateUA::GetSpyKillGainSpyProgress() const
{
	return m_iSpyKillGainSpyProgress;
}

int CvPlayerCityStateUA::GetResourceYieldModifier(ResourceTypes eResource, YieldTypes eYield) const
{
	if (!m_ppiResourceYieldModifiers) return 0;
	return m_ppiResourceYieldModifiers[(int)eResource][(int)eYield];
}

bool CvPlayerCityStateUA::HasResourceYieldModifiers() const
{
	return m_iResourceYieldModifierCount > 0;
}

int CvPlayerCityStateUA::GetCoastalCityGrowthThresholdModifier() const
{
	return m_iCoastalCityGrowthThresholdModifier;
}

int CvPlayerCityStateUA::GetDiplomaticPrestigePerCity() const
{
	return m_iDiplomaticPrestigePerCity;
}

int CvPlayerCityStateUA::GetImprovementYieldModifier(ImprovementTypes eImprovement, YieldTypes eYield) const
{
	if (!m_ppiImprovementYieldModifiers) return 0;
	return m_ppiImprovementYieldModifiers[(int)eImprovement][(int)eYield];
}

bool CvPlayerCityStateUA::HasImprovementYieldModifiers() const
{
	return m_iImprovementYieldModifierCount > 0;
}

int CvPlayerCityStateUA::GetImprovementHappiness(ImprovementTypes eImprovement) const
{
	return (eImprovement >= 0 && (int)eImprovement < (int)m_aiImprovementHappiness.size()) ? m_aiImprovementHappiness[(int)eImprovement] : 0;
}

bool CvPlayerCityStateUA::HasImprovementHappiness() const
{
	return m_iImprovementHappinessCount > 0;
}

int CvPlayerCityStateUA::GetSpecialistPointRate(SpecialistTypes eSpecialist) const
{
	return (eSpecialist >= 0 && (int)eSpecialist < (int)m_aiSpecialistPointRate.size()) ? m_aiSpecialistPointRate[(int)eSpecialist] : 0;
}

int CvPlayerCityStateUA::GetGreatWorkGreatPersonPointsForCity(const CvCity* pCity, SpecialistTypes eSpecialist) const
{
	if (!pCity) return 0;
	int iTotal = 0;
	for (size_t i = 0; i < m_vGreatWorkGreatPersonPoints.size(); i++)
	{
		const GreatWorkGreatPersonPointsEntry& entry = m_vGreatWorkGreatPersonPoints[i];
		if (entry.m_iSpecialistType != (int)eSpecialist || entry.m_iRate <= 0) continue;
		if (entry.m_bCapitalOnly && !pCity->isCapital()) continue;
		int iNumGW = pCity->GetCityBuildings()->GetNumGreatWorks((GreatWorkClass)entry.m_iGreatWorkClassType);
		iTotal += iNumGW * entry.m_iRate;
	}
	return iTotal;
}

bool CvPlayerCityStateUA::HasGreatWorkGreatPersonPoints() const
{
	return !m_vGreatWorkGreatPersonPoints.empty();
}

int CvPlayerCityStateUA::GetGreatPersonOneShotModifier(UnitClassTypes eUnitClass) const
{
	return (eUnitClass >= 0 && (int)eUnitClass < (int)m_aiGreatPersonOneShotModifier.size()) ? m_aiGreatPersonOneShotModifier[(int)eUnitClass] : 0;
}

int CvPlayerCityStateUA::GetEnemyCityNoHealBesiegeCount() const { return m_iEnemyCityNoHealBesiegeCount; }
const std::vector<PurchasedBuildingXPEntry>& CvPlayerCityStateUA::GetPurchasedBuildingXPEntries() const { return m_vPurchasedBuildingXP; }
const std::vector<UnitBornYieldEntry>& CvPlayerCityStateUA::GetUnitBornYieldEntries() const { return m_vUnitBornYield; }
