#include "Texture.h"
#include "stb/stb_image.h"

using namespace vlk;

Texture2D::Texture2D()
{
	this->data = nullptr;
	this->width = 0;
	this->height = 0;
	this->isStbBacked = false;
}

Texture2D::Texture2D(const Byte* data, ULong size, Int width, Int height)
{
	this->data = new Byte[size];
	memcpy(this->data, data, size);

	this->width = width;
	this->height = height;
	this->isStbBacked = false;
}

Texture2D::~Texture2D()
{
	if (this->isStbBacked)
	{
		stbi_image_free(data);
	}
	else
	{
		delete data;
	}
}

void Texture2D::LoadContent(const std::string& path, const std::string& name)
{
	if (data)
	{
		if (this->isStbBacked)
		{
			stbi_image_free(data);
		}
		else
		{
			delete data;
		}
	}

	stbi_set_flip_vertically_on_load(1);
	data = stbi_load(path.c_str(), &width, &height, nullptr, 4);

	if (!data)
	{
		LogError("Texture2D", "Failed to load content from file: " + path);
		throw std::exception("Failed to load content.");
	}

	this->isStbBacked = true;
}

const std::string& Texture2D::GetContentName() const
{
	static const std::string s("texture");
	return std::ref(s);
}