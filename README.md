# CS2010_Groop3
CS2010 Group Project

[Designed Alliance](https://docs.google.com/document/d/1UyrTdt7ziUO9ym-NqMIcgA-ffnqi0-jOYuKz_UiY-rc/edit?usp=sharing)

[Website](https://vd-15.github.io/CS2010_Groop3/)

[Game Design Document](https://docs.google.com/document/d/1HlYA44ce1aOPXcEmsJgU3fhtHeeATdx3xW4TToKK8kc/edit?usp=sharing)

[Software Quality Strategy](https://docs.google.com/document/d/1yV17NBRIyjFyiKwCO2mGBXwDT5RqDPu8jVpTrrYdjN0/edit?usp=sharing)
 
# Readable documentation for the engine:

## A quick note on packages:

The `engine` package should be used for things relating to the engine, the `game` package should be used for things relating to the game. This has been my ted talk.

## What is an entity component system?

An entity component system (ECS) is a system paradigm commonly used in game engines, where entities are modelled as a collection of individual components. Contrary to the name, in an ECS, entities do not have any behavior or data unique to themselves, rather their behavior, properties and logic are split up into small classes called components. These components, when combined, form what we view as an entity.

An ECS has the advantage of being able to reuse sections of code with relative ease as well as having naturally low coupling and high cohesion (I hope you remembered what that meant from last year). This can significantly reduce the amount of work required on the 'game' side of things. Consider for example a basic game with a player entity and an enemy entity. Now, let's say that both of these entities have the ability to 'walk' in some capacity. You could create a simple walker component to model this behavior and apply it to both the enemy and the player at the same time. The only thing that would need to change is where they're walking and that could be handled by a seperate component, but hey, ho, we've saved ourselves duplicated code! So how does this apply to our entity component system?

## Entities:

The core functionality of an ECS relies heavily on some funky generics behavior. Java is not nearly as flexible in this regard as C/C++ is, but I've done my best to abstract it. All entities in our ECS must inherit from a class called `Entity` that is located in `engine.core` at the time of writing. `Entity` defines a default constructor and an abstract destructor function called `Delete()` that you will have to override. Any components belonging to an entity should be created in the constructor and destroyed in the destructor. Because references to components are stored in multiple places at once, the garbage collector cannot remove them if you simply delete the entity by itself; hence the existence of `Delete()`. Below is an example of an entity:

```java
public class TestEntity2 extends Entity
{
    private TestComponent2 test;
	
    public TestEntity2()
    {
        test = new TestComponent2(this);
    }
	
    @Override
    public void Delete()
    {
        test.Delete();
    }
}
```

Normally in an ECS, entities aren't supposed to have any sort of data associated with them, so storing references to what components an entity owns is *technically* breaking paradigm, however the performance gain recieved in not having to constantly search for components to delete is in my opinion perfectally justifiable. There are also some cases where you may want to add a component to an entity midway through it's lifetime. This is perfectally acceptable, just make damn well sure that the component you add is deleted by the time the entity is or you **will** get a `NullPointerException`.

## Components:

Now for components: components encapsulate all the data associated with a particular aspect of an entity. Components can have primitive types, objects and even referencess to other components. In our implementation, all components inherit from a generic class called `Component<T>` that is located in `engine.core` at the time of writing. Now, `Component<T>` is a generic class, so what do you put as the type parameter? You put the class that inheriting from it. This is a design pattern known as the Curiously Recurring Template Pattern or CRTP. This idiom originated in C++, though the behaviour it refers to is present in most object-oriented languages. Basically this means that the derrived component is a `Component<T>` but `Component<T>` also utilizes our derrived component. This is to differentiate types of component from one another in the base class.

Part of this behavior bleeds over into the constructor of `Component<T>`, requiring the class of the base object to be passed into the constructor of the component along with the entity it belongs to. `Component<T>` also defines a destructor `Delete()` similar to `Entity` in the event that a component defines other components under its' exclusive control that it needs to delete. This function does not need to be overridden. Below is an example of a component class:

```java
public class TestComponent2 extends Component<TestComponent2>
{	
    public long myValue;

    public TestComponent2(Entity parent)
    {
        super(TestComponent2.class, parent);
        this.myValue = 128;
    }
	
}
```

So how do we give our components behavior? The answer to that is the event bus:

## The event bus

All of the signaling in our engine is done through a class called `EventBus<T>` located in `engine.core`. No exceptions. This is another powerful paradigm that further eliminates coupling and promotes cohesion. Instead of modules of the engine directly sending signals to each-other, resulting in enough code spaghetti to make even the hungriest of Italians flinch, modules send and listen to events through the event bus. 

## Events

An event is nothing more than a class that inherits from `Event<T>` located in `engine.core`. Events can be empty, or contain data but they don't normally have functions. Events are your primary method of sending data to other modules. In order to make an event, you need only to inherit from `Event<T>`, call the `super()` constructor and you're ready to be sent. The type parameter of `Event<T>` should be the event inheriting from it, similar to that of `Component<T>`. I've covered that before, so I won't repeat myself here. Below you can find an example of a basic class:

```java
public class UpdateEvent extends Event<UpdateEvent>
{
    public UpdateEvent()
    {
        super();
    }
}
```

## Adding event listeners

To recieve an event, you must first add an event listener. This is simply a function or anything that can qualify as a `java.util.function.Consumer` that returns void and accepts a single parameter, that being your event. You can then subscribe this listener to the event bus by finding the event bus associated with your event and subscribing the function. As a general rule of thumb, subscribing and removing event listeners ahould be done only once, at the beginning and end of the program respectively. Subscribing an event listener while an event is being sent **will** result in a `ConcurrentModificationException`. This can be done in the form of static `init()` and `destroy()` methods that are called before the game starts and after it ends:

```java
public static void init()
{
    EventBus.get(UpdateEvent.class).subscribeEvent(TestComponent1::onUpdate);
}

public static void destroy()
{
    EventBus.get(UpdateEvent.class).unsubscribeEvent(TestComponent1::onUpdate);
}

static void onUpdate(UpdateEvent ev)
{
    System.out.println("Update called for TestComponent1!");
	
    Allocator.get(TestComponent1.class).forEach((c) ->
    {
        System.out.println(c.toString());
    });
}
```

## Posting events

Once you've set up your event and event listeners, you're free to post an event. This will signal all event listeners that are subscribed to the event bus and send them the event you provided. The state of the event may be modified by it's listeners, so you can use it to poll for some data should you need to. Additionally, sending of an event may be cancelled at any time by calling the `cancel()` method provided by `Event<T>` this is a relatively safe way to stop any further processing of an event. Use this only for optimisation purposes, don't rely on it for logic.

```java
EventBus.get(UpdateEvent.class).postEvent(new UpdateEvent());
```

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
