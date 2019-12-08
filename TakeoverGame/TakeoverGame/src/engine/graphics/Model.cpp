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
			numFaces[meshGroups - 1]++;
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
	this->meshes.reserve(meshGroups);

	for (auto it = numFaces.begin(); it != numFaces.end(); it++)
	{
		//this assumes all faces will be triangular and have three elements: position, normal and UV
		this->meshes.push_back(std::vector<UInt>());
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
			std::string vert(line.substr(pos + 1));

			ULong split1 = vert.find_first_of(" ");
			ULong split2 = vert.find_last_of(" ");

			std::string verts[3]
			{
				vert.substr(0, split1),							//sub between start and first split
				vert.substr(split1 + 1, (split2 - 1) - split1),	//sub between both splits
				vert.substr(split2 + 1)							//sub between second split and end
			};

			for (Byte i = 0; i < 3; i++)
			{
				split1 = verts[i].find_first_of("/");
				split2 = verts[i].find_last_of("/");

				UInt iVert = std::stoi(verts[i].substr(0, split1));
				UInt iText = std::stoi(verts[i].substr(split1 + 1, (split2 - 1) - split1));
				UInt iNorm = std::stoi(verts[i].substr(split2 + 1));

				this->meshes[meshGroups - 1].push_back(iVert);
				this->meshes[meshGroups - 1].push_back(iText);
				this->meshes[meshGroups - 1].push_back(iNorm);

				((void)0);
			}

			continue;
		}
		else if (command == "usemtl")
		{
			materials.push_back(ContentManager<Material>::Get().GetContent(line.substr(pos + 1)));

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