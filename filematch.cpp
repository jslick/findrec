#include "filematch.h"

#include <boost/filesystem.hpp>

static bool always_true(const boost::filesystem::path& path)
{
    return true;
}

Matcher create_matcher(const std::string& needle, MatchType match_type)
{
    return always_true;
}
