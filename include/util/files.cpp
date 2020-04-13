
#include "util/files.hpp"

namespace ay::files
{


bool skip_bom(std::ifstream &infile)
{
    size_t bytes_needed = 3;
    char buffer[3];

    std::memset(buffer, '\0', bytes_needed);

    infile.read(buffer, static_cast<std::streamsize>(bytes_needed));

    if ((buffer[0] == '\xef') && (buffer[1] == '\xbb') && (buffer[2] == '\xbf'))
    {

        infile.seekg(3);
        return true;
    }

    infile.seekg(0);

    return false;
}

bool skip_elf(std::ifstream &infile)
{
    size_t bytes_needed = 4;
    char buffer[4];

    std::memset(buffer, '\0', bytes_needed);

    infile.read(buffer, static_cast<std::streamsize>(bytes_needed));

    if ((buffer[0] == '\x7f') && (buffer[1] == '\x45') && (buffer[2] == '\x4c')
        && (buffer[3] == '\x46'))
    {
        infile.close();
        return true;
    }

    infile.close();
    return false;
}

bool check_elf(const std::string &t_filename)
{
    std::ifstream infile(t_filename.c_str(),
                         std::ios::in | std::ios::ate | std::ios::binary);
    infile.seekg(0, std::ios::beg);

    if (!infile.is_open())
    {
        return false;
    }

    if (skip_elf(infile))
    {
        infile.close();
        return true;
    }
    infile.close();
    return false;
}

std::string load_file(const std::string &t_filename)
{
    std::ifstream infile(t_filename.c_str(),
                         std::ios::in | std::ios::ate | std::ios::binary);

    if (!infile.is_open())
    {
    }

    auto size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    assert(size >= 0);

    if (skip_bom(infile))
    {
        size -= 3;
        assert(size >= 0);
    }

    if (size == std::streampos(0))
    {
        return std::string();
    }
    else
    {
        std::vector<char> v(static_cast<size_t>(size));
        infile.read(&v[0], static_cast<std::streamsize>(size));
        return std::string(v.begin(), v.end());
    }
}


}  // namespace ay::files
