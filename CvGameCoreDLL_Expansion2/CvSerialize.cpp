#include "CvGameCoreDLLPCH.h"
#include "CvSerialize.h"
#include "FAutoVariable.h"

const std::string EmptyString = "";

void CvSyncArchiveCollectDeltas(FAutoArchive& syncArchive, const std::vector<FAutoVariableBase*>& vars)
{
	for (std::vector<FAutoVariableBase*>::const_iterator it = vars.begin(); it != vars.end(); ++it)
	{
		ICvSyncVar& syncVar = static_cast<ICvSyncVar&>(*(*it));
		if (syncVar.hasDelta())
		{
			syncArchive.touch(syncVar);
			syncVar.clearDelta();
		}
	}
}

// Called from FAutoVariable::compare when a synchronized variable disagrees with the other
// machine's copy: write the variable name and both values, so a desync report says which field
// drifted and what each side had.
void FAutoVariableDesync::Report(const std::string& varName, const std::string& localValue, const std::string& remoteValue)
{
	if(gDLL == NULL)
	{
		return;
	}

	// One desync usually drags a large batch of variables with it; cap per turn so the network
	// log stays readable.
	static int s_iLoggedTurn = -1;
	static int s_iLoggedThisTurn = 0;
	const int iTurn = GC.getGame().getGameTurn();
	if(iTurn != s_iLoggedTurn)
	{
		s_iLoggedTurn = iTurn;
		s_iLoggedThisTurn = 0;
	}

	if(s_iLoggedThisTurn >= 100)
	{
		return;
	}
	++s_iLoggedThisTurn;

	std::ostringstream strMsg;
	strMsg << "Desync, turn " << iTurn << ": " << varName
		<< " local=" << localValue
		<< " remote=" << remoteValue
		<< "\n";

	gDLL->netMessageDebugLog(strMsg.str());

	// Also into the mod log, so it lines up with the rest of the turn's output
	CUSTOMLOG("Desync, turn %d: %s local=%s remote=%s", iTurn, varName.c_str(), localValue.c_str(), remoteValue.c_str());
}

namespace
{
	// Variables whose received sync value is compared and logged but never written. They are
	// running totals the game maintains from state that is not in the sync check (worked plots,
	// production queues, city-state friendships), so a written value no longer matches that state
	// and the difference would survive every later change.
	//
	// One entry for now: it proves that a received sync check really goes through loadDelta.
	const char* const COMPARE_ONLY_VARIABLES[] =
	{
		"CvCity::m_aiBaseYieldRateFromTerrain",
	};

	bool IsCompareOnly(const std::string& varName)
	{
		for(int i = 0; i < _countof(COMPARE_ONLY_VARIABLES); ++i)
		{
			if(varName == COMPARE_ONLY_VARIABLES[i])
			{
				return true;
			}
		}

		return false;
	}
}

bool FAutoVariableDesync::AppliesRemoteValue(const std::string& varName)
{
	return !IsCompareOnly(varName);
}

void FAutoVariableDesync::NoteRemoteValueSkipped(const std::string& varName, const std::string& localValue, const std::string& remoteValue)
{
	if(gDLL == NULL)
	{
		return;
	}

	static int s_iLoggedTurn = -1;
	static int s_iLoggedThisTurn = 0;
	const int iTurn = GC.getGame().getGameTurn();
	if(iTurn != s_iLoggedTurn)
	{
		s_iLoggedTurn = iTurn;
		s_iLoggedThisTurn = 0;
	}

	if(s_iLoggedThisTurn >= 100)
	{
		return;
	}
	++s_iLoggedThisTurn;

	std::ostringstream strMsg;
	strMsg << "Sync, turn " << iTurn << ": " << varName
		<< " kept local=" << localValue
		<< ", refused remote=" << remoteValue
		<< "\n";

	gDLL->netMessageDebugLog(strMsg.str());

	CUSTOMLOG("Sync, turn %d: %s kept local=%s, refused remote=%s", iTurn, varName.c_str(), localValue.c_str(), remoteValue.c_str());
}