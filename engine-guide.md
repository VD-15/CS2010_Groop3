# Readable documentation for the engine:

The engine we're using is Valkyrie Engine, this was developed by me over the past half a year. It's still a but rough in places, so I ask you to bear with it and tell me about any potential engine-side bugs you encounter.

This guide assumes you've already read through the C++ guide I wrote. If you haven't done that, do that now. Don't be afraid to ask me any questions you have.

## A Quick Note on Namespaces:

The `tkv` namespace should be used for things relating to the game, the `vlk` namespace is reserved for things related to the engine. In order to access engine classes from within the game, you can type `using namespace vlk` to avoi putting `vlk::` before everything

## What is an Entity Component System?

An entity component system (ECS) is a system paradigm commonly used in game engines, where entities are modelled as a collection of individual components. Contrary to the name, in an ECS, entities do not have any behavior or data unique to themselves, rather their behavior, properties and logic are split up into small classes called components. These components, when combined, form what we view as an entity.

An ECS has the advantage of being able to reuse sections of code with relative ease as well as having naturally low coupling and high cohesion (I hope you remembered what that meant from last year). This can significantly reduce the amount of work required on the 'game' side of things. Consider for example a basic game with a player entity and an enemy entity. Now, let's say that both of these entities have the ability to 'walk' in some capacity. You could create a simple walker component to model this behavior and apply it to both the enemy and the player at the same time. The only thing that would need to change is where they're walking and that could be handled by a seperate component, but hey, ho, we've saved ourselves duplicated code! So how does this apply to our entity component system?

## Entities:

All entities in Valkyrie Engine must inherit from a generic class called `Entity<T>` that is located in `engine/core/GameEntity.hpp`. Now, `Entity<T>` is a generic class, so what do you put as the type parameter? You put the class that inheriting from it. This is a design pattern known as the Curiously Recurring Template Pattern or CRTP. This idiom originated in C++, though the behaviour it refers to is present in most object-oriented languages. Basically this means that the derrived entity is a `Entity<T>` but `Entity<T>` also utilizes our derrived component. This is to distinguish different types of entity from one another for the memory allocator.

`Entity` defines an abstract function called `OnDelete()` that must be overriden; this is where your components will be deleted. Any components belonging to an entity should be created in the constructor and destroyed in `OnDelete()`. Because pointers to components are stored in a seperate allocator on the heap, they will not be removed once the entity is deleted; hence the existence of `Delete()`. Below is an example of a basic entity:

```cpp
//TestEntity2.h

struct TestEntity2 : public Entity<TestEntity2>
{
	TestEntity2();
	void OnDelete() override;
    
	TransformComponent2D* transform;
}

//TestEntity2.cpp

TestEntity2::TestEntity2()
{
	this->transform = CreateComponent<TransformComponent2D>();
}

void TestEntity2::Delete()
{
	this->transform->Delete();
}
```

`Entity` defines a generic member function called `CreateComponent` that can be used to create a component with the current entity.

Normally in an ECS, entities aren't supposed to have any sort of data associated with them, so storing references to what components an entity owns is *technically* breaking paradigm, however the performance gain recieved in not having to constantly search for components to delete is in my opinion perfectally justifiable. There are also some cases where you may want to add a component to an entity midway through it's lifetime. This is perfectally acceptable, just make damn well sure that the component you add is deleted by the time the entity is or you **will** get an access violation.

To create an entity, you can call the `Entity<T>::CreateEntity(...)` function as oppose to just constructing it. This function will pass whatever parameters you put in the braces into the constructor for your entity, so put whatever parameters you would normally put in the entity's constructor in there.

## Components:

Now for components: components encapsulate all the data associated with a particular aspect of an entity. Components can have primitive types, objects and even pointers to other components. In Valkyrie Engine, all components inherit from a generic class called `Component<T>` that is located in `engine/core/GameComponent.hpp` at the time of writing. Similar to `Entity<T>`, it utilizes CRTP for the purpose of memory allocation. `Component<T>` also defines an `OnDelete()` function similar to `Entity`. this is used in cases where a component creates other components under it's exclusive control or other forms of data on the heap that require explicit deletion.

The constructor of `Component<T>` requires a pointer to an `IEntity`, as such, it's constructor must be specified in the initializer list of any component. Additionally, every component's constructor **must** have a pointer to an `IEntity` as the first parameter. The `CreateComponent()` member function in `Entity<T>` assumes this will be the case when constructing a component. If this is not the case and, the program will likely not compile. Below is an example of a component class that maintains control of another DrawTexture2D component:

```cpp
//TestComponent2.h

struct TestComponent2 : public Component<TestComponent2>
{
    TestComponent2(IEntity* e, const TransformComponent2D* transform);
	void OnDelete();

	const TransformComponent2D* transform;
	DrawTextureComponent2D* draw;
}

//TestComponent2.cpp

TestComponent2::TestComponent2(IEntity* e, TransformComponent2D* transform) :
	Component<TestComponent2>(e)
{
	this->transform = transform;
	this->draw = DrawTextureComponent2D::CreateComponent(e, transform, /*texture here*/)
}

void TestComponent2::OnDelete()
{
	this->Draw->Delete();
}
```

There are two ways to create components: either from an entity with `Entity<T>.CreateComponent<U>(...)` or with `Component<T>::CreateComponent(...)`. The two both pass whatever parameters you place in the brackets into the constructor of the component. The only difference is that the former will automatically pass the `IEntity*` of the entity you call it from as the first parameter to the constructor, whereas the latter does not:

```cpp
//From inside an Entity<T>:
this->CreateComponent<TestComponent2>(transform); //Will pass IEntity* automatically

//From outside an Entity<T>:
TestComponent2::CreateComponent(entity, transform); //Need to pass IEntity* manually
```

So how do we give our components behavior? The answer to that is the event bus:

## The Event Bus

All of the signaling in our engine is done through a class called `EventBus<T>` located in `engine/core/EventBus.hpp`. This is another powerful paradigm that further eliminates coupling and promotes cohesion. Instead of modules of the engine directly sending signals to each-other, resulting in enough code spaghetti to make even the hungriest of Italians flinch, modules send and listen to events through the event bus. 

## Events

An event is nothing more than a class that inherits from `Event` also located in `engine/core/EventBus.cpp`. Events can be empty, or contain data but they don't normally have functions. Events are your primary method of sending data to other modules. In order to make an event, you need only to inherit from `Event` and you're ready to be sent. Below you can find an example of a basic class:

```cpp
//MyEvent.h

struct MyEvent : public Event
{
    MyEvent(ULong _value, Float _time);

	const ULong value;
	Float time;
}

//MyEvent.cpp

MyEvent::MyEvent(ULong _value, Float _time) :
	value(value)
{
	this->time = _time
}
```

## Adding Event Listeners

To recieve an event, you must first add an event listener. This is simply a function or anything that can take a reference to the event you're listening to. You can then subscribe this listener to the event bus by finding the event bus associated with your event and adding the function. As a general rule of thumb, subscribing and removing event listeners ahould be done only once, at the beginning and end of the program respectively. Subscribing an event listener while an event is being sent **will** result in a crash. This can be done in the form of static `Init()` and `Destroy()` methods that are called before the game starts and after it ends:

```cpp
//MyExample.h

namespace MySystem
{
	void Init();
	void Destroy();
}

//MyExample.cpp

namespace
{
	//You can define this function anywhere, I just prefer to do it like this
	void OnMyEvent(MyEvent& ev)
	{
		//Do stuff
	}
}

void MySystem::Init();
{
    EventBus<MyEvent>::Get().AddEventListener(OnMyEvent);
}

void MySystem::Destroy();
{
    EventBus<MyEvent>::Get().RemoveEventListener(OnMyEvents);
}
```

## Posting Events

Once you've set up your event and event listeners, you're free to post an event. This will signal all event listeners that are subscribed to the event bus and send them the event you provided. The state of the event may be modified by it's listeners, so you can use it to poll for some data should you need to. Additionally, sending of an event may be cancelled at any time by calling the `Cancel()` method provided by `Event<T>` this is a relatively safe way to stop any further processing of an event. Use this only for optimisation purposes, don't rely on it for logic.

```cpp
//Empty events can be posted like this
EventBus<UpdateEvent>::Get().PostEvent({ });
```

## Giving Behaviour to Components

`Component<T>` defines a function called `ForEach`. This accepts a function that accepts a pointer to the type of component you're iterating over. You can pass an actual function here, but if you want to pass any other data to it, you need to use a lambda:

```
Int i = 15;
Int b = 0;

MyComponent::ForEach([i, &b](MyComponent* c)
{
	c->value += i;
	b++;
});
```

Pay attention to the square brackets just before the lambda. This is called a lambda capture. This allows you to pass various forms of data into a lambda. There are two methods of capturing a variable: by value or by reference. By default, C++ passes variables by value, copying them in the process. If you want to pass by reference, you can put a `&` before the variable name. Lambdas in C++ are very powerful and you'll be using them a lot, but there's a fair amount to them, so I highly recommend you do some [further reading](https://en.cppreference.com/w/cpp/language/lambda).

## The update cycle

There are multiple parts of the update cycle defined by five types of event that should be sent **exclusively by the engine**:

- `PreUpdate`
- `EarlyUpdate`
- `Update`
- `LateUpdate`
- `PostUpdate`

`PreUpdate` and `PostUpdate` are used primarily by the engine and should not be used to change any portion of the game logic. They are either side of the update cycle, and while they affect it, they are not part of it.

`EarlyUpdate`, `Update` and `LateUpdate` make up the three main parts of the update loop. These are the events you will recieve and listen to most often when developing the game.

`EarlyUpdate` is used for planning: anything that involves reading the game state **before** it updates. For example: deciding where entities are going to move and performing A.I logic.

`Update` is where the majority of the changes happen, entities move, get deleted and created.

`LateUpdate` is about reacting to the changes that happened in `Update`, this involves resolving collisions and all that good stuff.

To summarize:

|Name         |Description|
|:-----------:|:---------:|
|`EarlyUpdate`|Plan       |
|`Update`     |Act        |
|`LateUpdate` |React      |

## Content

Content loading in Valkyrie Engine is handled through a class called `ContentManager<T>` located in `engine/core/ContentManager.hpp`. This is more flexible than just loading content straight from the disk as and when you need it. Throughout the course of a game's development, external content changes a lot and updating code to reflect these changes can take a long time. This is where the content manager comes in: The content manager works by assigning a names to content then using an XML file to match these names to a file on the disk. This allows you to change what content is, where it is and how it is loaded while keeping the references to it intact.

To load content, it needs an entry in Content.xml, this is the file the content manager will look at to determine where the content is. Content in the XML file needs a type, a name and a path to be loaded properly. An example of a content entry for a texure looks like this:

```xml
<texture name="test_atlas" path="textures/test_atlas.png"></texture>
```

This texture can then be loaded and accessed by calling:

```cpp
ContentManager<Texture2D>::Get().LoadContent("test_atlas");
Texture2D* ta = ContentManager<Texture2D>::Get().GetContent("test_atlas");
ContentManager<Texture2D>::Get().UnloadContent("test_atlas");
```

Additionally, you can specify metadata for content that can be accessed in code. There's no real limit to what you can specify, it's rather open and it's all accessed by strings.

```xml
<texture name="debug" path="textures/dbg.png">
	<wrap_mode>mirror</wrap_mode>
</texture>
```

```cpp
Texture2D* debug = ContentManager<Texture2D>::Get().GetContent("debug");
std::string wrapMode = debug->GetMetaTag("wrap_mode"); //Returns "mirror"
```

You can specify your own types of content and how to load them with the abstract class `Content`. Again, this is pretty open and you can use this for basically whatever. The only thing you need to configure is the loading of the content in the `LoadContent(const std::string path)` method and specifying what tag the content manager will look for in the XML file with the `GetContentName()` function.

## Accounting for framerate

The following code adjusts the location of a `MoverComponent` every frame:

```cpp
MoverComponent::ForEach([](MoverComponent* c)
{
	c->transform->location += c->moveDirection;
});
```

The problem here is that the component's speed is dependant on the game's frame rate. This can lead to some [serious problems](https://www.youtube.com/watch?v=r4EHjFkVw-s). The solution to this is to scale the speed of the component by the game's frame rate. This will lead to the component appearing to move at the same speed regardless of how fast the game's running.

We can do this by measuring delta time. This is the amount of time that has elapsed since the last update of the game and this update. You can get the delta time by calling `VLKTime::DeltaTime<Float>()` or `VLKTime::DeltaTime<Double>()` and multiplying the amount you're moving by the delta time. Both of these functions are located in `engine/core/VLKTime.h`.

```cpp
MoverComponent::ForEach([](MoverComponent* c)
{
	c->transform->location += c->moveDirection * VLKTime::DeltaTime<Float>();
});
```