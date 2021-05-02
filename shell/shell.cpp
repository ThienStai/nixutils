#include "shell.h"
using namespace concolor;
int main(const int argc, const char* argv[])
{
	init();
	char* cwd = new char[65536]; //max path
	memset(cwd, 0, 65536);
	std::string cmd;
	std::cout << fg_green << "Windows Bash shell v 1.0" << fg_white << std::endl;
	while (true)
	{
		GetCurrentDirectoryA(65536, cwd);
		std::cout << fg_cyan << cwd << fg_white << " $ ";
		std::getline(std::cin, cmd);
		cmd = trim(cmd);
		ProcessCommand(cmd);
	}

}

int init()
{
	InitShellHelper();
	if (PathFileExistsA(".\\config\\.shellrc"))
	{
		std::ifstream input(".\\config\\.shellrc");

		for (std::string line; std::getline(input, line);)
		{
			ProcessCommand(line);
		}
	}
}
