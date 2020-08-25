#pragma once

#include "AIGCBase.h"

namespace AIGC
{
	class HttpHelper
	{
	
	public:
		typedef struct _Result
		{
			bool bSuccess;
			std::string sErrmessage;
			std::string sData;
		}Result;

		static Result Get(const std::string& sUrl);
		static Result Post(const std::string& sUrl, const std::map<std::string, std::string>& sData = {}, const std::vector<std::string>& vHeaders = {});
	};
}


