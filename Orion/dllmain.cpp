//----------------------------------------------------------------------------------
#include "Global.h"
#include "DataStream.h"
#include "Crypt/LoginCrypt.h"
#include "Crypt/GameCrypt.h"
#include "../../OrionUO/OrionUO/EnumList.h"
//----------------------------------------------------------------------------------
ENCRYPTION_TYPE g_EncryptionType = ET_NOCRYPT;
//----------------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	(void)hModule;
	(void)lpReserved;

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}
//----------------------------------------------------------------------------------
void __cdecl Init(const bool &mode, BYTE *array)
{
	if (mode)
		g_LoginCrypt.Init(array);
	else
	{
		if (g_EncryptionType != ET_NOCRYPT)
			g_BlowfishCrypt.Init();

		if (g_EncryptionType == ET_203 || g_EncryptionType == ET_TFISH)
		{
			g_TwofishCrypt.Init(array);

			if (g_EncryptionType == ET_TFISH)
				g_TwofishCrypt.Init_MD5();
		}
	}
}
//----------------------------------------------------------------------------------
void __cdecl Send(const bool &mode, BYTE *src, BYTE *dest, const int &size)
{
	if (g_EncryptionType == ET_NOCRYPT)
		memcpy(&dest[0], &src[0], size);
	else if (mode)
	{
		if (g_EncryptionType == ET_OLD_BFISH)
			g_LoginCrypt.Encrypt_Old(&src[0], &dest[0], size);
		else if (g_EncryptionType == ET_1_25_36)
			g_LoginCrypt.Encrypt_1_25_36(&src[0], &dest[0], size);
		else if (g_EncryptionType != ET_NOCRYPT)
			g_LoginCrypt.Encrypt(&src[0], &dest[0], size);
	}
	else if (g_EncryptionType == ET_203)
	{
		g_BlowfishCrypt.Encrypt(&src[0], &dest[0], size);
		g_TwofishCrypt.Encrypt(&dest[0], &dest[0], size);
	}
	else if (g_EncryptionType == ET_TFISH)
		g_TwofishCrypt.Encrypt(&src[0], &dest[0], size);
	else
		g_BlowfishCrypt.Encrypt(&src[0], &dest[0], size);
}
//----------------------------------------------------------------------------------
void __cdecl Decrypt(BYTE *src, BYTE *dest, const int &size)
{
	if (g_EncryptionType == ET_TFISH)
		g_TwofishCrypt.Decrypt(&src[0], &dest[0], size);
	else
		memcpy(&dest[0], &src[0], size);
}
//----------------------------------------------------------------------------------
extern "C" __declspec(dllexport) size_t __cdecl GetPluginsCount()
{
	return g_PluginsCount;
}
//----------------------------------------------------------------------------------
void __cdecl LoadPlugins(PLUGIN_INFO *result)
{
	if (g_RawData.size())
	{
		CDataReader file(&g_RawData[0], (int)g_RawData.size());

		uchar ver = file.ReadUInt8();

		file.Move(2);
		int len = file.ReadUInt8();
		file.Move(len + 39);

		if (ver >= 2)
		{
			file.Move(1);

			if (ver >= 3)
				file.Move(1);

			char count = file.ReadInt8();

			IFOR(i, 0, count)
			{
				short len = file.ReadInt16LE();
				string fileName = file.ReadString(len);
				memcpy(&result[i].FileName[0], &fileName.data()[0], fileName.length());

				file.Move(2);
				result[i].Flags = file.ReadUInt32LE();
				file.Move(2);

				len = file.ReadInt16LE();
				string functionName = file.ReadString(len);
				memcpy(&result[i].FunctionName[0], &functionName.data()[0], functionName.length());
			}
		}
	}
}
//----------------------------------------------------------------------------------
void ParseCommandLine()
{
	int argc = 0;
	LPWSTR *args = CommandLineToArgvW(GetCommandLineW(), &argc);

	IFOR(i, 0, argc)
	{
		if (!args[i] || *args[i] != L'-')
			continue;

		string str = ToLowerA(ToString(args[i] + 1));

		if (str == "nocrypt")
			g_EncryptionType = ET_NOCRYPT;
	}

	LocalFree(args);
}
//----------------------------------------------------------------------------------
UCHAR_LIST ApplyInstall(uchar *address, size_t size)
{
	UCHAR_LIST result;

	if (size)
	{
		g_RawData.resize(size);
		memcpy(&g_RawData[0], &address[0], size);

		CDataReader file(address, size);
		CDataWritter writter;

		uchar version = file.ReadInt8();
		writter.WriteUInt8(version);
		writter.WriteUInt8(0xFE); //dll version
		writter.WriteUInt8(0); //sub version

		g_EncryptionType = (ENCRYPTION_TYPE)file.ReadInt8();
		writter.WriteUInt8(file.ReadInt8()); //ClientVersion

		int len = file.ReadInt8();
		writter.WriteUInt8(len);
		writter.WriteString(file.ReadString(len), len, false);

		file.Move(14); //crypt keys & seed
#if defined(_M_IX86)
		writter.WriteUInt32LE((size_t)Init);
		writter.WriteUInt32LE((size_t)Send);
		writter.WriteUInt32LE((size_t)Decrypt);
		writter.WriteUInt32LE((size_t)LoadPlugins);
#else
		writter.WriteUInt64LE((size_t)Init);
		writter.WriteUInt64LE((size_t)Send);
		writter.WriteUInt64LE((size_t)Decrypt);
		writter.WriteUInt64LE((size_t)LoadPlugins);
#endif

		int mapsCount = 6;

		if (version < 4)
			writter.WriteUInt8(file.ReadInt8()); //InverseBuylist
		else
		{
			mapsCount = file.ReadInt8();
			writter.WriteUInt8(mapsCount);
		}

		IFOR(i, 0, mapsCount)
		{
			writter.WriteUInt16LE(file.ReadUInt16LE());
			writter.WriteUInt16LE(file.ReadUInt16LE());
		}

		uchar clientFlag = 0;
		uchar useVerdata = 0;

		if (version >= 2)
		{
			clientFlag = file.ReadInt8();

			if (version >= 3)
				useVerdata = file.ReadInt8();

			g_PluginsCount = file.ReadInt8();
		}

		writter.WriteUInt8(clientFlag);
		writter.WriteUInt8(useVerdata);

		result = writter.Data();
	}

	ParseCommandLine();

	return result;
}
//----------------------------------------------------------------------------------
extern "C" __declspec(dllexport) void InstallNew(uchar *address, size_t size, uchar *result, size_t &resultSize)
{
	UCHAR_LIST buf = ApplyInstall(address, size);
	resultSize = buf.size();
	memcpy(&result[0], &buf[0], resultSize);
}
//----------------------------------------------------------------------------------
extern "C" __declspec(dllexport) void Install(uchar *address, size_t size, UCHAR_LIST &result)
{
	result = ApplyInstall(address, size);
}
//----------------------------------------------------------------------------------
