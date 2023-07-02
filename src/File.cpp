#include "global.h"
#include "File.h"

_R2D_NAMESPACE_START_

File::File(const std::string& path, const OpenMode mode)
	: m_path(path), m_mode(mode),
	m_stream(path, std::ios::binary | ((mode & 1) ? std::ios::in : (std::ios::out | std::ios::trunc)))
{
	if (fail())
	{
		std::cout << "Failed to open file to path: \"" << path << "\" with state: " << state() << std::endl;
		return;
	}
	recalculate();
}

void File::write(const char* const buffer, size_t from, size_t to)
{
	if (isForReading() || m_stream.fail())
	{
		return;
	}

	size_t b_length = lengthof(buffer);
	size_t w_length = to > from ? to - from : b_length - from;

	// remove terminating null
	if (m_mode == WriteAscii)
	{
		w_length--;
		if (w_length <= 0) return;
	}

	m_stream.write(buffer, w_length);
	recalculate();
}

char* File::read(size_t len)
{
	if (isForWriting() || m_stream.fail())
	{
		return nullptr;
	}
	size_t reading_length = len > 0 ? len : m_space;

	// not enough reading space
	if (space() < len)
	{
		_pr_error(
			"Not enough data for buffer of size " + std::to_string(reading_length) + ", there is only " + std::to_string(space()) + " bytes for reading: returning 'nullptr'");
		return nullptr;
	}

	size_t value_length = reading_length;

	// creating space for terminating null
	if (m_mode == ReadAscii)
	{
		value_length++;
	}
	char* value = new char[value_length];

	m_stream.read(value, reading_length);

	// add terminating null
	if (m_mode == ReadAscii)
	{
		value[reading_length] = '\0';
	}

	recalculate();
	return value;
}

void File::recalculate()
{
	if (isForReading())
	{
		std::streampos current_cur = m_stream.tellg();
		m_stream.seekg(0, std::ios::end);
		size_t end_cur = (size_t)m_stream.tellg();
		m_size = end_cur;
		m_space = end_cur - current_cur;
		m_stream.seekg(current_cur);
	}
	else
	{
		m_size = (size_t)m_stream.tellp();
		m_space = -1;
	}
}

_R2D_NAMESPACE_END_