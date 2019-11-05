# Readable C++ guide for Java Devs

> Author: VD-15

## People Better Than Me:

[The Cherno Project](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw)

Does some fantastic C++ tutorials and has a good mic. He's how I originally learned and goes way more in-depth than I ever could in this document. If you don't want to have to be hounding my ass all the time, give him a look.

[C++ Reference](https://en.cppreference.com/w/)

[CPlusPlus.com](http://www.cplusplus.com/)

Both are great sources for information on standard library headers and language features. Make extensive use of both of these, it's really down to personal preference which one you use, I just click whatever comes up first in search results.

## Writing an Object Class

### Header & Source files

The most immediate difference between C++ and Java is that C++ code is seperated between two files: a `.h` file and a `.cpp` file. `.h` hiles are called header files and `.cpp` files are called source files. Code is declared in the header and then implemented in the source file; the header file specifies what a class does and the header specifies how it does it. This helps reduce dependencies and imporve compile times (which can get pretty bad, tbh). Code from a header file can be included in the source file with `#include [header-name]` at the top of the source file.

### Circular Dependencies

Something to make sure of when you're writing code is to never have two objects reference each-other. Encapsulation must only flow one way. This is a rule for general code design, but in C++, it is enforced by the language.

### Member Scope

Scopes are handled differently as well. Instead of defining `public`, `private` or `protected` for every member of a class individually, you declare them in blocks. This helps keep formatting neat and also saves the amount of typing you have to do. To declare members as public, type `public:` and any members declared below that will have public visibility. The same goes for `protected:` and `private:`.

### Classes vs Structs.

Classes and Structs are functionally identical. The only difference between them is that classes have `private` visibility by default, whereas structs have `public`.

### Example Class

In Java:

```Java
//MyClass.java

class MyClass
{
	public MyClass()
	{
		this.val = 0;
	}

	public MyClass(int i)
	{
		this.val = i;
	}

	public int foo()
	{
		return this.i * 2;
	}

	private int val;
}
```

In C++:

```CPP
//MyClass.h

class MyClass
{
	public:
	MyClass();
	MyClass(int i);

	int Foo();

	private:
	int val;
}

//MyClass.cpp
#include "MyClass.h"

MyClass::MyClass()
{
	this->val = 0;
}

MyClass::MyClass(int i)
{
	this->val = i;
}

int MyClass::Foo()
{
	return this->val * 2;
}
```

If you're wondering why we're using `->` on `this` as oppose to a `.`, that's because the `this` keyword is actually a pointer to the this object. We'll cover pointers in another segment. Just like in Java, using `this` is entirely optional, I just like it becaue it avoids name clashes with other variables.

If you're also wondering why we prepend the class name to all of it's functions, it's because you can define and implement functions in source files, they just won't be visible to anything outside of it. If we just typed `Foo()` as oppose to `MyClass::Foo()` we'd be defining and implementing a completely new function called `Foo()` that is not a member of `MyClass` and we wouldn't have access to any of it's members.

### Multiple Header & Source Files

Let's say you're writing a class and need to access another class or method from a different header. You can include this header by typing `#include [header-name]` in the header you're writing. This will cause the C++ compiler to literally just copy the code from that header into your one at compile time, so you will have access to anything defined in that header. 

This introduces a bunch of problems, however. The first being that your classes and code are already declared in the first header and copying all the code over would declare it twice, that isn't good. To get around this, in C, you had to arcane nonsense involving compiler definitions, but in C++, we can just put `#pragma once` at the very top of the header. This will make (most) compilers to only ever include your header once. Always make sure `#pragma once` is at the very top of all your headers.

### Generics

C++ handles generics completely different to Java. This stems from the fact that the two handle Objects very differently and I won't go into these differences here. What I will go into, however is templates. Templates are C++'s answer to generics and they look how you would expect them to but they behave very differently. In Java, all instances of a generic class have the same type (e.g. ArrayList, HashMap, etc.). In C++, they don't. There is a completely different class for every generic type you define (e.g. `vector<int>`, `vector<long>` and `vector<float>` are all completely different classes). This is done because C++'s compiler clones all the source code of the template class whenever you declare a new type and then effectively runs a find and replace for the template argument. This also means that any static members for a template class is only static for that type of template class.

Template classes are the one exception to the 'always implement your code in the .cpp file' rule. The reason for this is that the compiler actually needs access to the template code when it's being declared, you can't just declare it at then implement it later like you can normally. Because of this, I normally make template files with the `.hpp` file extension to show that this file contains both the source and header, though this is optional.

Here's what a generic C++ class looks like if you were wondering:

```CPP
//MyClass.hpp

template<class T>
class MyClass
{
	public:
	Class<T>()
	{

	}

	void Set(T t)
	{
		this->t = t;
	}

	T Get()
	{
		return this->t;
	}

	private:
	T t;
}
```

### Inlines

Inline functions are a very useful way of optimising code. They basically eliminate the overhead of calling the function and creating a new stack frame by just executing the code in whatever function it's written in. For small functions like those that just set or access a member variable, it's best to declare them as `inline`.

```CPP
//MyInline.h

class MyClass
{
	pulic:
	MyClass();

	inline int GetValue() { return this->value; }

	private:
	int value;
}
```

So why don't we use `inline` everywhere? Some functions can't be made inline: Constructors and destructors can't be made inline, an inline function can't call a non-inline function and it can't call itself either. On top of this, declaring large functions as inline will lead to some massive executable sizes that will actually run worse and you will eventually run out of space in stack memory to store the function.

## Memory

C++ gives you direct control of memory and with that comes all the power associated with it.

### Instantiating an Object

Now you know how to write a class, how do you instantiate one? Let's say we have a class `MyClass` with a default constructor and a constructor that takes an `int`: 

```cpp
//MyClass.h

class MyClass
{
	public:
	MyClass();
	MyClass(int i);
}

//Implementation omitted
```
Now let's say we want to instantiate it one. Take the following `main()` function:

```cpp
//Program.cpp

int main()
{
	MyClass c = new MyClass();
}
```

This is wrong and let me explain why. The `new` function instantiates an object on the heap and returns a pointer to it. In case you don't know what any of that means, don't worry, just know that it's not what we want to do right now. Instead, we can just type this to get the desired effect.

```cpp
//Program.cpp

int main()
{
	MyClass c;
}
```

You may notice that we didn't call a constructor there, but `c` is still instantiated. This is because C++ implements an idiom called RAII (or Resource Acquisition is Initialization). Basically this means that C++ will automatically call the default constructor for a given object when space for it is allocated. Nifty. But what if we don't want to call the default constructor? Well we can't call new so what about this?

```cpp
//Program.spp

int main()
{
	MyClass c = MyClass(2);
}
```

This will work perfectly fine, but you're executing redundant code. You're actually calling three functions here. The default constructor, the constructor with an `int` and the copy constructor. C++ is allocationg space for the `c` variable, initializing it (because resource acquisition *is* initialization, after all), initializing a temporary variable and then assigning `c` to that temporary variable. That's a lot of unnecessary code. How do we cut it out? Like this:

```cpp
//Program.cpp

int main()
{
	MyClass c(2);
}
```

This will initialize `c` using the constructor with an `int` without any of that redundant code above. Not the most readable thing in the world, but it's a hell of a lot better than before. It's worth pointing out that RAII only applies to objects. primitive types like `int`, `float` and the likes won't be initialized when you declare them, you need to do that the old fashioned way.

### Stack Memory

Stack memory is the best type of memory and the one you will be working the most with most often. Any object or primitive type not declared with `new` is stored on the stack. The stack is always local to whatever function you're in, is usually only a few kilobytes big and is automatically cleared when the function exits. Use stack memory wherever possible and convenient.

```cpp
//Example.cpp

int Foo()
{
	int i = 2;
	int j = 4;

	MyClass mine(4);

	return i * j * mine.Bar();
}//i, j & mine deleted here.
```

### Heap Memory

If you've used Java, you've used heap memory before. Heap memory is a massive chonk of memory whose main qualities include not getting wiped when you exit a function. Heap memory is often used to store large objects or objects that need to outlive the lifetime of the function they are created in.

### Pointers

A pointer is just a number that points to a location in memory, nothing more, nothing less. Pointers can even be added and subtracted, if you know what you're doing. The main way you'll be dealing with pointers, however is via `new`. The `new` operator creates an object in heap memory and returns a pointer to it. Pointers are denoted by a `*` after the class name. Pointers can also point to arrays if you're doing things the standard C way. This is most often done with C-style strings or C-strings for short. Pointers can also point to nothing in particular. This is called a void pointer and is basically C++'s equivalent of the Object class. Points to something, but you don't know what. Unless you're doing something really funky, you shouldn't need to use a void pointer.

You can get a pointer to an object by using the `&` operator and you can get the data at a pointer by using the `*` operator before the pointer; this is called dereferencing. For pointer objects we can use the arrow operator `->` instead of the dot operator `.` to access members of the object at that pointer.

```cpp
//Example.cpp

int main()
{
	//Pointer to an object;
	MyClass* c = new MyClass();

	//Access member of pointer
	c->Foo();
	c->bar = 5;

	//Pointer into array of 5 ints
	int* arr = new int[5];

	//Pointer to C-string
	const char* str = "Hello there!";

	//Create object on stack
	MyClass obj;

	//Pointer to stack allocated object
	MyClass* ptr = &obj;

	//Dereference pointer
	MyClass obj2 = *ptr;
}
```

It is important to note that C++ does not have a garbage collector like Java does. If you allocate memory on the heap with `new`, it will remain there until the program exits or until you call `delete` on it. **You must do this yourself**. Again, data on the stack will be deleted when the function exits. Also worth noting is that you don't need to worry about deleting strings if they're hard coded as that isn't technically on the heap.

```cpp
//Example.cpp

int main()
{
	//Pointer to an object;
	MyClass* c = new MyClass();
	int* arr = new int[5];

	//Delete object
	delete c;

	//Delete array
	delete[] arr;
}
```

Heap memory is useful for passing data between functions, take the following function:

```cpp
//Example.cpp

int* MakeInt()
{
	int i = 15;

	return &i;
}
```

The pointer returned by this function will point to garbage memory. Because `i` is deleted when the function returns, the value pointed to by the pointer is unitialized. C++ has rather lax error checking for this sort of stuff so make sure your pointers are still initialized when using them. The following will fix this problem, but you will have to `delete` the pointer yourself when you're done with it.

```cpp
//Example.cpp

int* MakeInt()
{
	int i = new int;
	*i = 15;

	return i;
}
```

### References

References are a wrapper around pointers that can help get rid of some of the syntax and headache as they can be treated like just a normal object. In Java, every object is treated as a reference. References are very helpful in C++ because they help avoid object cloning. Whenever an object is passed between functions in C++, it needs to be copied. This isn't good if you have a very large object. Pointers and references, however don't need to be copied and this makes them immesurably valuable.

```cpp
//Bad.cpp

void Foo(MyClass c)
{
	//Editing a local copy of c, changes won't be reflected once function returns.
	c.val = 15;
}

//Good.cpp

void Foo(MyClass& c)
{
	//Editing a reference of passed object, changes will be reflected.
	c.val = 15;
}
```

### Const

Something that is of note is `const` in C++. Let's say you have a function that takes a reference to an object. The function does not modify the object in any way and we want to guarantee that. This is where `const` comes in. By using `const` we can state that this object's value will not change in this function. This is a feature that I wish existed in Java.

```cpp
//Example.cpp

void Foo(const MyClass& c)
{
	//Do something with c
}
```
Any pointer, reference or object can be declared `const`. You can even perform certain member functions on `const` objects, provided that they are also labeled `const`. As a general rule of thimb, you should always pass objects into functions using const references or const pointers as they save a lot of copy overhead.

```cpp
//Example.h

class MyClass
{
	public:
	//Foo is guaranteed to not change the object state.
	int Foo() const;

	//Bar is not guaranteed to not change the object state
	int Bar();

	private:
	int val;
}

//Example.cpp

int MyClass::Foo() const
{
	return this->val;
}

int MyClass::Bar()
{
	this->val += 1;
	return this->val;
}
```

### Null Pointers

Pointers are numbers. Like numbers, pointers can be 0. This is called a null pointer or `nullptr`. A nullptr doesn't point to anything and dereferencing it will probably cause a crash. You can safely call delete on a nullptr, although nothing will happen. Null pointers can be pretty useful, however. If you want to check if a pointer points to something, you can type:

```cpp
void* ptr = something;

if (ptr)
{
	//ptr points to something
}
else
{
	//ptr is a null pointer
}
```

### Advanced pointers

Pointers can get pretty funky as you can have basically an endless number of pointers to pointers. Let's have an excersize in reading pointers. Take the following:

```cpp
const char** strings;
```

It helps to decode pointers backwards. Here we can see that we have a pointer to a pointer to a const char. In this example, this is most likely an array of C-strings. So what can we change with this? We can change where the first and second pointer point, but we can't change the data that they're pointing to. Try this one: 

```cpp
const char* const*
```

With this one, we can see that it's a pointer to a const pointer to a const char. We can change the first pointer, but not the second and not the data at the end. It's important to note that a `const char*` and a `char const*` are the same thing. It doesn't matter which way around the final const goes. You should never really have to do this, but it helps to be prepared.

### Destructors

Let's say that you have an object that had some data on the heap as one of it's member. To destroy this properly, you can define a destructor for your class. This is called automatically when `delete` is called on your object. A destructor must take no parameters and must be public. A destructor looks identical to a constructor but has a `~` in front of it.

```cpp
//MyClass.h

class MyClass
{
	public:
	//Constructor
	MyClass();

	//Destructor
	~MyClass();

	private:
	//Heap allocated data
	int* heapData;
}

//MyClass.cpp

MyClass::MyClass()
{
	this->heapData = new int;
}

MyClass::~MyClass()
{
	delete this->heapData;
}
```

## General C++ stuff

### Namespace STD

Namespace std is C++'s standard library. A combination of utility classes split up over several headers. Namespace std encapsulates everything inside of the C++ standard library. Below are some Java ADT's and their C++ equivalents.

|Java|C++|
|---:|:--|
|ArrayList|vector|
|HashMap|map|
|HashMap|unordered_map|
|HashSet|set|
|HashSet|unordered_set|

### Casting

### Iterators

Like in java, iterators exist and they're the preferred way of iterating through collections. Iterators are a wrapper around a pointer to an element in an array and they can be dereferenced like a pointer can. Iterators can also point to nothing. This is most often seen in end iterators that point to the end of a collection. Don't dereference these, use them only as a comparison to see when you're at the end of a collection. Below is an example of iteration through one of C++'s vectors:

```cpp
#include <vector>

int main()
{
	//Initialize a vector of size 10
	std::vector<int> myVec(10);

	//Obtain an iterator to the start of the vector
	//Use auto to save us having to type out std::vector<int>::iterator every time
	//While the iterator is not equal to the end iterator, increment it.
	for (auto it = myVec.begin(); it != myVec.end(); it++)
	{
		//Dereference the iterator and set the value of it's int to 5
		*it = 5;
	}
}
```

### Constexpr

War, war never changes. If war was a variable, it'd be a const expression or `constexpr`. Const expressions can be evaluated at compile time and can be used for some pretty neat stuff in the right hands. For example, the `constexpr if` this is a compile-time evaluated `if` statement that can be used to conditionally block out code in such a way that a comparason isn't even necessary at runtime. The uses for this are pretty limited, but constexpr goes beyond that and it's worth looking into if you have the time.

### Initilizer Lists

Let's say you have a member function for an object like an object without a default initializer, a reference or a const value. You absolutely can have these, you're just going to need to use an initializer list. C++ implements RAII on class members, too. So any data that has a default constructor will have it called. Anything that doesn't have a default constructor will need a constructor specified in an initializer list:

```cpp
//MyClass.h

class MyClass
{
	public:
	MyClass();

	private:

	//References can't be assigned, so this needs an initializer.
	int& ref;

	//Const variables can't be assigned, so this needs an initializer, too.
	const int c;
}

//MyClass.cpp

//This will initialize ref as 5 and c as 10. The object can function as normal now.
MyClass::MyClass() :
	ref(5),
	c(10)
{

}
```

### Inheritance

C++ does inheritance weird. For a start, it has multiple inheritance, where an object can inherit from multiple classes at once and second all classes it inherits from are treated as member variables that need to be initialized seperately using an initializer list, as oppose to a super constructor:

```cpp
//Base1.h

class Base1
{
	public:
	Base1(int i);
	//...
}

//Base2.h
class Base2
{
	public:
	Base2(float f);
	...
}

//MyClass.h

//MyClass inherits from both Base1 & Base2
class MyClass : Base1, Base2
{
	public:
	MyClass();
}

//MyClass.cpp

MyClass::MyClass() :
	Base1(5),
	Base2(10.0f)
{

}
```