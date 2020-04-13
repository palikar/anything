#pragma once

#include "std_header.hpp"

namespace ay::files
{

bool skip_bom(std::ifstream &infile);

bool skip_elf(std::ifstream &infile);

bool check_elf(const std::string &t_filename);

std::string load_file(const std::string &t_filename);


}  // namespace ay::files
