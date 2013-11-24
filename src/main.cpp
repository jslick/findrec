#include "findfiles.h"
#include "filematch.h"

#include <string>
#include <cstdio>

static struct Options
{
    enum StatusCode : int
    {
        Success     =  0,
        EarlyExit   = -1,
        MissingArg  = -2,
        InvalidArg  = -3,
    };

    std::string needle;
    MatchType   match_type;

    Options() : match_type(Expansion) {}
} options;

void print_usage(const char* progname)
{
    printf("Usage:  %s [options] search_term\n", progname);
    printf( "\nOptions are:\n"
            "    --regex, -r    The search term is a regular expression.\n"
            );
}

Options::StatusCode parse_args(int argc, char** argv)
{
    using namespace std;

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        const bool is_short_opt = arg.length() && arg.at(0) == '-';
        const bool is_long_opt = arg.compare(0, 2, "--") == 0;

        if (arg == "--help" || arg == "-h")
        {
            print_usage(argv[0]);
            return Options::EarlyExit;
        }
        else if (arg == "--regex" || arg == "-r")
        {
            options.match_type = Regex;
        }
        else if (is_long_opt)
        {
            fprintf(stderr, "Unrecognized option:  %s\n", argv[i]);
            return Options::InvalidArg;
        }
        else if (is_short_opt)
        {
            const char ch = arg.length() >= 2 ? arg.at(0) : ' ';
            fprintf(stderr, "Unrecognized option:  %c\n", ch);
            return Options::InvalidArg;
        }
        else
        {
            options.needle = arg;
        }
    }

    return Options::Success;
}

int main(int argc, char** argv)
{
    Options::StatusCode rv = parse_args(argc, argv);
    if (rv)
        return rv;

    boost::filesystem::path root = ".";
    find_files(root, create_matcher(options.needle, options.match_type), [](const boost::filesystem::path& path) {
        printf("%s\n", path.string().c_str());
    });

    return 0;
}
