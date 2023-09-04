#pragma once
#include <string>
#include <vector>

#ifndef FORCEINLINE
#define FORCEINLINE __forceinline
#define BITE_FORCEINLINE
#endif

namespace bite
{
	extern std::string strlower(const std::string &str);
	extern std::string strupper(const std::string &str);
	
	// modifies the dst to be a copy of src lowered(uncapitalized) from the start to 'len'
	extern void strlower(char *dst, const char *const src, const size_t len);
	
	// modifies the dst to be a copy of src uppered(capitalized) from the start to 'len'
	extern void strupper(char *dst, const char *const src, const size_t len);

	// stops at the first null char in 'src'
	// modifies the dst to be a copy of src lowered(uncapitalized) from the start
	// to the index of the first null char in src
	extern void strlower(char *dst, const char *const src);
	
	// stops at the first null char in 'src'
	// modifies the dst to be a copy of src uppered(capitalized) from the start
	// to the index of the first null char in src
	extern void strupper(char *dst, const char *const src);

	// modifies the cstring from the start to len to be upper
	extern void strupper(char *cstr, size_t len);
	
	// modifies the cstring from the start to len to be lower
	extern void strlower(char *cstr, size_t len);

	// modifies the cstring to be upper
	extern void strupper(char *cstr);
	
	// modifies the cstring to be lower
	extern void strlower(char *cstr);

	// retruns: a cstring copy of src to upper with size len + 1
	//	as the last additional char is a null char
	extern char *strupper(const char *const src, size_t len);
	
	// retruns: a cstring copy of src to lower with size len + 1
	//	as the last additional char is a null char
	extern char *strlower(const char *const src, size_t len);

	// stops at the first null char
	// retruns: a cstring copy of src to upper
	extern char *strupper(const char *const src);
	
	// stops at the first null char
	// retruns: a cstring copy of src to lower
	extern char *strlower(const char *const src);

	// returns a substr *copy* from index to len, doesn't modify the original str
	extern char *strslice(const std::string &src, const size_t index, const size_t len);
	extern char *strslice(const char *const src, const size_t index, const size_t len);
}

template <typename _T>
FORCEINLINE _T *ptrcpy(const _T *const ptr)
{
	return (_T *)memcpy(new _T{}, ptr, sizeof(_T));
}

template <typename _T, class... _Valty>
FORCEINLINE _T *ptrcpy(const _T *const ptr, _Valty&&... _Val)
{
	return (_T *)memcpy(new _T{ _Val... }, ptr, sizeof(_T));
}

template <typename _T>
FORCEINLINE _T *memdup(const _T *const src, size_t len)
{
	return (_T *)memcpy(new _T[ len ], src, sizeof(_T) * len);
}

FORCEINLINE void *memdup(const void *const src, size_t len)
{
	return memcpy((void *)new char[len], src, len);
}

#ifdef BITE_FORCEINLINE
#undef FORCEINLINE
#undef BITE_FORCEINLINE
#endif