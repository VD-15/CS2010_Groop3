#include "Material.h"

#include <fstream>
#include <array>

#include "../core/ContentManager.hpp"

using namespace vlk;

namespace
{
	Color ParseColor(std::string col)
	{
		ULong start = 0;

		ULong split1 = col.find(" ", start);
		ULong split2 = col.find(" ", split1 + 1);

		std::array<Float, 3> com;

		com[0] = std::stof(col.substr(start, split1));
		com[1] = std::stof(col.substr(split1, split2));
		com[2] = std::stof(col.substr(split2));

		return Color(
			com[0],
			com[1],
			com[2],
			1.0f
		);
	}
}

Material::Material() :
	ambient(Color::WHITE),
	diffuse(Color::WHITE),
	specular(Color::WHITE)
{
	const Texture2D* t = ContentManager<Texture2D>::Get().GetContent("VLK_DEFAULT_TEXTURE");

	diffuseMap = t;
	specularMap = t;
	exponentMap = t;
	alphaMap = t;
	this->transparency = 1.0f;
	this->exponent = 500.0f;
}

void Material::LoadContent(const std::string& path, const std::string& name)
{
	std::ifstream in;
	in.open(path, std::ios::in);

	if (!in.is_open())
	{
		LogError("Model", "Failed to open file: " + path);
		throw std::exception("Failed to open file.");
	}

	std::string line;
	Boolean found = false;

	while (std::getline(in, line))
	{
		ULong pos = line.find_first_of(" ");
		std::string command = line.substr(0, pos);
		
		if (command == "newmtl")
		{
			std::string args = line.substr(pos + 1);
			found = true;

			//find this material in material file
			if (args == name)
			{
				while (std::getline(in, line))
				{
					pos = line.find_first_of(" ");
					command = line.substr(0, pos);

					if (command == "newmtl")
					{
						return;
					}
					else if (command == "Ka")
					{
						std::string col(line.substr(pos + 1));

						this->ambient = ParseColor(line.substr(pos + 1));
					}
					else if (command == "Kd")
					{
						std::string col(line.substr(pos + 1));

						this->diffuse = ParseColor(line.substr(pos + 1));
					}
					else if (command == "Ks")
					{
						std::string col(line.substr(pos + 1));

						this->specular = ParseColor(line.substr(pos + 1));
					}
					else if (command == "Ns")
					{
						this->exponent = std::stof(line.substr(pos + 1));
					}
					else if (command == "d")
					{
						this->transparency = std::stof(line.substr(pos + 1));
					}
					else if (command == "Tr")
					{
						this->transparency = 1.0f - std::stof(line.substr(pos + 1));
					}
					else if (command == "map_Kd")
					{
						this->diffuseMap = ContentManager<Texture2D>::Get().GetContent(line.substr(pos + 1));
					}
					else if (command == "map_Ks")
					{
						this->specularMap = ContentManager<Texture2D>::Get().GetContent(line.substr(pos + 1));
					}
					else if (command == "map_Ns")
					{
						this->exponentMap = ContentManager<Texture2D>::Get().GetContent(line.substr(pos + 1));
					}
					else if (command == "map_d" || command == "map_Tr")
					{
						this->alphaMap = ContentManager<Texture2D>::Get().GetContent(line.substr(pos + 1));
					}
					else if (command == "#")
					{
						continue;
					}
					else
					{
						LogWarning("Material Library", "Unknown command: " + command);
					}
				}
			}
		}
	}

	if (!found)
	{
		LogError("Material", "Failed to find material " + name + " in file " + path);
		throw std::exception("Failed to find material");
	}
}

const std::string& Material::GetContentName() const
{
	static const std::string name("material");
	return name;
}