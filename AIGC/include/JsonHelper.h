#pragma once

#include "AIGCBase.h"

namespace AIGC
{
	class JsonHelper
	{
	public:

		static std::string GetValue(std::string sStr, std::vector<std::string> sNames);

		template<typename T>
		static std::string ConverObjectToString(T aStruct);

		template<typename T>
		static T& ConverStringToObject(std::string sStr, T& oRet);
	};
}

