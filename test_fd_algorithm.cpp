#include "fd_algorithm.hpp"
#include "util.hpp"
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

TEST(db_algorithm, closure_of_field_set) {
    auto R = make_set(A, B, C, D, E, F);
    auto fds = make_set(
            make_FD(make_set(A, B), C),
            make_FD(make_set(B, C), make_set(A, D)),
            make_FD(D, E),
            make_FD(make_set(C, F), B)
    );

    auto result = closure_of(make_set(A, B), fds);
    auto compared_set = make_set(A, B, C, D, E);

    ASSERT_EQ(result, compared_set);
}

TEST(db_algorithm, candidate_key) {
    auto R = make_set(A, B, C, D, E);
    auto fds = make_set(
            make_FD(make_set(A, B), C),
            make_FD(make_set(A, C), B),
            make_FD(make_set(B, C), make_set(D, E))
    );

    auto key = candidate_key(R, fds);
    auto compared_set = make_set(A, C);

    ASSERT_EQ(key, compared_set);
}

TEST(db_algorithm, equivalent_after_remove) {
    auto fds = make_set(
            make_FD(make_set(A, B), C),
            make_FD(make_set(A, C), B),
            make_FD(make_set(B, C), make_set(D, E)),
            make_FD(make_set(B, C, D), make_set(D, E))
    );

    auto fd1 = make_FD(make_set(B, C, D), make_set(D, E));
    auto fd2 = make_FD(make_set(B, C), make_set(D, E));

    ASSERT_TRUE(equivalent_after_remove(fds, fd1));
    fds = fds - make_set(fd1);
    ASSERT_FALSE(equivalent_after_remove(fds, fd2));
}

TEST(db_algorithm, non_redundant) {
    auto fds = make_set(
            make_FD(make_set(A, B), C),
            make_FD(make_set(A, C), B),
            make_FD(make_set(B, C), make_set(D, E)),
            make_FD(make_set(B, C, D), make_set(D, E))
    );

    auto fd = make_FD(make_set(B, C, D), make_set(D, E));
    auto compared_set = fds - make_set(fd);

    auto nonredundant = non_redundant(fds);

    ASSERT_EQ(nonredundant, compared_set);
}

TEST(db_algorithm, equivalent_after_replace) {
    auto fds = make_set(
            make_FD(make_set(A, B), C),
            make_FD(A, C),
            make_FD(make_set(A, C), B),
            make_FD(make_set(B, C), make_set(D, E))
    );
    auto oldfd = make_FD(make_set(A, C), B);
    auto newfd = make_FD(make_set(A), B);

    bool result = equivalent_after_replace(fds, oldfd, newfd);
    ASSERT_TRUE(result);
}

TEST(db_algorithm, equivalent_after_replace2) {
    std::stringstream input{
        "A B\n"
        "ABCD E\n"
        "EF G\n"
        "ACDF E\n"
        "ACDF G\n"
    };

    FDSet fds;
    input >> fds;

    FD replaced = make_FD(make_set(A, B, C, D), E);
    auto replacing1 = make_FD(make_set(B, C, D), E);
    auto replacing2 = make_FD(make_set(A, C, D), E);

    ASSERT_FALSE(equivalent_after_replace(fds, replaced, replacing1));
    ASSERT_TRUE(equivalent_after_replace(fds, replaced, replacing2));
}

TEST(db_algorithm, minimal_cover) {
    auto fds = make_set(
            make_FD(A, make_set(B, C)),
            make_FD(B, C), 
            make_FD(A, B),
            make_FD(make_set(A, B), C)
    );

    auto minimal = minimal_cover(fds);
    auto compared_set = make_set(
            make_FD(A, B),
            make_FD(B, C)
    );
    ASSERT_EQ(minimal, compared_set);
}

TEST(db_algorithm, minimal_cover2) {
    std::stringstream input{
        "A B\n"
        "ABCD E\n"
        "EF G\n"
        "ACDF EG\n"
    };

    std::stringstream output {
        "A B\n"
        "ACD E\n"
        "EF G\n"
    };

    FDSet fds;
    input >> fds;

    FDSet expected;
    output >> expected;
    auto result = minimal_cover(fds);
    ASSERT_EQ(result, expected);
}

TEST(db_algorithm, minimal_cover3) {
    auto U = make_set(A, B, C);
    auto fds = make_set(
            make_FD(B, A),
            make_FD(make_set(A, B), C)
    );

    auto result = minimal_cover(fds);
    auto compared = make_set(
            make_FD(B, A),
            make_FD(B, C)
    );
    ASSERT_EQ(result, compared);
}

TEST(db_algorithm, minimal_cover4) {
    std::stringstream input{
        "A BC\n"
        "CD GHI\n"
        "C EF\n"
        "E F\n"
        "I G\n"
    };

    FieldSet U = {A, B, C, D, E, F, G, H, I};

    std::stringstream output {
        "A B\n"
        "A C\n"
        "C E\n"
        "E F\n"
        "CD H\n"
        "CD I\n"
        "I G\n"
    };

    FDSet fds;
    input >> fds;

    FDSet expected;
    output >> expected;
    auto result = minimal_cover(fds);
    ASSERT_EQ(result, expected);

    auto key = candidate_key(U, fds);
    auto compared_key = make_set(A, D);
    ASSERT_EQ(key, compared_key);
}
