#ifndef DB_ALGORITHM_HPP
#define DB_ALGORITHM_HPP

#include "util.hpp"

FieldSet closure_of(const FieldSet& set, const FDSet& fds);

FieldSet candidate_key(const FieldSet& U, const FDSet& fds);

bool equivalent_after_remove(const FDSet& fds, const FD& fd);

bool equivalent_after_replace(const FDSet& fds, const FD& oldfd, const FD& newfd);

FDSet non_redundant(const FDSet& fds);

FDSet minimal_cover(const FDSet& fds);

#endif
