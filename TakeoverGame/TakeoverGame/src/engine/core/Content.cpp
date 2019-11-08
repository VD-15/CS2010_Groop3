#include "Content.h"

using namespace vlk;

void Content::AddMetaTag(const std::string& name, const std::string& value)
{
	metaData[name] = value;
}

const std::string& Content::GetMetaTag(const std::string& name) const
{
	auto it = metaData.find(name);

	if (it != metaData.end())
	{
		return std::forward<const std::string>(it->second);
	}
	else
	{
		return "";
	}
}