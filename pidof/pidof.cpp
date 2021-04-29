#include <iostream>
#include <windows.h>
#include "cxxopts.hpp" 
#include "helper.hpp"
#include <string>
#define BUF_SIZE 256
void print_help();
int main(const int argc, const char* argv[])
{
	BOOL single = FALSE;
	cxxopts::Options options("pidof", "One line description of pidof");
	options.add_options()
		("s,single", "onepid")
		("h,help", "help")
		;
	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);
	auto il = result.unmatched();
	wchar_t target[BUF_SIZE] = { 0 };
	if (result.count("help")) {
		print_help();
	}
	single = result.count("s");
	for (int j = 1; j < il.size(); j++) {
		swprintf(target, BUF_SIZE, L"%hs", il[j].c_str());
		std::vector<DWORD> found;
		found = GetProcId(target);
		if (found.size() && single) {
			std::cout << found[0];
			continue;
		}
		for (size_t i = 0; i < found.size(); i++)
			std::cout << found[i] << " ";
	}
	std::cout << std::endl;
	return 0;

}
void print_help() {
	std::cout << "Usage: pidof options procname.exe" << std::endl;
	std::cout << " -s --single        only output 1 pid per proceses specified" << std::endl;
	std::cout << " -h --help          print this message" << std::endl;
	exit(0);
}
