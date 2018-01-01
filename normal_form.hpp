#ifndef DB_NORMAL_FORM_HPP
#define DB_NORMAL_FORM_HPP

#include "fd_algorithm.hpp"
#include <vector>

std::vector<FieldSet> convert_3nf(const FieldSet& U, const FDSet& F);

#endif
