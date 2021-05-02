#pragma once
#include <Windows.h>
#include <WinBase.h>
#include <Shlwapi.h>
#include <string>
#include <iostream>
#include <regex>
#include <map>
void DEBUG(std::string x);
DWORD ProcessCommand(std::string& command);
DWORD HandleAliases(std::string& command);
void InitShellHelper();
// Replase the FIRST ocurrence of a in src to b
DWORD wrReplace(std::string& src, std::string a, std::string b);

DWORD AddAlias(std::string& command);
