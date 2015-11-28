#define TEST_MAIN
#include "UnitTest.hh"
#include "currying.hh"

/** pass by value */
int normal_function(int p1, int p2, int p3, int p4)
{
    return p1 + p2 + p3 + p4;
}

TEST_CASE(curry_normal_function)
{
    TEST_CHECK(curry(normal_function)(1)(2)(3)(4) == 10);   
}

int (*p_to_normal_function)(int, int, int, int) = normal_function;

TEST_CASE(curry_function_pointer)
{
    TEST_CHECK(curry(p_to_normal_function)(5)(4)(3)(2) == 14);   
}

TEST_CASE(stl_function)
{
    /** pass by reference */
    std::function<void(std::string&)> mod = [](std::string& s) { s = "hello"; };
    std::string s;
    curry(mod)(s);
    TEST_CHECK(s == "hello");
}

TEST_CASE(lambda)
{
    /** pass by const refrence */
    TEST_CHECK(curry([](const std::string& s1, const std::string& s2)
    {
        return s1 + " " + s2;
    })("hello")("world") == "hello world");
}

struct T
{
    /** return refrence */
    std::string& operator()(std::string, const std::string&, std::string, std::string& s)
    {
        return s;
    }
};

struct CT
{
    std::string& operator()(std::string& s) const
    {
        return s = "ISO";
    }
};

TEST_CASE(function_object)
{
    std::string s;
    T t;
    const CT ct{};
    static_assert(std::is_reference<decltype(curry(t)(s)(s)(s)(s))>::value, "");
    curry(t)(s)(s)(s)(s) = "curry";
    TEST_CHECK(s == "curry");

    std::string& rs = curry(ct)(s);
    TEST_CHECK(rs == "ISO"); 
}
