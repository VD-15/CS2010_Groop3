#include "Material.h"

#include <fstream>

using namespace vlk;

Material::Material() :
	ambient(Color::WHITE),
	diffuse(Color::WHITE),
	specular(Color::WHITE)
{
	diffuseMap = nullptr;
	specularMap = nullptr;
	exponentMap = nullptr;
	alphaMap = nullptr;
	this->transparency = 0.0f;
	this->exponent = 500.0f;
}

MaterialLibrary::MaterialLibrary()
{

}

MaterialLibrary::~MaterialLibrary()
{
	for (auto it = materials.begin(); it != materials.end(); it++)
	{
		delete it->second;
	}
}

void MaterialLibrary::LoadContent(const std::string& path)
{
	for (auto it = materials.begin(); it != materials.end(); it++)
	{
		delete it->second;
	}

	materials.clear();

	std::ifstream in;
	in.open(path, std::ios::in);

	if (!in.is_open())
	{
		LogError("Model", "Failed to open file: " + path);
		throw std::exception("Failed to open file.");
	}

	std::string line;
	Material* mat = nullptr;

	while (std::getline(in, line))
	{
		ULong pos = line.find_first_of(" ");
		std::string command = line.substr(0, pos);

		if (command == "newmtl")
		{
			mat = new Material();
			materials.emplace(line.substr(pos + 1), mat);
		}
		else if (command == "Ka")
		{
			mat->ambient = Color(
				std::stof(line, &pos),
				std::stof(line, &pos),
				std::stof(line, &pos),
				1.0f
			);
		}
		else if (command == "Kd")
		{
			mat->diffuse = Color(
				std::stof(line, &pos),
				std::stof(line, &pos),
				std::stof(line, &pos),
				1.0f
			);
		}
		else if (command == "Ks")
		{
			mat->specular = Color(
				std::stof(line, &pos),
				std::stof(line, &pos),
				std::stof(line, &pos),
				1.0f
			);
		}
		else if (command == "Ns")
		{
			mat->exponent = std::stof(line, &pos);
		}
		else if (command == "d")
		{
			mat->transparency = std::stof(line, &pos);
		}
		else if (command == "Tr")
		{
			mat->transparency = 1.0f - std::stof(line, &pos);
		}
		else if (command == "map_Kd")
		{
			
		}
		else if (command == "map_Ks")
		{

		}
		else if (command == "map_Ns")
		{

		}
		else if (command == "map_d" || command == "map_Tr")
		{

		}
		else
		{
			LogWarning("Material Library", "Unknown command: " + command);
		}
	}
}

const std::string& MaterialLibrary::GetContentName() const
{
	static const std::string name("material");
	return name;
}

const Material* MaterialLibrary::GetMaterial(const std::string& name) const
{
	auto it = materials.find(name);

	if (it == materials.end())
	{
		LogError("Material Library", "Failed to find material with name: " + name);
		throw std::exception("Failed to find material.");
		return nullptr;
	}
	else
	{
		return it->second;
	}
}