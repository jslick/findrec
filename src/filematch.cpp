#include "filematch.h"

#include <functional>
#include <regex>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>

// A Matcher callback
// Don't filter any files
static bool match_tautology(const boost::filesystem::path& path)
{
    return true;
}

// Matches filenames by regular expression
static bool match_regex(const std::regex& regex, const boost::filesystem::path& path)
{
    using namespace std;

    smatch match;
    return regex_match(path.filename().string(), match, regex);
}

static std::string escape_regex(std::string str)
{
    using namespace boost::algorithm;

    replace_all(str, "\\", "\\\\");   // must come first
    replace_all(str, "*", "\\*");
    replace_all(str, ".", "\\.");
    replace_all(str, "+", "\\+");
    replace_all(str, "/", "\\/");
    replace_all(str, "?", "\\?");
    replace_all(str, "(", "\\(");
    replace_all(str, ")", "\\)");
    replace_all(str, "[", "\\[");
    replace_all(str, "]", "\\]");
    replace_all(str, "{", "\\{");
    replace_all(str, "}", "\\}");
    return str;
}

Matcher create_matcher(const std::string& needle, MatchType match_type)
{
    using namespace std;
    using namespace std::placeholders;
    using namespace std::regex_constants;

    if (needle.length())
    {
        switch (match_type)
        {
        case Expansion:
            {
                string expansion = escape_regex(needle);
                boost::algorithm::replace_all(expansion, "\\*", ".*");

                regex needle_regex = regex(expansion, ECMAScript | icase);
                return std::bind(&match_regex, needle_regex, _1);
            }

        case Regex:
            {
                regex needle_regex = regex(needle, ECMAScript | icase);
                return std::bind(&match_regex, needle_regex, _1);
            }

        default:
            throw FindException("Match type not implemented");
        }
    }
    else
        return match_tautology;
}
