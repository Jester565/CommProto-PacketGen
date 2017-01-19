#pragma once
#include <string>
#include <unordered_map>

class Type;

class TypeMap
{
public:
		TypeMap();

		void addType(const std::string& key, const std::string& val, int numBytes);

		bool convertType(Type* typePtr);

		~TypeMap();

private:
		std::unordered_map<std::string, std::pair<std::string, int>> types;
};

