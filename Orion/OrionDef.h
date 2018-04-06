//----------------------------------------------------------------------------------
#ifndef ORIONDEF_H
#define ORIONDEF_H
//----------------------------------------------------------------------------------
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned __int64 uint64;
typedef uchar *puchar;
typedef ushort *pushort;
typedef uint *puint;
typedef uint64 *puint64;
typedef char *pchar;
typedef short *pshort;
typedef int *pint;
typedef __int64 *pint64;
//----------------------------------------------------------------------------------
#define DEBUGGIND_OUTPUT 1

#if DEBUGGIND_OUTPUT == 1
void DebugMsg(const char *format, ...);
void DebugMsg(const wchar_t *format, ...);
void DebugDump(puchar data, const int &size);
#else
#define DebugMsg(...)
#define DebugDump(buf, size)
#endif
//----------------------------------------------------------------------------------
#define RELEASE_POINTER(ptr) \
if (ptr != NULL) \
{ \
	delete ptr; \
	ptr = NULL; \
}
//----------------------------------------------------------------------------------
//!Incremented ordinary for
#define IFOR(var, start, stop) for (int var = start; var < stop; var ++)
//!Decremented ordinary for
#define DFOR(var, start, stop) for (int var = start; var >= stop; var --)
//----------------------------------------------------------------------------------
#define IN_RANGE(name, id1, id2) (name >= id1 && name <= id2)
#define OUT_RANGE(name, id1, id2) (name < id1 || name > id2)
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property
#define SETGET(type, name) \
	protected: \
	type m_##name; \
	public: \
	inline void __fastcall Set##name(const type &val) { m_##name = val; } \
	inline type Get##name() const { return m_##name; }
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property with inc/dec/add functions
#define SETGETEX(type, name) \
	SETGET(type, name); \
	inline void Inc##name() { m_##name++; } \
	inline void Dec##name() { m_##name--; } \
	inline void Add##name(type val) { m_##name += val; }
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property with event
#define SETGETE(type, name, event) \
	protected: \
	type m_##name; \
	public: \
	void event(const type &val); \
	inline void __fastcall Set##name(const type &val) { event(val); m_##name = val; } \
	inline type Get##name() const { return m_##name; }
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property with inc/dec/add functions
#define SETGETEXE(type, name, event) \
	SETGETE(type, name, event); \
	inline void Inc##name() { event(m_##name + 1); m_##name++; } \
	inline void Dec##name() { event(m_##name - 1); m_##name--; } \
	inline void Add##name(type val) { event(m_##name + val); m_##name += val; }
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
