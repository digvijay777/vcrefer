#pragma once

namespace ComReg
{
	// �û��Ƿ�����޸��������ҳ
	BOOL	IsIEStartPageEnabled();
	BOOL	SetIEStartPageEnabled(BOOL bEnabled = TRUE);

	// ��ֹ�����
	BOOL	IsRestrictGpedit();
	BOOL	SetRestrictGpedit(BOOL bRun = TRUE);

	// ��ֹע���
	BOOL	IsRegistryToolEnabled();
	BOOL	SetRegistryToolEnabled(BOOL bEnabled);
}