#include "shellhelper.h"
bool debugging_enabled = false;
std::map<std::string, std::string> aliases;

DWORD AddAlias(std::string& command)
{
	extern std::map<std::string, std::string> aliases;
	if (_strnicmp(command.c_str(), "alias", 5))
		return -1; //not a alias x='x' command
	try
	{
		std::string ap = command.substr(command.find(" ") + 1, command.length() - command.find(" ") + 1);
		std::string an = ap.substr(0, ap.find("="));  //name of the alias,before=
		std::string ac = ap.substr(ap.find("="), ap.size() - 1);
		if (aliases.count(an))
			return -1;
		aliases[an] = ac;
		return 0;
	}
	catch (std::out_of_range& oor)
	{
		std::string what = oor.what();
		DEBUG("Error: " + what);
		return -1;
	}
}
void InitShellHelper()
{
	extern std::map<std::string, std::string> aliases;
	aliases["cat"] = "dir";
	aliases["ls"] = "dir";
}
DWORD ProcessCommand(std::string& command)
{
	if (!AddAlias(command))
	{
		DEBUG("Alias added!");
		return 0;
	}
	if (!_strnicmp(command.c_str(), "exit", strlen("exit")))
	{
		exit(0);
	}

	if (!_strnicmp(command.c_str(), "cd ", strlen("cd ")))
	{
		std::string newCwd;
		for (size_t i = 0; i < command.size(); i++)
			newCwd[i] = command[i + 3];
		return SetCurrentDirectoryA(newCwd.c_str());
	}
	if (command[0] != '\\')
	{
		HandleAliases(command);
		return system(command.c_str());
	}
	command = command.substr(1, 25);
	return system(command.c_str());
}


DWORD HandleAliases(std::string& command)
{
	extern std::map<std::string, std::string> aliases;
	DWORD err = 0;
	std::string frw = command.substr(0, command.find(" "));
	if (aliases.count(frw))
	{

		wrReplace(command, frw, aliases[frw]);
	}
	DEBUG("Wrapper returned " + std::to_string(err));
	DEBUG("New command = " + command);
	return err;
}

DWORD wrReplace(std::string& src, std::string a, std::string b)
{
	if (src.substr(0, src.size() - 1) == a && src[-1] == ' ')//"ls ","ls","dir" ->"dir "
	{
		DEBUG("src = a + \" \"");
		src = b + " ";
		return 0;
	}
	if (0 == _stricmp(src.c_str(), a.c_str())) //the same(case insensitive)
	{
		DEBUG("src = a");
		src = b;
		return 0;
	}
	try
	{
		std::regex atob("\\b(" + a + " )([^ ]*)", std::regex_constants::icase);
		if (std::regex_match(src, atob))
		{
			DEBUG("Using regex ");
			src = std::regex_replace(src, atob, b + " $2");
			return 0;
		}
	}
	catch (std::regex_error& err)
	{
		std::string what(err.what());
		DEBUG("Malformed input:" + what);
		return -1;
	}
	std::cout << "out of work" << std::endl;
	return 0;

}
void DEBUG(std::string x)
{
	do
	{
		if (debugging_enabled)
		{
			std::cout << x << std::endl;
		}
	} while (0);
}
