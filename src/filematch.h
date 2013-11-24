/**
 * @file    filematch.h
 */

#pragma once

#include "findfiles.h"

enum MatchFileType : int
{
    Directories     = 0x001,
    Files           = 0x010,
};

/**
 * @param[in]   needle              The search term
 * @param[in]   match_type          Algorithm used to match filenames to filter files
 * @param[in]   match_file_type     Types of files to match
 *
 * @return  A matcher based on the given constraints
 */
Matcher create_matcher(const std::string& needle, MatchType match_type, MatchFileType match_file_type);
