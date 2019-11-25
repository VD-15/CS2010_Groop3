#include "XmlDocument.h"

using namespace vlk;

XmlDocument::XmlDocument()
{

}

XmlDocument::~XmlDocument()
{

}

void XmlDocument::LoadContent(const std::string& path, const std::string& name)
{
	pugi::xml_parse_result result = this->load_file(name.c_str());

	if (result.status != pugi::xml_parse_status::status_ok)
	{
		LogError("XMLDocument", "Failed to open content file: " + std::string(result.description()));
		throw std::exception("Failed to open content file.");
	}
}

const std::string& XmlDocument::GetContentName() const
{ 
	static std::string name("xml_document"); 
	return name; 
}