#ifndef DB_UTIL_HPP
#define DB_UTIL_HPP

#include <set>
#include <string>
#include <iostream>

template <typename T>
using Set = std::set<T>;

class Field {
private:
    friend std::istream& operator >> (std::istream& in, Field& field);
    friend std::ostream& operator << (std::ostream& out, const Field& field);

    std::string string_;

public:
    Field(const char *s): string_{s} {}

    Field(std::string s): string_{std::move(s)} {}

    Field() = default;

    Field(const Field& other) {
        string_ = other.string_;
    }

    Field(Field&& other) {
        string_ = other.string_;
    }

    bool operator == (const Field& other) const {
        return string_ == other.string_;
    }

    bool operator == (Field&& other) const {
        return string_ == other.string_;
    }

    bool operator < (const Field& other) const {
        return string_ < other.string_;
    }

    bool operator < (Field&& other) const {
        return string_ < other.string_;
    }

    Field& operator = (const Field& other) {
        string_ = other.string_;
        return *this;
    }

    Field& operator = (Field&& other) {
        string_ = other.string_;
        return *this;
    }

    operator Set<Field>() const {
        return Set<Field>{*this};
    }
};

using FieldSet = Set<Field>;

using FD = std::pair<FieldSet, FieldSet>;

using FDSet = Set<FD>;

namespace detail {

template <typename Set>
struct is_set: std::false_type {};

template <typename E>
struct is_set<Set<E>>: std::true_type {};

template <typename T>
using remove_rcv_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <bool Value, bool ... Values>
struct all_of {
    static constexpr bool value = Value && all_of<Values...>::value;
};

template <bool Value>
struct all_of<Value> {
    static constexpr bool value = Value;
};

template <typename ... Sets>
using all_are_set_t = std::enable_if_t<
    all_of<is_set<remove_rcv_t<Sets>>::value ...>::value>;

} // namespace detail

using detail::all_are_set_t;

template <typename Field>
auto make_set(Field&& field) {
    return Set<
        std::remove_cv_t<std::remove_reference_t<Field>>
    >{std::forward<Field>(field)};
}

template <typename Field, typename ... Fields>
auto make_set(Field&& field, Fields&& ... fields) {
    auto set = make_set(std::forward<Fields>(fields)...);
    set.insert(std::forward<Field>(field));
    return set;
}

template <typename Set, typename Element, typename = all_are_set_t<Set>>
bool set_contains(Set&& set, Element&& e) {
    return set.find(std::forward<Element>(e)) != set.end();
}

template <typename ... Args>
FD make_FD(Args&& ... args) {
    return FD{args ...};
}

template <typename Set1, typename Set2, typename = all_are_set_t<Set1, Set2>>
auto set_union(Set1&& set1, Set2&& set2) {
    using ReturnType = std::remove_cv_t<std::remove_reference_t<Set1>>;
    using ReturnType2 = std::remove_cv_t<std::remove_reference_t<Set2>>;
    static_assert(std::is_same<ReturnType, ReturnType2>::value, "Same parameter's types");

    ReturnType result{set1};
    result.insert(set2.begin(), set2.end());
    return result;
}

template <typename Set1, typename Set2, typename = all_are_set_t<Set1, Set2>>
auto operator + (Set1&& set1, Set2&& set2) {
    return set_union(std::forward<Set1>(set1), std::forward<Set2>(set2));
}

template <typename Set1, typename Set2>
all_are_set_t<Set1, Set2>
operator += (Set1&& set1, Set2&& set2) {
    for (auto& e: set2) {
        set1.insert(e);
    }
}

template <typename Set1, typename Set2, typename = all_are_set_t<Set1, Set2>>
auto set_difference(Set1&& set1, Set2&& set2) {
    using ReturnType = std::remove_cv_t<std::remove_reference_t<Set1>>;
    using ReturnType2 = std::remove_cv_t<std::remove_reference_t<Set2>>;
    static_assert(std::is_same<ReturnType, ReturnType2>::value, "Same parameter's types");

    ReturnType result{set1};
    for (auto& e: set2) {
        auto it = result.find(e);
        if (it != result.end())
            result.erase(it);
    }
    return result;
}

template <typename Set1, typename Set2, typename = all_are_set_t<Set1, Set2>>
auto operator - (Set1&& set1, Set2&& set2) {
    return set_difference(std::forward<Set1>(set1), std::forward<Set2>(set2));
}

template <typename Set1, typename Set2>
all_are_set_t<Set1, Set2>
operator -= (Set1&& set1, Set2&& set2) {
    for (auto& e: set2) {
        auto it = set1.find(e);
        if (it != set1.end()) {
            set1.erase(it);
        }
    }
}

template <typename Set1, typename Set2, typename = all_are_set_t<Set1, Set2>>
auto set_intersection(Set1&& set1, Set2&& set2) {
    using ReturnType = std::remove_cv_t<std::remove_reference_t<Set1>>;
    using ReturnType2 = std::remove_cv_t<std::remove_reference_t<Set2>>;
    static_assert(std::is_same<ReturnType, ReturnType2>::value, "Same parameter's types");

    ReturnType result;
    for (auto& e: set2) {
        if (set1.find(e) != set1.end()) {
            result.insert(e);
        }
    }
    return result;
}

template <typename Set1, typename Set2, typename = all_are_set_t<Set1, Set2>>
auto operator * (Set1&& set1, Set2&& set2) {
    return set_intersection(std::forward<Set1>(set1), std::forward<Set2>(set2));
}

template <typename Set1, typename Set2, typename = all_are_set_t<Set1, Set2>>
bool is_subset(Set1&& set1, Set2&& set2) {
    for (auto& e: set1) {
        if (set2.find(e) == set2.end())
            return false;
    }
    return true;
}

FieldSet field_set_from(const FDSet& set);

FieldSet field_set_from(FDSet&& set);

std::istream& operator >> (std::istream& in, FDSet& set);

std::ostream& operator << (std::ostream& out, const FDSet& set);

std::ostream& operator << (std::ostream& out, FDSet&& set);

#endif // DB_UTIL_HPP
