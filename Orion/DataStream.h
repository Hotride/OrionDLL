//----------------------------------------------------------------------------------
#ifndef DATASTREAM_H
#define DATASTREAM_H
//----------------------------------------------------------------------------------
#include "Global.h"
//----------------------------------------------------------------------------------
class CDataWritter
{
	SETGET(bool, AutoResize);
	SETGET(puchar, Ptr);

protected:
	UCHAR_LIST m_Data;

public:
	CDataWritter();
	CDataWritter(const int &size, const bool &autoResize = true);

	virtual ~CDataWritter();

	UCHAR_LIST Data() const { return m_Data; }
	int Size() { return (int)m_Data.size(); }

	void Resize(const int &newSize, const bool &resetPtr = false);
	void ResetPtr() { m_Ptr = &m_Data[0]; }

	void Move(const int &offset);

	void WriteDataBE(const puchar data, const int &size, const int offset = 0);
	void WriteDataLE(const puchar data, const int &size, const int offset = 0);

	void WriteUInt8(const uchar &val, const int &offset = 0) { WriteDataBE((puchar)&val, sizeof(uchar), offset); }

	void WriteUInt16BE(const ushort &val, const int &offset = 0) { WriteDataBE((puchar)&val, sizeof(ushort), offset); }
	void WriteUInt16LE(const ushort &val, const int &offset = 0) { WriteDataLE((puchar)&val, sizeof(ushort), offset); }

	void WriteUInt32BE(const uint &val, const int &offset = 0) { WriteDataBE((puchar)&val, sizeof(uint), offset); }
	void WriteUInt32LE(const uint &val, const int &offset = 0) { WriteDataLE((puchar)&val, sizeof(uint), offset); }

	void WriteUInt64BE(const uint64 &val, const int &offset = 0) { WriteDataBE((puchar)&val, sizeof(uint64), offset); }
	void WriteUInt64LE(const uint64 &val, const int &offset = 0) { WriteDataLE((puchar)&val, sizeof(uint64), offset); }

	void WriteInt8(const char &val, const int &offset = 0) { WriteDataBE((puchar)&val, sizeof(char), offset); }

	void WriteInt16BE(const short &val, const int &offset = 0) { WriteDataBE((puchar)&val, sizeof(short), offset); }
	void WriteInt16LE(const short &val, const int &offset = 0) { WriteDataLE((puchar)&val, sizeof(short), offset); }

	void WriteInt32BE(const int &val, const int &offset = 0) { WriteDataBE((puchar)&val, sizeof(int), offset); }
	void WriteInt32LE(const int &val, const int &offset = 0) { WriteDataLE((puchar)&val, sizeof(int), offset); }

	void WriteInt64BE(const __int64 &val, const int &offset = 0) { WriteDataBE((puchar)&val, sizeof(__int64), offset); }
	void WriteInt64LE(const __int64 &val, const int &offset = 0) { WriteDataLE((puchar)&val, sizeof(__int64), offset); }

	void WriteFloatBE(const float &val, const int &offset = 0) { WriteDataBE((puchar)&val, sizeof(float), offset); }
	void WriteFloatLE(const float &val, const int &offset = 0) { WriteDataLE((puchar)&val, sizeof(float), offset); }

	void WriteDoubleBE(const double &val, const int &offset = 0) { WriteDataBE((puchar)&val, sizeof(double), offset); }
	void WriteDoubleLE(const double &val, const int &offset = 0) { WriteDataLE((puchar)&val, sizeof(double), offset); }

	void WriteString(const string &val, int length = 0, const bool &nullTerminated = true, const int &offset = 0);
	void WriteWString(const wstring &val, int length = 0, const bool &bigEndian = true, const bool &nullTerminated = true, const int &offset = 0);
};
//----------------------------------------------------------------------------------
class CDataReader
{
	SETGET(puchar, Start);
	SETGET(int, Size);
	SETGET(puchar, End);
	SETGET(puchar, Ptr);

public:
	CDataReader();
	CDataReader(puchar start, const int &size);

	virtual ~CDataReader();

	void SetData(puchar start, const int &size, const int &offset = 0);
	void ResetPtr() { m_Ptr = m_Start; }

	bool IsEOF() { return m_Ptr >= m_End; }

	void Move(const int &offset) { m_Ptr += offset; }

	void ReadDataBE(puchar data, const int &size, const int offset = 0);
	void ReadDataLE(puchar data, const int &size, const int offset = 0);

	uchar ReadUInt8(const int &offset = 0) { uchar val = 0; ReadDataBE((puchar)&val, sizeof(uchar), offset); return val; }

	ushort ReadUInt16BE(const int &offset = 0) { ushort val = 0; ReadDataBE((puchar)&val, sizeof(ushort), offset); return val; }
	ushort ReadUInt16LE(const int &offset = 0) { ushort val = 0; ReadDataLE((puchar)&val, sizeof(ushort), offset); return val; }

	uint ReadUInt32BE(const int &offset = 0) { uint val = 0; ReadDataBE((puchar)&val, sizeof(uint), offset); return val; }
	uint ReadUInt32LE(const int &offset = 0) { uint val = 0; ReadDataLE((puchar)&val, sizeof(uint), offset); return val; }

	char ReadInt8(const int &offset = 0) { char val = 0; ReadDataBE((puchar)&val, sizeof(char), offset); return val; }

	short ReadInt16BE(const int &offset = 0) { short val = 0; ReadDataBE((puchar)&val, sizeof(short), offset); return val; }
	short ReadInt16LE(const int &offset = 0) { short val = 0; ReadDataLE((puchar)&val, sizeof(short), offset); return val; }

	int ReadInt32BE(const int &offset = 0) { int val = 0; ReadDataBE((puchar)&val, sizeof(int), offset); return val; }
	int ReadInt32LE(const int &offset = 0) { int val = 0; ReadDataLE((puchar)&val, sizeof(int), offset); return val; }

	__int64 ReadInt64BE(const int &offset = 0) { __int64 val = 0; ReadDataBE((puchar)&val, sizeof(__int64), offset); return val; }
	__int64 ReadInt64LE(const int &offset = 0) { __int64 val = 0; ReadDataLE((puchar)&val, sizeof(__int64), offset); return val; }

	float ReadFloatBE(const int &offset = 0) { float val = 0.0f; ReadDataBE((puchar)&val, sizeof(float), offset); return val; }
	float ReadFloatLE(const int &offset = 0) { float val = 0.0f; ReadDataLE((puchar)&val, sizeof(float), offset); return val; }

	double ReadDoubleBE(const int &offset = 0) { double val = 0.0; ReadDataBE((puchar)&val, sizeof(double), offset); return val; }
	double ReadDoubleLE(const int &offset = 0) { double val = 0.0; ReadDataLE((puchar)&val, sizeof(double), offset); return val; }

	string ReadString(int size = 0, const int &offset = 0);
	wstring ReadWString(int size = 0, const bool &bigEndian = true, const int &offset = 0);
};
//----------------------------------------------------------------------------------
#endif

