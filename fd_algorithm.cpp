#include "fd_algorithm.hpp"
#include <algorithm>

FieldSet closure_of(const FieldSet& set, const FDSet& fds) {
    FieldSet result = set;
    FieldSet X, Y;
    auto pred = [&X, &Y, &result](auto fd) {
        X = fd.first;
        Y = fd.second;
        return X.size() > 0 && is_subset(X, result) && !is_subset(Y, result);
    };

    while (std::any_of(fds.begin(), fds.end(), pred)) {
        result = result + Y;
    }
    return result;
}

FieldSet candidate_key(const FieldSet& U, const FDSet& fds) {
    FieldSet result = U;
    for (auto& field: U) {
        result.erase(field);
        if (closure_of(result, fds) != U) {
            result.insert(field);
        }
    }
    return result;
}

bool equivalent_after_remove(const FDSet& fds, const FD& fd) {
    FDSet removed = fds - make_set(fd);
    auto Y = closure_of(fd.first, removed);
    if (is_subset(fd.second, Y)) 
        return true;
    else
        return false;
}

bool equivalent_after_replace(const FDSet& fds, const FD& oldfd, const FD& newfd) {
    FDSet newfds = fds;
    newfds.erase(oldfd);
    newfds.insert(newfd);

    auto Y1 = closure_of(newfd.first, fds);
    auto Y2 = closure_of(oldfd.first, newfds); 
    if (is_subset(newfd.second, Y1) && is_subset(oldfd.second, Y2))
        return true;
    else
        return false;
}

FDSet non_redundant(const FDSet& fds) {
    FDSet result = fds;
    for (auto& fd: fds) {
        if (equivalent_after_remove(result, fd))
            result.erase(fd);
    }
    return result;
}

FDSet minimal_cover(const FDSet& fds) {
    FDSet result;
    // Step 1 
    for (auto& fd: fds) {
        for (auto& field: fd.second) {
            result.insert(make_FD(fd.first, field));
        }
    }

    // Step 2 
    auto copy = result;
    for (auto& fd: copy) {
        auto modified_set = fd.first;
        for (auto& field: fd.first) {
            modified_set.erase(field);
            if (modified_set.size() == 0)
                break;

            if (equivalent_after_replace(result, fd, make_FD(modified_set, fd.second))) {
                result.erase(fd);
                result.insert(make_FD(modified_set, fd.second));
                break;
            }
            modified_set.insert(field);
        }
    }

    // Step 3
    return non_redundant(result);
}
