#include "StringHelper.h"

namespace AIGC
{
	void StringHelper::TrimLeft(std::string& pStr, std::string pRemove)
	{
		if (pStr.length() <= 0 || pRemove.length() <= 0)
			return;

		while (true)
		{
			int index = pStr.find(pRemove);
			if (index != 0)
				return;

			pStr = pStr.substr(pRemove.length());
		}
	}

	void StringHelper::TrimRight(std::string& pStr, std::string pRemove)
	{
		if (pStr.length() <= 0 || pRemove.length() <= 0)
			return;

		while (true)
		{
			int index = pStr.find_last_of(pRemove);
			if (index != pStr.length() - 1)
				return;

			pStr = pStr.substr(0, pStr.length() - pRemove.length());
		}
	}

	void StringHelper::Trim(std::string& pStr, std::string pRemove)
	{
		TrimLeft(pStr, pRemove);
		TrimRight(pStr, pRemove);
	}
}
