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
#if defined(_WIN32)
namespace
{
// The EXE registers every *.Civ5Pkg below its Assets folder (its package finder searches the folder
// recursively) without checking its version. A registered package's folder joins the file catalog, so
// an XML file there with the name of a base game database file replaces that file at every start,
// whatever the Mods menu says; that is also how a modpack brings its own database. An old modpack or
// an old copy of another mod left below Assets\DLC therefore sets up its own version of the tables
// this DLL reads, and verifying the game files does not remove it.
const char* const s_aszFiraxisPackageIDs[] =
{
	"293C1EE3-1176-44f6-AC1F-59663826DE74", // DLC_01
	"B685D5DE-7CCA-4e75-81B4-2F60754E6330", // DLC_02
	"ECF7C605-BA11-4CAC-8D80-D71306AAC471", // DLC_03
	"B3030D39-C0D8-4bc7-91B1-7AD1CAF585AB", // DLC_04
	"112C22B2-5308-42b6-B734-171CCAB3037B", // DLC_05
	"BBB0D085-A0B1-4475-B007-3E549CF3ADC3", // DLC_06
	"EA67AED5-5859-4875-BF3A-360FE9E55D1B", // DLC_07
	"7459BA32-5764-44ae-8E95-01AD0E0EFD48", // DLC_Deluxe
	"3F49DF54-68B6-44d1-A930-A168628FAA59", // DLC_SP_Maps
	"46EAEFFC-7B1D-443d-BFC8-F825DFEFB094", // DLC_SP_Maps_2
	"4255F5F7-D3AB-4e55-ACEE-4670082040ED", // DLC_SP_Maps_3
	"0E3751A1-F840-4e1b-9706-519BF484E59D", // Expansion
	"6DA07636-4123-4018-B643-6575B4EC336B", // Expansion2
	"8871E748-29A4-4910-8C57-8C99E32D0167", // Shared
	"CBE91AF6-0683-4326-BC77-58BB489E509F", // Tablet
};

struct CvContentPackage
{
	std::string strFile;         // relative to the game folder
	std::string strName;
	std::string strGameplayFile; // first database file in the package folder, empty if there is none
};

std::string WideToUtf8(const wchar_t* wszText)
{
	if(wszText == NULL)
	{
		return std::string();
	}

	const int iLength = WideCharToMultiByte(CP_UTF8, 0, wszText, -1, NULL, 0, NULL, NULL);
	if(iLength <= 1)
	{
		return std::string();
	}

	std::string strResult(iLength, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wszText, -1, &strResult[0], iLength, NULL, NULL);
	strResult.resize(iLength - 1);	// drop the terminator WideCharToMultiByte wrote
	return strResult;
}

bool IsFiraxisPackageID(const std::string& strID)
{
	for(size_t i = 0; i < _countof(s_aszFiraxisPackageIDs); i++)
	{
		if(_stricmp(strID.c_str(), s_aszFiraxisPackageIDs[i]) == 0)
		{
			return true;
		}
	}
	return false;
}

// Read a small text file whole; empty on failure or if larger than cchMax.
std::string ReadWholeFile(const std::wstring& strPath, size_t cchMax)
{
	std::string strContents;

	FILE* pFile = NULL;
	if(_wfopen_s(&pFile, strPath.c_str(), L"rb") != 0 || pFile == NULL)
	{
		return strContents;
	}

	strContents.resize(cchMax);
	const size_t cbRead = fread(&strContents[0], 1, cchMax, pFile);
	fclose(pFile);
	strContents.resize(cbRead);

	return strContents;
}

// Text between the first opening tag named szTag (attributes allowed) and its closing tag.
std::string GetElementText(const std::string& strXml, const char* szTag, std::string::size_type uiFrom = 0)
{
	std::string strOpen = "<";
	strOpen += szTag;

	std::string::size_type uiStart = strXml.find(strOpen, uiFrom);
	if(uiStart == std::string::npos)
	{
		return std::string();
	}

	// Reject a longer tag that merely starts with this one, such as <NameX> for <Name>.
	const std::string::size_type uiAfter = uiStart + strOpen.size();
	if(uiAfter < strXml.size())
	{
		const char cNext = strXml[uiAfter];
		if(cNext != '>' && cNext != ' ' && cNext != '\t' && cNext != '\r' && cNext != '\n')
		{
			return std::string();
		}
	}

	const std::string::size_type uiOpenEnd = strXml.find('>', uiAfter);
	if(uiOpenEnd == std::string::npos)
	{
		return std::string();
	}

	std::string strClose = "</";
	strClose += szTag;
	strClose += ">";

	const std::string::size_type uiEnd = strXml.find(strClose, uiOpenEnd + 1);
	if(uiEnd == std::string::npos)
	{
		return std::string();
	}

	return strXml.substr(uiOpenEnd + 1, uiEnd - uiOpenEnd - 1);
}

std::string TrimWhitespace(const std::string& strText)
{
	const std::string::size_type uiStart = strText.find_first_not_of(" \t\r\n");
	if(uiStart == std::string::npos)
	{
		return std::string();
	}

	const std::string::size_type uiEnd = strText.find_last_not_of(" \t\r\n");
	return strText.substr(uiStart, uiEnd - uiStart + 1);
}

// Package GUIDs are written as {XXXXXXXX-....-XXXXXXXXXXXX}; compare without the braces.
std::string TrimBraces(const std::string& strText)
{
	std::string strResult = TrimWhitespace(strText);

	if(!strResult.empty() && strResult[0] == '{')
	{
		strResult.erase(0, 1);
	}
	if(!strResult.empty() && strResult[strResult.size() - 1] == '}')
	{
		strResult.erase(strResult.size() - 1);
	}

	return strResult;
}

// First XML file below strDir that defines game data, relative to strDir; empty if there is none.
std::wstring FindDatabaseXml(const std::wstring& strDir, const std::wstring& strRelative, int iDepth)
{
	if(iDepth > 8)
	{
		return std::wstring();
	}

	WIN32_FIND_DATAW kFindData;
	HANDLE hFind = FindFirstFileW((strDir + L"\\*").c_str(), &kFindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return std::wstring();
	}

	std::wstring strFound;

	do
	{
		if(wcscmp(kFindData.cFileName, L".") == 0 || wcscmp(kFindData.cFileName, L"..") == 0)
		{
			continue;
		}

		// A junction or symbolic link can point back up the tree, and the depth limit alone would
		// then rescan the same files; skip them.
		if((kFindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0)
		{
			continue;
		}

		const std::wstring strEntry = strDir + L"\\" + kFindData.cFileName;
		const std::wstring strEntryRelative = strRelative.empty() ? std::wstring(kFindData.cFileName) : strRelative + L"\\" + kFindData.cFileName;

		if((kFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			strFound = FindDatabaseXml(strEntry, strEntryRelative, iDepth + 1);
		}
		else
		{
			const size_t cchName = wcslen(kFindData.cFileName);
			if(cchName > 4 && _wcsicmp(kFindData.cFileName + cchName - 4, L".xml") == 0)
			{
				// The root element is at the top of the file; a game data file can be tens of
				// megabytes, so only its head is read.
				const std::string strXml = ReadWholeFile(strEntry, 4 * 1024);
				if(strXml.find("<GameData") != std::string::npos)
				{
					strFound = strEntryRelative;
				}
			}
		}

		if(!strFound.empty())
		{
			break;
		}
	}
	while(FindNextFileW(hFind, &kFindData));

	FindClose(hFind);
	return strFound;
}

void FindContentPackages(const std::wstring& strDir, const std::string& strRelative, int iDepth, std::vector<CvContentPackage>& vPackages)
{
	if(iDepth > 8)
	{
		return;
	}

	WIN32_FIND_DATAW kFindData;
	HANDLE hFind = FindFirstFileW((strDir + L"\\*").c_str(), &kFindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if(wcscmp(kFindData.cFileName, L".") == 0 || wcscmp(kFindData.cFileName, L"..") == 0)
		{
			continue;
		}

		const std::wstring strEntry = strDir + L"\\" + kFindData.cFileName;
		const std::string strEntryRelative = strRelative.empty() ? WideToUtf8(kFindData.cFileName) : strRelative + "\\" + WideToUtf8(kFindData.cFileName);

		if((kFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			FindContentPackages(strEntry, strEntryRelative, iDepth + 1, vPackages);
		}
		else
		{
			const size_t cchName = wcslen(kFindData.cFileName);
			if(cchName <= 8 || _wcsicmp(kFindData.cFileName + cchName - 8, L".Civ5Pkg") != 0)
			{
				continue;
			}

			const std::string strXml = ReadWholeFile(strEntry, 64 * 1024);
			if(strXml.empty())
			{
				continue;
			}

			std::string strID = TrimBraces(GetElementText(strXml, "GUID"));
			if(strID.empty())
			{
				strID = TrimBraces(GetElementText(strXml, "PackageID"));
			}
			if(strID.empty())
			{
				strID = TrimBraces(GetElementText(strXml, "ID"));
			}

			// Firaxis packages are the reference data: they are what the game is supposed to read.
			// A package without a readable ID is not skipped: it is worth naming too.
			if(IsFiraxisPackageID(strID))
			{
				continue;
			}

			CvContentPackage kPackage;
			kPackage.strFile = strEntryRelative;

			// The name sits in a <Name><Value language="..."> block.
			const std::string strNameBlock = GetElementText(strXml, "Name");
			kPackage.strName = TrimWhitespace(GetElementText(strNameBlock, "Value"));
			if(kPackage.strName.empty())
			{
				kPackage.strName = TrimWhitespace(strNameBlock);
			}

			kPackage.strGameplayFile = WideToUtf8(FindDatabaseXml(strDir, std::wstring(), 0).c_str());

			vPackages.push_back(kPackage);
		}
	}
	while(FindNextFileW(hFind, &kFindData));

	FindClose(hFind);
}

// Every package below the game's Assets folder that Firaxis did not ship, scanned once per DLL load.
const std::vector<CvContentPackage>& GetContentPackages()
{
	static std::vector<CvContentPackage> s_vPackages;
	static bool s_bScanned = false;

	if(!s_bScanned)
	{
		wchar_t wszExePath[MAX_PATH];
		if(GetModuleFileNameW(NULL, wszExePath, _countof(wszExePath)) > 0)
		{
			std::wstring strAssets = wszExePath;
			const std::wstring::size_type uiSlash = strAssets.find_last_of(L'\\');
			if(uiSlash != std::wstring::npos)
			{
				strAssets = strAssets.substr(0, uiSlash) + L"\\Assets";
				FindContentPackages(strAssets, std::string(), 0, s_vPackages);
			}
		}

		// Set last: until the list is complete, another caller must not read it.
		s_bScanned = true;
	}

	return s_vPackages;
}

// One line per package that can stand in for the game's database, for the failure log.
std::string DescribeSuspiciousPackages()
{
	std::string strSummary;

	const std::vector<CvContentPackage>& vPackages = GetContentPackages();
	for(std::vector<CvContentPackage>::const_iterator it = vPackages.begin(); it != vPackages.end(); ++it)
	{
		strSummary += "\n  ";
		strSummary += it->strFile;
		if(!it->strName.empty())
		{
			strSummary += " (";
			strSummary += it->strName;
			strSummary += ")";
		}
		if(!it->strGameplayFile.empty())
		{
			strSummary += " replaces game database files with ";
			strSummary += it->strGameplayFile;
		}
	}
	return strSummary;
}
}
#endif
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
		// Read the reason before discarding the result: its destructor goes through the database
		// layer, which may reset the connection's last error.
		const std::string strError = GetErrorMessage();

		delete pResults;

		// A failed prepare here means the gameplay database is missing tables or columns (corrupted
		// install, broken mod, or a package below Assets that sets up an older version of the tables).
		// Most callers dereference the NULL we return unchecked, so record why it failed before that
		// turns into an unexplained NULL dereference somewhere else.
		//
		// A broken database fails hundreds of queries in a row, so the package list is spelled out
		// once and merely referred to afterwards.
		static bool s_bPackagesLogged = false;

		if(s_bPackagesLogged)
		{
			CUSTOMLOG("Failed to prepare database query '%s': %s (see the first failure for the package list)\nQuery: %s",
				strKey.c_str(), strError.c_str(), szStmt);
		}
		else
		{
			s_bPackagesLogged = true;
#if defined(_WIN32)
			const std::string strPackages = DescribeSuspiciousPackages();
			CUSTOMLOG("Failed to prepare database query '%s': %s%s\nQuery: %s",
				strKey.c_str(), strError.c_str(),
				strPackages.empty() ? " (no package by another author below Assets)" : strPackages.c_str(),
				szStmt);
#else
			CUSTOMLOG("Failed to prepare database query '%s': %s\nQuery: %s", strKey.c_str(), strError.c_str(), szStmt);
#endif
		}
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