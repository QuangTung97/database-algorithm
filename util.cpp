#include "util.hpp"

FieldSet field_set_from(const FDSet& set) {
    FieldSet result;
    for (auto& e: set) {
        for (auto& f: e.first) {
            result.insert(f);
        }
        for (auto& f: e.second) {
            result.insert(f);
        }
    }
    return result;
}

FieldSet field_set_from(FDSet&& set) {
    return field_set_from(set);
}

// Field
std::ostream& operator << (std::ostream& out, const Field& field) {
    out << field.string_;
    return out;
}

// For FieldSet
std::istream& operator >> (std::istream& in, FieldSet& set) {
    std::string s;
    in >> s;
    for (char ch: s) {
        Field field = std::string{ch};
        set.insert(field);
    }
    return in;
}

std::ostream& operator << (std::ostream& out, const FieldSet& set) {
    for (auto& field: set) {
        out << field;
    }
    return out;
}


// For Functional Dependencies
std::istream& operator >> (std::istream& in, FD& fd) {
    FieldSet f1, f2;
    in >> f1 >> f2;
    fd = make_FD(f1, f2);
    return in;
}

std::ostream& operator << (std::ostream& out, const FD& fd) {
    out << fd.first << " " << fd.second;
    return out;
}

std::ostream& operator << (std::ostream& out, FD&& fd) {
    return out << fd;
}

std::istream& operator >> (std::istream& in, FDSet& set) {
    FD fd;
    while (in >> fd) {
        set.insert(fd);
    }
    return in;
}

std::ostream& operator << (std::ostream& out, const FDSet& set) {
    for (auto& e: set) {
        out << e << "\n";
    }
    return out;
}

std::ostream& operator << (std::ostream& out, FDSet&& set) {
    return out << set;
}
