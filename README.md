# Internet-Connection-Test

-This was just a test app.

The section of the app in which I was researching for was the following lines.
------------------------------------------------------------------------------
```
std::string urlx = "http://192.168.0.10:21845";
bool bConnect = InternetCheckConnectionA(urlx.c_str(), FLAG_ICC_FORCE_CONNECTION, 0);
if (bConnect) { msgOut = "True"; }
```
Which evolved into the following.
------------------------------------------------------------------------------
```
#pragma region "Test conectivity"
	bool WeHaveInternetConnection(std::string host, std::string port) {
		std::string urlx = "http://"; 
		urlx += host;
		urlx += ":";
		urlx += port;

		DWORD dwStart = GetTickCount();
		bool bConnect = InternetCheckConnectionA(urlx.c_str(), FLAG_ICC_FORCE_CONNECTION, 0);
		DWORD dwEnd = GetTickCount();
		dwPingTime = (dwEnd - dwStart);
		return bConnect;
	}
#pragma endregion
```
------------------------------------------------------------------------------


-l)ragon
