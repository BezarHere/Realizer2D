#pragma once

_R2D_NAMESPACE_START_

template <class T>
class TreeStructure
{
public:
	template <class TN>
	struct TreeNode
	{
		TN content;
		std::vector<TreeNode<TN>> children;
	};
public:




private:
	TreeNode<T> m_root;
};

_R2D_NAMESPACE_END_