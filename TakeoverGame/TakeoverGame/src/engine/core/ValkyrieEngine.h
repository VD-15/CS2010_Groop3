#pragma once
#include <string>
#include <typeinfo>

namespace vlk
{
	typedef uint8_t		Byte;
	typedef uint8_t		Char;
	typedef int8_t		SByte;
	typedef uint16_t	UShort;
	typedef int16_t		Short;
	typedef uint32_t	UInt;
	typedef int32_t		Int;
	typedef uint64_t	ULong;
	typedef int64_t		Long;

	typedef bool		Boolean;
	typedef bool		Bool;

	typedef double		Double;
	typedef float		Single;
	typedef float		Float;

	typedef size_t		Size;

	const Float PiOverTwo =	1.57079632679f;
	const Float Pi =		3.14159265359f;
	const Float TwoPi =		6.28318530718f;

	typedef void LogFun(const std::string& source, const std::string& message);

	//Logs a mesasage with trace severity.
	//Used to map flow within a function or algorithm.
	LogFun LogTrace;

	//Logs a mesasage with verbose severity.
	//Used to map flow between functions and routines.
	LogFun LogVerbose;

	//Logs a mesasage with info severity.
	//Used to map overall program flow and important routines.
	LogFun LogInfo;

	//Logs a mesasage with warning severity.
	//Used to warn when something could lead to an error.
	LogFun LogWarning;

	//Logs a mesasage with error severity.
	//Used when an error has occured that the program can recover from.
	LogFun LogError;

	//Logs a mesasage with fatal severity.
	//Used when an error has occured that the program can not recover from.
	LogFun LogFatal;

	//Logs a mesasage with announce severity.
	LogFun LogAnnounce;

	//Initializes ValkyrieEngine and it's dependencies
	void Init();

	//Destroys ValkyrieEngine and it's dependencies
	void Destroy();

	//Converts a pointer to a hexadecimal string
	std::string PointerToString(const void* ptr);

	//Converts an std::type_info to a string
	std::string TypeInfoToString(const std::type_info& t);

	//Check if a string ends with another string
	Boolean StringEndsWith(const std::string& str, const std::string& end);

	//Check if a string begins with another string
	Boolean StringBeginsWith(const std::string& str, const std::string& beg);

	//Converts a type to a string
	template<class T>
	std::string TypeToString()
	{
		return TypeInfoToString(typeid(T));
	}

	//Duplicates a C-string and returns a pointer to the duplicate.
	//Use this as an alternative to strdup and _strdup
	//The pointer returned must be freed by delete[]
	const char* DuplicateString(const char* src);

	//Compares two primitive types.
	// returns -1 if left has a lower value than right
	// returns  0 if left is equal to right
	// returns  1 if left is greater than right
	template<class T>
	Int Compare(T left, T right)
	{
		if (left < right)
		{
			return -1;
		}
		else if (left > right)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	template<class T, class F>
	T* Select(T* left, T* right, F f)
	{
		if (f(left)) return left;
		if (f(right)) return right;
		return nullptr;
	}

	template<class T, class U, class V>
	void uncertain_cast(V* v1, V* v2, T*& t, U*& u)
	{
		T* _t = dynamic_cast<T*>(v1);

		if (_t)
		{
			t = _t;
			u = dynamic_cast<U*>(v2);
		}
		else
		{
			t = dynamic_cast<T*>(v2);
			u = dynamic_cast<U*>(v1);
		}
	}
}