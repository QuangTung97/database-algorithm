#include "lossless_decomposition.hpp"

namespace detail {

int offset_of(const Field& field, const TableHeader& header) {
    auto it = std::find(header.begin(), header.end(), field);
    return it - header.begin();
}

Row init_row(int row_index, const TableHeader& header) {
    Row row(header.size());
    for (auto& item: row) {
        item.determized = false;
        item.row_index = row_index;
    }
    return row;
}

bool row_equals(const TableHeader& header, 
        const Row& row1, const Row& row2, const FieldSet& X) 
{
    return std::all_of(X.begin(), X.end(), 
                [&row1, &row2, &header](const Field& field) {
            int item_offset = offset_of(field, header);
            return row1[item_offset] == row2[item_offset];
    });
}

bool row_homogenize(const TableHeader& header, 
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

void init_table(Table& table, const TableHeader& header, 
        const std::vector<FieldSet>& relation_list) {
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

bool is_lossless_decomposition(const FieldSet& U, 
        const FDSet& F, const std::vector<FieldSet>& relation_list) {
    using namespace detail;

    // Initialize table
    Table table;
    TableHeader header;
    for (auto& field: U) {
        header.push_back(field);
    }

    init_table(table, header, relation_list);

    auto pred = [&table, &header] (const FD& fd) -> bool {
        bool changed = false;
        for (size_t i = 0; i < table.size() - 1; i++)
            for (size_t j = i + 1; j < table.size(); j++) {
                if (row_equals(header, table[i], table[j], fd.first)) {
                    changed = row_homogenize(header, table[i], table[j], fd.second);
                }
            }
        return changed;
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
