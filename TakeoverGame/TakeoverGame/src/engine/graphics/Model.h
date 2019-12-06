#pragma once

#include "../core/Content.h"
#include "Material.h"

namespace vlk
{
	class Model : public Content
	{
		public:
		Model();
		~Model();

		void LoadContent(const std::string& path, const std::string& name) final override;
		const std::string& GetContentName() const final override;

		inline const std::vector<Float>& GetVertices() const { return vertices; }
		inline const std::vector<Float>& GetCoords() const { return coords; }
		inline const std::vector<Float>& GetNormals() const { return normals; }

		inline const std::vector<std::vector<UInt>>& GetMeshes() const { return meshes; }
		inline const std::vector<const Material*>& GetMaterials() const { return materials; }

		private:
		std::vector<Float> vertices;
		std::vector<Float> coords;
		std::vector<Float> normals;

		//TODO: separate materials from meshes
		std::vector<std::vector<UInt>> meshes;
		std::vector<const Material*> materials;
	};
}