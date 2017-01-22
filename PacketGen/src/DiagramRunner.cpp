#include <DiagramRunner.h>
#include <DiagramManager.h>
#include <FileManager.h>
#include <iostream>

const std::string DiagramRunner::DEFAULT_FILE_ENDING = "png";

DiagramRunner::DiagramRunner(CommandArgumentManager * cmdArgManager)
		:Runner(cmdArgManager, "")
{
}

bool DiagramRunner::run(int argc, char * argv[])
{
		diagramManager = new DiagramManager();
		if (!checkArguments())
		{
				return false;
		}
		if (!parse())
		{
				return false;
		}
		if (!linkFields())
		{
				return false;
		}
		if (!convertFields())
		{
				return false;
		}
		if (!write())
		{
				return false;
		}
		return true;
}

DiagramRunner::~DiagramRunner()
{
		if (diagramManager != nullptr)
		{
				delete diagramManager;
				diagramManager = nullptr;
		}
}

bool DiagramRunner::checkArguments()
{
		msgInPath = *cmdArgManager->getValue("I");
		for (int i = 0; i < msgInPath.size(); i++)
		{
				if (msgInPath.at(i) == '\\')
				{
						msgInPath.at(i) = '/';
				}
		}
		if (!FileManager::IsFile(msgInPath))
		{
				std::cerr << "Input path does not exist" << std::endl;
				return false;
		}

		msgOutPath = *cmdArgManager->getValue("O");
		if (msgOutPath.size() > 0)
		{
				for (int i = 0; i < msgOutPath.size(); i++)
				{
						if (msgOutPath.at(i) == '\\')
						{
								msgOutPath.at(i) = '/';
						}
				}
				if (msgOutPath.back() == '/')
				{
						msgOutPath.pop_back();
				}
		}

		if (!FileManager::IsDirectory(msgOutPath))
		{
				std::cerr << "Output path does not exist" << std::endl;
				return false;
		}

		fileEnding = (*cmdArgManager->getValue("E"));
		if (fileEnding.empty())
		{
				fileEnding = DEFAULT_FILE_ENDING;
		}

		return true;
}

bool DiagramRunner::write()
{
		if (!diagramManager->init())
		{
				std::cerr << "Failed to initialize diagramManager" << std::endl;
				return false;
		}
		for (int i = 0; i < parser->messages.size(); i++)
		{
				if (!diagramManager->makeDiagram(parser->messages.at(i), msgOutPath, fileEnding))
				{
						return false;
				}
		}
		return true;
}
