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

		void LoadContent(const std::string& path) final override;
		const std::string& GetContentName() const final override;

		private:
		const MaterialLibrary* lib;
		std::vector<Float> vertices;
		std::vector<Float> coords;
		std::vector<Float> normals;
		std::map<const Material*, std::vector<UInt>> faces;
	};
}