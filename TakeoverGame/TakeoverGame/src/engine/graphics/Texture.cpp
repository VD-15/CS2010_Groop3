#include "Texture.h"
#include "stb/stb_image.h"

using namespace vlk;

Texture2D::Texture2D()
{
	this->data = nullptr;
	this->width = 0;
	this->height = 0;
}

Texture2D::~Texture2D()
{
	stbi_image_free(data);
}

void Texture2D::LoadContent(const std::string& path)
{
	stbi_set_flip_vertically_on_load(1);
	data = stbi_load(path.c_str(), &width, &height, nullptr, 4);

	if (!data)
	{
		LogError("Texture2D", "Failed to load content from file: " + path);
		throw std::exception("Failed to load content.");
	}
}

const std::string& Texture2D::GetContentName() const
{
	static std::string s("texture");
	return std::ref(s);
}