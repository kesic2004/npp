// Real_Kesic_Lee_Dynamic_Link_Library_Load_And_Proc_Address_Type.h
#ifndef __REAL_KESIC_LEE_DYNAMIC_LINK_LIBRARY_LOAD_AND_PROC_ADDRESS_TYPE_H__
#define __REAL_KESIC_LEE_DYNAMIC_LINK_LIBRARY_LOAD_AND_PROC_ADDRESS_TYPE_H__ 1

#include "Real_Kesic_Lee_Dynamic_Link_Library_Load_And_Proc_Address_Define.h"

/***********************************************************************************************************************************************************************************************************/

extern CRYPT_MSG_CLOSE                       RealKesicLeeCryptMsgClose;
extern CRYPT_MSG_GET_PARAM                   RealKesicLeeCryptMsgGetParam;
extern CERT_CLOSE_STORE                      RealKesicLeeCertCloseStore;
extern CERT_FIND_CERTIFICATE_IN_STORE        RealKesicLeeCertFindCertificateInStore;
extern CERT_FREE_CERTIFICATE_CONTEXT         RealKesicLeeCertFreeCertificateContext;
extern CERT_GET_NAME_STRING                  RealKesicLeeCertGetNameString;
extern CRYPT_QUERY_OBJECT                    RealKesicLeeCryptQueryObject;
extern CERT_GET_CERTIFICATE_CONTEXT_PROPERTY RealKesicLeeCertGetCertificateContextProperty;
extern CERT_NAME_TO_STR                      RealKesicLeeCertNameToStr;

extern GET_FILE_VERSION_INFO_SIZE            RealKesicLeeGetFileVersionInfoSize;
extern GET_FILE_VERSION_INFO                 RealKesicLeeGetFileVersionInfo;
extern VER_QUERY_VALUE                       RealKesicLeeVerQueryValue;

extern IS_DESTINATION_REACHABLE              RealKesicLeeIsDestinationReachable;
extern IS_NETWORK_ALIVE                      RealKesicLeeIsNetworkAlive;

extern IMAGE_NT_HEADER                       RealKesicLeeImageNtHeader;

extern WIN_VERIFY_TRUST                      RealKesicLeeWinVerifyTrust;

/***********************************************************************************************************************************************************************************************************/

extern IMAGE_LIST_DRAW              RealKesicLeeImageListDraw;
extern IMAGE_LIST_GET_IMAGE_INFO    RealKesicLeeImageListGetImageInfo;
extern TRACK_MOUSE_EVENT            RealKesicLeeTrackMouseEvent;
extern IMAGE_LIST_CREATE            RealKesicLeeImageListCreate;
extern IMAGE_LIST_DESTROY           RealKesicLeeImageListDestroy;
extern IMAGE_LIST_REPLACE_ICON      RealKesicLeeImageListReplaceIcon;
extern IMAGE_LIST_GET_IMAGE_COUNT   RealKesicLeeImageListGetImageCount;
extern IMAGE_LIST_ADD_MASKED        RealKesicLeeImageListAddMasked;
extern INIT_COMMON_CONTROLS_EX      RealKesicLeeInitCommonControlsEx;
extern INIT_COMMON_CONTROLS         RealKesicLeeInitCommonControls;
extern IMAGE_LIST_SET_ICON_SIZE     RealKesicLeeImageListSetIconSize;
extern IMAGE_LIST_BEGIN_DRAG        RealKesicLeeImageListBeginDrag;
extern IMAGE_LIST_END_DRAG          RealKesicLeeImageListEndDrag;
extern IMAGE_LIST_DRAG_ENTER        RealKesicLeeImageListDragEnter;
extern IMAGE_LIST_DRAG_MOVE         RealKesicLeeImageListDragMove;
extern IMAGE_LIST_DRAG_SHOW_NO_LOCK RealKesicLeeImageListDragShowNolock;

extern IMAGE_LIST_DESTROY           RealKesicLeeImageImageListDestroy;

/***********************************************************************************************************************************************************************************************************/

#endif

