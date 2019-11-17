#pragma once

#include "../core/Content.h"

namespace vlk
{
	/*
	Valid meta tags:
	[wrap_mode]	repeat, mirror, clamp, border (default: border)
	[mipmap] true, false (default: true)
	[filter_mode] linear, nearest (default: linear) (overrides to linear if mipmap is true)
	*/
	class Texture2D : public Content
	{
		public:
		Texture2D();
		Texture2D(const Byte* data, ULong size, Int width, Int height);
		~Texture2D();

		inline const Byte* const Data() const { return data; }
		inline const Int Width() const { return width; }
		inline const Int Height() const { return height; }

		void LoadContent(const std::string& path, const std::string& name) override;
		const std::string& GetContentName() const override;

		private:
		Byte* data;
		Int width;
		Int height;
		Boolean isStbBacked;
	};
}