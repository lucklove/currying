C++[柯里化](https://en.wikipedia.org/wiki/Currying)(Currying)
============================================================

##Example:
```c++
#include <iostream>
#include "currying.hh"

int add3num(int x, int y, int z)
{
    return x + y + z;
}

int main()
{
    std::cout << curry(add3num)(1)(2)(3) << std::endl;      /**< output 6 */

    std::string s1 = "world", s2 = "hello";
    curry([](std::string& l, std::string& r)                /** swap two string */
    {
        std::swap(l, r);
    })(s1)(s2);
    std::cout << s1 + " " + s2 << std::endl;                /**< output "hello world" */
}
```
More examples in [test](test/test.cc)
