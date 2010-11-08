#include "stdafx.h"
#include <atlrx.h>	// ±ØÐë¿â

void TestATLRegExp()
{
	CAtlRegExp<> reUrl;

	REParseError status = reUrl.Parse(_T("^{[0-9]?[0-9]}:{[0-9][0-9]}$"));
	if (REPARSE_ERROR_OK != status)
	{
		// Unexpected error.
		return ;
	}

	CAtlREMatchContext<> mcUrl;
	if (!reUrl.Match(_T("23:56"),
		&mcUrl))
	{
		// Unexpected error.
		return ;
	}

	for (UINT nGroupIndex = 0; nGroupIndex < mcUrl.m_uNumGroups;
		++nGroupIndex)
	{
		const CAtlREMatchContext<>::RECHAR* szStart = 0;
		const CAtlREMatchContext<>::RECHAR* szEnd = 0;
		mcUrl.GetMatch(nGroupIndex, &szStart, &szEnd);

		ptrdiff_t nLength = szEnd - szStart;
		_tprintf(_T("%d: \"%.*s\"\n"), nGroupIndex, nLength, szStart);
	}
}