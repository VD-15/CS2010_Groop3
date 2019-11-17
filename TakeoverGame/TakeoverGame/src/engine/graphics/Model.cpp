#include "Model.h"
#include "../core/ContentManager.hpp"
#include <fstream>

using namespace vlk;

Model::Model()
{

}

Model::~Model()
{

}

void Model::LoadContent(const std::string& path, const std::string& name)
{
	this->vertices.clear();
	this->coords.clear();
	this->normals.clear();
	this->meshes.clear();

	std::ifstream in;
	in.open(path, std::ios::in);

	if (!in.is_open())
	{
		LogError("Model", "Failed to open file: " + path);
		throw std::exception("Failed to open file.");
	}

	std::string line;

	ULong numVertices = 0;
	ULong numCoords = 0;
	ULong numNormals = 0;
	std::map<ULong, ULong> numFaces;
	ULong meshGroups = 0;

	//preliminary parse to determine size of vectors and number of materials
	while (std::getline(in, line))
	{
		ULong pos = line.find_first_of(" ");
		std::string command = line.substr(0, pos);

		if (command == "v")
		{
			//vertex
			numVertices++;
		}
		else if (command == "vt")
		{
			//texture coordinate
			numCoords++;
		}
		else if (command == "vn")
		{
			//vertex normal
			numNormals++;
		}
		else if (command == "f")
		{
			//face
			numFaces[meshGroups]++;
		}
		else if (command == "usemtl")
		{
			//use material
			meshGroups++;
		}
	}

	//resize vectors once because we don't hate our heap memory
	this->vertices.reserve((numVertices) * 3);
	this->coords.reserve((numCoords) * 2);
	this->normals.reserve((numNormals) * 3);
	this->materials.reserve(meshGroups);

	for (auto)

	for (auto it = numFaces.begin(); it != numFaces.end(); it++)
	{
		//this assumes all faces will be triangular and have three elements: position, normal and UV
		this->meshes[it->first].reserve((it->second) * 9);
	}

	in.clear();
	in.seekg(0, std::ios::beg);
	meshGroups = 0;

	//secondary pass to fill vectors
	while (std::getline(in, line))
	{
		ULong pos = line.find_first_of(" ");
		std::string command = line.substr(0, pos);

		if (command == "v")
		{
			std::string vert(line.substr(pos + 1));
			ULong start = 0;

			//vertex, expects precisely three floats
			for (UInt i = 0; i < 3; i++)
			{
				ULong end = vert.find(" ", start) + 1;
				vertices.push_back(std::stof(vert.substr(start, start - end)));
				start = end;
			}

			continue;
		}
		else if (command == "vt")
		{
			std::string vert(line.substr(pos + 1));
			ULong start = 0;

			//texture coordinate, expects precisely two floats
			for (UInt i = 0; i < 2; i++)
			{
				ULong end = vert.find(" ", start) + 1;
				coords.push_back(std::stof(vert.substr(start, start - end)));
				start = end;
			}

			continue;
		}
		else if (command == "vn")
		{
			std::string vert(line.substr(pos + 1));
			ULong start = 0;

			//vertex normal, expexts precisely three floats
			for (UInt i = 0; i < 3; i++)
			{
				ULong end = vert.find(" ", start) + 1;
				normals.push_back(std::stof(vert.substr(start, start - end)));
				start = end;
			}

			continue;
		}
		else if (command == "f")
		{
			ULong newPos = 0;

			for (Byte i = 0; i < 3; i++) //loop whitespace
			{
				pos = line.find_first_of(" ", pos); //find next ' ' character

				for (Byte j = 0; j < 3; j++) //loop slashes
				{
					newPos = line.find_first_of("/", pos); //find next '/' character
					std::string sub = line.substr(pos + 1, newPos);

					meshes[meshGroups - 1].push_back(std::stoul(sub));

					pos = newPos;
				}
			}
			continue;
		}
		else if (command == "usemtl")
		{
			materials[meshGroups] = ContentManager<Material>::Get().GetContent(line.substr(pos + 1));

			//use material
			meshGroups++;
		}
		else if (command == "#")
		{
			//comment
			continue;
		}
		else
		{
			//unknown
			LogWarning("Model", "Unknown command while passing file: " + command);
		}
	}
}

const std::string& Model::GetContentName() const
{
	static const std::string name("model");
	return name;
}