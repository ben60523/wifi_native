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
PDOT11_SSID pDotSSid = NULL;


VOID CALLBACK notificationCallback(__in PWLAN_NOTIFICATION_DATA pNotifData, __in_opt PVOID pContext) {
	if (pNotifData != NULL) {
		switch (pNotifData->NotificationSource)
		{
		case WLAN_NOTIFICATION_SOURCE_ACM:
			if (pNotifData->NotificationCode == wlan_notification_acm_connection_complete) {
				WLAN_BSS_LIST WlanBssList;
				int ret;
				ret = WlanGetNetworkBssList(hClient, &pIfInfo->InterfaceGuid, pDotSSid, dot11_BSS_type_infrastructure, FALSE, NULL, &WlanBssList);
				if (ret != ERROR_SUCCESS) {
					wprintf(L"WlanGetNetworkBssList failed with error: %u\n", ret);
					return 1;
				}
				else {
					for (int i = 0; i < WlanBssList.dwNumberOfItems; i++) {
						wprintf(L"SSID: %s\n", WlanBssList.wlanBssEntries->dot11Ssid);
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

int wmain()
{

	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	GUID interfaceGuid;
	DWORD dwPrevNotifType = 0;

	unsigned int i, k = 0;

	// variables used for WlanEnumInterfaces

	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;

	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return 1;
	}

	dwResult = WlanRegisterNotification(hClient, WLAN_NOTIFICATION_SOURCE_ALL, FALSE, notificationCallback, NULL, NULL, &dwPrevNotifType);
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
		for (i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO*)&pIfList->InterfaceInfo[i];
			WLAN_RAW_DATA WlanRawData;
			dwResult = WlanScan(hClient, &pIfInfo->InterfaceGuid, pDotSSid, &WlanRawData, NULL);
			if (dwResult != ERROR_SUCCESS) {
				wprintf(L"WlanScan failed with error: %u\n", dwResult);
				return 1;
			}
			else {
				k++;
			}
		}

	}
	if (pConnectInfo != NULL) {
		WlanFreeMemory(pConnectInfo);
		pConnectInfo = NULL;
	}

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}
	if (hClient != NULL) {
		WlanCloseHandle(hClient, NULL);
	}
	if (k == i) {
		wprintf(L"WlanScan_ok");
	}
	return dwRetVal;
}