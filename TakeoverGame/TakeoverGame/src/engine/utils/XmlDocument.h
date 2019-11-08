#pragma once

#include "pugixml/pugixml.hpp"

#include "../core/Content.h"
#include "../core/ValkyrieEngine.h"

namespace vlk
{
	struct XmlDocument : public pugi::xml_document, public Content
	{
		XmlDocument();
		virtual ~XmlDocument();

		void LoadContent(const std::string& path) override;
		const std::string& GetContentName() const override;
	};
}