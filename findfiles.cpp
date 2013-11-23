#include "findfiles.h"

#include <cstdio>

static void traverse_directory(const boost::filesystem::path& dir, Matcher file_matcher, Emitter emitter)
{
    using namespace boost::filesystem;

    for (auto it = recursive_directory_iterator(dir); it != recursive_directory_iterator(); it++)
    {
        if (file_matcher(*it))
        {
            if (emitter)
                emitter(*it);
        }
    }
}

void find_files(const boost::filesystem::path& root, Matcher file_matcher, Emitter emitter)
{
    using namespace boost::filesystem;

    if (!exists(root))
    {
        fprintf(stderr, "Path does not exist:  %s\n", root);
        throw FindException("path does not exist");
    }

    if (is_directory(root))
        traverse_directory(root, file_matcher, emitter);
    else
        ;   // TODO
}
