#ifndef LOSSLESS_DECOMPOSTITION_HPP
#define LOSSLESS_DECOMPOSTITION_HPP

#include "util.hpp"
#include <vector>
#include <algorithm>

namespace detail {

struct Item {
    bool determized = false;
    int row_index;

    bool operator == (const Item& other) const {
        return (determized && other.determized) ? true : 
            determized == other.determized && row_index == other.row_index;
    }
};

using Row = std::vector<Item>;
using Table = std::vector<Row>;
using TableHeader = std::vector<Field>;

int offset_of(const Field& field, const TableHeader& header);

Row init_row(int row_index, const TableHeader& header);

bool row_equals(const TableHeader& header, const Row& row1, 
                const Row& row2, const FieldSet& X);

bool row_homogenize(const TableHeader& header, 
                    Row& row1, Row& row2, const FieldSet& X); 
void init_table(Table& table, const TableHeader& header, 
                const std::vector<FieldSet>& relation_list);

} // namespace detail

bool is_lossless_decomposition(const FieldSet& U, const FDSet& F, 
                                const std::vector<FieldSet>& relation_list);

template <typename ... Sets, typename = all_are_set_t<Sets ...>>
bool is_lossless_decomposition(const FieldSet& U, const FDSet& F, Sets&& ... relations) {
    std::vector<FieldSet> relation_list{relations ...};
    return is_lossless_decomposition(U, F, relation_list);
}

#endif
