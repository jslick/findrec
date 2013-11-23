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

enum MatchType
{
    Literal,
    Regex,
};

typedef std::function<bool(const boost::filesystem::path&)> Matcher;
typedef std::function<void(const boost::filesystem::path&)> Emitter;

void find_files(const boost::filesystem::path& root, Matcher file_matcher, Emitter emitter);
