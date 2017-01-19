#include <iostream>
#include <string>
#include <stdio.h>

#include "Runner.h"
#include "CppRunner.h"
#include "CsRunner.h"
#include "CommandArgumentManager.h"
#include "Parser.h"

bool readArguments(CommandArgumentManager* cmdArgManager, int argc, char* argv[])
{
		cmdArgManager->addArgument("I", true);
		cmdArgManager->addArgument("O", true, "./");
		cmdArgManager->addArgument("L", true, "cpp");
		cmdArgManager->addArgument("E", true, "");
		if (!cmdArgManager->readArgs(argc, argv))
		{
				std::cerr << "Arguments failed terminating..." << std::endl;
				return false;
		}
		return true;
}

int run(int argc, char* argv[])
{
		Runner* runner = nullptr;
		CommandArgumentManager* cmdArgManager = new CommandArgumentManager();
		if (!readArguments(cmdArgManager, argc, argv))
		{
				return 1;
		}
		std::string lang = *cmdArgManager->getValue("L");
		if (lang == "cpp")
		{
				runner = new CppRunner(cmdArgManager);
		}
		else if (lang == "cs")
		{
				runner = new CsRunner(cmdArgManager);
		}
		else
		{
				std::cerr << lang << " is not a language" << std::endl;
				return 1;
		}
		if (!runner->run(argc, argv))
		{
				return 1;
		}
		return 0;
}

int main(int argc, char *argv[])
{
		if (run(argc, argv) == 0)
		{
				std::cout << "Success" << std::endl;
		}
		else
		{
				std::cout << "Failure" << std::endl;
		}
}