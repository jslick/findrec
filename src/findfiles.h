/**
 * @file    findfiles.h
 */

#pragma once

#include <string>
#include <stdexcept>
#include <boost/filesystem.hpp>

class FindException : public std::runtime_error
{
public:
    explicit FindException(const std::string& message)
    : std::runtime_error(message)
    {}

    virtual const char* what() const throw()
    {
        return message.c_str();
    }

    virtual ~FindException() throw() {}

private:
    std::string message;
};

/** Different ways to match files */
enum MatchType
{
    Expansion,  /** Match by basic expansion (e.g. some*text) */
    Regex,      /** Match by regular expression */
};

typedef std::function<bool(const boost::filesystem::path&)> Matcher;    /// Filters files
typedef std::function<void(const boost::filesystem::path&)> Emitter;    /// Callback matched files

/**
 * Finds files recursively in a directory
 *
 * @param[in]   root            Directory to start from
 * @param[in]   file_matcher    Callback to filter files
 * @param[in]   emitter         Callback invoked for each matched file
 */
void find_files(const boost::filesystem::path& root, Matcher file_matcher, Emitter emitter);
