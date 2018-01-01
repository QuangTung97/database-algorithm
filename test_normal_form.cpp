#include "normal_form.hpp"
#include <gmock/gmock.h>
#include "lossless_decomposition.hpp"

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

static bool contains(const std::vector<FieldSet>& vec, const FieldSet& R) {
    auto it = std::find(vec.begin(), vec.end(), R);
    return it != vec.end();
}

TEST(normal_form, convert_3nf_non_lossless_decomposition) {
    const auto fds = make_set(
                make_FD(A, B),
                make_FD(make_set(A, C, D), E),
                make_FD(make_set(E, F), G));
    const auto U = field_set_from(fds) + make_set(H);

    ASSERT_EQ(U.size(), 8);

    auto key = candidate_key(U, fds);
    ASSERT_EQ(key, make_set(A, C, D, F, H));

    std::vector<FieldSet> R_list = convert_3nf(U, fds);
    ASSERT_TRUE(contains(R_list, make_set(A, C, D, F, H)));
}

TEST(normal_form, convert_3nf_when_non_lossless_decomposition_non_empty_remains) {
    const auto fds = make_set(
                make_FD(A, B),
                make_FD(make_set(A, C, D), E),
                make_FD(make_set(E, F), G));
    const auto U = field_set_from(fds) + make_set(H);

    std::vector<FieldSet> R_list = convert_3nf(U, fds);

    ASSERT_EQ(R_list.size(), 4);
    ASSERT_TRUE(contains(R_list, make_set(A, B)));
    ASSERT_TRUE(contains(R_list, make_set(A, C, D, E)));
    ASSERT_TRUE(contains(R_list, make_set(E, F, G)));
    ASSERT_TRUE(contains(R_list, make_set(A, C, D, F, H)));

    ASSERT_TRUE(is_lossless_decomposition(U, fds, R_list));
}

TEST(normal_form, convert_3nf_when_lossless_decomposition_empty_remains) {
    const auto fds = make_set(
                make_FD(A, B),
                make_FD(make_set(A, C, D, F), E),
                make_FD(make_set(E, F), G));
    const auto U = field_set_from(fds);

    ASSERT_EQ(fds, minimal_cover(fds));
    ASSERT_EQ(candidate_key(U, fds), make_set(A, C, D, F));

    std::vector<FieldSet> R_list = convert_3nf(U, fds);

    ASSERT_EQ(R_list.size(), 3);
    ASSERT_TRUE(contains(R_list, make_set(A, B)));
    ASSERT_TRUE(contains(R_list, make_set(A, C, D, F, E)));
    ASSERT_TRUE(contains(R_list, make_set(E, F, G)));
}

TEST(normal_form, convert_3nf_when_non_lossless_decomposition_empty_remains) {
    const auto fds = make_set(
                make_FD(A, B),
                make_FD(make_set(A, C, D), E),
                make_FD(make_set(E, F), G));
    const auto U = field_set_from(fds);

    ASSERT_EQ(fds, minimal_cover(fds));

    std::vector<FieldSet> R_list = convert_3nf(U, fds);

    ASSERT_EQ(make_set(A, C, D, F), candidate_key(U, fds));

    ASSERT_EQ(R_list.size(), 4);
    ASSERT_TRUE(contains(R_list, make_set(A, B)));
    ASSERT_TRUE(contains(R_list, make_set(A, C, D, E)));
    ASSERT_TRUE(contains(R_list, make_set(E, F, G)));
    ASSERT_TRUE(contains(R_list, make_set(A, C, D, F)));
}
