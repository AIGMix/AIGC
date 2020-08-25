#pragma once

#include "AIGCBase.h"

namespace AIGC
{
	class StringHelper
	{
	public:
		static void TrimLeft(std::string& pStr, std::string pRemove = " ");
		static void TrimRight(std::string& pStr, std::string pRemove = " ");
		static void Trim(std::string& pStr, std::string pRemove = " ");
	};
}

