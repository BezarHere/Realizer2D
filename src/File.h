#pragma once
#include "global.h"
#include <fstream>
#include <istream>
#include <ostream>

_R2D_NAMESPACE_START_

class File
{
public:

	/// TODO: Implement utf-8
	enum OpenMode : uint8_t
	{
		WriteBinary,
		ReadBinary,
		WriteAscii,
		ReadAscii,
		//WriteUTF8, 
		//ReadUTF8,
	};

public:
	File() = delete;
	File(const std::string& path, const OpenMode mode = OpenMode::ReadAscii);

	inline bool isForReading() const;
	inline bool isForWriting() const;
	inline const std::string& getPath() const;
	inline size_t size() const;
	inline size_t space() const;

	inline bool good() const;

	// if fail is true, all reading and writing will fail
	// failing happens if the path is invalid or the program can't access the file ( former is more propeple )
	inline bool fail() const;

	inline std::ios::iostate state() const;

	// brief writes the buffer to the file
	// - if 'to' is set to a value smaller or equal to the paramter 'from' or remained the default value (0)
	//   then the buffer will be written to the end
	virtual void write(const char* const buffer, size_t from = 0U, size_t to = 0U);

	// if length is set to value larger then 0 then the set length of bytes will be returned
	// otherwise the rest of the file will be returned ( Not recomended )
	virtual char* read(size_t len = 0U);

	// flushes the buffer to the file system, called automaticly on closing
	void flush();

	// closes and flushes, making the file accessible for writing by other processes
	void close();

	bool valid() const;

	bool canRead() const;
	bool canWrite() const;
	
	// [syntax sugar] same as valid
	// if the stream is valid this will return true, otherwise false
	operator bool() const;

private:
	void recalculate();
private:
	std::fstream m_stream;
	const OpenMode m_mode;
	const std::string m_path;
	size_t m_size;
	size_t m_space;
};

_R2D_NAMESPACE_END_