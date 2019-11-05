#include "Texture.h"

using namespace vlk;

Texture2D::Texture2D(const TextureAtlas* atlas, Pair<Float> location, Pair<Float> size, Pair<Float> origin) :
	location(	location.x	/ static_cast<Float>(atlas->Width()), location.y	/ static_cast<Float>(atlas->Height())),
	size(		size.x		/ static_cast<Float>(atlas->Width()), size.y		/ static_cast<Float>(atlas->Height())),
	origin(		origin.x	/ static_cast<Float>(atlas->Width()), origin.y		/ static_cast<Float>(atlas->Height())),
	topLeft(	(location.x)			/ static_cast<Float>(atlas->Width()),	(location.y)			/ static_cast<Float>(atlas->Height())),
	topRight(	(location.x + size.x)	/ static_cast<Float>(atlas->Width()),	(location.y)			/ static_cast<Float>(atlas->Height())),
	bottomRight((location.x + size.x)	/ static_cast<Float>(atlas->Width()),	(location.y + size.y)	/ static_cast<Float>(atlas->Height())),
	bottomLeft(	(location.x)			/ static_cast<Float>(atlas->Width()),	(location.y + size.y)	/ static_cast<Float>(atlas->Height())),
	atlas(atlas)
{

}