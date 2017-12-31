#include "lossless_decomposition.hpp"
#include <gmock/gmock.h>

const Field A = "A";
const Field B = "B";
const Field C = "C";
const Field D = "D";
const Field E = "E";
const Field F = "F";
const Field G = "G";
const Field H = "H";
const Field I = "I";
const Field J = "J";

using namespace detail;

const TableHeader header{A, B, C, D, E};

TEST(lossless_decomposition, detail_offset_of) {
    ASSERT_EQ(offset_of(A, header), 0);
    ASSERT_EQ(offset_of(E, header), 4);
}

TEST(lossless_decomposition, detail_init_row) {
    Row row = init_row(3, header);
    ASSERT_EQ(row.size(), header.size());

    auto item = Item{false, 3};
    ASSERT_EQ(row[0], item);
    ASSERT_EQ(row[1], item);
    ASSERT_EQ(row[2], item);
    ASSERT_EQ(row[3], item);
    ASSERT_EQ(row[4], item);
}

TEST(lossless_decomposition, row_equals) 
{
    Row row1{
        {true, 1}, {false, 1}, {true, 1}, {false, 1}, {false, 1}
    };

    Row row2{
        {true, 2}, {false, 2}, {true, 2}, {false, 2}, {false, 2}
    };

    Row row3{
        {true, 3}, {false, 3}, {true, 3}, {false, 1}, {false, 3}
    };

    Row row4{
        {false, 1}, {false, 1}, {true, 1}, {false, 1}, {false, 1}
    };


    ASSERT_TRUE(row_equals(header, row1, row2, make_set(A, C)));
    ASSERT_FALSE(row_equals(header, row1, row4, make_set(A, C)));
    ASSERT_FALSE(row_equals(header, row1, row2, make_set(A, D)));
    ASSERT_TRUE(row_equals(header, row1, row3, make_set(A, D)));
}

TEST(lossless_decomposition, init_table) {
    Table table;
    auto R1 = make_set(A, B, C);
    auto R2 = make_set(A, C, D);
    auto R3 = make_set(B, E);

    init_table(table, header, R1, R2, R3);

    Table expected = {
        {{true, 0}, {true, 0}, {true, 0}, {false, 0}, {false, 0}},
        {{true, 1}, {false, 1}, {true, 1}, {true, 1}, {false, 1}},
        {{false, 2}, {true, 2}, {false, 2}, {false, 2}, {true, 2}},
    };

    ASSERT_EQ(table, expected);
}

TEST(lossless_decomposition, row_homogenize) {
    auto U = make_set(A, B, C, D, E);
    Row row1{
        {true, 1}, {false, 1}, {true, 1}, {false, 1}, {false, 1},
    };

    Row expected_row1{
        {true, 1}, {false, 1}, {true, 1}, {false, 1}, {true, 1},
    };

    Row row2{
        {false, 2}, {false, 2}, {true, 2}, {false, 2}, {true, 2},
    };

    Row expected_row2 {
        {false, 2}, {false, 1}, {true, 2}, {false, 2}, {true, 2},
    };

    ASSERT_FALSE(row_equals(header, row1, expected_row1, U));

    bool changed = row_homogenize(header, row1, row2, make_set(B, C, E));

    ASSERT_TRUE(row_equals(header, row1, expected_row1, U));
    ASSERT_TRUE(row_equals(header, row2, expected_row2, U));
    ASSERT_TRUE(changed);

    changed = row_homogenize(header, row1, row1, U);
    ASSERT_FALSE(changed);
}

TEST(lossless_decomposition, is_lossless_decomposition_example1) {
    auto R1 = make_set(A, B);
    auto R2 = make_set(B, D);
    auto R3 = make_set(A, B, C);
    auto R4 = make_set(B, C, D);

    auto F = make_set(
            make_FD(A, C),
            make_FD(B, C),
            make_FD(make_set(C, D), B),
            make_FD(C, D)
    );
    auto U = field_set_from(F);

    bool result = is_lossless_decomposition(U, F, R1, R2, R3, R4);
    ASSERT_TRUE(result);
}

TEST(lossless_decomposition, is_lossless_decomposition_example2) {
    auto R1 = make_set(A, B);
    auto R2 = make_set(C, D);

    auto F = make_set(
            make_FD(A, B),
            make_FD(C, D)
    );
    auto U = field_set_from(F);

    ASSERT_FALSE(is_lossless_decomposition(U, F, R1, R2));
}

TEST(lossless_decomposition, is_lossless_decomposition_example3) {
    auto R1 = make_set(A, B);
    auto R2 = make_set(B, C);

    auto F = make_set(
            make_FD(make_set(A, B), C),
            make_FD(C, B)
    );
    auto U = field_set_from(F);

    ASSERT_FALSE(is_lossless_decomposition(U, F, R1, R2));
}

TEST(lossless_decomposition, is_lossless_decomposition_example4) {
    auto R1 = make_set(A, B);
    auto R2 = make_set(B, C);

    auto F = make_set(
            make_FD(A, B),
            make_FD(B, C),
            make_FD(C, A)
    );
    auto U = field_set_from(F);

    ASSERT_TRUE(is_lossless_decomposition(U, F, R1, R2));
}
