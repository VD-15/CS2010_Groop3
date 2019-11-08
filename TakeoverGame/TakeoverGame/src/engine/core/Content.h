#pragma once

#include "ValkyrieEngine.h"

#include <map>

namespace vlk
{
	struct Content
	{
		virtual void LoadContent(const std::string& path) = 0;
		virtual const std::string& GetContentName() const = 0;

		void AddMetaTag(const std::string& name, const std::string& value);
		const std::string& GetMetaTag(const std::string& name) const;

		private:
		std::map<std::string, std::string> metaData;
	};
}