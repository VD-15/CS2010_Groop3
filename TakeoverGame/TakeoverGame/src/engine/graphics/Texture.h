#pragma once

#include "TextureAtlas.h"
#include "../core/ValkyrieEngine.h"
#include "../utils/Vector.h"

namespace vlk
{
	class Texture2D final : public NonClonable
	{
		public:
		Texture2D(const TextureAtlas* atlas, Pair<Float> location, Pair<Float> size, Pair<Float> origin);

		const Vector2 location;
		const Vector2 size;
		const Vector2 origin;

		/*
		* All of these are relative to the texture atlas the texture is a part of.
		* Values should be within the range of 0.0f to 1.0f inclusive
		*/

		const Vector2 topLeft;
		const Vector2 topRight;
		const Vector2 bottomLeft;
		const Vector2 bottomRight;

		const TextureAtlas* const atlas;
	};
}