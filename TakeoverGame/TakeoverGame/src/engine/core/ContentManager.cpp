#include "ContentManager.hpp"

#include <unordered_map>
#include <fstream>

#include "utf8/utf8.h"

#include "EventBus.hpp"
#include "../utils/VLKIO.h"
#include "../graphics/Texture.h"

using namespace vlk;

namespace
{
	pugi::xml_document contentDoc;
	Texture2D* DEFAULT_TEXTURE;

	void OnApplicationStart(ApplicationStartEvent& ev)
	{
	}

	void OnApplicationExit(ApplicationExitEvent& ev)
	{
	}
}

void ContentSystem::Init()
{
	pugi::xml_parse_result result = contentDoc.load_file("Content.xml");

	if (result.status != pugi::xml_parse_status::status_ok)
	{
		LogError("ContentManager", "Failed to open content file: " + std::string(result.description()));
		throw std::exception("Failed to open content file.");
	}

	//init default content:
	Byte data[4] { 255, 255, 255, 255 };
	DEFAULT_TEXTURE = new Texture2D(data, 4, 1, 1);
	DEFAULT_TEXTURE->AddMetaTag("wrap_mode", "repeat");
	ContentManager<Texture2D>::Get().AddContent("VLK_DEFAULT_TEXTURE", DEFAULT_TEXTURE);
}

void ContentSystem::Destroy()
{
	//Destroy default content
	ContentManager<Texture2D>::Get().UnloadContent("VLK_DEFAULT_TEXTURE");
	DEFAULT_TEXTURE = nullptr;

	contentDoc.reset();
}

pugi::xml_node ContentSystem::GetRootNode()
{
	return contentDoc.first_child();
}

/*
void ContentManager::SetContentDirectory(const std::string& path)
{
	if (!contentDoc.empty())
	{
		LogError("ContentManager", "Attempted to change content directory once already open. This must be set before ContentManager::Init()");
		throw std::exception("Attempted to change content directory once already open.");
	}
	else
	{
		if (!vlk::StringEndsWith(path, "/"))
		{
			LogWarning("ContentManager", "Path does not end with a trailing '/', loading content may fail.");
		}

		contentDir = path;
	}
}

template<>
const TextureAtlas* ContentManager::LoadContent<TextureAtlas>(const std::string& name)
{
	LogInfo("ContentManager " + TypeToString<TextureAtlas>(), "Loading content: " + name);

	//Check if conflicting texture is loaded
	if (name.empty())
	{
		LogError("ContentManager " + TypeToString<TextureAtlas>(), "Attempted to load content with no name.");
		throw std::exception("Attempted to load content with no name.");
		return nullptr;
	}
	else if (atlases.find(name) != atlases.end())
	{
		LogError("ContentManager " + TypeToString<TextureAtlas>(), "Content was either already loaded or has conflicting names: " + name);
		throw std::exception("Attempted to load content with conflicting names.");
		return nullptr;
	}
	else if (textures.find(name) != textures.end())
	{
		LogError("ContentManager " + TypeToString<Texture2D>(), "Content has conflicting names: " + name);
		throw std::exception("Attempted to load content with conflicting names.");
		return nullptr;
	}

	pugi::xml_node atlas = FindNodeByName(name, "texture_atlas");

	if (!atlas)
	{
		LogError("ContentManager " + TypeToString<TextureAtlas>(), "Failed to load texture atlas with name: " + name);
		throw std::exception("Failed to load content.");
		return nullptr;
	}

	//Get path to content
	const char* path = atlas.child("path").text().as_string();

	//Construct atlas and load texture
	TextureAtlas* ta = new TextureAtlas();
	Boolean loadResult = ta->LoadFromFile(contentDir + path);

	//Check for load failure
	if (!loadResult)
	{
		LogError("ContentManager" + TypeToString<TextureAtlas>(), "Failed to load texture atlas at: " + std::string(path));
		throw std::exception("Failed to load content.");
		delete ta;
		return nullptr;
	}

	//Add to map
	atlases.insert({ name, ta });

	//Send texture atlas loaded event
	EventBus<TextureAtlasLoadedEvent>::Get().PostEvent({ ta, name });

	Int width = ta->Width();
	Int height = ta->Height();

	//Make default texture
	Texture2D* def = new Texture2D(ta, { 0.0f, 0.0f }, { static_cast<Float>(width), static_cast<Float>(height) }, { width / 2.0f, height / 2.0f });
	textures.insert({ name, def });

	//Send texture loaded event
	EventBus<Texture2DLoadedEvent>::Get().PostEvent({ def, name });

	//Make child textures
	for (pugi::xml_node child = atlas.child("texture"); child; child = child.next_sibling("texture"))
	{
		constexpr Float defValue = std::numeric_limits<Float>::min();

		std::string texName(child.attribute("name").as_string());

		if (texName.empty())
		{
			LogWarning("ContentManager" + TypeToString<Texture2D>(), "Child texture for {" + name + "} was found, but did not have a name.");
			continue;
		}
		else if (textures.find(texName) != textures.end())
		{
			LogError("ContentManager" + TypeToString<Texture2D>(), "Attempted to load textures with conflicting names: " + texName);
			continue;
		}

		Float xVal = child.child("x").text().as_float(0.0f);
		Float yVal = child.child("y").text().as_float(0.0f);
		Float wVal = child.child("width").text().as_float(0.0f);
		Float hVal = child.child("height").text().as_float(0.0f);
		Float oxVal = child.child("origin_x").text().as_float(defValue);
		Float oyVal = child.child("origin_y").text().as_float(defValue);
		if (oxVal == defValue) oxVal = wVal / 2.0f;
		if (oyVal == defValue) oyVal = hVal / 2.0f;

		Texture2D* chi = new Texture2D(ta, { xVal, yVal }, { wVal, hVal }, { oxVal, oyVal });
		textures.insert({ texName, chi });

		//Send texture loaded event
		EventBus<Texture2DLoadedEvent>::Get().PostEvent({ chi, texName });
	}

	return ta;
}

template<>
const StringCollection* ContentManager::LoadContent<StringCollection>(const std::string& name)
{
	typedef StringCollection T;

	LogInfo("ContentManager " + TypeToString<T>(), "Loading content: " + name);

	//Check if conflicting texture is loaded
	if (name.empty())
	{
		LogError("ContentManager " + TypeToString<T>(), "Attempted to load content with no name.");
		throw std::exception("Attempted to load content with no name.");
		return nullptr;
	}
	else if (strings.find(name) != strings.end())
	{
		LogError("ContentManager " + TypeToString<T>(), "Content was already loaded or has conflicting names: " + name);
		throw std::exception("Attempted to load content with conflicting names.");
		return nullptr;
	}

	pugi::xml_node string = FindNodeByName(name, "string_collection");

	if (!string)
	{
		LogError("ContentManager " + TypeToString<T>(), "Failed to find content with name: " + name);
		throw std::exception("Failed to find content.");
		return nullptr;
	}

	//Get path to content
	const char* path = string.child("path").text().as_string();

	//Construct and load string collection
	StringCollection* sc = new StringCollection(IO::ReadFile(path));

	//Add to map
	strings.insert({ name, sc });

	//Send loaded event
	EventBus<StringCollectionLoadedEvent>::Get().PostEvent({ sc, name });

	return sc;

}

template<>
const XmlDocument* ContentManager::LoadContent<XmlDocument>(const std::string& name)
{
	typedef XmlDocument T;

	LogInfo("ContentManager " + TypeToString<T>(), "Loading content: " + name);

	if (name.empty())
	{
		LogError("ContentManager " +  TypeToString<T>(), "Attempted to load content with no name.");
		throw std::exception("Attempted to load content with no name.");
	}
	else if (xmls.find(name) != xmls.end())
	{
		LogError("ContentManager " + TypeToString<T>(), "Content was already loaded or has conflicting names: " + name);
		throw std::exception("Attempted to load content with conflicting names.");
		return nullptr;
	}

	pugi::xml_node xml = FindNodeByName(name, "xml_document");

	if (!xml)
	{
		LogError("ContentManager " + TypeToString<T>(), "Failed to find content with name: " + name);
		throw std::exception("Failed to find content.");
		return nullptr;
	}

	//Get path to content
	const char* path = xml.child("path").text().as_string();

	//Construct and load string collection
	XmlDocument* doc = new XmlDocument();
	pugi::xml_parse_result result = doc->load_file((contentDir + path).c_str());

	if (!result)
	{
		LogError("ContentManager " + TypeToString<T>(), "PugiXML failed to load XML document: ");
		throw std::exception("Failed to load XML document.");
		return nullptr;
	}

	//Add to map
	xmls.insert({ name, doc });

	EventBus<XMLDocumentLoadedEvent>::Get().PostEvent({ doc, name });

	return doc;
}

template<>
const SpriteFont* ContentManager::LoadContent<SpriteFont>(const std::string& name)
{
	typedef SpriteFont T;

	LogInfo("ContentManager " + TypeToString<T>(), "Loading content: " + name);

	if (name.empty())
	{
		LogError("ContentManager " +  TypeToString<T>(), "Attempted to load content with no name.");
		throw std::exception("Attempted to load content with no name.");
	}
	else if (fonts.find(name) != fonts.end())
	{
		LogError("ContentManager " + TypeToString<T>(), "Content was already loaded or has conflicting names: " + name);
		throw std::exception("Attempted to load content with conflicting names.");
		return nullptr;
	}

	pugi::xml_node node = FindNodeByName(name, "sprite_font");

	if (!node)
	{
		LogError("ContentManager " + TypeToString<T>(), "Failed to find content with name: " + name);
		throw std::exception("Failed to find content.");
		return nullptr;
	}

	//Get path to content
	const char* path = node.child("path").text().as_string();

	SpriteFont* sf = new SpriteFont();
	Boolean loadResult = sf->LoadFromFile(contentDir + path);

	if (!loadResult)
	{
		LogError("ContentManager " + TypeToString<T>(), "Failed to load spritefont texture");
		throw std::exception("Failed to load content");

		delete sf;

		return nullptr;
	}

	fonts.insert({ name, sf });

	//Send spriteFont loaded event
	EventBus<SpriteFontLoadedEvent>::Get().PostEvent({ sf, name });

	//Send texture atlas loaded event
	EventBus<TextureAtlasLoadedEvent>::Get().PostEvent({ dynamic_cast<TextureAtlas*>(sf), name });

	//Make child textures
	for (pugi::xml_node child = node.child("char"); child; child = child.next_sibling("char"))
	{
		constexpr Float defValue = std::numeric_limits<Float>::max();

		UInt encoding = child.child("encoding").text().as_uint();
		
		Float xVal = child.child("x").text().as_float(0.0f);
		Float yVal = child.child("y").text().as_float(0.0f);
		Float wVal = child.child("width").text().as_float(0.0f);
		Float hVal = child.child("height").text().as_float(0.0f);
		Float oxVal = child.child("origin_x").text().as_float(0.0f);
		Float oyVal = child.child("origin_y").text().as_float(hVal);

		Texture2D* chi = new Texture2D(dynamic_cast<TextureAtlas*>(sf), { xVal, yVal }, { wVal, hVal }, { oxVal, oyVal });
		sf->AddCharEncoding(encoding, chi);
	}

	UInt defaultChar = node.child("default_char").text().as_uint(0);

	sf->SetDefaultChar(defaultChar);

	return sf;
}

template<>
const Texture2D* ContentManager::GetContent<Texture2D>(const std::string& name)
{
	auto it = textures.find(name);

	if (it != textures.end())
	{
		return it->second;
	}
	else
	{
		LogError("ContentManager " + TypeToString<Texture2D>(), "Failed to find content with name: " + name);
		throw std::exception("Failed to find content.");
		return nullptr;
	}
}

template<>
const StringCollection* ContentManager::GetContent<StringCollection>(const std::string& name)
{
	auto it = strings.find(name);

	if (it != strings.end())
	{
		return it->second;
	}
	else
	{
		LogError("ContentManager " + TypeToString<StringCollection>(), "Failed to find content with name: " + name);
		throw std::exception("Failed to find content.");
		return nullptr;
	}
}

template<>
const XmlDocument* ContentManager::GetContent<XmlDocument>(const std::string& name)
{
	auto it = xmls.find(name);

	if (it != xmls.end())
	{
		return it->second;
	}
	else
	{
		LogError("ContentManager " + TypeToString<XmlDocument>(), "Failed to find content with name: " + name);
		throw std::exception("Failed to find content.");
		return nullptr;
	}
}

template<>
const SpriteFont* ContentManager::GetContent<SpriteFont>(const std::string& name)
{
	auto it = fonts.find(name);

	if (it != fonts.end())
	{
		return it->second;
	}
	else
	{
		LogError("ContentManager " + TypeToString<SpriteFont>(), "Failed to find content with name: " + name);
		throw std::exception("Failed to find content");
		return nullptr;
	}
}

template<>
void ContentManager::UnloadContent<TextureAtlas>(const std::string& name)
{
	LogInfo("ContentManager " + TypeToString<TextureAtlas>(), "Unloading content: " + name);

	//Unload child textures
	for (pugi::xml_node atlas = contentDoc.first_child().child("texture_atlas"); atlas; atlas = atlas.next_sibling("texture_atlas"))
	{
		//Find texture atlas with matching name
		if (!strcmp(atlas.attribute("name").value(), name.c_str()))
		{
			//Find child textures
			for (pugi::xml_node child = atlas.child("texture"); child; child = child.next_sibling("texture"))
			{
				const char* childName = child.attribute("name").as_string();
				auto it = textures.find(childName);

				if (it != textures.end())
				{
					Texture2D* def = it->second;

					//Post texture unloaded event
					{
						Texture2DUnloadedEvent ev { def, name };
						EventBus<Texture2DUnloadedEvent>::Get().PostEvent(ev);
					}

					//Delete texture
					textures.erase(it);
					delete def;
				}
				else
				{
					LogError("ContentManager " + TypeToString<TextureAtlas>(), "Failed to remove texture {" + name + "} for texture atlas: " + name);
				}
			}
		}
	}

	//Unload default texture
	{
		auto it = textures.find(name);

		if (it != textures.end())
		{
			Texture2D* def = it->second;

			//Post texture unloaded event
			{
				Texture2DUnloadedEvent ev { def, name };
				EventBus<Texture2DUnloadedEvent>::Get().PostEvent(ev);
			}

			//Delete texture
			textures.erase(it);
			delete def;
		}
		else
		{
			LogError("ContentManager " + TypeToString<TextureAtlas>(), "Failed to remove default texture for texture atlas: " + name);
		}
	}

	//Unload atlas
	{
		auto it = atlases.find(name);

		if (it != atlases.end())
		{
			TextureAtlas* tex = it->second;

			EventBus<TextureAtlasUnloadedEvent>::Get().PostEvent({tex, name});

			//Delete atlas
			atlases.erase(it);
			delete tex;
		}
		else
		{
			LogError("ContentManager " + TypeToString<TextureAtlas>(), "Failed to remove texture atlas with name: " + name);
		}
	}
}

template<>
void ContentManager::UnloadContent<StringCollection>(const std::string& name)
{
	LogInfo("ContentManager " + TypeToString<StringCollection>(), "Unloading content: " + name);

	auto it = strings.find(name);

	if (it != strings.end())
	{
		StringCollection* sc = it->second;

		//Post unloaded event
		EventBus<StringCollectionUnloadedEvent>::Get().PostEvent({ sc, name });

		//Delete string collection
		strings.erase(it);
		delete sc;
	}
	else
	{
		LogError("ContentManager " + TypeToString<StringCollection>(), "Failed to remove content with name: " + name);
	}
}

template<>
void ContentManager::UnloadContent<XmlDocument>(const std::string& name)
{
	LogInfo("ContentManager " + TypeToString<XmlDocument>(), "Unloading content: " + name);

	auto it = xmls.find(name);

	if (it != xmls.end())
	{
		XmlDocument* xml = it->second;

		EventBus<XMLDocumentUnloadedEvent>::Get().PostEvent({ xml, name });

		xmls.erase(it);
		delete xml;
	}
	else
	{
		LogError("ContentManager " + TypeToString<XmlDocument>(), "Failed to remove content with name: " + name);
	}
}

template<>
void ContentManager::UnloadContent<SpriteFont>(const std::string& name)
{
	LogInfo("ContentManager " + TypeToString<SpriteFont>(), "Unloading content: " + name);

	auto it = fonts.find(name);

	if (it != fonts.end())
	{
		SpriteFont* sf = it->second;

		EventBus<SpriteFontUnloadedEvent>::Get().PostEvent({ sf, name });

		fonts.erase(it);
		delete sf;
	}
	else
	{
		LogError("ContentManager " + TypeToString<SpriteFont>(), "Failed to remove content with name: " + name);
	}
}
*/