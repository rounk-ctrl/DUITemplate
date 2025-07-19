// DuiPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <regex>
#include <map>
#include <fstream>
#include <sstream>
#include <filesystem>

std::map<std::wstring, std::wstring> map_defines = {};
void load_defines(wchar_t* file)
{
	std::wifstream filestream(file);
	std::wregex pattern(LR"(^\s*#define\s+(\w+)\s+(.+))");

	std::wstring line;
	while (std::getline(filestream, line))
	{
		std::wsmatch match;
		if (std::regex_match(line, match, pattern))
		{
			map_defines[match[1]] = match[2];
		}
	}

	filestream.close();
}

void preprocess(wchar_t* file)
{
	std::wifstream duifile(file);
	std::wstringstream buffer;
	buffer << duifile.rdbuf();

	std::wstring content = buffer.str();
	for (const auto& [key, value] : map_defines)
	{
		std::wregex word_regex(L"\\b" + key + L"\\b");
		content = std::regex_replace(content, word_regex, value);
	}

	std::wregex space_regex(LR"(^[ \t]+(?=(<|\w+=)))");
	std::wregex comment_regex(LR"(\s<!--.*?-->\n)");
	std::wregex blank_regex(LR"(\n\s*\n)");
	content = std::regex_replace(content, comment_regex, L"");
	content = std::regex_replace(content, space_regex, L"");
	content = std::regex_replace(content, blank_regex, L"\n");

	duifile.close();

	std::filesystem::path filepath = file;
	std::wofstream processedfile(filepath.replace_extension("uipp"), std::ios::out | std::ios::trunc);
	processedfile << content;
	processedfile.close();
}

int wmain(int argc, wchar_t* argv[])
{
	if (argc <= 2)
	{
		std::cout << "Usage: DuiPP.exe <dui file> <header file 1> <header file 2> ...\n" ;
	}
	std::wcout << std::filesystem::path(argv[1]).filename().wstring();

	int headers = argc - 2;
	for (int i = 0; i < headers; ++i)
	{
		load_defines(argv[2 + i]);
	}
	preprocess(argv[1]);
	return 0;
}