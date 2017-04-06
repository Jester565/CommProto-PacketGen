#pragma once
#include "Runner.h"

class CsRunner : public Runner
{
public:
	std::string getContainerFunc(std::vector<Type*> subTypes) {
		if (subTypes.size() == 1) {
			if (!subTypes.at(0)->isPtr) {
				return "Unique";
			}
		}
		else
		{
			std::string str;
			if (subTypes.at(1)->isPtr) {
				return "UniqueKey";
			}
			else
			{
				return "UniqueKeyVal";
			}
		}
		return "";
	}

	std::string getICollect(std::vector<Type*> subTypes, std::string& name) {
		std::string str = "ICollection<";
		if (subTypes.size() == 1) {
			str += "ObjSerializable";
		}
		else
		{
			str += "KeyValuePair<ObjSerializable, ObjSerializable>";
		}
		str += ">";
		str += " ";
		str += "iCollect";
		str += name;
		str += " = ";
		str += name;
		str += ";";
		return str;
	}

		void HandleConstructor(Message* msg, std::ofstream& fileOut, int space)
		{
				fileOut << "public ";
				fileOut << msg->getName() << "(";
				std::vector <Field*> orderedFields;
				for (int i = 0; i < msg->getFields().size(); i++) {
					if (!msg->getFields().at(i)->isCustomType() && msg->getFields().at(i)->subTypes.size() == 0) {
						orderedFields.push_back(msg->getFields().at(i));
					}
				}
				int noDefaultArgI = 0;
				for (int i = 0; i < orderedFields.size(); i++)
				{
					if (orderedFields.at(i)->assignmentMode != Field::ASSIGN_MODE_SET_DEFAULT)
					{
							Field* temp = orderedFields.at(i);
							orderedFields.at(i) = orderedFields.at(noDefaultArgI);
							orderedFields.at(noDefaultArgI) = temp;
							noDefaultArgI++;
					}
				}
				for (int i = 0; i < orderedFields.size(); i++)
				{
					fileOut << orderedFields.at(i)->type->name << " " << *orderedFields.at(i)->name;
					if (orderedFields.at(i)->assignmentMode == Field::ASSIGN_MODE_SET_DEFAULT)
					{
							fileOut << " = ";
							fileOut << *orderedFields.at(i)->defaultArg;
					}
					if (i != orderedFields.size() - 1)
					{
							fileOut << ",";
							Runner::NewLine(fileOut, space + TAB_SPACE);
					}
				}
				fileOut << ")";
				Runner::NewLine(fileOut, space + TAB_SPACE);
				if (msg->getIsObj()) {
					fileOut << ": base()";
				}
				else
				{
					fileOut << ": base(\"";
					fileOut << msg->getName();
					fileOut << "\")";
				}
				Runner::NewLine(fileOut, space);
				fileOut << "{";
				for (int i = 0; i < orderedFields.size(); i++)
				{
					Runner::NewLine(fileOut, space + TAB_SPACE);
					fileOut << "this." << *orderedFields.at(i)->name << " = ";
					fileOut << *orderedFields.at(i)->name;
					fileOut << ";";
				}
				for (int i = 0; i < msg->getFields().size(); i++) {
					if (msg->getFields().at(i)->isCustomType()) {
						Runner::NewLine(fileOut, space + TAB_SPACE);
						fileOut << "this." << *msg->getFields().at(i)->name << " = ";
						fileOut << "new " << msg->getFields().at(i)->type->name << "();";
					}
					else if (msg->getFields().at(i)->subTypes.size() > 0) {
						Field* field = msg->getFields().at(i);
						Runner::NewLine(fileOut, space + TAB_SPACE);
						fileOut << "this." << *field->name << " = new " << field->type->name << " <";
						for (int j = 0; j < field->subTypes.size(); j++) {
							fileOut << "ObjSerializable";
							if (j < field->subTypes.size() - 1) {
								fileOut << ", ";
							}
						}
						fileOut << ">();";
					}
				}
				Runner::NewLine(fileOut, space);
				fileOut << "}";
		}

		void HandlePackBody(Message* msg, std::ofstream& fileOut, int space)
		{
			for (int i = 0; i < msg->getFields().size(); i++)
			{
				if (msg->getFields().at(i)->isCustomType()) {
					fileOut << *msg->getFields().at(i)->name;
					fileOut << ".Input(obj);";
				}
				else if (msg->getFields().at(i)->subTypes.size() > 0) {
					fileOut << "obj.Input" << getContainerFunc(msg->getFields().at(i)->subTypes) << "(";
					fileOut << *msg->getFields().at(i)->name;
					fileOut << ");";
				}
				else
				{
					fileOut << "obj.Input(";
					fileOut << *msg->getFields().at(i)->name;
					fileOut << ");";
				}
				if (i != msg->getFields().size() - 1)
				{
						Runner::NewLine(fileOut, space);
				}
			}
		}

		void HandleUnpackBody(Message* msg, std::ofstream& fileOut, int space)
		{
			for (int i = msg->getFields().size() - 1; i >= 0; i--)
			{
				if (msg->getFields().at(i)->isCustomType()) {
					fileOut << *msg->getFields().at(i)->name;
					fileOut << ".Output(obj);";
				}
				else if (msg->getFields().at(i)->subTypes.size() > 0) {
					Field* field = msg->getFields().at(i);
					fileOut << getICollect(field->subTypes, *field->name);
					Runner::NewLine(fileOut, space);
					fileOut << "obj.Output" << getContainerFunc(field->subTypes) << "(";
					fileOut << "ref iCollect" << *field->name;
					for (int j = 0; j < field->subTypes.size(); j++) {
						fileOut << ", new ";
						fileOut << field->subTypes.at(j)->name;
						if (!field->subTypes.at(j)->custom) {
							fileOut << "Ser";
						}
						fileOut << "()";
					}
					fileOut << ");";
				}
				else
				{
					fileOut << *msg->getFields().at(i)->name;
					fileOut << " = obj.Output";
					fileOut << msg->getFields().at(i)->type->name;
					fileOut << "();";
				}
				if (i != 0)
				{
					Runner::NewLine(fileOut, space);
				}
			}
		}

		void HandleData(Message* msg, std::ofstream& fileOut, int space)
		{
			for (int i = 0; i < msg->getFields().size(); i++)
			{
				if (msg->getFields().at(i)->comment != nullptr)
				{
						Runner::HandleSurroundComment(*msg->getFields().at(i)->comment, fileOut, space);
				}
				fileOut << "public ";
				fileOut << msg->getFields().at(i)->type->name;
				if (msg->getFields().at(i)->subTypes.size() > 0)
				{
					Field* field = msg->getFields().at(i);
					fileOut << " <";
					for (int j = 0; j < field->subTypes.size(); j++) {
						fileOut << "ObjSerializable";
						if (j < field->subTypes.size() - 1) {
							fileOut << ", ";
						}
					}
					fileOut << ">";
				}
				fileOut << " ";
				fileOut << *msg->getFields().at(i)->name;
				fileOut << " { get; set; }";
				if (i != msg->getFields().size() - 1)
				{
					Runner::NewLine(fileOut, space);
				}
			}
		}

		static const std::string TEMPLATE_PATH;
		static const std::string OBJ_TEMPLATE_PATH;
		static const std::string DEFAULT_FILE_ENDING;

		CsRunner(CommandArgumentManager* cmdArgManager);

		virtual bool checkArguments() override;

		virtual bool linkFields() override;

		virtual bool linkStatements() override;

		~CsRunner();
};

