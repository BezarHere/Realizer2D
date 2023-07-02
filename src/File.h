#pragma once
#include "global.h"
#include <fstream>
#include <istream>
#include <ostream>

// why the fuck am i creating this while the main rendering shit is still a mess
// this shitty shit can't handle fucking unicode wtf
// can somebody do something about this mess, thanks!

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

	inline bool isForReading() const { return (m_mode & 1); }
	inline bool isForWriting() const { return !(m_mode & 1); }
	inline const std::string& getPath() const { return m_path; }
	inline size_t size() const { return m_size; }
	inline size_t space() const { return m_space; }

	inline bool good() const { return m_stream.good(); }
	// if fail is true, all reading and writing will fail
	// failing happens if the path is invalid or the program can't access the file ( former is more propeple )
	inline bool fail() const { return m_stream.fail(); }
	inline std::ios::iostate state() const { return m_stream.rdstate(); }

	// brief writes the buffer to the file
	// - if 'to' is set to a value smaller or equal to the paramter 'from' or remained the default value (0)
	//   then the buffer will be written to the end
	virtual void write(const char* const buffer, size_t from = 0U, size_t to = 0U);

	// if length is set to value larger then 0 then the set length of bytes will be returned
	// otherwise the rest of the file will be returned ( Not recomended )
	virtual char* read(size_t len = 0U);


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