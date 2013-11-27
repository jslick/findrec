/*
 * Copyright (c) 2013 findrec developers
 *
 * See the file license.txt for copying permission.
 */

#include "filematch.h"

#include <functional>
#include <vector>
#include <regex>
#include <cassert>
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

Matcher get_filename_matcher(const std::string& needle, MatchType match_type)
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

static Matcher get_filetype_matcher(MatchFileType match_file_type)
{
    Matcher matcher = [match_file_type](const boost::filesystem::path& path)->bool {
        if (static_cast<int>(match_file_type & Directories) == 0 && is_directory(path))
            return false;
        if (static_cast<int>(match_file_type & Files) == 0 && is_directory(path) == false)
            return false;
        return true;
    };
    return matcher;
}

Matcher create_matcher(const std::string& needle, MatchType match_type, MatchFileType match_file_type)
{
    using namespace std;
    using namespace std::placeholders;

    vector<Matcher> filters;
    filters.push_back(get_filename_matcher(needle, match_type));
    filters.push_back(get_filetype_matcher(match_file_type));

    function<bool(vector<Matcher>,const boost::filesystem::path&)> part_matcher = [](vector<Matcher> filters, const boost::filesystem::path& path)->bool {
        for (size_t i = 0; i < filters.size(); i++)
        {
            Matcher matcher = filters.at(i);
            assert(matcher);
            bool matches = matcher(path);
            if (!matches)
                return false;
        }
        return true;
    };
    return std::bind(part_matcher, move(filters), _1);
}
