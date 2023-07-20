#pragma once

namespace R2D::utils
{
	class VertexBufferWraper
	{
	public:
		// takes ownership of the buffer
		VertexBufferWraper(const sf::VertexBuffer& buffer);
		VertexBufferWraper(sf::PrimitiveType type, sf::VertexBuffer::Usage usage, size_t size = 0);
		~VertexBufferWraper();

		__forceinline sf::VertexBuffer& getBuffer() { return m_buffer; }
		__forceinline const sf::VertexBuffer& getBuffer() const { return m_buffer; }

	private:
		sf::VertexBuffer m_buffer;
	};
}
