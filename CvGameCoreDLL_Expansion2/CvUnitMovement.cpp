#include "CvGameCoreDLLPCH.h"
#include "CvPlot.h"
#include "CvCity.h"
#include "CvUnit.h"
#include "CvGlobals.h"
#include "CvUnitMovement.h"
#include "CvGameCoreUtils.h"
#include "CvCityStateUAClasses.h"
//	---------------------------------------------------------------------------
void CvUnitMovement::GetCostsForMove(const CvUnit* pUnit, const CvPlot* pFromPlot, const CvPlot* pToPlot, int iBaseMoves, int& iRegularCost, int& iRouteCost, int& iRouteFlatCost)
{
	CvPlayerAI& kPlayer = GET_PLAYER(pUnit->getOwner());
	CvPlayerTraits* pTraits = kPlayer.GetPlayerTraits();
	bool bFasterAlongRiver = pTraits->IsFasterAlongRiver();
	bool bFasterInHills = pTraits->IsFasterInHills();
	bool bIgnoreTerrainCost = pUnit->ignoreTerrainCost();
	//int iBaseMoves = pUnit->baseMoves(isWater()?DOMAIN_SEA:NO_DOMAIN);
	TeamTypes eUnitTeam = pUnit->getTeam();
	CvTeam& kUnitTeam = GET_TEAM(eUnitTeam);
	int iMoveDenominator = GC.getMOVE_DENOMINATOR();
	bool bRiverCrossing = pFromPlot->isRiverCrossing(directionXY(pFromPlot, pToPlot));
	FeatureTypes eFeature = pToPlot->getFeatureType();
	CvFeatureInfo* pFeatureInfo = (eFeature > NO_FEATURE) ? GC.getFeatureInfo(eFeature) : 0;
	TerrainTypes eTerrain = pToPlot->getTerrainType();
	CvTerrainInfo* pTerrainInfo = (eTerrain > NO_TERRAIN) ? GC.getTerrainInfo(eTerrain) : 0;

	if(bIgnoreTerrainCost || (bFasterAlongRiver && pToPlot->isRiver()) || (bFasterInHills && pToPlot->isHills()))
	{
		iRegularCost = 1;
	}
	else
	{
		iRegularCost = ((eFeature == NO_FEATURE) ? (pTerrainInfo ? pTerrainInfo->getMovementCost() : 0) : (pFeatureInfo ? pFeatureInfo->getMovementCost() : 0));

		// Hill cost, except for when a City is present here, then it just counts as flat land
		if((PlotTypes)pToPlot->getPlotType() == PLOT_HILLS && !pToPlot->isCity())
		{
			iRegularCost += GC.getHILLS_EXTRA_MOVEMENT();
		}

		if(iRegularCost > 0)
		{
			iRegularCost = std::max(1, (iRegularCost - pUnit->getExtraMoveDiscount()));
		}
	}

	// Is a unit's movement consumed for entering rough terrain?
	if(pToPlot->isRoughGround() && pUnit->IsRoughTerrainEndsTurn())
	{
		iRegularCost = INT_MAX;
	}

	else
	{
		if(!(bIgnoreTerrainCost || bFasterAlongRiver) && bRiverCrossing)
		{
			iRegularCost += GC.getRIVER_EXTRA_MOVEMENT();
		}

		iRegularCost *= iMoveDenominator;

		if(pToPlot->isHills() && pUnit->isHillsDoubleMove())
		{
			iRegularCost /= 2;
		}

		else if(pToPlot->isRiver() && pFromPlot->isRiver() && pUnit->isRiverDoubleMove())
		{
			iRegularCost /= 2;
		}

		else if((eFeature == NO_FEATURE) ? pUnit->isTerrainDoubleMove(eTerrain) : pUnit->isFeatureDoubleMove(eFeature))
		{
			iRegularCost /= 2;
		}

#if defined(MOD_PROMOTIONS_HALF_MOVE)
		else if((pToPlot->getFeatureType() == NO_FEATURE) ? pUnit->isTerrainHalfMove(pToPlot->getTerrainType()) : pUnit->isFeatureHalfMove(pToPlot->getFeatureType()))
		{
			iRegularCost *= 2;
		}
#endif
	}

	iRegularCost = std::min(iRegularCost, (iBaseMoves * iMoveDenominator + pUnit->GetExtraMoveTimesXX()));

	if(pFromPlot->isValidRoute(pUnit) && pToPlot->isValidRoute(pUnit) && ((kUnitTeam.isBridgeBuilding() || !(pFromPlot->isRiverCrossing(directionXY(pFromPlot, pToPlot))))))
	{
		CvRouteInfo* pFromRouteInfo = GC.getRouteInfo(pFromPlot->getRouteType());
		CvAssert(pFromRouteInfo != NULL);

		int iFromMovementCost = (pFromRouteInfo != NULL)? pFromRouteInfo->getMovementCost() : 0;
		int iFromFlatMovementCost = (pFromRouteInfo != NULL)? pFromRouteInfo->getFlatMovementCost() : 0;

		CvRouteInfo* pRouteInfo = GC.getRouteInfo(pToPlot->getRouteType());
		CvAssert(pRouteInfo != NULL);

		int iMovementCost = (pRouteInfo != NULL)? pRouteInfo->getMovementCost() : 0;
		int iFlatMovementCost = (pRouteInfo != NULL)? pRouteInfo->getFlatMovementCost() : 0;
		iFromMovementCost += kUnitTeam.getRouteChange(pFromPlot->getRouteType()) + pUnit->getRouteMovementChanges(pFromPlot->getRouteType());
		iMovementCost += kUnitTeam.getRouteChange(pToPlot->getRouteType()) + pUnit->getRouteMovementChanges(pToPlot->getRouteType());
		iRouteCost = std::max(iFromMovementCost,iMovementCost);
		iRouteFlatCost = std::max(iFromFlatMovementCost * iBaseMoves, iFlatMovementCost * iBaseMoves);
	}
	else if((MOD_TRAIT_WOOD_AS_ROAD_SP || pUnit->getOwner() == pToPlot->getOwner()) && (eFeature == FEATURE_FOREST || eFeature == FEATURE_JUNGLE) && pTraits->IsMoveFriendlyWoodsAsRoad())
	{
		CvRouteInfo* pRoadInfo = GC.getRouteInfo(ROUTE_ROAD);
		iRouteCost = pRoadInfo->getMovementCost();
		iRouteFlatCost = pRoadInfo->getFlatMovementCost() * iBaseMoves;
	}
	else
	{
		iRouteCost = INT_MAX;
		iRouteFlatCost = INT_MAX;
	}

	TeamTypes eTeam = pToPlot->getTeam();
	if(eTeam != NO_TEAM)
	{
		CvTeam* pPlotTeam = &GET_TEAM(eTeam);
		CvPlayer* pPlotPlayer = &GET_PLAYER(pToPlot->getOwner());

		// Great Wall increases movement cost by 1
		if(pPlotTeam->isBorderObstacle() || pPlotPlayer->isBorderObstacle())
		{
			if(!pToPlot->isWater() && pUnit->getDomainType() == DOMAIN_LAND)
			{
				// Don't apply penalty to OUR team or players we've given open borders to
				if(eUnitTeam != eTeam && !pPlotPlayer->IsAllowsOpenBordersToPlayer(pUnit->getOwner()))
				{
					iRegularCost += iMoveDenominator;
				}
			}
		}

#if defined(MOD_ROG_CORE)

		CvCity* pOwner = pToPlot->getWorkingCity();

		if (pOwner != NULL && GET_TEAM(pOwner->getTeam()).isAtWar(kPlayer.getTeam()))
		{
			if (pToPlot->isWater())
			{
				int iTempCost = pToPlot->getWorkingCity()->getWaterTileMovementReduce();
				iTempCost += GET_PLAYER(pOwner->getOwner()).GetWaterTileMovementReduceGlobal();
				if (iTempCost > 0)
				{
					iRegularCost += iMoveDenominator * iTempCost;
				}
			}

			else
			{
				int iTempCost = pToPlot->getWorkingCity()->getLandTileMovementReduce();
				iTempCost += GET_PLAYER(pOwner->getOwner()).GetLandTileMovementReduceGlobal();
				if (iTempCost > 0)
				{
				  iRegularCost += iMoveDenominator * iTempCost;
				}
			}
		}
#endif
	}
}

//	---------------------------------------------------------------------------
int CvUnitMovement::MovementCost(const CvUnit* pUnit, const CvPlot* pFromPlot, const CvPlot* pToPlot, int iBaseMoves, int iMaxMoves, int iMovesRemaining /*= 0*/)
{
	int iRegularCost;
	int iRouteCost;
	int iRouteFlatCost;

	CvAssertMsg(pToPlot->getTerrainType() != NO_TERRAIN, "TerrainType is not assigned a valid value");

	if(ConsumesAllMoves(pUnit, pFromPlot, pToPlot))
	{
		if (iMovesRemaining > 0)
			return iMovesRemaining;
		else
			return iMaxMoves;
	}
	else if(CostsOnlyOne(pUnit, pFromPlot, pToPlot))
	{
		return GC.getMOVE_DENOMINATOR();
	}
	else if(IsSlowedByZOC(pUnit, pFromPlot, pToPlot))
	{
		if (iMovesRemaining > 0)
			return iMovesRemaining;
		else
			return iMaxMoves;
	}

	GetCostsForMove(pUnit, pFromPlot, pToPlot, iBaseMoves, iRegularCost, iRouteCost, iRouteFlatCost);

	return std::max(1, std::min(iRegularCost, std::min(iRouteCost, iRouteFlatCost)));
}

//	---------------------------------------------------------------------------
int CvUnitMovement::MovementCostNoZOC(const CvUnit* pUnit, const CvPlot* pFromPlot, const CvPlot* pToPlot, int iBaseMoves, int iMaxMoves, int iMovesRemaining /*= 0*/)
{
	int iRegularCost;
	int iRouteCost;
	int iRouteFlatCost;

	CvAssertMsg(pToPlot->getTerrainType() != NO_TERRAIN, "TerrainType is not assigned a valid value");

	if(ConsumesAllMoves(pUnit, pFromPlot, pToPlot))
	{
		if (iMovesRemaining > 0)
			return iMovesRemaining;
		else
			return iMaxMoves;
	}
	else if(CostsOnlyOne(pUnit, pFromPlot, pToPlot))
	{
		return GC.getMOVE_DENOMINATOR();
	}

	GetCostsForMove(pUnit, pFromPlot, pToPlot, iBaseMoves, iRegularCost, iRouteCost, iRouteFlatCost);

	return std::max(1, std::min(iRegularCost, std::min(iRouteCost, iRouteFlatCost)));
}

//	---------------------------------------------------------------------------
bool CvUnitMovement::ConsumesAllMoves(const CvUnit* pUnit, const CvPlot* pFromPlot, const CvPlot* pToPlot)
{
	if(!pToPlot->isRevealed(pUnit->getTeam()) && pUnit->isHuman())
	{
		return true;
	}

	if (!pUnit->isEmbarked() && (pToPlot->IsAllowsWalkWater() || pFromPlot->IsAllowsWalkWater()))
	{
		return false;
	}

	if(!pFromPlot->isValidDomainForLocation(*pUnit))
	{
		// If we are a land unit that can embark, then do further tests.
#if defined(MOD_PATHFINDER_DEEP_WATER_EMBARKATION)
		if(pUnit->getDomainType() != DOMAIN_LAND || pUnit->canMoveAllTerrain())
			return true;
			
		if(pUnit->IsHoveringUnit()) {
			if (!pUnit->IsEmbarkDeepWater()) {
				return true;
			}
		} else {
			if (!pUnit->CanEverEmbark()) {
				return true;
			}
		}
#else
		if(pUnit->getDomainType() != DOMAIN_LAND || pUnit->IsHoveringUnit() || pUnit->canMoveAllTerrain() || !pUnit->CanEverEmbark())
			return true;
#endif
	}

	// if the unit can embark and we are transitioning from land to water or vice versa
#if defined(MOD_PATHFINDER_TERRAFIRMA)
	bool bFromWater = !pFromPlot->isTerraFirma(pUnit);
	bool bToWater = !pToPlot->isTerraFirma(pUnit);
	bool bCanEmbark = pUnit->CanEverEmbark();
#if defined(MOD_PATHFINDER_DEEP_WATER_EMBARKATION)
	if (pUnit->IsHoveringUnit() && pUnit->IsEmbarkDeepWater()) {
		bCanEmbark = true;
	}
#endif
	if(bToWater != bFromWater && bCanEmbark)
#else
	if(pToPlot->isWater() != pFromPlot->isWater() && pUnit->CanEverEmbark())
#endif
	{
		// Is the unit from a civ that can disembark for just 1 MP?
#if defined(MOD_PATHFINDER_TERRAFIRMA)
		if(!bToWater && bFromWater && pUnit->isEmbarked() && GET_PLAYER(pUnit->getOwner()).GetPlayerTraits()->IsEmbarkedToLandFlatCost())
#else
		if(!pToPlot->isWater() && pFromPlot->isWater() && pUnit->isEmbarked() && GET_PLAYER(pUnit->getOwner()).GetPlayerTraits()->IsEmbarkedToLandFlatCost())
#endif
		{
			return false;	// Then no, it does not.
		}

		if(!pUnit->canMoveAllTerrain())
		{
			return true;
		}
	}

	return false;
}

//	---------------------------------------------------------------------------
bool CvUnitMovement::CostsOnlyOne(const CvUnit* pUnit, const CvPlot* pFromPlot, const CvPlot* pToPlot)
{
	if(!pToPlot->isValidDomainForAction(*pUnit))
	{
		// If we are a land unit that can embark, then do further tests.
		if(pUnit->getDomainType() != DOMAIN_LAND || pUnit->IsHoveringUnit() || pUnit->canMoveAllTerrain() || !pUnit->CanEverEmbark())
			return true;
	}

	CvAssert(!pUnit->IsImmobile());

	if(pUnit->flatMovementCost() || pUnit->getDomainType() == DOMAIN_AIR)
	{
		return true;
	}

	// Is the unit from a civ that can disembark for just 1 MP?
	if(!pToPlot->isWater() && pFromPlot->isWater() && pUnit->isEmbarked() && GET_PLAYER(pUnit->getOwner()).GetPlayerTraits()->IsEmbarkedToLandFlatCost())
	{
		return true;
	}

	return false;
}

#if defined(MOD_SP_UNIQUE_CITYSTATE)
//	--------------------------------------------------------------------------------
// Belgrade UA support: is any alive player currently holding a ZOC-range bonus?
// The lookup is cached per game turn, so the A* hot path (IsSlowedByZOC) pays O(1) for the query itself
// in the common case (nobody allied with Belgrade). Note this only picks the scan window size: the scan
// is 3x3 while no bonus exists and expands globally to 5x5 while any bonus is active.
// All clients derive the same value from the deterministic CSUA state, so multiplayer stays in sync;
// a mid-turn diplomacy change may lag by one turn, which is tolerated.
static bool AnyPlayerHasZOCRangeBonus()
{
	static int s_iCachedTurn = -1;
	static bool s_bCached = false;

	int iTurn = GC.getGame().getGameTurn();
	if (iTurn != s_iCachedTurn)
	{
		s_iCachedTurn = iTurn;
		s_bCached = false;

		for (int iPlayer = 0; iPlayer < MAX_CIV_PLAYERS; iPlayer++)
		{
			CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iPlayer);
			if (!kPlayer.isAlive())
			{
				continue;
			}

			CvPlayerCityStateUA* pCSUA = kPlayer.GetPlayerCityStateUA();
			if (pCSUA != NULL && pCSUA->GetZOCRangeBonus() > 0)
			{
				s_bCached = true;
				break;
			}
		}
	}

	return s_bCached;
}
#endif

//	--------------------------------------------------------------------------------
bool CvUnitMovement::IsSlowedByZOC(const CvUnit* pUnit, const CvPlot* pFromPlot, const CvPlot* pToPlot)
{
	if (pUnit->IsIgnoreZOC() || CostsOnlyOne(pUnit, pFromPlot, pToPlot))
	{
		return false;
	}

	// Zone of Control
	if(GC.getZONE_OF_CONTROL_ENABLED() > 0)
	{
		IDInfo* pAdjUnitNode;
		CvUnit* pLoopUnit;

		int iFromPlotX = pFromPlot->getX();
		int iFromPlotY = pFromPlot->getY();
		int iToPlotX = pToPlot->getX();
		int iToPlotY = pToPlot->getY();
		TeamTypes unit_team_type     = pUnit->getTeam();
		DomainTypes unit_domain_type = pUnit->getDomainType();
		bool bIsVisibleEnemyUnit     = pToPlot->isVisibleEnemyUnit(pUnit);
		CvTeam& kUnitTeam = GET_TEAM(unit_team_type);

#if defined(MOD_SP_UNIQUE_CITYSTATE)
		// Belgrade UA: an ally's units exert ZOC at +1 range. iScanRange is the largest ZOC radius any
		// source may have this turn. When no player holds a ZOC-range bonus (the common case) it stays 1
		// and the scan below is a 3x3 window; when any bonus exists it expands globally to 5x5.
		int iScanRange = 1;
		if (MOD_SP_UNIQUE_CITYSTATE && AnyPlayerHasZOCRangeBonus())
		{
			iScanRange = 2;
		}
#else
		int iScanRange = 1;
#endif

		// Scan the neighbourhood of the start plot within iScanRange for ZOC sources. For iScanRange == 1
		// the plotDistance filter keeps exactly the six hex neighbours (the same source set as the original
		// implementation), though the loop itself now visits 3x3 plots instead of 6 directions.
		for(int iDX = -iScanRange; iDX <= iScanRange; iDX++)
		{
			for(int iDY = -iScanRange; iDY <= iScanRange; iDY++)
			{
				CvPlot* pAdjPlot = GC.getMap().plot(iFromPlotX + iDX, iFromPlotY + iDY);
				if(NULL == pAdjPlot)
				{
					continue;
				}

				// A ZOC source must be strictly away from, yet within reach of, the mover's start plot.
				int iFromDist = plotDistance(iFromPlotX, iFromPlotY, pAdjPlot->getX(), pAdjPlot->getY());
				if(iFromDist <= 0 || iFromDist > iScanRange)
				{
					continue;
				}

				// check city zone of control (cities always exert ZOC at radius 1)
				if(iFromDist == 1 && pAdjPlot->isEnemyCity(*pUnit))
				{
					// Destination adjacent to enemy city?
					if(plotDistance(iToPlotX, iToPlotY, pAdjPlot->getX(), pAdjPlot->getY()) == 1)
					{
						return true;
					}
				}

				pAdjUnitNode = pAdjPlot->headUnitNode();
				// Loop through all units to see if there's an enemy unit here
				while(pAdjUnitNode != NULL)
				{
					if((pAdjUnitNode->eOwner >= 0) && pAdjUnitNode->eOwner < MAX_PLAYERS)
					{
						pLoopUnit = (GET_PLAYER(pAdjUnitNode->eOwner).getUnit(pAdjUnitNode->iID));
					}
					else
					{
						pLoopUnit = NULL;
					}

					pAdjUnitNode = pAdjPlot->nextUnitNode(pAdjUnitNode);

					if(!pLoopUnit) continue;

					TeamTypes unit_loop_team_type = pLoopUnit->getTeam();

					if(pLoopUnit->isInvisible(unit_team_type,false)) continue;

					// Combat unit?
					if(!pLoopUnit->IsCombatUnit())
					{
						continue;
					}

					// At war with this unit's team?
					if(unit_loop_team_type == BARBARIAN_TEAM || kUnitTeam.isAtWar(unit_loop_team_type))
					{

						// Same Domain?

						DomainTypes loop_unit_domain_type = pLoopUnit->getDomainType();
						if(loop_unit_domain_type != unit_domain_type)
						{
							// this is valid
							if(loop_unit_domain_type == DOMAIN_SEA && unit_domain_type)
							{
								// continue on
							}
#if defined(MOD_BUGFIX_HOVERING_PATHFINDER)
							// hovering units always exert a ZOC
							else if (pLoopUnit->IsHoveringUnit()) {
								// continue on
							}
#endif
							else
							{
								continue;
							}
						}

						// Embarked?
						if(unit_domain_type == DOMAIN_LAND && pLoopUnit->isEmbarked())
						{
							continue;
						}

						// Belgrade UA: this source's own ZOC radius is 1 unless its owner holds a
						// ZOC-range bonus (read per source, so normal units keep radius 1).
						int iSrcRange = 1;
#if defined(MOD_SP_UNIQUE_CITYSTATE)
						if (MOD_SP_UNIQUE_CITYSTATE)
						{
							CvPlayerCityStateUA* pSrcCSUA = GET_PLAYER(pLoopUnit->getOwner()).GetPlayerCityStateUA();
							if (pSrcCSUA != NULL && pSrcCSUA->GetZOCRangeBonus() > 0)
							{
								iSrcRange = 1 + pSrcCSUA->GetZOCRangeBonus();
							}
						}
#endif

						// The mover's start must be inside this source's ZOC range...
						if(iFromDist > iSrcRange)
						{
							continue;
						}

						// Don't check Enemy Unit's plot
						if(!bIsVisibleEnemyUnit)
						{
							// ...and its destination must also be inside the source's ZOC range.
							int iToDist = plotDistance(iToPlotX, iToPlotY, pAdjPlot->getX(), pAdjPlot->getY());
							if(iToDist > 0 && iToDist <= iSrcRange)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}
