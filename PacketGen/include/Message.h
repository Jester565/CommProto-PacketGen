#pragma once
#include <vector>
#include <string>

struct Type
{
		Type(const std::string& name)
				:name(name), numBytes(0)
		{

		}
		Type(const std::string& name, int numBytes, const std::string& defaultValue)
				:name(name), numBytes(numBytes), defaultVal(defaultValue)
		{

		}
		std::string name;
		uint8_t numBytes;
		std::string defaultVal;
};

struct Field
{
		static const int ASSIGN_MODE_NONE = 0;
		static const int ASSIGN_MODE_SET_DEFAULT = 1;
		static const int ASSIGN_MODE_CREATE_CONSTRUCT = 2;

		Field()
				:type(nullptr), name(nullptr), defaultArg(nullptr), comment(nullptr), assignmentMode(ASSIGN_MODE_NONE)
		{
				
		}

		Type* type;
		std::string* name;
		std::string* comment;
		std::string* defaultArg;
		int assignmentMode;

		~Field()
		{
				if (type != nullptr) {
						delete type;
						type = nullptr;
				}
				if (name != nullptr) {
						delete name;
						name = nullptr;
				}
				if (comment != nullptr) {
						delete comment;
						comment = nullptr;
				}
				if (defaultArg != nullptr) {
						delete defaultArg;
						defaultArg = nullptr;
				}
		}
};

class Message
{
public:
		Message();

		bool hasName()
		{
				return name != nullptr;
		}

		std::string getName()
		{
				return *name;
		}

		void setName(const std::string& nameStr)
		{
				name = new std::string(nameStr);
		}

		void setComment(const std::string& commentStr)
		{
				comment = new std::string(commentStr);
		}

		std::string* getComment()
		{
				return comment;
		}

		bool hasScopeStarted()
		{
				return scopeStarted;
		}

		void setScopeStarted()
		{
				scopeStarted = true;
		}

		void addField(Field* field);

		std::vector<Field*>& getFields()
		{
				return fields;
		}

		~Message();

private:
		bool scopeStarted;
		std::string* comment;
		std::string* name;
		std::vector<Field*> fields;
};

