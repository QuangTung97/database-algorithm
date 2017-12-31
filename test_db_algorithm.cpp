#include "db_algorithm.hpp"
#include "util.hpp"
#include <gmock/gmock.h>

TEST(db_algorithm, closure_of_field_set) {
    Field A = "A";
    Field B = "B";
    Field C = "C";
    Field D = "D";
    Field E = "E";
    Field F = "F";

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
    Field A = "A";
    Field B = "B";
    Field C = "C";
    Field D = "D";
    Field E = "E";

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
    Field A = "A";
    Field B = "B";
    Field C = "C";
    Field D = "D";
    Field E = "E";

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
    Field A = "A";
    Field B = "B";
    Field C = "C";
    Field D = "D";
    Field E = "E";

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
    Field A = "A";
    Field B = "B";
    Field C = "C";
    Field D = "D";
    Field E = "E";

    auto fds = make_set(
            make_FD(make_set(A, B), C),
            make_FD(make_set(A, B, C), B),
            make_FD(make_set(B, C), make_set(D, E))
    );
    auto oldfd = make_FD(make_set(A, B, C), B);
    auto newfd = make_FD(make_set(A, C), B);

    bool result = equivalent_after_replace(fds, oldfd, newfd);
    ASSERT_TRUE(result);
}

TEST(db_algorithm, minimal_cover) {
    Field A = "A";
    Field B = "B";
    Field C = "C";

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
