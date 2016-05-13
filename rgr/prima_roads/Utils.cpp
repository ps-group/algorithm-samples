#include "stdafx.h"
#include "Utils.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

namespace
{
class CPipeHandle
{
public:
    CPipeHandle(FILE *pipe)
        : m_pipe(pipe)
    {
    }

    ~CPipeHandle()
    {
        if (m_pipe)
        {
            pclose(m_pipe);
        }
    }

    operator FILE*()const
    {
        return m_pipe;
    }

private:
    FILE *m_pipe = nullptr;
};
}

bool CUtils::RunProcess(const std::string &command)
{
    CPipeHandle pipe(popen(command.c_str(), "r"));
    if (!pipe)
    {
        return false;
    }

    std::vector<char> buffer(256);

    while (std::fgets(buffer.data(), int(buffer.size()), pipe))
    {
        std::fputs(buffer.data(), stderr);
    }

    if (std::feof(pipe))
    {
        return true;
    }

    // process run failed, print error.
    std::fputs(std::strerror(errno), stderr);
    return false;
}

std::string CUtils::GetResourcesRoot()
{
#ifdef _WIN32
	return "..\\..\\..\\..\\..\\rgr\prima_roads\res";
#else
	return "res";
#endif
}
