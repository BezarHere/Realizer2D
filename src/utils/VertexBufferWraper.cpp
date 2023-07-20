#include "global.h"
#include "VertexBufferWraper.h"

namespace R2D::utils
{
	VertexBufferWraper::VertexBufferWraper(const sf::VertexBuffer &buffer)
		: m_buffer(buffer)
	{
	}

	VertexBufferWraper::VertexBufferWraper(sf::PrimitiveType type, sf::VertexBuffer::Usage usage, size_t size)
		: VertexBufferWraper(sf::VertexBuffer(type, usage))
	{
		m_buffer.create(size);
	}

	VertexBufferWraper::~VertexBufferWraper()
	{
		m_buffer.create(0);
	}
}
