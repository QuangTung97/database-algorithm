#include "normal_form.hpp"
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
const Field K = "K";

static bool contains(const std::vector<FieldSet>& vec, const FieldSet& R) {
    auto it = std::find(vec.begin(), vec.end(), R);
    return it != vec.end();
}

TEST(db_algorithm, problem_section_4) {
    std::stringstream input{
        "A BCD\n"
        "CD B\n"
        "EF GH\n"
        "E GIJ\n"
        "I J\n"
    };

    FDSet fds;
    input >> fds;

    auto U = field_set_from(fds);
    U.insert(K);

    ASSERT_EQ(U.size(), 11);

    auto key = candidate_key(U, fds);
    ASSERT_EQ(key, make_set(A, E, F, K));

    auto non_redundant_cover = non_redundant(fds);
    std::stringstream non_redundant_cover_input {
        "A BCD\n"
        "CD B\n"
        "EF GH\n"
        "E GIJ\n"
        "I J\n"
    };
    FDSet expected_non_redundant;
    non_redundant_cover_input >> expected_non_redundant;
    ASSERT_EQ(non_redundant_cover, expected_non_redundant);

    auto minimal = minimal_cover(fds);
    std::stringstream minimal_input{
        "A C\n"
        "A D\n"
        "CD B\n"
        "E G\n"
        "EF H\n"
        "E I\n"
        "I J\n"
    };
    FDSet expected_minimal;
    minimal_input >> expected_minimal;

    ASSERT_EQ(minimal, expected_minimal);
    ASSERT_EQ(minimal, minimal_cover(minimal));

    auto R_list = convert_3nf(U, minimal);
    ASSERT_EQ(R_list.size(), 6);
    ASSERT_TRUE(contains(R_list, make_set(A, C, D)));
    ASSERT_TRUE(contains(R_list, make_set(C, D, B)));
    ASSERT_TRUE(contains(R_list, make_set(E, G, I)));
    ASSERT_TRUE(contains(R_list, make_set(E, F, H)));
    ASSERT_TRUE(contains(R_list, make_set(I, J)));
    ASSERT_TRUE(contains(R_list, make_set(A, E, F, K)));
}
