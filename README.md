# Integers Modulo N

Header-only wrapper class for arithmetic modulo N using std::int64_t.

# Usage
Here's a basic example:
```
#include <iostream>
#include "int_mod.h"
using namespace math_nerd::int_mod; // For demonstration purposes.

int main()
{
    int_mod<17> a{ 12 };
    
    std::cout << a + 7 << '\n';
    
    return EXIT_SUCCESS;
}
```

Outputs `2`.