#pragma once

#include "../../core/ValkyrieEngine.h"

namespace vlk
{
	struct GLBuffer
	{
		GLBuffer();
		~GLBuffer();

		inline UInt Handle() const { return handle; }
		inline UInt Size() const { return size; }

		void Resize(vlk::Size newSize, UInt usage, const Byte* data = nullptr);
		void Fill(const Byte* data, vlk::Size length);
		void Bind(UInt target);
		void Unbind();

		private:
		const UInt handle;
		UInt size;
		UInt target;
	};

}