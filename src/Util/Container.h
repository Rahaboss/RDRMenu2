#pragma once

#include "pch.h"

namespace Util
{
	template<typename T>
	inline void ContainerSort(T& Container)
	{
		std::sort(Container.begin(), Container.end());
	}

	template<typename E, class T>
	inline bool ContainerContains(const T& Container, const E& Element)
	{
		return std::find(Container.cbegin(), Container.cend(), Element) != Container.cend();
	}
}
