// Real_Kesic_Lee_Dynamic_Link_Library_Load_And_Proc_Address_Define.h
#ifndef __REAL_KESIC_LEE_DYNAMIC_LINK_LIBRARY_LOAD_AND_PROC_ADDRESS_DEFINE_H__
#define __REAL_KESIC_LEE_DYNAMIC_LINK_LIBRARY_LOAD_AND_PROC_ADDRESS_DEFINE_H__

#include <wincrypt.h>
#include <winver.h>
#include <DbgHelp.h>
#include <WinTrust.h>
#include <SensAPI.h>
#include <stdexcept>
#include <CommCtrl.h>

/***********************************************************************************************************************************************************************************************************/
typedef /*WINCRYPT32API*/ BOOL(/*WINAPI*/           *CRYPT_MSG_CLOSE)                       (HCRYPTMSG);
typedef /*WINCRYPT32API*/ BOOL(/*WINAPI*/           *CRYPT_MSG_GET_PARAM)                   (HCRYPTMSG, DWORD, DWORD, void *, DWORD *);
typedef /*WINCRYPT32API*/ BOOL(/*WINAPI*/           *CERT_CLOSE_STORE)                      (HCERTSTORE, DWORD);
typedef /*WINCRYPT32API*/ PCCERT_CONTEXT(/*WINAPI*/ *CERT_FIND_CERTIFICATE_IN_STORE)        (HCERTSTORE, DWORD, DWORD, DWORD, const void *, PCCERT_CONTEXT);
typedef /*WINCRYPT32API*/ BOOL(/*WINAPI*/           *CERT_FREE_CERTIFICATE_CONTEXT)         (PCCERT_CONTEXT);

#ifdef UNICODE // !
typedef /*WINCRYPT32API*/ DWORD(/*WINAPI*/          *CERT_GET_NAME_STRING)                  (PCCERT_CONTEXT, DWORD, DWORD, void *, LPWSTR, DWORD);
#else
typedef /*WINCRYPT32API*/ DWORD(/*WINAPI*/          *CERT_GET_NAME_STRING)                  (PCCERT_CONTEXT, DWORD, DWORD, void *, LPSTR, DWORD);
#endif // !UNICODE

typedef /*WINCRYPT32API*/ BOOL(/*WINAPI*/           *CRYPT_QUERY_OBJECT)                    (DWORD, const void *, DWORD, DWORD, DWORD, DWORD *, DWORD *, DWORD *, HCERTSTORE *, HCRYPTMSG *, const void **);

#ifdef UNICODE // !
typedef                   DWORD(/*APIENTRY*/        *GET_FILE_VERSION_INFO_SIZE)            (LPCWSTR, LPDWORD);
#else
typedef                   DWORD(/*APIENTRY*/        *GET_FILE_VERSION_INFO_SIZE)            (LPCSTR, LPDWORD);
#endif // !UNICODE

#ifdef UNICODE // !
typedef                   BOOL(/*APIENTRY*/         *GET_FILE_VERSION_INFO)                 (LPCWSTR, DWORD, DWORD, LPVOID);
#else
typedef                   BOOL(/*APIENTRY*/         *GET_FILE_VERSION_INFO)                 (LPCSTR, DWORD, DWORD, LPVOID);
#endif // !UNICODE

#ifdef UNICODE // !
typedef                   BOOL(/*APIENTRY*/         *VER_QUERY_VALUE)                       (LPCVOID, LPCWSTR, LPVOID *, PUINT);
#else
typedef                   BOOL(/*APIENTRY*/         *VER_QUERY_VALUE)                       (LPCVOID, LPCSTR, LPVOID *, PUINT);
#endif // !UNICODE

typedef                   PIMAGE_NT_HEADERS(*IMAGE_NT_HEADER)                       (PVOID);
typedef /*WINCRYPT32API*/ BOOL(/*WINAPI*/           *CERT_GET_CERTIFICATE_CONTEXT_PROPERTY) (PCCERT_CONTEXT, DWORD, void *, DWORD *);

#ifdef UNICODE // !
typedef /*WINCRYPT32API*/ DWORD(/*WINAPI*/          *CERT_NAME_TO_STR)                      (DWORD, PCERT_NAME_BLOB, DWORD, LPWSTR, DWORD);
#else
typedef /*WINCRYPT32API*/ DWORD(/*WINAPI*/          *CERT_NAME_TO_STR)                      (DWORD, PCERT_NAME_BLOB, DWORD, LPSTR, DWORD);
#endif // !UNICODE

typedef /*extern*/        LONG(/*WINAPI*/           *WIN_VERIFY_TRUST)                      (HWND, GUID *, LPVOID);

#ifdef UNICODE // !
typedef                   BOOL(/*APIENTRY*/         *IS_DESTINATION_REACHABLE)              (LPCWSTR, LPQOCINFO);
#else
typedef                   BOOL(/*APIENTRY*/         *IS_DESTINATION_REACHABLE)              (LPCSTR, LPQOCINFO);
#endif // !UNICODE

typedef                   BOOL(/*APIENTRY*/         *IS_NETWORK_ALIVE)                      (LPDWORD);

/***********************************************************************************************************************************************************************************************************/

typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * IMAGE_LIST_DRAW)             (HIMAGELIST, int, HDC, int, int, UINT);
typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * IMAGE_LIST_GET_IMAGE_INFO)   (HIMAGELIST, int, IMAGEINFO *);
typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * TRACK_MOUSE_EVENT)           (LPTRACKMOUSEEVENT);
typedef /*WINCOMMCTRLAPI*/ HIMAGELIST(/*WINAPI*/ * IMAGE_LIST_CREATE)           (int, int, UINT, int, int);
typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * IMAGE_LIST_DESTROY)          (HIMAGELIST);
typedef /*WINCOMMCTRLAPI*/ int(/*WINAPI*/        * IMAGE_LIST_REPLACE_ICON)     (HIMAGELIST, int, HICON);
typedef /*WINCOMMCTRLAPI*/ int(/*WINAPI*/        * IMAGE_LIST_GET_IMAGE_COUNT)  (HIMAGELIST);
typedef /*WINCOMMCTRLAPI*/ int(/*WINAPI*/        * IMAGE_LIST_ADD_MASKED)       (HIMAGELIST, HBITMAP, COLORREF);
typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * INIT_COMMON_CONTROLS_EX)     (const INITCOMMONCONTROLSEX *);
typedef /*WINCOMMCTRLAPI*/ void(/*WINAPI*/       * INIT_COMMON_CONTROLS)        (void);
typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * IMAGE_LIST_SET_ICON_SIZE)    (HIMAGELIST, int, int);
typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * IMAGE_LIST_BEGIN_DRAG)       (HIMAGELIST, int, int, int);
typedef /*WINCOMMCTRLAPI*/ void(/*WINAPI*/       * IMAGE_LIST_END_DRAG)         (void);
typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * IMAGE_LIST_DRAG_ENTER)       (HWND, int, int);
typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * IMAGE_LIST_DRAG_MOVE)        (int, int);
typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * IMAGE_LIST_DRAG_SHOW_NO_LOCK)(BOOL);

typedef /*WINCOMMCTRLAPI*/ BOOL(/*WINAPI*/       * IMAGE_LIST_DESTROY) (HIMAGELIST);

/***********************************************************************************************************************************************************************************************************/

#endif // !__REAL_KESIC_LEE_DYNAMIC_LINK_LIBRARY_LOAD_AND_PROC_ADDRESS_DEFINE_H__