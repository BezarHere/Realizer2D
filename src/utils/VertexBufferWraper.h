#pragma once

namespace R2D::utils
{
	class VertexBufferWraper
	{
	public:
		// takes ownership of the buffer
		VertexBufferWraper(sf::VertexBuffer* buffer);
		VertexBufferWraper(sf::PrimitiveType type, sf::VertexBuffer::Usage usage, size_t size);
		~VertexBufferWraper();

		inline sf::VertexBuffer* getBuffer() const { return m_buffer.get(); }

	private:
		const std::unique_ptr<sf::VertexBuffer> m_buffer;
	};
}
