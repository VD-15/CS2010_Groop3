#pragma once

#include "../core/Content.h"
#include "../graphics/Texture.h"
#include "../utils/Color.h"

#include <vector>

namespace vlk
{
	struct Material
	{
		Material();

		Color ambient;					//Ka
		Color diffuse;					//Kd

		Color specular;					//Ks
		Float exponent;					//Ns

		Float transparency;				//d / Tr

		const Texture2D* diffuseMap;	//map_Kd

		const Texture2D* specularMap;	//map_Ks
		const Texture2D* exponentMap;	//map_Ns

		const Texture2D* alphaMap;		//map_d / map_Tr
	};

	class MaterialLibrary : public Content
	{
		public:
		MaterialLibrary();
		~MaterialLibrary();

		void LoadContent(const std::string& path) final override;
		const std::string& GetContentName() const final override;

		const Material* GetMaterial(const std::string& name) const;

		private:
		std::map<std::string, Material*> materials;
	};
}