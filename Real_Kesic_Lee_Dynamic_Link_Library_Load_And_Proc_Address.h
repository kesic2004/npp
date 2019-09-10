// Real_Kesic_Lee_Dynamic_Link_Library_Load_And_Proc_Address.h
#ifndef __REAL_KESIC_LEE_DYNAMIC_LINK_LIBRARY_LOAD_AND_PROC_ADDRESS_H__
#define __REAL_KESIC_LEE_DYNAMIC_LINK_LIBRARY_LOAD_AND_PROC_ADDRESS_H__ 1

#include "Real_Kesic_Lee_Dynamic_Link_Library_Load_And_Proc_Address_Define.h"

/***********************************************************************************************************************************************************************************************************/

CRYPT_MSG_CLOSE                       RealKesicLeeCryptMsgClose                     = nullptr;
CRYPT_MSG_GET_PARAM                   RealKesicLeeCryptMsgGetParam                  = nullptr;
CERT_CLOSE_STORE                      RealKesicLeeCertCloseStore                    = nullptr;
CERT_FIND_CERTIFICATE_IN_STORE        RealKesicLeeCertFindCertificateInStore        = nullptr;
CERT_FREE_CERTIFICATE_CONTEXT         RealKesicLeeCertFreeCertificateContext        = nullptr;
CERT_GET_NAME_STRING                  RealKesicLeeCertGetNameString                 = nullptr;
CRYPT_QUERY_OBJECT                    RealKesicLeeCryptQueryObject                  = nullptr;
CERT_GET_CERTIFICATE_CONTEXT_PROPERTY RealKesicLeeCertGetCertificateContextProperty = nullptr;
CERT_NAME_TO_STR                      RealKesicLeeCertNameToStr                     = nullptr;

GET_FILE_VERSION_INFO_SIZE            RealKesicLeeGetFileVersionInfoSize = nullptr;
GET_FILE_VERSION_INFO                 RealKesicLeeGetFileVersionInfo     = nullptr;
VER_QUERY_VALUE                       RealKesicLeeVerQueryValue          = nullptr;

IS_DESTINATION_REACHABLE              RealKesicLeeIsDestinationReachable = nullptr;
IS_NETWORK_ALIVE                      RealKesicLeeIsNetworkAlive         = nullptr;

IMAGE_NT_HEADER                       RealKesicLeeImageNtHeader = nullptr;

WIN_VERIFY_TRUST                      RealKesicLeeWinVerifyTrust = nullptr;

/***********************************************************************************************************************************************************************************************************/

IMAGE_LIST_DRAW              RealKesicLeeImageListDraw           = nullptr;
IMAGE_LIST_GET_IMAGE_INFO    RealKesicLeeImageListGetImageInfo   = nullptr;
TRACK_MOUSE_EVENT            RealKesicLeeTrackMouseEvent         = nullptr;
IMAGE_LIST_CREATE            RealKesicLeeImageListCreate         = nullptr;
IMAGE_LIST_DESTROY           RealKesicLeeImageListDestroy        = nullptr;
IMAGE_LIST_REPLACE_ICON      RealKesicLeeImageListReplaceIcon    = nullptr;
IMAGE_LIST_GET_IMAGE_COUNT   RealKesicLeeImageListGetImageCount  = nullptr;
IMAGE_LIST_ADD_MASKED        RealKesicLeeImageListAddMasked      = nullptr;
INIT_COMMON_CONTROLS_EX      RealKesicLeeInitCommonControlsEx    = nullptr;
INIT_COMMON_CONTROLS         RealKesicLeeInitCommonControls      = nullptr;
IMAGE_LIST_SET_ICON_SIZE     RealKesicLeeImageListSetIconSize    = nullptr;
IMAGE_LIST_BEGIN_DRAG        RealKesicLeeImageListBeginDrag      = nullptr;
IMAGE_LIST_END_DRAG          RealKesicLeeImageListEndDrag        = nullptr;
IMAGE_LIST_DRAG_ENTER        RealKesicLeeImageListDragEnter      = nullptr;
IMAGE_LIST_DRAG_MOVE         RealKesicLeeImageListDragMove       = nullptr;
IMAGE_LIST_DRAG_SHOW_NO_LOCK RealKesicLeeImageListDragShowNolock = nullptr;

IMAGE_LIST_DESTROY           RealKesicLeeImageImageListDestroy   = nullptr;

/***********************************************************************************************************************************************************************************************************/

#define Real_Kesic_Lee_Dynamic_Link_Library_Load_A "Crypt32.dll"
#define Real_Kesic_Lee_Dynamic_Link_Library_Load_B "Api-ms-win-core-version-l1-1-0.dll"
#define Real_Kesic_Lee_Dynamic_Link_Library_Load_C "Sensapi.dll"
#define Real_Kesic_Lee_Dynamic_Link_Library_Load_D "Dbghelp.dll"
#define Real_Kesic_Lee_Dynamic_Link_Library_Load_E "Wintrust.dll"
#define Real_Kesic_Lee_Dynamic_Link_Library_Load_F "Comctl32.dll"

class RealKesicLeeDynamicLinkLibraryLoad;
const RealKesicLeeDynamicLinkLibraryLoad & getRealKesicLeeDynamicLinkLibraryLoad();
void initialRealKesicLeeDynamicLinkLibraryLoad(const RealKesicLeeDynamicLinkLibraryLoad & obj);
class RealKesicLeeDynamicLinkLibraryLoad
{
private:
	HMODULE crypt32                 = nullptr;	// Crypt32.dll                          wincrypt.h
	HMODULE apiMsWinCoreVersionL110 = nullptr;	// Api-ms-win-core-version-l1-1-0.dll   winver.h
	HMODULE sensapi                 = nullptr;	// Sensapi.dll                          SensAPI.h
	HMODULE dbghelp                 = nullptr;	// Dbghelp.dll                          DbgHelp.h
	HMODULE wintrust                = nullptr;	// Wintrust.dll                         WinTrust.h
	HMODULE comctl32                = nullptr;	// Comctl32.dll                         CommCtrl.h
private:
	RealKesicLeeDynamicLinkLibraryLoad()
	{
		crypt32                 = LoadLibrary(TEXT(Real_Kesic_Lee_Dynamic_Link_Library_Load_A));
		if (crypt32 == nullptr)
		{
			throw std::runtime_error(Real_Kesic_Lee_Dynamic_Link_Library_Load_A);
		}

		apiMsWinCoreVersionL110 = LoadLibrary(TEXT(Real_Kesic_Lee_Dynamic_Link_Library_Load_B));
		if (apiMsWinCoreVersionL110 == nullptr)
		{
			FreeLibrary(crypt32);
			throw std::runtime_error(Real_Kesic_Lee_Dynamic_Link_Library_Load_B);
		}

		sensapi                 = LoadLibrary(TEXT(Real_Kesic_Lee_Dynamic_Link_Library_Load_C));
		if (sensapi == nullptr)
		{
			FreeLibrary(crypt32);
			FreeLibrary(apiMsWinCoreVersionL110);
			throw std::runtime_error(Real_Kesic_Lee_Dynamic_Link_Library_Load_C);
		}

		dbghelp                 = LoadLibrary(TEXT(Real_Kesic_Lee_Dynamic_Link_Library_Load_D));
		if (dbghelp == nullptr)
		{
			FreeLibrary(crypt32);
			FreeLibrary(apiMsWinCoreVersionL110);
			FreeLibrary(sensapi);
			throw std::runtime_error(Real_Kesic_Lee_Dynamic_Link_Library_Load_D);
		}

		wintrust                = LoadLibrary(TEXT(Real_Kesic_Lee_Dynamic_Link_Library_Load_E));
		if (wintrust == nullptr)
		{
			FreeLibrary(crypt32);
			FreeLibrary(apiMsWinCoreVersionL110);
			FreeLibrary(sensapi);
			FreeLibrary(dbghelp);
			throw std::runtime_error(Real_Kesic_Lee_Dynamic_Link_Library_Load_E);
		}
		comctl32                = LoadLibrary(TEXT(Real_Kesic_Lee_Dynamic_Link_Library_Load_F));
		if (comctl32 == nullptr)
		{
			FreeLibrary(crypt32);
			FreeLibrary(apiMsWinCoreVersionL110);
			FreeLibrary(sensapi);
			FreeLibrary(dbghelp);
			FreeLibrary(wintrust);
			throw std::runtime_error(Real_Kesic_Lee_Dynamic_Link_Library_Load_F);
		}
	}
public:
	~RealKesicLeeDynamicLinkLibraryLoad()
	{
		FreeLibrary(crypt32);
		FreeLibrary(apiMsWinCoreVersionL110);
		FreeLibrary(sensapi);
		FreeLibrary(dbghelp);
		FreeLibrary(wintrust);
		FreeLibrary(comctl32);
	}
	friend const RealKesicLeeDynamicLinkLibraryLoad & getRealKesicLeeDynamicLinkLibraryLoad();
	friend void initialRealKesicLeeDynamicLinkLibraryLoad(const RealKesicLeeDynamicLinkLibraryLoad & obj);

};
const RealKesicLeeDynamicLinkLibraryLoad & getRealKesicLeeDynamicLinkLibraryLoad()
{
	static const RealKesicLeeDynamicLinkLibraryLoad o;
	return o;
}
void initialRealKesicLeeDynamicLinkLibraryLoad(const RealKesicLeeDynamicLinkLibraryLoad & obj)
{
	/***********************************************************************************************************************************************************************************************************/

	RealKesicLeeCryptMsgClose = (CRYPT_MSG_CLOSE)GetProcAddress(obj.crypt32, "CryptMsgClose");
	RealKesicLeeCryptMsgGetParam = (CRYPT_MSG_GET_PARAM)GetProcAddress(obj.crypt32, "CryptMsgGetParam");
	RealKesicLeeCertCloseStore = (CERT_CLOSE_STORE)GetProcAddress(obj.crypt32, "CertCloseStore");
	RealKesicLeeCertFindCertificateInStore = (CERT_FIND_CERTIFICATE_IN_STORE)GetProcAddress(obj.crypt32, "CertFindCertificateInStore");
	RealKesicLeeCertFreeCertificateContext = (CERT_FREE_CERTIFICATE_CONTEXT)GetProcAddress(obj.crypt32, "CertFreeCertificateContext");
#ifdef UNICODE
#else
#endif // !UNICODE

#ifdef UNICODE
	RealKesicLeeCertGetNameString = (CERT_GET_NAME_STRING)GetProcAddress(obj.crypt32, "CertGetNameStringW");
#else
	RealKesicLeeCertGetNameString = (CERT_GET_NAME_STRING)GetProcAddress(obj.crypt32, "CertGetNameStringA");
#endif // !UNICODE

	RealKesicLeeCryptQueryObject = (CRYPT_QUERY_OBJECT)GetProcAddress(obj.crypt32, "CryptQueryObject");
	RealKesicLeeCertGetCertificateContextProperty = (CERT_GET_CERTIFICATE_CONTEXT_PROPERTY)GetProcAddress(obj.crypt32, "CertGetCertificateContextProperty");
#ifdef UNICODE
	RealKesicLeeCertNameToStr = (CERT_NAME_TO_STR)GetProcAddress(obj.crypt32, "CertNameToStrW");
#else
	RealKesicLeeCertNameToStr = (CERT_NAME_TO_STR)GetProcAddress(obj.crypt32, "CertNameToStrA");
#endif // !UNICODE

#ifdef UNICODE
	RealKesicLeeGetFileVersionInfoSize = (GET_FILE_VERSION_INFO_SIZE)GetProcAddress(obj.apiMsWinCoreVersionL110, "GetFileVersionInfoSizeW");
#else
	RealKesicLeeGetFileVersionInfoSize = (GET_FILE_VERSION_INFO_SIZE)GetProcAddress(obj.apiMsWinCoreVersionL110, "GetFileVersionInfoSizeA");
#endif // !UNICODE

#ifdef UNICODE
	RealKesicLeeGetFileVersionInfo = (GET_FILE_VERSION_INFO)GetProcAddress(obj.apiMsWinCoreVersionL110, "GetFileVersionInfoW");
#else
	RealKesicLeeGetFileVersionInfo = (GET_FILE_VERSION_INFO)GetProcAddress(obj.apiMsWinCoreVersionL110, "GetFileVersionInfoA");
#endif // !UNICODE

#ifdef UNICODE
	RealKesicLeeVerQueryValue = (VER_QUERY_VALUE)GetProcAddress(obj.apiMsWinCoreVersionL110, "VerQueryValueW");
#else
	RealKesicLeeVerQueryValue = (VER_QUERY_VALUE)GetProcAddress(obj.apiMsWinCoreVersionL110, "VerQueryValueA");
#endif // !UNICODE

#ifdef UNICODE
	RealKesicLeeIsDestinationReachable = (IS_DESTINATION_REACHABLE)GetProcAddress(obj.sensapi, "IsDestinationReachableW");
#else
	RealKesicLeeIsDestinationReachable = (IS_DESTINATION_REACHABLE)GetProcAddress(obj.sensapi, "IsDestinationReachableA");
#endif // !UNICODE

	RealKesicLeeIsNetworkAlive = (IS_NETWORK_ALIVE)GetProcAddress(obj.sensapi, "IsNetworkAlive");

	RealKesicLeeImageNtHeader = (IMAGE_NT_HEADER)GetProcAddress(obj.dbghelp, "ImageNtHeader");

	RealKesicLeeWinVerifyTrust = (WIN_VERIFY_TRUST)GetProcAddress(obj.wintrust, "WinVerifyTrust");

	if (RealKesicLeeCryptMsgClose == nullptr)
	{
		throw std::runtime_error("CryptMsgClose");
	}

	if (RealKesicLeeCryptMsgGetParam == nullptr)
	{
		throw std::runtime_error("CryptMsgGetParam");
	}

	if (RealKesicLeeCertCloseStore == nullptr)
	{
		throw std::runtime_error("CertCloseStore");
	}

	if (RealKesicLeeCertFindCertificateInStore == nullptr)
	{
		throw std::runtime_error("CertFindCertificateInStore");
	}

	if (RealKesicLeeCertFreeCertificateContext == nullptr)
	{
		throw std::runtime_error("CertFreeCertificateContext");
	}
	if (RealKesicLeeCertGetNameString == nullptr)
	{
		throw std::runtime_error("CertGetNameString");
	}

	/***********************************************************************************************************************************************************************************************************/

	RealKesicLeeImageListDraw           = (IMAGE_LIST_DRAW)GetProcAddress(obj.comctl32, "ImageList_Draw");
	RealKesicLeeImageListGetImageInfo   = (IMAGE_LIST_GET_IMAGE_INFO)GetProcAddress(obj.comctl32, "ImageList_GetImageInfo");
	RealKesicLeeTrackMouseEvent         = (TRACK_MOUSE_EVENT)GetProcAddress(obj.comctl32, "_TrackMouseEvent");
	RealKesicLeeImageListCreate         = (IMAGE_LIST_CREATE)GetProcAddress(obj.comctl32, "ImageList_Create");
	RealKesicLeeImageListDestroy        = (IMAGE_LIST_DESTROY)GetProcAddress(obj.comctl32, "ImageList_Destroy");
	RealKesicLeeImageListReplaceIcon    = (IMAGE_LIST_REPLACE_ICON)GetProcAddress(obj.comctl32, "ImageList_ReplaceIcon");
	RealKesicLeeImageListGetImageCount  = (IMAGE_LIST_GET_IMAGE_COUNT)GetProcAddress(obj.comctl32, "ImageList_GetImageCount");
	RealKesicLeeImageListAddMasked      = (IMAGE_LIST_ADD_MASKED)GetProcAddress(obj.comctl32, "ImageList_AddMasked");
	RealKesicLeeInitCommonControlsEx    = (INIT_COMMON_CONTROLS_EX)GetProcAddress(obj.comctl32, "InitCommonControlsEx");
	RealKesicLeeInitCommonControls      = (INIT_COMMON_CONTROLS)GetProcAddress(obj.comctl32, "InitCommonControls");
	RealKesicLeeImageListSetIconSize    = (IMAGE_LIST_SET_ICON_SIZE)GetProcAddress(obj.comctl32, "ImageList_SetIconSize");
	RealKesicLeeImageListBeginDrag      = (IMAGE_LIST_BEGIN_DRAG)GetProcAddress(obj.comctl32, "ImageList_BeginDrag");
	RealKesicLeeImageListEndDrag        = (IMAGE_LIST_END_DRAG)GetProcAddress(obj.comctl32, "ImageList_EndDrag");
	RealKesicLeeImageListDragEnter      = (IMAGE_LIST_DRAG_ENTER)GetProcAddress(obj.comctl32, "ImageList_DragEnter");
	RealKesicLeeImageListDragMove       = (IMAGE_LIST_DRAG_MOVE)GetProcAddress(obj.comctl32, "ImageList_DragMove");
	RealKesicLeeImageListDragShowNolock = (IMAGE_LIST_DRAG_SHOW_NO_LOCK)GetProcAddress(obj.comctl32, "ImageList_DragShowNolock");

	RealKesicLeeImageImageListDestroy   = (IMAGE_LIST_DESTROY)GetProcAddress(obj.comctl32, "ImageList_Destroy");

	/***********************************************************************************************************************************************************************************************************/
}
#endif

