#pragma once

class CUtils
{
public:
    CUtils() = delete;

    static bool RunProcess(std::string const& command);
	static std::string GetResourcesRoot();
};
