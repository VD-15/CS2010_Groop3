#pragma once

#include "ValkyrieEngine.h"
#include "Content.h"
#include "../core/EventBus.hpp"

#include "pugixml/pugixml.hpp"

#include <type_traits>

namespace vlk
{
	template <class T>
	struct ContentLoadedEvent : public Event
	{
		ContentLoadedEvent<T>(const T* _content, const std::string& _name) :
			content(_content),
			name(_name)
		{

		}

		const T* content;
		const std::string& name;
	};

	template <class T>
	struct ContentUnloadedEvent : public Event
	{
		ContentUnloadedEvent<T>(const T* _content, const std::string& _name) :
			content(_content),
			name(_name)
		{

		}

		const T* content;
		const std::string& name;
	};

	namespace ContentSystem
	{
		void Init();
		void Destroy();

		pugi::xml_node GetRootNode();
	};

	template<class T>
	class ContentManager
	{
		ContentManager<T>()
		{
			//Ensure T inherits from Content & has a default constructor
			static_assert(std::is_base_of<Content, T>::value, "T must inherit from Content");
			static_assert(std::is_constructible<T>::value, "T must have a void constructor");
		}

		std::map<std::string, T*> content;

		public:
		static ContentManager<T>& Get()
		{
			static ContentManager<T> instance;
			return std::ref<ContentManager<T>>(instance);
		}

		ContentManager<T>(const ContentManager<T>&) = delete;
		ContentManager<T>(ContentManager<T>&&) = delete;
		ContentManager<T>& operator=(const ContentManager<T>&) = delete;

		const T* LoadContent(const std::string& name)
		{
			if (name.empty())
			{
				LogError("ContentManager " + TypeToString<T>(), "Attempted to load content with no name.");
				throw std::exception("Attempted to load content with no name.");
				return nullptr;
			}
			else if (content.find(name) != content.end())
			{
				LogError("ContentManager " + TypeToString<T>(), "Content was either already loaded or has conflicting names: " + name);
				throw std::exception("Attempted to load content with conflicting names.");
				return nullptr;
			}

			T* t = new T();

			const std::string& contentName(t->GetContentName());

			//Find path in content doc
			for (pugi::xml_node node = ContentSystem::GetRootNode().child(contentName.c_str()); node; node = node.next_sibling(contentName.c_str()))
			{
				//Check if the names match
				if (!strcmp(node.attribute("name").value(), name.c_str()))
				{
					const char* path = node.attribute("path").value();

					t->LoadContent(path, name);

					//Add meta tags
					for (pugi::xml_node att = node.first_child(); att; att = att.next_sibling())
					{
						t->AddMetaTag(att.name(), att.text().as_string());
					}

					content.emplace(name, t);

					EventBus<ContentLoadedEvent<T>>::Get().PostEvent({ t, name });

					return t;
				}
			}

			LogError("ContentManager " + TypeToString<T>(), "Failed to load content: " + name);
			throw std::exception("Failed to load content");
			delete t;
			return nullptr;
		}

		void AddContent(const std::string& name, T* content)
		{
			this->content.emplace(name, content);
		}

		const T* GetContent(const std::string& name) const
		{
			auto it = content.find(name);

			if (it != content.end())
			{
				return it->second;
			}
			else
			{
				LogError("ContentManager " + TypeToString<T>(), "Failed to find content with name: " + name);
				throw std::exception("Failed to find content.");
				return nullptr;
			}
		}

		void UnloadContent(const std::string& name)
		{
			auto it = content.find(name);

			if (it != content.end())
			{
				T* t = it->second;

				EventBus<ContentUnloadedEvent<T>>::Get().PostEvent({ t, name });

				content.erase(it);
				delete t;
			}
			else
			{
				LogError("ContentManager " + TypeToString<T>(), "Failed to remove content with name: " + name);
			}
		}
	};
}
