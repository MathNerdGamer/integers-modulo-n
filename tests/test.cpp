#include <sstream>

#include "math_nerd/int_mod.h"

#define CATCH_DEFINE_MAIN
#include "catch.hpp"

namespace im = math_nerd::int_mod;

TEST_CASE("Testing gcd()")
{
    SECTION("gcd with 1 = 1")
    {
        REQUIRE(im::impl_details::gcd(1, 1234) == 1);
        REQUIRE(im::impl_details::gcd(1, 1) == 1);
        REQUIRE(im::impl_details::gcd(777, 1) == 1);
        REQUIRE(im::impl_details::gcd(1, 66) == 1);
        REQUIRE(im::impl_details::gcd(66, 1) == 1);
    }

    SECTION("gcd with a prime = prime or 1 (if not a factor)")
    {
        REQUIRE(im::impl_details::gcd(2, 1234) == 2);
        REQUIRE(im::impl_details::gcd(1234, 2) == 2);
        REQUIRE(im::impl_details::gcd(7, 14) == 7);
        REQUIRE(im::impl_details::gcd(9, 3) == 3);
        REQUIRE(im::impl_details::gcd(7, 8) == 1);
    }

    SECTION("gcd with multiple factors")
    {
        REQUIRE(im::impl_details::gcd(40320, 3456) == 1152);
        REQUIRE(im::impl_details::gcd(210, 308) == 14);
    }

    SECTION("gcd with negatives")
    {
        REQUIRE(im::impl_details::gcd(40320, -3456) == 1152);
        REQUIRE(im::impl_details::gcd(-7, 14) == 7);
        REQUIRE(im::impl_details::gcd(-9, 3) == 3);
        REQUIRE(im::impl_details::gcd(7, -8) == 1);
        REQUIRE(im::impl_details::gcd(-7, -14) == 7);
    }
}

TEST_CASE("Testing euler_phi()")
{
    SECTION("Euler Phi of Prime = Prime - 1")
    {
        REQUIRE(im::impl_details::euler_phi(7) == 6);
        REQUIRE(im::impl_details::euler_phi(13) == 12);
        REQUIRE(im::impl_details::euler_phi(101) == 100);
        REQUIRE(im::impl_details::euler_phi(983083) == 983082);
    }

    SECTION("Euler Phi of Square-Free Integers are The Product of (PrimeFactor - 1)")
    {
        REQUIRE(im::impl_details::euler_phi(6) == 2);    // 6 = 2 * 3, so euler_phi(6) = 6*(1 - 1/2)(1 - 1/3) = 2
        REQUIRE(im::impl_details::euler_phi(102) == 32); // 102 = 2 * 3 * 17, so euler_phi(102) = 102(1 - 1/2)(1 - 1/3)(1 - 1/17) = 32
        REQUIRE(im::impl_details::euler_phi(95) == 72);
        REQUIRE(im::impl_details::euler_phi(111) == 72);
    }

    SECTION("Euler Phi of n^2 = n * Euler Phi of n")
    {
        REQUIRE(im::impl_details::euler_phi(4) == 2);
        REQUIRE(im::impl_details::euler_phi(9) == 6);
        REQUIRE(im::impl_details::euler_phi(36) == 12);
        REQUIRE(im::impl_details::euler_phi(144) == 48);
    }

    SECTION("Euler Phi with Multiple Factors")
    {
        REQUIRE(im::impl_details::euler_phi(12) == 4);
        REQUIRE(im::impl_details::euler_phi(123456) == 41088);
        REQUIRE(im::impl_details::euler_phi(1337) == 1140);
        REQUIRE(im::impl_details::euler_phi(1000000000) == 400000000);
    }
}

TEST_CASE("Testing ipow()")
{
    SECTION("Powers of 1 (mod N) are Always 1")
    {
        REQUIRE(im::impl_details::ipow<2>(13, 24) == 1);
        REQUIRE(im::impl_details::ipow<12>(13, 89) == 1);
        REQUIRE(im::impl_details::ipow<6>(7, 14564) == 1);
        REQUIRE(im::impl_details::ipow<999>(1, 919293) == 1);
    }

    SECTION("Powers of 0 (mod N) are Always 1")
    {
        REQUIRE(im::impl_details::ipow<13>(13, 24) == 0);
        REQUIRE(im::impl_details::ipow<2>(12, 89) == 0);
        REQUIRE(im::impl_details::ipow<7>(49, 14564) == 0);
        REQUIRE(im::impl_details::ipow<999>(0, 919293) == 0);
    }

    SECTION("General Powers")
    {
        REQUIRE(im::impl_details::ipow<5>(3, 8) == 1);         // 3^2 = 9 = 4 (mod 5), so 3^8 = 4^4 = 256 = 255 + 1 = 1 (mod 5)
        REQUIRE(im::impl_details::ipow<17>(7, 81) == 7);
        REQUIRE(im::impl_details::ipow<1337>(420, 69) == 567);
        REQUIRE(im::impl_details::ipow<1000000000>(123456789, 987654321) == 974933589);
    }
}

TEST_CASE("Testing inverse_of<N>()")
{
    SECTION("Inverses Exist for Numbers Coprime to the Modulus")
    {
        REQUIRE(im::impl_details::inverse_of<13>(12) == 12);
        REQUIRE(im::impl_details::inverse_of<14>(11) == 9);
        REQUIRE(im::impl_details::inverse_of<69>(1337) == 8);
        REQUIRE(im::impl_details::inverse_of<1000000000>(1337) == 325355273);
    }

    SECTION("Inverses Do Not Exist For Numbers With Factors In Common with the Modulus")
    {
        try
        {
            im::impl_details::inverse_of<1234>(2);
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "2 is not invertible modulo 1234 because gcd(2, 1234) = 2, which is not 1.\n");
        }

        try
        {
            im::impl_details::inverse_of<7>(49);
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "49 is not invertible modulo 7 because gcd(49, 7) = 7, which is not 1.\n");
        }

        try
        {
            im::impl_details::inverse_of<12>(22);
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "22 is not invertible modulo 12 because gcd(22, 12) = 2, which is not 1.\n");
        }
    }
}

TEST_CASE("Testing standard_modulo<N>")
{
    SECTION("The Standard Form of Any Multiple of N (mod N) is 0")
    {
        REQUIRE(im::impl_details::standard_modulo<13>(13) == 0);
        REQUIRE(im::impl_details::standard_modulo<25>(25) == 0);
        REQUIRE(im::impl_details::standard_modulo<17>(34) == 0);
        REQUIRE(im::impl_details::standard_modulo<1337>(1787569) == 0);
        REQUIRE(im::impl_details::standard_modulo<420>(28980) == 0);
    }

    SECTION("The Standard Form of Any Integer Between 0 and N-1 (mod N) is That Integer")
    {
        REQUIRE(im::impl_details::standard_modulo<41>(12) == 12);
        REQUIRE(im::impl_details::standard_modulo<71>(1) == 1);
        REQUIRE(im::impl_details::standard_modulo<83>(0) == 0);
        REQUIRE(im::impl_details::standard_modulo<1000>(999) == 999);
        REQUIRE(im::impl_details::standard_modulo<1337>(69) == 69);
    }

    SECTION("The Standard Form of Any Integer > N (mod N) is The Remainder After Division by N")
    {
        REQUIRE(im::impl_details::standard_modulo<2>(15) == 1);
        REQUIRE(im::impl_details::standard_modulo<9>(74) == 2);
        REQUIRE(im::impl_details::standard_modulo<12>(183) == 3);
        REQUIRE(im::impl_details::standard_modulo<88>(188) == 12);
        REQUIRE(im::impl_details::standard_modulo<3>(8) == 2);
    }

    SECTION("The Standard Form of Any Negative Integers")
    {
        REQUIRE(im::impl_details::standard_modulo<15>(-1) == 14);
        REQUIRE(im::impl_details::standard_modulo<2>(-123456) == 0);
        REQUIRE(im::impl_details::standard_modulo<73>(-3) == 70);
        REQUIRE(im::impl_details::standard_modulo<21>(-6) == 15);
        REQUIRE(im::impl_details::standard_modulo<11>(-77) == 0);
    }
}

TEST_CASE("Testing int_mod<N>::modulus()/value()/inverse()")
{
    SECTION("modulus()")
    {
        REQUIRE(im::int_mod<13>(13).modulus() == 13);
        REQUIRE(im::int_mod<14>(28).modulus() == 14);
        REQUIRE(im::int_mod<15>(-1).modulus() == 15);
        REQUIRE(im::int_mod<2>(-123456).modulus() == 2);
        REQUIRE(im::int_mod<1337>(69).modulus() == 1337);
    }

    SECTION("value()")
    {
        REQUIRE(im::int_mod<13>(13).value() == 0);
        REQUIRE(im::int_mod<14>(28).value() == 0);
        REQUIRE(im::int_mod<15>(-1).value() == 14);
        REQUIRE(im::int_mod<2>(-123456).value() == 0);
        REQUIRE(im::int_mod<1337>(69).value() == 69);
    }

    SECTION("inverse()")
    {
        SECTION("Inverses Exist for Numbers Coprime to the Modulus")
        {
            REQUIRE(im::int_mod<13>(12).inverse() == 12);
            REQUIRE(im::int_mod<14>(11).inverse() == 9);
            REQUIRE(im::int_mod<69>(1337).inverse() == 8);
        }

        SECTION("Inverses Do Not Exist For Numbers With Factors In Common with the Modulus")
        {
            try
            {
                im::int_mod<1234>(2).inverse();
            }
            catch( std::invalid_argument const &e )
            {
                REQUIRE(std::string(e.what()) == "2 is not invertible modulo 1234 because gcd(2, 1234) = 2, which is not 1.\n");
            }

            try
            {
                im::int_mod<7>(49).inverse();
            }
            catch( std::invalid_argument const &e )
            {
                REQUIRE(std::string(e.what()) == "0 is not invertible modulo 7 because gcd(0, 7) = 7, which is not 1.\n");
            }

            try
            {
                im::int_mod<12>(22).inverse();
            }
            catch( std::invalid_argument const &e )
            {
                REQUIRE(std::string(e.what()) == "10 is not invertible modulo 12 because gcd(10, 12) = 2, which is not 1.\n");
            }
        }
    }
}

TEST_CASE("Testing Comparison Operators")
{
    SECTION("int_mod<N> == int_mod<N>")
    {
        REQUIRE(im::int_mod<13>(0) == im::int_mod<13>(13));
        REQUIRE(im::int_mod<14>(0) == im::int_mod<14>(28));
        REQUIRE(im::int_mod<15>(14) == im::int_mod<15>(-1));
        REQUIRE(im::int_mod<2>(0) == im::int_mod<2>(-123456));
        REQUIRE(im::int_mod<1337>(69) == im::int_mod<1337>(69));
    }

    SECTION("int_mod<N> == s64")
    {
        REQUIRE(im::int_mod<13>(13) == 0);
        REQUIRE(im::int_mod<14>(28) == 0);
        REQUIRE(im::int_mod<15>(-1) == 14);
        REQUIRE(im::int_mod<2>(-123456) == 0);
        REQUIRE(im::int_mod<1337>(69) == 69);
    }
    
    SECTION("s64 == int_mod<N>")
    {
        REQUIRE(0 == im::int_mod<13>(13));
        REQUIRE(0 == im::int_mod<14>(28));
        REQUIRE(14 == im::int_mod<15>(-1));
        REQUIRE(0 == im::int_mod<2>(-123456));
        REQUIRE(69 == im::int_mod<1337>(69));
    }

    SECTION("int_mod<N> != int_mod<N>")
    {
        REQUIRE(im::int_mod<13>(1) != im::int_mod<13>(13));
        REQUIRE(im::int_mod<14>(-1) != im::int_mod<14>(28));
        REQUIRE(im::int_mod<15>(13) != im::int_mod<15>(-1));
        REQUIRE(im::int_mod<2>(99) != im::int_mod<2>(-123456));
        REQUIRE(im::int_mod<1337>(-69) != im::int_mod<1337>(69));
    }

    SECTION("int_mod<N> != s64")
    {
        REQUIRE(im::int_mod<13>(13) != 1);
        REQUIRE(im::int_mod<14>(28) != -1);
        REQUIRE(im::int_mod<15>(-1) != 13);
        REQUIRE(im::int_mod<2>(-123456) != 99);
        REQUIRE(im::int_mod<1337>(69) != -69);
    }

    SECTION("s64 != int_mod<N>")
    {
        REQUIRE(1 != im::int_mod<13>(13));
        REQUIRE(-1 != im::int_mod<14>(28));
        REQUIRE(13 != im::int_mod<15>(-1));
        REQUIRE(99 != im::int_mod<2>(-123456));
        REQUIRE(-69 != im::int_mod<1337>(69));
    }
}

TEST_CASE("Testing Increment/Decrement Operators")
{
    im::int_mod<13> test_subject1{ 7 };     //  7 (mod  13)
    im::int_mod<81> test_subject2{ -4 };    // 77 (mod  81)
    im::int_mod<145> test_subject3{ 1337 }; // 32 (mod 145)

    SECTION("Prefix / Pre-increment")
    {
        REQUIRE(++test_subject1 == 8);
        REQUIRE(  test_subject1 == 8);

        REQUIRE(++test_subject2 == 78);
        REQUIRE(  test_subject2 == 78);

        REQUIRE(++test_subject3 == 33);
        REQUIRE(  test_subject3 == 33);
    }

    SECTION("Suffix / Post-increment")
    {
        REQUIRE(test_subject1++ == 7);
        REQUIRE(test_subject1   == 8);

        REQUIRE(test_subject2++ == 77);
        REQUIRE(test_subject2   == 78);

        REQUIRE(test_subject3++ == 32);
        REQUIRE(test_subject3   == 33);
    }

    SECTION("Prefix / Pre-decrement")
    {
        REQUIRE(--test_subject1 == 6);
        REQUIRE(  test_subject1 == 6);

        REQUIRE(--test_subject2 == 76);
        REQUIRE(  test_subject2 == 76);

        REQUIRE(--test_subject3 == 31);
        REQUIRE(  test_subject3 == 31);
    }

    SECTION("Suffix / Post-decrement")
    {
        REQUIRE(test_subject1-- == 7);
        REQUIRE(test_subject1   == 6);

        REQUIRE(test_subject2-- == 77);
        REQUIRE(test_subject2   == 76);

        REQUIRE(test_subject3-- == 32);
        REQUIRE(test_subject3   == 31);
    }
}

TEST_CASE("Testing Unary Operators")
{
    im::int_mod<13> test_subject1{ 7 };     //  7 (mod  13)
    im::int_mod<81> test_subject2{ -4 };    // 77 (mod  81)
    im::int_mod<145> test_subject3{ 1337 }; // 32 (mod 145)

    SECTION("+test")
    {
        REQUIRE(+test_subject1 == 7);
        REQUIRE(+test_subject2 == 77);
        REQUIRE(+test_subject3 == 32);
    }

    SECTION("-test")
    {
        REQUIRE(-test_subject1 == 6);
        REQUIRE(-test_subject2 == 4);
        REQUIRE(-test_subject3 == 113);
    }
}

TEST_CASE("Testing Assignment Operators")
{
    im::int_mod<13> test_subject1{ 7 };     //  7 (mod  13)
    im::int_mod<81> test_subject2{ -4 };    // 77 (mod  81)
    im::int_mod<145> test_subject3{ 1337 }; // 32 (mod 145)

    SECTION("Assignment")
    {
        SECTION("s64 version")
        {
            test_subject1 = 18;
            REQUIRE(test_subject1 == 18);

            test_subject2 = -1;
            REQUIRE(test_subject2 == 80);

            test_subject3 = 999;
            REQUIRE(test_subject3 == 129);
        }
    }
    
    SECTION("Addition Assignment")
    {
        SECTION("int_mod<N> version")
        {
            test_subject1 += im::int_mod<13>(18);
            REQUIRE(test_subject1 == 12);

            test_subject2 += im::int_mod<81>(-1);
            REQUIRE(test_subject2 == 76);

            test_subject3 += im::int_mod<145>(999);
            REQUIRE(test_subject3 == 16);
        }

        SECTION("s64 version")
        {
            test_subject1 += 18;
            REQUIRE(test_subject1 == 12);

            test_subject2 += -1;
            REQUIRE(test_subject2 == 76);

            test_subject3 += 999;
            REQUIRE(test_subject3 == 16);
        }
    }

    SECTION("Subtraction Assignment")
    {
        SECTION("int_mod<N> version")
        {
            test_subject1 -= im::int_mod<13>(18);
            REQUIRE(test_subject1 == 2);

            test_subject2 -= im::int_mod<81>(-1);
            REQUIRE(test_subject2 == 78);

            test_subject3 -= im::int_mod<145>(999);
            REQUIRE(test_subject3 == 48);
        }

        SECTION("s64 version")
        {
            test_subject1 -= 18;
            REQUIRE(test_subject1 == 2);

            test_subject2 -= -1;
            REQUIRE(test_subject2 == 78);

            test_subject3 -= 999;
            REQUIRE(test_subject3 == 48);
        }
    }

    SECTION("Multiplication Assignment")
    {
        SECTION("int_mod<N> version")
        {
            test_subject1 *= im::int_mod<13>(18);
            REQUIRE(test_subject1 == 9);

            test_subject2 *= im::int_mod<81>(-1);
            REQUIRE(test_subject2 == 4);

            test_subject3 *= im::int_mod<145>(999);
            REQUIRE(test_subject3 == 68);
        }

        SECTION("s64 version")
        {
            test_subject1 *= 18;
            REQUIRE(test_subject1 == 9);

            test_subject2 *= -1;
            REQUIRE(test_subject2 == 4);

            test_subject3 *= 999;
            REQUIRE(test_subject3 == 68);
        }
    }

    SECTION("Division Assignment")
    {
        SECTION("int_mod<N> version")
        {
            test_subject1 /= im::int_mod<13>(18);
            REQUIRE(test_subject1 == 4);

            test_subject2 /= im::int_mod<81>(-1);
            REQUIRE(test_subject2 == 4);

            test_subject3 /= im::int_mod<145>(999);
            REQUIRE(test_subject3 == 143);
        }

        SECTION("s64 version")
        {
            test_subject1 /= 18;
            REQUIRE(test_subject1 == 4);

            test_subject2 /= -1;
            REQUIRE(test_subject2 == 4);

            test_subject3 /= 999;
            REQUIRE(test_subject3 == 143);
        }
    }

    SECTION("Division Assignment Failure")
    {
        try
        {
            test_subject1 /= 0;
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "0 is not invertible modulo 13 because gcd(0, 13) = 13, which is not 1.\n");
        }

        try
        {
            test_subject2 /= 9;
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "9 is not invertible modulo 81 because gcd(9, 81) = 9, which is not 1.\n");
        }
    }
}

TEST_CASE("Testing Arithmetic Operators")
{
    SECTION("int_mod<N> + int_mod<N>")
    {
        REQUIRE(im::int_mod<13>(12) + im::int_mod<13>(20) == 6);
        REQUIRE(im::int_mod<14>(7) + im::int_mod<14>(-17) == 4);
        REQUIRE(im::int_mod<15>(-1) + im::int_mod<15>(-3) == 11);
        REQUIRE(im::int_mod<2>(-123456) + im::int_mod<2>(123456) == 0);
        REQUIRE(im::int_mod<1337>(420) + im::int_mod<1337>(69) == 489);
    }

    SECTION("int_mod<N> + s64")
    {
        REQUIRE(im::int_mod<13>(12) + 20 == 6);
        REQUIRE(im::int_mod<14>(7) + (-17) == 4);
        REQUIRE(im::int_mod<15>(-1) + (-3) == 11);
        REQUIRE(im::int_mod<2>(-123456) + 123456 == 0);
        REQUIRE(im::int_mod<1337>(420) + 69 == 489);
    }

    SECTION("s64 + int_mod<N>")
    {
        REQUIRE(12 + im::int_mod<13>(20) == 6);
        REQUIRE(7 + im::int_mod<14>(-17) == 4);
        REQUIRE(-1 + im::int_mod<15>(-3) == 11);
        REQUIRE(-123456 + im::int_mod<2>(123456) == 0);
        REQUIRE(420 + im::int_mod<1337>(69) == 489);
    }

    SECTION("int_mod<N> - int_mod<N>")
    {
        REQUIRE(im::int_mod<13>(12) - im::int_mod<13>(20) == 5);
        REQUIRE(im::int_mod<14>(7) - im::int_mod<14>(-17) == 10);
        REQUIRE(im::int_mod<15>(-1) - im::int_mod<15>(-3) == 2);
        REQUIRE(im::int_mod<2>(-123456) - im::int_mod<2>(123456) == 0);
        REQUIRE(im::int_mod<1337>(420) - im::int_mod<1337>(69)  == 351);
    }

    SECTION("int_mod<N> - s64")
    {
        REQUIRE(im::int_mod<13>(12) - 20 == 5);
        REQUIRE(im::int_mod<14>(7) - (-17) == 10);
        REQUIRE(im::int_mod<15>(-1) - (-3) == 2);
        REQUIRE(im::int_mod<2>(-123456) - 123456 == 0);
        REQUIRE(im::int_mod<1337>(420) - 69 == 351);
    }

    SECTION("s64 - int_mod<N>")
    {
        REQUIRE(12 - im::int_mod<13>(20) == 5);
        REQUIRE(7 - im::int_mod<14>(-17) == 10);
        REQUIRE(-1 - im::int_mod<15>(-3) == 2);
        REQUIRE(-123456 - im::int_mod<2>(123456) == 0);
        REQUIRE(420 - im::int_mod<1337>(69) == 351);
    }

    SECTION("int_mod<N> * int_mod<N>")
    {
        REQUIRE(im::int_mod<13>(12) * im::int_mod<13>(20) == 6);
        REQUIRE(im::int_mod<14>(7) * im::int_mod<14>(-17) == 7);
        REQUIRE(im::int_mod<15>(-1) * im::int_mod<15>(-3) == 3);
        REQUIRE(im::int_mod<2>(-123456) * im::int_mod<2>(123456) == 0);
        REQUIRE(im::int_mod<1337>(420) * im::int_mod<1337>(69) == 903);
    }

    SECTION("int_mod<N> * s64")
    {
        REQUIRE(im::int_mod<13>(12) * 20 == 6);
        REQUIRE(im::int_mod<14>(7) * (-17) == 7);
        REQUIRE(im::int_mod<15>(-1) * (-3) == 3);
        REQUIRE(im::int_mod<2>(-123456) * 123456 == 0);
        REQUIRE(im::int_mod<1337>(420) * 69 == 903);
    }

    SECTION("s64 * int_mod<N>")
    {
        REQUIRE(12 * im::int_mod<13>(20) == 6);
        REQUIRE(7 * im::int_mod<14>(-17) == 7);
        REQUIRE(-1 * im::int_mod<15>(-3) == 3);
        REQUIRE(-123456 * im::int_mod<2>(123456) == 0);
        REQUIRE(420 * im::int_mod<1337>(69) == 903);
    }

    SECTION("int_mod<N> / int_mod<N>")
    {
        REQUIRE(im::int_mod<13>(12) / im::int_mod<13>(20) == 11);
        REQUIRE(im::int_mod<14>(7) / im::int_mod<14>(-17) == 7);

        try
        {
            im::int_mod<15>(-1) / im::int_mod<15>(-3);
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "12 is not invertible modulo 15 because gcd(12, 15) = 3, which is not 1.\n");
        }
        
        try
        {
            im::int_mod<2>(-123456) / im::int_mod<2>(123456);
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "0 is not invertible modulo 2 because gcd(0, 2) = 2, which is not 1.\n");
        }
        
        REQUIRE(im::int_mod<1337>(420) / im::int_mod<1337>(69) == 413);
    }

    SECTION("int_mod<N> / s64")
    {
        REQUIRE(im::int_mod<13>(12) / 20 == 11);
        REQUIRE(im::int_mod<14>(7) / (-17) == 7);

        try
        {
            im::int_mod<15>(-1) / (-3);
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "-3 is not invertible modulo 15 because gcd(-3, 15) = 3, which is not 1.\n");
        }

        try
        {
            im::int_mod<2>(-123456) / 123456;
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "123456 is not invertible modulo 2 because gcd(123456, 2) = 2, which is not 1.\n");
        }

        REQUIRE(im::int_mod<1337>(420) / 69 == 413);
    }

    SECTION("s64 / int_mod<N>")
    {
        REQUIRE(12 / im::int_mod<13>(20) == 11);
        REQUIRE(7 / im::int_mod<14>(-17) == 7);

        try
        {
            -1 / im::int_mod<15>(-3);
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "12 is not invertible modulo 15 because gcd(12, 15) = 3, which is not 1.\n");
        }

        try
        {
            -123456 / im::int_mod<2>(123456);
        }
        catch( std::invalid_argument const &e )
        {
            REQUIRE(std::string(e.what()) == "0 is not invertible modulo 2 because gcd(0, 2) = 2, which is not 1.\n");
        }

        REQUIRE(420 / im::int_mod<1337>(69) == 413);
    }
}

TEST_CASE("Testing I/O Operators")
{
    std::stringstream stream;

    im::int_mod<13> test_subject1{ 7 };     //  7 (mod  13)
    im::int_mod<81> test_subject2{ -4 };    // 77 (mod  81)
    im::int_mod<145> test_subject3{ 1337 }; // 32 (mod 145)

    SECTION("Testing Output <<")
    {
        stream << test_subject1;
        REQUIRE(stream.str() == std::string("7"));
        stream.str( std::string() );
        stream.clear();

        stream << test_subject2;
        REQUIRE(stream.str() == std::string("77"));
        stream.str(std::string());
        stream.clear();

        stream << test_subject3;
        REQUIRE(stream.str() == std::string("32"));
        stream.str(std::string());
        stream.clear();
    }
    
    SECTION("Testing Input >>")
    {
        stream.str("81");
        stream >> test_subject1;
        REQUIRE(test_subject1 == 3);
        stream.str(std::string());
        stream.clear();

        stream.str("81");
        stream >> test_subject2;
        REQUIRE(test_subject2 == 0);
        stream.str(std::string());
        stream.clear();

        stream.str("81");
        stream >> test_subject3;
        REQUIRE(test_subject3 == 81);
    }
}
