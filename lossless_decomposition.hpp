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

static int offset_of(const Field& field, const TableHeader& header) {
    auto it = std::find(header.begin(), header.end(), field);
    return it - header.begin();
}

static Row init_row(int row_index, const TableHeader& header) {
    Row row(header.size());
    for (auto& item: row) {
        item.determized = false;
        item.row_index = row_index;
    }
    return row;
}

static bool row_equals(const TableHeader& header, const Row& row1, const Row& row2, const FieldSet& X) {
    return std::all_of(X.begin(), X.end(), 
                [&row1, &row2, &header](const Field& field) {
            int item_offset = offset_of(field, header);
            return row1[item_offset] == row2[item_offset];
    });
}

static bool row_homogenize(const TableHeader& header, 
        Row& row1, Row& row2, const FieldSet& X) {
    bool changed = false;
    for (auto& field: X) {
        auto offset = offset_of(field, header);
        auto& item1 = row1[offset];
        auto& item2 = row2[offset];

        if (item1.determized ^ item2.determized) {
            item1.determized = true;
            item2.determized = true;
            changed = true;
        }
        
        if (!item1.determized && !item2.determized &&
                item1.row_index != item2.row_index) {
            item2.row_index = item1.row_index;
            changed = true;
        }
    }
    return changed;
}

template <typename ... Sets, typename = all_are_set_t<Sets ...>>
static void init_table(Table& table, const TableHeader& header, Sets&& ... relations) {
    std::vector<FieldSet> relation_list{relations ...};

    table.clear();
    int row_index = 0;
    for (auto& relation: relation_list) {
        auto row = init_row(row_index, header);
        for (auto& field: relation) {
            row[offset_of(field, header)].determized = true;
        }
        table.push_back(row);
        row_index++;
    }
}

} // namespace detail

template <typename ... Sets, typename = all_are_set_t<Sets ...>>
bool is_lossless_decomposition(const FieldSet& U, const FDSet& F, Sets&& ... relations) {
    using namespace detail;

    // Initialize table
    Table table;
    TableHeader header;
    for (auto& field: U) {
        header.push_back(field);
    }
    init_table(table, header, std::forward<Sets>(relations) ...);

    auto pred = [&table, &header] (const FD& fd) -> bool {
        for (size_t i = 0; i < table.size() - 1; i++)
            for (size_t j = i + 1; j < table.size(); j++) {
                if (row_equals(header, table[i], table[j], fd.first)) {
                    row_homogenize(header, table[i], table[j], fd.second);
                }
            }
        return false;
    };

    while (std::any_of(F.begin(), F.end(), pred));

    auto satisfy_pred = [](const Row& row) {
        return std::all_of(row.begin(), row.end(), 
                [](const Item& item) { return item.determized; });
    };

    if (std::any_of(table.begin(), table.end(), satisfy_pred))
        return true;
    else
        return false;
}

#endif
