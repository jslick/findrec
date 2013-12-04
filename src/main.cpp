/*
 * Copyright (c) 2013 findrec developers
 *
 * See the file license.txt for copying permission.
 */

#include "findfiles.h"
#include "filematch.h"

#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <boost/utility/binary.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#  define IS_WINDOWS 1
#endif

static struct Options
{
    enum StatusCode : int
    {
        Success     =  0,
        EarlyExit   = -1,
        MissingArg  = -2,
        InvalidArg  = -3,
    };

    enum ExecMode
    {
        NoExec      = BOOST_BINARY(000),
        ExecAll     = BOOST_BINARY(001),
        ExecEach    = BOOST_BINARY(010),
    };

    std::string     needle;
    MatchType       match_type;
    MatchFileType   match_file_type;

    ExecMode        exec_mode;
    std::vector<std::string> exec_command;

    Options()
    : match_type(Expansion),
      match_file_type((MatchFileType)(Directories | Files)),
      exec_mode(NoExec)
    {}
} options;

static void print_usage(const char* progname)
{
    printf("Usage:  %s [options] search_term\n", progname);
    printf( "\nOptions are:\n"
            "    --regex, -r    The search term is a regular expression.\n"
            "    --directories, -d\n"
            "                   Match only directories.\n"
            "    --files, -f    Match only files that are not directories.\n"
            "    --exec-all command\n"
            "                   Execute a command with the matched files as arguments.\n"
            "                   Specify $* to specify where in the command matched paths\n"
            "                   should be placed.  If omitted, they will be appended to the\n"
            "                   end of the command.\n"
            );
}

static Options::StatusCode parse_args(int argc, char** argv)
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
        else if (arg == "--directories" || arg == "-d")
        {
            options.match_file_type = (MatchFileType)(options.match_file_type & ~Files);
        }
        else if (arg == "--files" || arg == "-f")
        {
            options.match_file_type = (MatchFileType)(options.match_file_type & ~Directories);
        }
        else if (arg == "--exec-all")
        {
            options.exec_mode = Options::ExecAll;
            if (i >= argc - 1)
            {
                fprintf(stderr, "Missing argument to %s\n", argv[i]);
                return Options::MissingArg;
            }
            while (i < argc - 1)
            {
                string arg = argv[++i];
                if (arg == "end")
                    break;
                options.exec_command.push_back(std::move(arg));
            }
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

static void matched_plain_printer(const boost::filesystem::path& path)
{
    printf("%s\n", path.string().c_str());
}

static void matched_vector_pusher(std::vector<boost::filesystem::path>& matched_paths, const boost::filesystem::path& path)
{
    matched_paths.push_back(path);
    matched_plain_printer(path);
}

// Create command from executable, params, and matched paths
static std::string create_command(const std::vector<std::string>& exe_pieces, const std::vector<boost::filesystem::path>& paths)
{
    using namespace std;

    // Concatenate matched paths
    string paths_param;
    for (size_t i = 0; i < paths.size(); i++)
    {
        string absolute_path = boost::filesystem::canonical(paths.at(i)).string();
        paths_param += " \"" + absolute_path + "\"";
    }

    // Concatenate exec, params, and matched paths
    string command;
    bool contains_placement = false;
    for (size_t i = 0; i < exe_pieces.size(); i++)
    {
        if (exe_pieces.at(i) == "$*")  // Look for placement parameter
        {
            contains_placement = true;
            command += " " + paths_param;
        }
        else
            command += " \"" + exe_pieces.at(i) + "\" ";
    }
    // If placement parameter is not specified, put the matched paths at the end
    if (!contains_placement)
        command += paths_param;

    #if IS_WINDOWS
    command = "\"" + command + "\"";    // cmd gets really confused about spaces and quotations
    #endif
    return command;
}

int main(int argc, char** argv)
{
    using namespace std;
    using namespace std::placeholders;

    Options::StatusCode rv = parse_args(argc, argv);
    if (rv)
        return rv;

    boost::filesystem::path root = ".";
    Matcher matcher = create_matcher(options.needle, options.match_type, options.match_file_type);

    // Figure out what callback to call on match
    function<void(const boost::filesystem::path&)> match_callback = matched_plain_printer;
    vector<boost::filesystem::path> matched_paths;
    if (options.exec_mode == Options::ExecAll)
        match_callback = std::bind(&matched_vector_pusher, std::ref(matched_paths), _1);

    find_files(root, matcher, match_callback);

    if (options.exec_mode == Options::ExecAll && options.exec_command.size() && matched_paths.size())
    {
        const std::string command = create_command(options.exec_command, matched_paths);
        #if 0
        printf("Executing:\n%s\n", command.c_str());
        #endif
        system(command.c_str());
    }

    return 0;
}
