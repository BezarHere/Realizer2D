#pragma once
#include "_bite_utility.h"

namespace bite::filesys
{
	typedef struct
	{
		const std::string path;
		const bool dir; // is the entry a directory? otherwise it's a file
	} DirEntry_t;

	extern bool exists(const std::string &path);
	extern bool exists(const char *const path);

}
