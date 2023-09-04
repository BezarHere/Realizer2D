#pragma once
#include "_bite_utility.h"
#include <fstream>
#include <memory>

namespace bite
{

	typedef std::unique_ptr<char[]> BufferSmartPtr_t;
	typedef std::shared_ptr<char[]> BufferSharedPtr_t;
	typedef std::vector<BufferSharedPtr_t> BufferVector_t;

	enum class EndianOrder
	{
		Little,
		Big
	};

	template <typename STREAM>
	class StreamFrame
	{
	public:
		using stream_type = STREAM;

		StreamFrame();
		StreamFrame(std::shared_ptr<STREAM> stream, EndianOrder order);

		void setStream(STREAM *stream);
		virtual void setStream(std::shared_ptr<STREAM> stream);
		std::shared_ptr<STREAM> getStream();
		const std::shared_ptr<const STREAM> getStream() const;

		EndianOrder order() const;

		virtual void move(intptr_t offset) = 0;

		virtual size_t cursor() const = 0;
		virtual size_t size() const = 0;

		std::ios::iostate state() const;
		bool valid() const;

		// same as valid
		operator bool() const;
		_NODISCARD bool operator!() const;

	protected:
		const EndianOrder m_order;
		std::shared_ptr<STREAM> m_stream;
	};

	class StreamReader : public StreamFrame<std::istream>
	{
	public:
		StreamReader();
		StreamReader(std::shared_ptr<stream_type> stream, EndianOrder order);
		StreamReader(stream_type *stream, EndianOrder order);
		StreamReader(const std::string &path, EndianOrder order);

		void move(intptr_t offset) override;

		size_t cursor() const override;
		size_t size() const override;

		// [risky to use whith objects]
		// creates a new instance of 'T' and returns it after loading it
		// if ORDER is true then the endian ordering will be apllied
		// when reading integrals/reals 'ORDER' should always be true to make sure accurate readins
		template <typename T, bool ORDER = true>
		T read();

		// [risky to use whith objects]
		// reads without advancing
		// if ORDER is true then the endian ordering will be apllied
		// when reading integrals/reals 'ORDER' should always be true to make sure accurate readins
		template <typename T, bool ORDER = true>
		T peek();

		// 'do_endianeness' can be true if you want an accurate integrale/real value loading
		// but 'do_endianeness' mostly bugs with object loading ( wich you shouldn't do )
		void load(char *buffer, size_t length, bool do_endianeness = false);
		BufferSmartPtr_t read(size_t length);
		BufferSmartPtr_t readCstr(size_t length);
		BufferSmartPtr_t peek(size_t length);
	};

	class StreamWriter : public StreamFrame<std::ostream>
	{
	public:
		StreamWriter();
		StreamWriter(std::shared_ptr<stream_type> stream, EndianOrder order);
		StreamWriter(stream_type *stream, EndianOrder order);
		StreamWriter(const std::string &path, EndianOrder order);

		~StreamWriter();

		void move(intptr_t offset) override;

		size_t cursor() const override;
		size_t size() const override;

		void flush();
		void close();

		// if order, the current endian order is applyed to make sure integrale/real numpers are written correctly
		// rememper to calls flush to flush content to file
		template <typename T, bool ORDER = true>
		void write(const T& value);

		// rememper to calls flush to flush content to file
		void write(const char *buffer, size_t length);

		// will not advance cursor like write()
		// rememper to calls flush to flush content to file
		void insert(const char *buffer, size_t length);

		// rememper to calls flush to flush content to file
		void writeCstr(const char *buffer, size_t length);

		// will not advance cursor like writeCstr()
		// rememper to calls flush to flush content to file
		void insertCstr(const char *buffer, size_t length);
	};


	extern void M_EndianOrder(char *dst, const char *src, size_t length, EndianOrder order);
	extern BufferSmartPtr_t M_Hexfy(uint8_t *ptr, size_t length);
	// if padding it true, all overflow bytes are set to zero
	extern BufferVector_t M_SplitBuffer(const char *src, size_t src_len, size_t block_len, size_t *last_block_len = nullptr);

	inline const std::string &M_EndianName(const EndianOrder order)
	{
		return std::string(order == EndianOrder::Little ? "little" : "big");
	}

	inline constexpr bool M_EndianNativeToMemory(const EndianOrder order)
	{
#if defined(_WIN32) || defined(__APPLE__) || defined(__MACH__)
		return order == EndianOrder::Little;
#endif
		// unix-based
		return order == EndianOrder::Big;
	}

	inline constexpr EndianOrder M_EndianReverse(const EndianOrder order)
	{
		return (EndianOrder)(((int)order) ^ 1);
	}

	// idk why ORDER is a template.

	template <typename T, bool ORDER>
	inline T StreamReader::read()
	{
		T value{};
		load((char *)&value, sizeof(value), ORDER);
		return value;
	}

	template <typename T, bool ORDER>
	inline T StreamReader::peek()
	{
		T value{};
		load((char *)&value, sizeof(value), ORDER);
		move(-(intptr_t)sizeof(value)); // yeah
		return value;
	}

	template <typename T, bool ORDER>
	inline void StreamWriter::write(const T &value)
	{
		constexpr size_t length{ sizeof(T) };
		if (ORDER)
		{
			if (!M_EndianNativeToMemory(order())) {
				char buffer[ length ];
				M_EndianOrder(buffer, (char *)&value, length, order());
				write(buffer, length);
			}
			else
			{
				write((char *)&value, length);
			}
		}
		else
		{
			write((char *)&value, length);
		}
	}

}
