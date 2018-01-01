#include "normal_form.hpp"
#include "lossless_decomposition.hpp"
#include <map>

std::vector<FieldSet> convert_3nf(const FieldSet& U, const FDSet& F) {
    std::vector<FieldSet> result;
    auto remain_fields = U - field_set_from(F);

    std::map<FieldSet, FieldSet> relations;

    for (auto& fd: F) {
        relations[fd.first] += fd.second;
    }

    for (auto& e: relations) {
        result.push_back(e.first + e.second);
    }

    bool lossless = is_lossless_decomposition(U, F, result);
    if (!lossless) {
        remain_fields += candidate_key(U, F);
        result.push_back(remain_fields);
    }
    else if (!remain_fields.empty()) {
        result.push_back(remain_fields);
    }
    
    return result;
}
