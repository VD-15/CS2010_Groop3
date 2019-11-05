#pragma once

#include "ValkyrieEngine.h"
#include "../graphics/Texture.h"
#include "../graphics/SpriteFont.h"
#include "../utils/StringCollection.h"
#include "../core/EventBus.hpp"

#include "pugixml/pugixml.hpp"

namespace vlk
{
	typedef pugi::xml_document XmlDocument;

	struct TextureAtlasLoadedEvent : public Event
	{ 
		TextureAtlasLoadedEvent(const TextureAtlas* atlas, const std::string& name);

		const TextureAtlas* const atlas; 
		const std::string& name; 
	};

	struct TextureAtlasUnloadedEvent : public Event
	{ 
		TextureAtlasUnloadedEvent(const TextureAtlas* atlas, const std::string& name);

		const TextureAtlas* const atlas; 
		const std::string& name; 
	};

	struct Texture2DLoadedEvent : public Event
	{
		Texture2DLoadedEvent(const Texture2D* texture, const std::string& name);

		const Texture2D* const texture; 
		const std::string& name;
	};

	struct Texture2DUnloadedEvent : public Event
	{
		Texture2DUnloadedEvent(const Texture2D* texture, const std::string& name);

		const Texture2D* const texture; 
		const std::string& name; 
	};

	struct StringCollectionLoadedEvent : public Event
	{ 
		StringCollectionLoadedEvent(const StringCollection* collection, const std::string& name);

		const StringCollection* const collection; 
		const std::string& name; 
	};

	struct StringCollectionUnloadedEvent : public Event
	{
		StringCollectionUnloadedEvent(const StringCollection* collection, const std::string& name);

		const StringCollection* const collection; 
		const std::string& name; 
	};

	struct XMLDocumentLoadedEvent : public Event
	{ 
		XMLDocumentLoadedEvent(const XmlDocument* document, const std::string& name);

		const XmlDocument* const document; 
		const std::string& name; 
	};

	struct XMLDocumentUnloadedEvent : public Event
	{ 
		XMLDocumentUnloadedEvent(const XmlDocument* document, const std::string& name);

		const XmlDocument* const document; 
		const std::string& name; 
	};

	struct SpriteFontLoadedEvent : public Event
	{ 
		SpriteFontLoadedEvent(const SpriteFont* font, const std::string& name);

		const SpriteFont* const font; 
		const std::string& name; 
	};

	struct SpriteFontUnloadedEvent : public Event
	{ 
		SpriteFontUnloadedEvent(const SpriteFont* font, const std::string& name);

		const SpriteFont* const font; 
		const std::string& name; 
	};

	namespace ContentManager
	{
		void Init();
		void Destroy();

		void SetContentDirectory(const std::string& path);

		//TODO: Remove LoadContent<XMLDocument>
		//TODO: Add LoadContent<std::string>

		//Loads content from Content.xml; Valid for:
		//TextureAtlas
		//StringCollection
		//XmlDocument
		template<class T>
		const T* LoadContent(const std::string& name);

		//Retrieves loaded content. Valid for:
		//Texture2D
		//StringCollection
		//XmlDocument
		template<class T>
		const T* GetContent(const std::string& name);

		//Unloads content. Valid for:
		//TextureAtlas
		//StringCollection
		//XmlDocument
		template<class T>
		void UnloadContent(const std::string& name);
	}
}
