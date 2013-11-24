/**
 * @file    filematch.h
 */

#pragma once

#include "findfiles.h"

/**
 * @param[in]   needle      The search term
 * @param[in]   match_type
 *
 * @return  A matcher based on the given constraints
 */
Matcher create_matcher(const std::string& needle, MatchType match_type);
