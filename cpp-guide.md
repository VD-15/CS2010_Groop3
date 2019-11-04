# Readable C++ guide for Java Devs

> Author: VD-15

## Writing an Object Class

### Header & Source files

The most immediate difference between C++ and Java is that C++ code is seperated between two files: a `.h` file and a `.cpp` file. `.h` hiles are called header files and `.cpp` files are called source files. Code is declared in the header and then implemented in the source file, similar to the structure of Java's interface. There's a good reason for this and we'll cover exactly what this is later on. Code from a header file can be included in the source file with `#include [header-name]` at the top of the source file.

### Member Scope

Scopes are handled differently as well. Instead of defining `public`, `private` or `protected` for every member of a class individually, you declare them in blocks. This helps keep formatting neat and also saves the amount of typing you have to do. To declare members as public, type `public:` and any members declared below that will have public visibility. The same goes for `protected:` and `private:`.

### Classes vs Structs.

Classes and Structs are functionally identical. The only difference between them is that classes have `private` visibility by default, whereas structs have `public`.

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

If you're wondering why we're using `->` on `this` as oppose to a `.`, that's because the `this` keyword is actually a pointer to the object. We'll cover pointers in another segment. Just like in Java, using `this` is entirely optional, I just like it becaue it avoids name clashes.

If you're also wondering why we prepend the class name to all of it's functions, it's because you can define and implement functions in source files, they just won't be visible to anything outside of it. If we just typed `Foo()` as oppose to `MyClass::Foo()` we'd be defining and implementing a completely new function called `Foo()` that is not a member of `MyClass` and we wouldn't have access to any of it's members.

### Multiple Header & Source Files

Let's say you're writing a class and need to access another class or method from a different header. You can include this header by typing `#include [header-name]` in the header you're writing. This will cause the C++ compiler to literally just copy the code from that header into your one at compile time, so you will have access to anything defined in that header. 

This introduces a bunch of problems, however. The first being that your classes and code are already declared in the first header and copying all the code over would declare it twice, which you can't do. In C, you had to arcane nonsense involving compiler definitions, but in C++, we can just put `#pragma once` at the very top of the header. This will make the compiler check if your header has already been declared and if it has, then it will just use the declarations it already made. Always make sure `#pragma once` is at the very top of all your headers.

This is also why we can't have source code included in the header. 