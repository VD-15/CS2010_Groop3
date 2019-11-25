#pragma once

#include "../core/Content.h"
#include "../graphics/Texture.h"
#include "../utils/Color.h"

#include <vector>

namespace vlk
{
	struct Material : public Content
	{
		Material();

		void LoadContent(const std::string& path, const std::string& name) final override;
		const std::string& GetContentName() const final override;

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
}