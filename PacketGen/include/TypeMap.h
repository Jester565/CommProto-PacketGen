#pragma once
#include <string>
#include <unordered_map>

class Type;

class TypeMap
{
public:
		TypeMap();

		void addType(const std::string& packGenTypeName, const std::string& val, int numBytes, const std::string& defaultVal);

		void addType(const std::string& packGenTypeName, int numBytes, const std::string& defaultVal);

		bool setLangTypeName(const std::string& packGenTypeName, const std::string& langTypeName);

		bool convertType(Type* typePtr);

		~TypeMap();

private:
		std::unordered_map<std::string, Type*> types;
};

