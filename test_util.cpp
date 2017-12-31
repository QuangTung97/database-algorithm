#include "util.hpp"
#include "testing/util/util.hpp"
#include <gmock/gmock.h>

TEST(util, set_contains) {
    Field f = "A";
    Field fx = "B";
    make_set(f);
    auto set = make_set(f);
    ASSERT_TRUE(set_contains(set, f));
    ASSERT_FALSE(set_contains(set, fx));
}

TEST(util, make_FD) {
    Field f1 = "X";
    Field f2 = "Y";
    FD fd = make_FD(f1, f2);
    ASSERT_EQ(fd.first, make_set(f1));
    ASSERT_EQ(fd.second, make_set(f2));
}

TEST(util, make_set_with_fields) {
    Field f1 = "X";
    Field f2 = "Y";
    auto result = make_set(f1, f2);
    ASSERT_EQ(result.size(), 2);
    ASSERT_TRUE(result.find(f1) != result.end());
    ASSERT_TRUE(result.find(f2) != result.end());
}

TEST(util, make_set_with_FD) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    
    FD f1 = make_FD(X, Y);
    FD f2 = make_FD(Y, Z);
    FDSet set = make_set(f1, f2);
    ASSERT_EQ(set.size(), 2);
    ASSERT_TRUE(set_contains(set, f1));
    ASSERT_TRUE(set_contains(set, f2));
}

TEST(util, set_union) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    auto set1 = make_set(X, Y);
    auto set2 = make_set(Y, Z);
    auto set = set_union(set1, set2);
    ASSERT_EQ(set.size(), 3);
    ASSERT_TRUE(set_contains(set, X));
    ASSERT_TRUE(set_contains(set, Y));
    ASSERT_TRUE(set_contains(set, Z));
}

TEST(util, set_intersection) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field T = "T";
    auto set1 = make_set(X, Y, Z);
    auto set2 = make_set(Y, Z, T);
    auto set = set_intersection(set1, set2);
    ASSERT_EQ(set.size(), 2);
    ASSERT_TRUE(set_contains(set, Y));
    ASSERT_TRUE(set_contains(set, Z));
}

TEST(util, set_difference) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field T = "T";
    auto set1 = make_set(X, Y, Z);
    auto set2 = make_set(Z, T);
    auto set = set_difference(set1, set2);
    ASSERT_EQ(set.size(), 2);
    ASSERT_TRUE(set_contains(set, X));
    ASSERT_TRUE(set_contains(set, Y));
}

TEST(util, operator_plus) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field T = "T";
    auto set1 = make_set(X, Y);
    auto set2 = make_set(Y, Z, T);
    auto set = set1 + set2;
    ASSERT_EQ(set.size(), 4);
    ASSERT_TRUE(set_contains(set, X));
    ASSERT_TRUE(set_contains(set, Y));
    ASSERT_TRUE(set_contains(set, Z));
    ASSERT_TRUE(set_contains(set, T));
}

TEST(util, operator_minus) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field T = "T";
    auto set1 = make_set(X, Y, Z);
    auto set2 = make_set(Z, T);
    auto set = set1 - set2;
    ASSERT_EQ(set.size(), 2);
    ASSERT_TRUE(set_contains(set, X));
    ASSERT_TRUE(set_contains(set, Y));
}

TEST(util, operator_multiply) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field T = "T";
    auto set1 = make_set(X, Y, Z);
    auto set2 = make_set(Y, Z, T);
    auto set = set1 * set2;
    ASSERT_EQ(set.size(), 2);
    ASSERT_TRUE(set_contains(set, Y));
    ASSERT_TRUE(set_contains(set, Z));
}

TEST(util, equals_fields) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    auto set1 = make_set(X, Y);
    auto set2 = make_set(X, Y);
    auto set3 = make_set(Y, Z);

    ASSERT_TRUE(set1 == set2);
    ASSERT_FALSE(set1 == set3);
}

TEST(util, equals_FDs) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";

    FD f1 = make_FD(X, Y);
    FD f2 = make_FD(Y, Z);
    FD f3 = make_FD(X, Z);

    ASSERT_FALSE(f1 == f3);

    auto set1 = make_set(f1, f2);
    auto set2 = make_set(f1, f2);
    auto set3 = make_set(f1, f3);

    ASSERT_TRUE(set1 == set2);
    ASSERT_TRUE(set1 != set3);
    ASSERT_FALSE(set1 == set3);
}

TEST(util, field_set_from) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field T = "T";

    FD f1 = make_FD(make_set(X, T), Y);
    FD f2 = make_FD(Y, Z);
    FD f3 = make_FD(X, Z);
    auto fd_set = make_set(f1, f2, f3);
    auto field_set = field_set_from(fd_set);

    auto compared_set = make_set(X, Y, Z, T);
    ASSERT_EQ(field_set, compared_set);
}

TEST(util, load_FDSet) {
    std::stringstream in{
        "XY Z\n" 
        "Y Z\n"
        "Z K\n"
    };
    FDSet fds;
    in >> fds;

    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field K = "K";

    FDSet compared_set = make_set(
            make_FD(make_set(X, Y), Z),
            make_FD(Y, Z),
            make_FD(Z, K)
    );
    ASSERT_EQ(fds, compared_set);
}

TEST(util, store_FDSet) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field K = "K";

    FDSet out = make_set(
            make_FD(X, Y),
            make_FD(Y, Z),
            make_FD(Z, K)
    );

    std::stringstream ss;
    FDSet in;
    ss << out;
    ss >> in;

    ASSERT_EQ(in, out);
}

TEST(util, is_subset) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field T = "T";
    Field A = "A";

    auto set = make_set(X, Y, Z, T);
    auto sub = make_set(X, Z);
    auto sub2 = make_set(X, A, Y);
    ASSERT_TRUE(is_subset(sub, set));
    ASSERT_FALSE(is_subset(sub2, set));
}

TEST(util, operator_add_assign) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field T = "T";
    Field A = "A";

    auto set = make_set(X, Y, Z, T);
    auto compared_set = set + make_set(A);
    set += make_set(Z, A);
    ASSERT_EQ(set, compared_set);
}

TEST(util, operator_minus_assign) {
    Field X = "X";
    Field Y = "Y";
    Field Z = "Z";
    Field T = "T";
    Field A = "A";

    auto set = make_set(X, Y, Z, T);
    auto compared_set = set - make_set(Z);
    set -= make_set(Z, A);
    ASSERT_EQ(set, compared_set);
}
