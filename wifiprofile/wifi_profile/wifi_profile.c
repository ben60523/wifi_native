
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wlanapi.h>
#include <Windot11.h>
#include <objbase.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE hClient = NULL;
PWLAN_INTERFACE_INFO pIfInfo = NULL;
PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
PDOT11_SSID pDotSSid = NULL;


void notificationCallback(PWLAN_NOTIFICATION_DATA pNotifData, PVOID pContext) {
	if (pNotifData != NULL) {
		switch (pNotifData->NotificationSource)
		{
		case WLAN_NOTIFICATION_SOURCE_ACM:
			break;
		default:
			break;
		}
	}
}

int wmain(int argc, LPWSTR argv[]) {

	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	GUID interfaceGuid;
	DWORD dwPrevNotifType = 0;
	PWLAN_BSS_LIST WlanBssList;


	// variables used for WlanEnumInterfaces


	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return 1;
	}

	dwResult = WlanRegisterNotification(hClient, WLAN_NOTIFICATION_SOURCE_ALL, FALSE, (WLAN_NOTIFICATION_CALLBACK)notificationCallback, NULL, NULL, &dwPrevNotifType);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanRegisterNotification failed with error: %u\n", dwResult);
		return 1;
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		return 1;
	}
	else {
		wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
		wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
		for (int ifaceNum = 0; ifaceNum < (int)pIfList->dwNumberOfItems; ifaceNum++) {
			pIfInfo = (WLAN_INTERFACE_INFO*)&pIfList->InterfaceInfo[ifaceNum];
			/*WCHAR GuidString[40] = { 0 };
			StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR)&GuidString, 39);*/
			if (strcmp(&pIfInfo->strInterfaceDescription, argv[2])){
				wprintf(L"Ready for setting profile");
				//WlanSetProfile(hClient, &pIfInfo->InterfaceGuid, 0, )
			}
		}
	}

	return 0;
}