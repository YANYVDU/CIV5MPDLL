/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */

#include "CvGameCoreDLLPCH.h"
#include "CvDatabaseUtility.h"

// must be included after all other headers
#include "LintFree.h"

CvDatabaseUtility::CvDatabaseUtility()
{

}
//------------------------------------------------------------------------------
CvDatabaseUtility::~CvDatabaseUtility()
{
	//Remove any cached statements
	ClearResults();
}
//------------------------------------------------------------------------------
void CvDatabaseUtility::ClearResults()
{
	for(ResultsMap::const_iterator it = m_storedResults.begin(); it != m_storedResults.end(); ++it)
	{
		Database::Results* pResults = it->second;
		delete pResults;
	}

	m_storedResults.clear();
}
//------------------------------------------------------------------------------
void CvDatabaseUtility::ClearResults(const std::string& strKey)
{
	Database::Results* pResults = GetResults(strKey);
	if(pResults)
	{
		delete pResults;
		m_storedResults.erase(strKey);
	}
}
//------------------------------------------------------------------------------
Database::Results* CvDatabaseUtility::GetResults(const std::string& strKey)
{
	ResultsMap::const_iterator it = m_storedResults.find(strKey);
	if(it != m_storedResults.end())
	{
		it->second->Reset();
		return it->second;
	}

	return NULL;
}
//------------------------------------------------------------------------------
Database::Results* CvDatabaseUtility::PrepareResults(const std::string& strKey, const char* szStmt)
{
	Database::Results* pResults = new Database::Results();

	if(DB.Execute(*pResults, szStmt))
	{
		Database::Results* pOldResults = GetResults(strKey);
		if(pOldResults)
			delete pOldResults;

		m_storedResults[strKey] = pResults;
		return pResults;
	}
	else
	{
		delete pResults;
	}

	return NULL;
}
//------------------------------------------------------------------------------
bool CvDatabaseUtility::Initialize2DArray(int**& ppArray, const char* szTable1Name, const char* szTable2Name, int iDefault /* = 0 */)
{
	const int iCount1 = MaxRows(szTable1Name);
	const int iCount2 = MaxRows(szTable2Name);

	if(iCount1 <= 0 || iCount2 <= 0)
	{
		ppArray = NULL;
		CvAssertMsg(false, "Cannot initialize array to 0 size.");
		return false;
	}

	unsigned int iNumBytes = iCount1 * sizeof(int*) + iCount1 * iCount2 * sizeof(int);
	unsigned char* pData = FNEW(unsigned char[iNumBytes], c_eCiv5GameplayDLL, 0);
	ppArray = (int**)pData;
	ppArray[0] = (int*)(pData + iCount1 * sizeof(int*));
	for(int i = 0; i < iCount2; ++i)
	{
		ppArray[0][i] = iDefault;
	}
	for(int i = 1; i < iCount1; i++)
	{
		ppArray[i] = ppArray[i-1] + iCount2;
		for(int j = 0; j < iCount2; ++j)
		{
			ppArray[i][j] = iDefault;
		}
	}

	return true;
}
//------------------------------------------------------------------------------
void CvDatabaseUtility::Initialize2DArray(int**& ppArray, const size_t iCount1, const size_t iCount2, int iDefault /*= 0*/)
{
	if(iCount1 <= 0 || iCount2 <= 0)
	{
		CvAssertMsg(false, "Cannot initialize array to 0 size.");
		return;
	}

	unsigned int iNumBytes = iCount1 * sizeof(int*) + iCount1 * iCount2 * sizeof(int);
	unsigned char* pData = FNEW(unsigned char[iNumBytes], c_eCiv5GameplayDLL, 0);
	ppArray = (int**)pData;
	ppArray[0] = (int*)(pData + iCount1 * sizeof(int*));
	for(unsigned int i = 0; i < iCount2; ++i)
	{
		ppArray[0][i] = iDefault;
	}
	for(unsigned int i = 1; i < iCount1; i++)
	{
		ppArray[i] = ppArray[i-1] + iCount2;
		for(unsigned int j = 0; j < iCount2; ++j)
		{
			ppArray[i][j] = iDefault;
		}
	}
}
//------------------------------------------------------------------------------
void CvDatabaseUtility::SafeDelete2DArray(int**& ppArray)
{
	if(ppArray != NULL && ReleaseShared2DArray(ppArray))
	{
		ppArray = NULL;
		return;
	}
	delete[] ppArray;
	ppArray = NULL;
}

//------------------------------------------------------------------------------
// Shared all-zero 2D tables (Share2DArrayIfZero): one per table shape, reference counted, on read-only pages.
// The database is loaded and unloaded on one thread, as is the releasing (the Info destructors).
namespace
{
struct Shared2DArray
{
	size_t iCount1;
	size_t iCount2;
	size_t iRefs;
	int** ppArray;
};
const size_t MAX_SHARED_2D_ARRAYS = 64;
// No shared table has more rows than this; a bigger "shape" means the layout was not recognized.
const size_t MAX_SHARED_TABLE_ROWS = 1024;
Shared2DArray s_aShared2DArrays[MAX_SHARED_2D_ARRAYS];
size_t s_iNumShared2DArrays = 0;

size_t Size2DArray(size_t iCount1, size_t iCount2)
{
	return iCount1 * sizeof(int*) + iCount1 * iCount2 * sizeof(int);
}

int** CreateShared2DArray(size_t iCount1, size_t iCount2)
{
	// Same layout as Initialize2DArray. VirtualAlloc returns zeroed pages; only the row pointers are written.
	const size_t iNumBytes = Size2DArray(iCount1, iCount2);
	unsigned char* pData = (unsigned char*)VirtualAlloc(NULL, iNumBytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if(pData == NULL)
		return NULL;

	int** ppArray = (int**)pData;
	int* pRows = (int*)(pData + iCount1 * sizeof(int*));
	for(size_t i = 0; i < iCount1; ++i)
		ppArray[i] = pRows + i * iCount2;

	DWORD dwOldProtect = 0;
	if(!VirtualProtect(pData, iNumBytes, PAGE_READONLY, &dwOldProtect))
	{
		VirtualFree(pData, 0, MEM_RELEASE);
		return NULL;
	}
	return ppArray;
}
}
//------------------------------------------------------------------------------
bool CvDatabaseUtility::IsShared2DArray(const int* const* ppArray)
{
	for(size_t i = 0; i < s_iNumShared2DArrays; ++i)
	{
		if(s_aShared2DArrays[i].ppArray == ppArray)
			return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool CvDatabaseUtility::ReleaseShared2DArray(const int* const* ppArray)
{
	for(size_t i = 0; i < s_iNumShared2DArrays; ++i)
	{
		Shared2DArray& kShared = s_aShared2DArrays[i];
		if(kShared.ppArray != ppArray)
			continue;

		// A shared block is released exactly once per share; anything else is a contract violation
		if(kShared.iRefs == 0)
		{
			CvAssertMsg(false, "Shared 2D array released more often than shared.");
			return true;
		}

		if(--kShared.iRefs == 0)
		{
			VirtualFree(kShared.ppArray, 0, MEM_RELEASE);
			s_aShared2DArrays[i] = s_aShared2DArrays[--s_iNumShared2DArrays];
		}
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
void CvDatabaseUtility::Share2DArrayIfZero(int**& ppArray)
{
	if(ppArray == NULL || IsShared2DArray(ppArray))
		return;

	// Read the shape back from the Initialize2DArray layout: iCount1 row pointers, then iCount1 rows of iCount2 ints,
	// row i at ppArray[0] + i * iCount2. Anything else is left alone.
	const unsigned char* pBlock = (const unsigned char*)ppArray;
	const unsigned char* pFirstRow = (const unsigned char*)ppArray[0];
	if(pFirstRow <= pBlock || (size_t)(pFirstRow - pBlock) % sizeof(int*) != 0)
		return;
	const size_t iCount1 = (size_t)(pFirstRow - pBlock) / sizeof(int*);
	if(iCount1 < 2 || iCount1 > MAX_SHARED_TABLE_ROWS)
		return; // too few rows to read the stride back, or not the Initialize2DArray layout at all
	if(ppArray[1] <= ppArray[0])
		return;
	const size_t iCount2 = (size_t)(ppArray[1] - ppArray[0]);
	if(iCount2 != (size_t)NUM_YIELD_TYPES)
		return; // every table shared through this helper is rows-by-Yields; anything else is not ours
	for(size_t i = 2; i < iCount1; ++i)
	{
		if(ppArray[i] != ppArray[0] + i * iCount2)
			return;
	}

	const int* pRows = ppArray[0];
	for(size_t i = 0; i < iCount1 * iCount2; ++i)
	{
		if(pRows[i] != 0)
			return;
	}

	Shared2DArray* pShared = NULL;
	for(size_t i = 0; i < s_iNumShared2DArrays; ++i)
	{
		if(s_aShared2DArrays[i].iCount1 == iCount1 && s_aShared2DArrays[i].iCount2 == iCount2)
		{
			pShared = &s_aShared2DArrays[i];
			break;
		}
	}
	if(pShared == NULL)
	{
		if(s_iNumShared2DArrays == MAX_SHARED_2D_ARRAYS)
			return; // keep the table's own block
		int** ppNew = CreateShared2DArray(iCount1, iCount2);
		if(ppNew == NULL)
			return; // keep the table's own block
		pShared = &s_aShared2DArrays[s_iNumShared2DArrays++];
		pShared->iCount1 = iCount1;
		pShared->iCount2 = iCount2;
		pShared->iRefs = 0;
		pShared->ppArray = ppNew;
	}

	delete[] ppArray;
	ppArray = pShared->ppArray;
	pShared->iRefs++;
}

//------------------------------------------------------------------------------
bool CvDatabaseUtility::PopulateArrayByExistence(bool*& pArray, const char* szTypeTableName, const char* szDataTableName, const char* szTypeColumn, const char* szFilterColumn, const char* szFilterValue)
{
	InitializeArray(pArray, MaxRows(szTypeTableName), false);

	std::string strKey = "_PABE_";
	strKey.append(szTypeTableName);
	strKey.append(szDataTableName);
	strKey.append(szFilterColumn);

	Database::Results* pResults = GetResults(strKey);
	if(pResults == NULL)
	{
		char szSQL[512];
		sprintf_s(szSQL, "select %s.ID from %s inner join %s on %s = %s.Type where %s = ?", szTypeTableName, szDataTableName, szTypeTableName, szTypeColumn, szTypeTableName, szFilterColumn);

		pResults = PrepareResults(strKey, szSQL);
		if(pResults == NULL)
			return false;
	}

	if(!pResults->Bind(1, szFilterValue, false))
	{
		CvAssertMsg(false, GetErrorMessage());
		return false;
	}

	while(pResults->Step())
	{
		const int idx = pResults->GetInt(0);
		pArray[idx] = true;
	}

	pResults->Reset();

	return true;
}
//------------------------------------------------------------------------------
bool CvDatabaseUtility::PopulateArrayByExistence(int*& pArray, const char* szTypeTableName, const char* szDataTableName, const char* szTypeColumn, const char* szFilterColumn, const char* szFilterValue)
{
	InitializeArray(pArray, MaxRows(szTypeTableName), -1);

	std::string strKey = "_PABE_";
	strKey.append(szTypeTableName);
	strKey.append(szDataTableName);
	strKey.append(szFilterColumn);

	Database::Results* pResults = GetResults(strKey);
	if(pResults == NULL)
	{
		char szSQL[512];
		sprintf_s(szSQL, "select %s.ID from %s inner join %s on %s = %s.Type where %s = ?", szTypeTableName, szDataTableName, szTypeTableName, szTypeColumn, szTypeTableName, szFilterColumn);
		pResults = PrepareResults(strKey, szSQL);
		if(pResults == NULL)
			return false;
	}

	if(!pResults->Bind(1, szFilterValue, false))
	{
		CvAssertMsg(false, GetErrorMessage());
		return false;
	}

	int idx = 0;
	while(pResults->Step())
	{
		pArray[idx++] = pResults->GetInt(0);
	}

	pResults->Reset();

	return true;
}
//------------------------------------------------------------------------------
bool CvDatabaseUtility::PopulateArrayByExistence(std::tr1::unordered_set<int>& set, const char* szTypeTableName, const char* szDataTableName, const char* szTypeColumn, const char* szFilterColumn, const char* szFilterValue)
{
	std::string strKey = "_PABE_";
	strKey.append(szTypeTableName);
	strKey.append(szDataTableName);
	strKey.append(szFilterColumn);

	Database::Results* pResults = GetResults(strKey);
	if(pResults == NULL)
	{
		char szSQL[512];
		sprintf_s(szSQL, "select %s.ID from %s inner join %s on %s = %s.Type where %s = ?", szTypeTableName, szDataTableName, szTypeTableName, szTypeColumn, szTypeTableName, szFilterColumn);
		pResults = PrepareResults(strKey, szSQL);
		if(pResults == NULL)
			return false;
	}

	if(!pResults->Bind(1, szFilterValue, false))
	{
		CvAssertMsg(false, GetErrorMessage());
		return false;
	}

	int idx = 0;
	while(pResults->Step())
	{
		set.insert(pResults->GetInt(0));
	}

	pResults->Reset();

	return true;
}
//------------------------------------------------------------------------------
bool CvDatabaseUtility::PopulateArrayByExistence(std::vector<int>& vector, const char* szTypeTableName, const char* szDataTableName, const char* szTypeColumn, const char* szFilterColumn, const char* szFilterValue)
{
	std::string strKey = "_PABE_";
	strKey.append(szTypeTableName);
	strKey.append(szDataTableName);
	strKey.append(szFilterColumn);

	Database::Results* pResults = GetResults(strKey);
	if(pResults == NULL)
	{
		char szSQL[512];
		sprintf_s(szSQL, "select %s.ID from %s inner join %s on %s = %s.Type where %s = ?", szTypeTableName, szDataTableName, szTypeTableName, szTypeColumn, szTypeTableName, szFilterColumn);
		pResults = PrepareResults(strKey, szSQL);
		if(pResults == NULL)
			return false;
	}

	if(!pResults->Bind(1, szFilterValue, false))
	{
		CvAssertMsg(false, GetErrorMessage());
		return false;
	}

	int idx = 0;
	while(pResults->Step())
	{
		vector.push_back(pResults->GetInt(0));
	}

	pResults->Reset();

	return true;
}
//------------------------------------------------------------------------------
bool CvDatabaseUtility::PopulateArrayByValue(int*& pArray, const char* szTypeTableName, const char* szDataTableName, const char* szTypeColumn, const char* szFilterColumn, const char* szFilterValue, const char* szValueColumn, int iDefaultValue /* = 0 */, int iMinArraySize /* = 0 */)
{
	int iSize = MaxRows(szTypeTableName);
	InitializeArray(pArray, (iSize<iMinArraySize)?iMinArraySize:iSize, iDefaultValue);

	std::string strKey = "_PABV_";
	strKey.append(szTypeTableName);
	strKey.append(szDataTableName);
	strKey.append(szFilterColumn);
	strKey.append(szValueColumn);

	Database::Results* pResults = GetResults(strKey);
	if(pResults == NULL)
	{
		char szSQL[512];
		sprintf_s(szSQL, "select %s.ID, %s from %s inner join %s on %s = %s.Type where %s = ?", szTypeTableName, szValueColumn, szDataTableName, szTypeTableName, szTypeColumn, szTypeTableName, szFilterColumn);
		pResults = PrepareResults(strKey, szSQL);
		if(pResults == NULL)
			return false;
	}

	if(!pResults->Bind(1, szFilterValue, false))
	{
		CvAssertMsg(false, GetErrorMessage());
		return false;
	}
	while(pResults->Step())
	{
		const int idx = pResults->GetInt(0);
		const int value = pResults->GetInt(1);
		pArray[idx] = value;
	}

	pResults->Reset();

	return true;
}
//------------------------------------------------------------------------------
bool CvDatabaseUtility::SetFlavors(int*& pFlavorsArray,
                                   const char* szTableName,
                                   const char* szFilterColumn,
                                   const char* szFilterValue,
                                   int iDefaultValue)
{
	return PopulateArrayByValue(pFlavorsArray, "Flavors", szTableName,
	                            "FlavorType", szFilterColumn, szFilterValue, "Flavor", iDefaultValue);
}
//------------------------------------------------------------------------------
bool CvDatabaseUtility::SetYields(int*& pYieldsArray,
                                  const char* szTableName,
                                  const char* szFilterColumn,
                                  const char* szFilterValue)
{
	return PopulateArrayByValue(pYieldsArray, "Yields", szTableName,
	                            "YieldType", szFilterColumn, szFilterValue, "Yield");
}
//------------------------------------------------------------------------------
int CvDatabaseUtility::MaxRows(const char* szTableName)
{
	char szSQL[256] = {0};
	sprintf_s(szSQL, "SELECT max(rowid) from %s", szTableName);
	Database::Results kResults;
	int maxValue = 0;
	if(DB.Execute(kResults, szSQL))
	{
		if(kResults.Step())
		{
			//Since some rowid's start at 0 in Civ, let's increase this # by 1.
			maxValue = kResults.GetInt(0) + 1;
		}
	}

	return maxValue;
}
//------------------------------------------------------------------------------
const char* CvDatabaseUtility::GetErrorMessage() const
{
	return DB.ErrorMessage();
}
//------------------------------------------------------------------------------