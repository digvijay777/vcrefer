#pragma once

namespace ComReg
{
	// 用户是否可以修改浏览器首页
	BOOL	IsIEStartPageEnabled();
	BOOL	SetIEStartPageEnabled(BOOL bEnabled = TRUE);

	// 禁止组策略
	BOOL	IsRestrictGpedit();
	BOOL	SetRestrictGpedit(BOOL bRun = TRUE);

	// 禁止注册表
	BOOL	IsRegistryToolEnabled();
	BOOL	SetRegistryToolEnabled(BOOL bEnabled);
}