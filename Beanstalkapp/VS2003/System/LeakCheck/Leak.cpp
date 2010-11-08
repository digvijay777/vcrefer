#include "stdafx.h"

#import "c:\windows\system32\wuapi.dll" no_namespace named_guids raw_interfaces_only

BOOL GetDownLoadContation(IUpdateDownloadContentCollection* pDownloadContentCollection);

//////////////////////////////////////////////////////////////////////////
BOOL CheckLick()
{
		CoInitialize(NULL);	
		IUpdateSession* pUpdateSession = NULL;	
		IUpdateServiceManager* pUpdateServiceManager = NULL;	
		IUpdateService* pUpdateService = NULL;	
		IUpdateSearcher* pUpdateSearcher = NULL;	
// 	//	m_errorScan = failure;	
		do	{
			// create update session object.		
// 			Log(llTrace, (CPString)L"Windows Update Scan : Create update session object");		
			HRESULT hr = CoCreateInstance ( CLSID_UpdateSession, NULL, CLSCTX_INPROC_SERVER, IID_IUpdateSession, 
				(void**) &pUpdateSession );		
			if ( FAILED(hr) || pUpdateSession == NULL )		
			{			
				break;		
			}		// create update service manager object.		
// 			Log(llTrace, (CPString)L"Windows Update Scan : Create update service manager object");		
// 			hr = CoCreateInstance ( CLSID_UpdateServiceManager, NULL, CLSCTX_INPROC_SERVER, IID_IUpdateServiceManager, 
// 				(void**) &pUpdateServiceManager );		
// 			if ( FAILED(hr) || pUpdateServiceManager == NULL )	
// 			{		
// 				break;	
// 			}		// create update searcher object.		
// 			Log(llTrace, (CPString)L"Windows Update Scan : Create update searcher object");		
			hr = pUpdateSession->CreateUpdateSearcher(&pUpdateSearcher);	
			if ( FAILED(hr) || pUpdateSearcher == NULL )	
			{		
				break;	
			}	
			// select the update server source.		
// 			Log(llTrace, (CPString)L"Windows Update Scan : Select the update server source");	
			hr = pUpdateSearcher->put_ServerSelection(ssWindowsUpdate);	
			hr = pUpdateSearcher->put_CanAutomaticallyUpgradeService(VARIANT_TRUE);	
			hr = pUpdateSearcher->put_IncludePotentiallySupersededUpdates(VARIANT_TRUE);
// 			hr = pUpdateSearcher->put_ServerSelection(ssOthers);
// 			hr = pUpdateSearcher->put_ServiceID(BSTR(L"http://localhost/update/"));
			hr = pUpdateSearcher->put_Online(VARIANT_TRUE);		
			ISearchResult* pSearchResult = NULL;	
// 			Log(llTrace, (CPString)L"Windows Update Scan : Start search");	
			hr = pUpdateSearcher->Search(BSTR(L"IsInstalled=-1"), &pSearchResult);	
			if ( FAILED(hr) || pSearchResult == NULL )		
			{		
				break;
			}		// retrive and write the category collection content.	
// 			ICategoryCollection* pCategoryCollection = NULL;	
// 			hr = pSearchResult->get_RootCategories(&pCategoryCollection);
// 			if ( FAILED(hr) || pCategoryCollection == NULL )	
// 			{			
// 				break;	
// 			}		
// 			else	
// 			{	
// 				long	lCont		= 0;
// 
// 				hr = pCategoryCollection->get_Count(&lCont);
// 				TRACE("%d\n", lCont);
// 				//WriteCategoryCollection(pCategoryCollection, m_aWindowsUpdateCategories);	
// 			}

			OperationResultCode		ResultCode;
			IUpdateCollection*		pUpdateCollection;
			LONG					lUpdateCount;

			hr = pSearchResult->get_ResultCode(&ResultCode);
			if(FAILED(hr) || orcSucceeded != ResultCode)
				break;

			hr = pSearchResult->get_Updates(&pUpdateCollection);
			if(FAILED(hr))
				break;

			hr = pUpdateCollection->get_Count(&lUpdateCount);
			for(long i = 0; i < lUpdateCount; i++)
			{
				IUpdate*				pUpdate;
				hr = pUpdateCollection->get_Item(i, &pUpdate);
				if(FAILED(hr))
					continue;
				
				VARIANT_BOOL		bAutoSelectOnWebSites;
				VARIANT_BOOL		bCanRequireSource;
				VARIANT				vDeadline;
				VARIANT_BOOL		bDeltaCompressedContentAvailable;
				VARIANT_BOOL		bDeltaCompressedContentPreferred;
				BSTR				bstrDescription;
				VARIANT_BOOL		bEulaAccepted;
				BSTR				bstrEulaText;
				BSTR				bstrHandlID;
				VARIANT_BOOL		bBeta;
				VARIANT_BOOL		bIsDownLoaded;
				VARIANT_BOOL		bHidden;
				VARIANT_BOOL		bInstalled;
				VARIANT_BOOL		bMandatory;
				VARIANT_BOOL		bUninstallable;
				DATE				date;
				DECIMAL				MaxDownloadSize;
				DECIMAL				MinDownloadSize;
				BSTR				bstrMsrcSeverity;
				LONG				lRecommendedCpuSpeed;
				LONG				lRecommendedHardDiskSpace;
				LONG				lRecommendedMemory;
				BSTR				bstrReleaseNotes;
				BSTR				bstrSupportUrl;
				BSTR				bstrTitle;

				pUpdate->get_AutoSelectOnWebSites(&bAutoSelectOnWebSites);
				//pUpdate->get_BundledUpdates()
				pUpdate->get_CanRequireSource(&bCanRequireSource);
				//pUpdate->get_Categories()
				pUpdate->get_Deadline(&vDeadline);
				pUpdate->get_DeltaCompressedContentAvailable(&bDeltaCompressedContentAvailable);
				pUpdate->get_DeltaCompressedContentPreferred(&bDeltaCompressedContentPreferred);
				// pUpdate->get_DeploymentAction();
				pUpdate->get_Description(&bstrDescription);
				IUpdateDownloadContentCollection*		pDownloadContentCollection;
				hr = pUpdate->get_DownloadContents(&pDownloadContentCollection);
				if(NULL != pDownloadContentCollection)
				{
					GetDownLoadContation(pDownloadContentCollection);
					pDownloadContentCollection->Release();
				}
				//pUpdate->get_DownloadPriority();
				pUpdate->get_EulaAccepted(&bEulaAccepted);
				pUpdate->get_EulaText(&bstrEulaText);
				pUpdate->get_HandlerID(&bstrHandlID);
				//pUpdate->get_Identity();
				//pUpdate->get_Image();
				//pUpdate->get_InstallationBehavior();
				pUpdate->get_IsBeta(&bBeta);
				pUpdate->get_IsDownloaded(&bIsDownLoaded);
				pUpdate->get_IsHidden(&bHidden);
				pUpdate->get_IsInstalled(&bInstalled);
				pUpdate->get_IsMandatory(&bMandatory);
				pUpdate->get_IsUninstallable(&bUninstallable);
				//pUpdate->get_KBArticleIDs();
				//pUpdate->get_Languages();
				pUpdate->get_LastDeploymentChangeTime(&date);
				pUpdate->get_MaxDownloadSize(&MaxDownloadSize);
				pUpdate->get_MinDownloadSize(&MinDownloadSize);
				//pUpdate->get_MoreInfoUrls();
				pUpdate->get_MsrcSeverity(&bstrMsrcSeverity);
				pUpdate->get_RecommendedCpuSpeed(&lRecommendedCpuSpeed);
				pUpdate->get_RecommendedHardDiskSpace(&lRecommendedHardDiskSpace);
				pUpdate->get_RecommendedMemory(&lRecommendedMemory);
				pUpdate->get_ReleaseNotes(&bstrReleaseNotes);
				//pUpdate->get_SecurityBulletinIDs();
				//pUpdate->get_SupersededUpdateIDs();
				pUpdate->get_SupportUrl(&bstrSupportUrl);
				pUpdate->get_Title(&bstrTitle);
				//pUpdate->get_Type();
				//pUpdate->get_UninstallationBehavior();
				//pUpdate->get_UninstallationSteps();
				pUpdate->Release();
			}
		}while(false);

		return true;
}

//////////////////////////////////////////////////////////////////////////
BOOL GetDownLoadContation(IUpdateDownloadContentCollection* pDownloadContentCollection)
{
	LONG				lSize		= 0;
	HRESULT				hr;

	hr = pDownloadContentCollection->get_Count(&lSize);
	if(FAILED(hr))
		return FALSE;
	for(long i = 0; i < lSize; i++)
	{
		IUpdateDownloadContent*				pDownload;

		hr = pDownloadContentCollection->get_Item(i, &pDownload);
		if(FAILED(hr))
			continue;

		BSTR				bstrUrl;			
		pDownload->get_DownloadUrl(&bstrUrl);
		//pDownload->
		pDownload->Release();
	}
	return TRUE;
}