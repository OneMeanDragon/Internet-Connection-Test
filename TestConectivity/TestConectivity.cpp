// TestConectivity.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi")

//////////////////////////// sec2 ///////////////
//#include <winsock2.h>
//#include <ws2ipdef.h>
//#include <iphlpapi.h>
//#include <stdio.h>

//#pragma comment(lib, "iphlpapi.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x)) 
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
////////////////////////////////////////// sec2 //////////////////////////


class CNetworkListManagerEvent : public INetworkListManagerEvents
{
public:
	CNetworkListManagerEvent() : m_ref(1)
	{

	}

	virtual ~CNetworkListManagerEvent()
	{

	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject)
	{
		HRESULT Result = S_OK;
		if (IsEqualIID(riid, IID_IUnknown))
		{
			*ppvObject = (IUnknown *)this;
		}
		else if (IsEqualIID(riid, IID_INetworkListManagerEvents))
		{
			*ppvObject = (INetworkListManagerEvents *)this;
		}
		else
		{
			Result = E_NOINTERFACE;
		}

		return Result;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return (ULONG)InterlockedIncrement(&m_ref);
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		LONG Result = InterlockedDecrement(&m_ref);
		if (Result == 0)
			delete this;
		return (ULONG)Result;
	}

	virtual HRESULT STDMETHODCALLTYPE ConnectivityChanged(
		/* [in] */ NLM_CONNECTIVITY NewConnectivity)
	{
		auto bInternet = false;
		if ((NewConnectivity & NLM_CONNECTIVITY_IPV4_INTERNET)
			|| (NewConnectivity & NLM_CONNECTIVITY_IPV4_INTERNET)) {
			bInternet = true;
		}

		if (bInternet)
			PostQuitMessage(0);

		return S_OK;
	}
private:

	LONG m_ref;
};

bool WeHaveInternetConnection() {
	bool result_return = false;

	IUnknown *pUnknown = NULL;

	HRESULT Result = CoCreateInstance(CLSID_NetworkListManager,
		NULL, CLSCTX_ALL, IID_IUnknown, (void **)&pUnknown);
	if (SUCCEEDED(Result))
	{
		INetworkListManager *pNetworkListManager = NULL;
		Result = pUnknown->QueryInterface(IID_INetworkListManager,
			(void **)&pNetworkListManager);
		if (SUCCEEDED(Result))
		{
			VARIANT_BOOL IsConnect = VARIANT_FALSE;
			Result = pNetworkListManager->get_IsConnectedToInternet(&IsConnect);
			if (SUCCEEDED(Result))
			{
				//printf("IsConnect Result %s\n", IsConnect == VARIANT_TRUE ? "TRUE" : "FALSE");
				result_return = (IsConnect == VARIANT_TRUE);
			}
			pNetworkListManager->Release();
		}

		pUnknown->Release();
	}
	return result_return;
}

int main()
{
	CoInitialize(NULL);


	//while (true) {
		std::cout << "\nDo we have internet connection: " << (WeHaveInternetConnection() == TRUE ? "Yes" : "No") << "\r\n";
	//	Sleep(1000);
	//}


	CoUninitialize();
	system("pause");
	return 0;









	/////////////////////////////////////////////////////////////////
	//////////////////////////////////////////
	///////////////////////////////////////////////////
	//////////////////////////////////

	// Declare and initialize variables
	PIP_INTERFACE_INFO pInfo = NULL;
	ULONG ulOutBufLen = 0;

	DWORD dwRetVal = 0;
	int iReturn = 1;

	int i;

	// Make an initial call to GetInterfaceInfo to get
	// the necessary size in the ulOutBufLen variable
	dwRetVal = GetInterfaceInfo(NULL, &ulOutBufLen);
	if (dwRetVal == ERROR_INSUFFICIENT_BUFFER) {
		pInfo = (IP_INTERFACE_INFO *)MALLOC(ulOutBufLen);
		if (pInfo == NULL) {
			printf
			("Unable to allocate memory needed to call GetInterfaceInfo\n");
			return 1;
		}
	}
	// Make a second call to GetInterfaceInfo to get
	// the actual data we need
	dwRetVal = GetInterfaceInfo(pInfo, &ulOutBufLen);
	if (dwRetVal == NO_ERROR) {
		printf("Number of Adapters: %ld\n\n", pInfo->NumAdapters);
		for (i = 0; i < pInfo->NumAdapters; i++) {
			printf("Adapter Index[%d]: %ld\n", i,
				pInfo->Adapter[i].Index);
			printf("Adapter Name[%d]: %ws\n\n", i,
				pInfo->Adapter[i].Name);
		}
		iReturn = 0;
	}
	else if (dwRetVal == ERROR_NO_DATA) {
		printf
		("There are no network adapters with IPv4 enabled on the local system\n");
		iReturn = 0;
	}
	else {
		printf("GetInterfaceInfo failed with error: %d\n", dwRetVal);
		iReturn = 1;
	}

	FREE(pInfo);
	system("pause");
	return (iReturn);













	///////////////////////////////////////// sec1
	DWORD flags;
	bool isConnected = InternetGetConnectedState(&flags, 0);
	std::string msgOut = "False";
	if (isConnected) { msgOut = "True"; }

	std::cout << "Local Connection Established: " << msgOut.c_str() << "\r\n";
	msgOut = "False";
	/*if (system("ping www.google.com")) {
		std::cout << "\nNot connnected to the internet\n\n";
	}
	else {
		std::cout << "\nConnected to the internet\n\n";

	}*/

	/*while (InternetGetConnectedState(&flags, 0)) { //We have our local connection
		//
	}*/
	std::string urlx = "http://192.168.0.10:21845"; //botnet.bnetdocs.org:21845
	/*char url[128];
	strcpy_s(url, urlx.c_str());
	LPCWSTR *thisurl = (LPCWSTR*)url;*/
	bool bConnect = InternetCheckConnectionA(urlx.c_str(), FLAG_ICC_FORCE_CONNECTION, 0);
	if (bConnect) { msgOut = "True"; }

	std::cout << "Internet Connection Established: " << msgOut.c_str() << "\r\n";

	/*while (true) { //this also only gets local private lan connection
		if (IsInternetAvailable()) {
			break;
		}
	}*/
	DWORD igcFlags; //damned local garbage I need info from the adapter!!!!!!!!
	bool InternetConnection = InternetGetConnectedState(&igcFlags, 0);

	system("pause");
	return 0;
}


bool IsInternetAvailable()
{
	bool bIsInternetAvailable = false;
	// Get the required buffer size
	DWORD dwBufferSize = 0;
	if (ERROR_INSUFFICIENT_BUFFER == GetIpForwardTable(NULL, &dwBufferSize, false))
	{
		BYTE *pByte = new BYTE[dwBufferSize];
		if (pByte)
		{
			PMIB_IPFORWARDTABLE pRoutingTable = reinterpret_cast<PMIB_IPFORWARDTABLE>(pByte);
			// Attempt to fill buffer with routing table information
			if (NO_ERROR == GetIpForwardTable(pRoutingTable, &dwBufferSize, false))
			{
				DWORD dwRowCount = pRoutingTable->dwNumEntries; // Get row count
																// Look for default route to gateway
				for (DWORD dwIndex = 0; dwIndex < dwRowCount; ++dwIndex)
				{
					if (pRoutingTable->table[dwIndex].dwForwardDest == 0)
					{ // Default route designated by 0.0.0.0 in table
						bIsInternetAvailable = true; // Found it
						break; // Short circuit loop
					}
				}
			}
			delete[] pByte; // Clean up. Just say "No" to memory leaks
		}
	}
	return bIsInternetAvailable;
}