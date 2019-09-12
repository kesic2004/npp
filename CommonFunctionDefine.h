/********************************
* 移动自：Common.h            *
* 由不明真相的吃瓜群众移动至此 *
********************************/
#ifndef __REAL_KESIC_LEE_COMMON_FUNCTION_DEFINE_H__
#define __REAL_KESIC_LEE_COMMON_FUNCTION_DEFINE_H__ 1

#define generic_strtol   wcstol
#define generic_strncpy  wcsncpy
#define generic_stricmp  wcsicmp
#define generic_strncmp  wcsncmp
#define generic_strnicmp wcsnicmp
#define generic_strncat  wcsncat
#define generic_strchr   wcschr
#define generic_atoi     _wtoi
#define generic_itoa     _itow
#define generic_atof     _wtof
#define generic_strtok   wcstok
#define generic_strftime wcsftime
#define generic_fprintf  fwprintf
#define generic_sprintf  swprintf
#define generic_sscanf   swscanf
#define generic_fopen    _wfopen
#define generic_fgets    fgetws

#endif
