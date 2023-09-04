#pragma once
#include "_bite_utility.h"

namespace bite
{
	struct _ReallocGrowth
	{
		// used to calculate new capacity growth
		inline static constexpr size_t Grow(size_t oldCap)
		{
			if (oldCap > 1024)
			{
				return (size_t)((float)oldCap * 1.6f);
			}
			return oldCap * 2;
		}
	};

	template <typename _Elm, typename _ReAlloc = _ReallocGrowth>
	class BinaryTree
	{
	public:
		using _Index = size_t;
		struct _tree_node
		{
			_Index parent;
			_Index left, right;
		};
		using Node = _tree_node;

		inline size_t size() const noexcept
		{
			return m_size;
		}

		inline size_t capacity() const noexcept
		{
			return m_capacity;
		}

		inline void reserve(size_t elms_)
		{
			if (elms_ <= m_capacity)
				return;
			_reallocCapacity(elms_);
		}

		inline void shrink_to_size()
		{
			_reallocCapacity(size());
		}

	private:
		inline void _reallocCapacity(size_t newCap)
		{
			if (newCap == m_capacity)
				return;
			if (newCap < size())
				throw std::bad_array_new_length("new capcity in a tree can't be smaller then said tree size");
			m_Elms = (_tree_node *)memmove(new _tree_node[ newCap ], m_Elms, sizeof(_tree_node) * m_capacity);
			m_capacity = newCap;
		}



		void _isRoot();

		
		_tree_node *m_Elms;
		size_t m_size;
		size_t m_capacity;
	};
}
