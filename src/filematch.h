/*
 * Copyright (c) 2013 findrec developers
 *
 * See the file license.txt for copying permission.
 */

/**
 * @file    filematch.h
 */

#pragma once

#include "findfiles.h"

#include <boost/utility/binary.hpp>

/** Different ways to match files */
enum MatchType
{
    Expansion,  /** Match by basic expansion (e.g. some*text) */
    Regex,      /** Match by regular expression */
};

enum MatchFileType : int
{
    Directories     = BOOST_BINARY(001),
    Files           = BOOST_BINARY(010),
};

/**
 * @param[in]   needle              The search term
 * @param[in]   match_type          Algorithm used to match filenames to filter files
 * @param[in]   match_file_type     Types of files to match
 *
 * @return  A matcher based on the given constraints
 */
Matcher create_matcher(const std::string& needle, MatchType match_type, MatchFileType match_file_type);
