#include <iostream>
#include <windows.h>
#include <Shlwapi.h>
#include "cxxopts.hpp" 
#include "helper.hpp"
#include <string>
#define BUF_SIZE 256

void print_help();
int main(const int argc, const char* argv[])
{
	BOOL nocreate = FALSE;
	BOOL discard_existing = FALSE;
	BOOL existed = FALSE;
	cxxopts::Options options("touch", "touch");
	options.add_options()
		("c,no-create", "no-create", cxxopts::value<bool>()->default_value("false"))
		("h,help", "help", cxxopts::value<bool>()->default_value("false"))
		("d,discard-existing", ",discard-existing", cxxopts::value<bool>()->default_value("false"));
	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);
	auto fl = result.unmatched();

	nocreate = result.count("c");
	discard_existing = result.count("d");

	const char* curfile;
	if (result.count("help"))
		print_help();
	for (size_t i = 0; i < fl.size(); i++) {
		curfile = fl[i].c_str();
		existed = PathFileExistsA(curfile);
		if (!existed && !nocreate) {
			FILE* tmp;
			fopen_s(&tmp, curfile, "w+");
			if (tmp == nullptr) {
				continue;
			}
			fclose(tmp);
			continue;
		}
		if (!existed && nocreate) {
			continue;
		}
		if (existed && discard_existing) {
			FILE* tmp;
			fopen_s(&tmp, curfile, "w+");
			if (tmp == nullptr) {
				continue;
			}
			fclose(tmp);
			continue;
		}
		if (existed && !discard_existing) {
			continue;
		}

	}
}
void print_help() {
	std::cout << "Uasge: touch [options] filename1 filename2..." << std::endl;
	std::cout << " -c --no-create           if the specified file don't exist, don't create it" << std::endl;
	std::cout << " -d --discard-existing    if the specified file already exist, erase its content" << std::endl;
	std::cout << " -h --help                show this message" << std::endl;
	exit(0);
}
