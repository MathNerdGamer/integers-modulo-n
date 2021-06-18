/*
MIT License

Copyright (c) 2018 Math Nerd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#ifndef MATH_NERD_INT_MOD_H
#define MATH_NERD_INT_MOD_H

/** \file int_mod.h
    \brief std::int64_t wrapper for arithmetic modulo N.
 */
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <utility>

 /** \namespace math_nerd
     \brief Namespace for all of my projects.
  */
namespace math_nerd
{
    /** \namespace math_nerd::int_mod
        \brief Namespace for int_mod<N> implementation.
     */
    namespace int_mod
    {

        /** \name Signed 64-bit integer
         */
        using s64 = std::int64_t;


        /** \namespace math_nerd::int_mod::impl_details
            \brief Contains implementation details.
         */
        namespace impl_details
        {
            /** \fn constexpr auto gcd(s64 a, s64 b) noexcept -> s64
                \brief Computes the greatest common divisor of two integers using the Euclidean algorithm.
             */
            constexpr auto gcd(s64 a, s64 b) noexcept -> s64;

            /** \fn constexpr auto euler_phi(s64 n) noexcept -> s64
                \brief Computes the Euler phi of an integer.

                Uses the product formula: \f$\displaystyle{\varphi\left(N\right) = n\prod_{p\mid n}\left(1-\frac{1}{p}\right)}\f$
             */
            constexpr auto euler_phi(s64 N) noexcept -> s64;

            /** \fn constexpr auto ipow(s64 const base, s64 const exponent) -> s64
                \brief Computes base to the power exponent modulo N.
                \details This function uses a divide-and-conquer algorithm which takes advantage of integer division to
                         divide the exponent in half at each level of recursion until an exponent of 0 is reached, for
                         which it returns 1. It then multiplies the results together while taking remainders after division by N.
                         Throws std::invalid_argument if exponent is negative.
             */
            template<s64 N>
            constexpr auto ipow(s64 const base, s64 const exponent)->s64;

            /** \fn auto inverse_of(s64 const n) -> s64
                \brief Computes the inverse of an integer modulo N. Throws std::invalid_argument if not invertible.
                \brief By Euler's theorem, \f$a^{\phi\left(N\right)}\equiv 1\ \left(\mathrm{mod}\ N\right)\f$ for \f$\gcd\left(a,N\right)=1,\f$
                       which implies that that \f$a^{\phi\left(N\right)-1}\equiv a^{-1}\ \left(\mathrm{mod}\ N\right)\f$ under the same condition.
             */
            template <s64 N>
            constexpr auto inverse_of(s64 const n)->s64;

            /** \fn auto standard_modulo(s64 rhs) -> s64
                \brief Returns the standard form of rhs modulo N. The standard form is the integer
                       between 0 and N-1 (inclusive) which is equivalent to rhs modulo N.
             */
            template <s64 N>
            constexpr auto standard_modulo(s64 rhs)->s64;

        } // namespace impl_details


        /** \class int_mod<N>
            \brief Wrapper for 64-bit integer for arithmetic modulo N.
         */
        template <s64 N>
        class int_mod
        {
            static_assert(N > 1, "Modulus N of int_mod<N> must be at least 2.");
            static_assert(N <= 1000000000, "Modulus N > 100000000 of int_mod<N> are currently not supported due to lack of adequate overflow checks.");

        private:
            /** \property s64 element_
                \brief The integer which will be taken modulo N. Default initializes to 0.
             */
            s64 element_{ 0 };

        public:
            constexpr int_mod() = default;

            constexpr int_mod(s64 num) noexcept
            {
                element_ = impl_details::standard_modulo<N>(num);
                element_ %= N;
            }

            /** \fn constexpr auto modulus() const noexcept -> s64
                \brief Returns the modulus N.
             */
            constexpr auto modulus() const noexcept -> s64
            {
                return N;
            }

            /** \fn constexpr auto value() const noexcept -> s64
                \brief Returns the stored value.
             */
            constexpr auto value() const noexcept -> s64
            {
                return element_;
            }

            /** \fn constexpr auto inverse() const -> s64
                \brief Returns the inverse modulo N of the stored value. Throws std::invalid_argument if not invertible.
             */
            constexpr auto inverse() const -> s64
            {
                s64 inv;
                try
                {
                    inv = impl_details::inverse_of<N>(element_);
                }
                catch( std::invalid_argument const & )
                {
                    throw;
                }
                return inv;
            }

            /** \fn constexpr explicit operator s64() const
                \brief Explicit type conversion back to a signed 64-bit integer.
             */
            constexpr explicit operator s64() const
            {
                return element_;
            }

            /** \name Increment/Decrement operators */
            /** \fn constexpr auto operator++() noexcept -> int_mod<N> &
                \brief Pre-increments element_ and reduces modulo N.
             */
            constexpr auto operator++() noexcept -> int_mod<N> &;

            /** \fn constexpr auto operator++(int) noexcept -> int_mod<N>
                \brief Post-increments element_ and reduces modulo N.
             */
            constexpr auto operator++(int) noexcept -> int_mod<N>;

            /** \fn constexpr auto operator--() noexcept -> int_mod<N> &
                \brief Pre-decrements element_ and reduces modulo N.
             */
            constexpr auto operator--() noexcept -> int_mod<N> &;

            /** \fn constexpr auto operator--(int) noexcept -> int_mod<N>
                \brief Post-decrements element_ and reduces modulo N.
             */
            constexpr auto operator--(int) noexcept -> int_mod<N>;

            /** \name Unary operators */
            /** \fn constexpr auto operator+() const noexcept -> int_mod<N>
                \brief Returns the *this.
             */
            constexpr auto operator+() const noexcept -> int_mod<N>;

            /** \fn constexpr auto operator-() const noexcept -> int_mod<N>
                \brief Returns the additive inverse modulo N.
             */
            constexpr auto operator-() const noexcept -> int_mod<N>;

            /** \name Assignment operators */
            /** \fn constexpr auto operator+=(int_mod<N> const rhs) noexcept -> int_mod<N> &
                \brief Adds rhs to element_ and reduces modulo N.
             */
            constexpr auto operator+=(int_mod<N> const rhs) noexcept -> int_mod<N> &;

            /** \fn constexpr auto operator-=(int_mod<N> const rhs) noexcept -> int_mod<N> &
                \brief Subtracts rhs from element_ and reduces modulo N.
             */
            constexpr auto operator-=(int_mod<N> const rhs) noexcept -> int_mod<N> &;

            /** \fn constexpr auto operator*=(int_mod<N> const rhs) noexcept -> int_mod<N> &
                \brief Multiples rhs to element_ and reduces modulo N.
             */
            constexpr auto operator*=(int_mod<N> const rhs) noexcept -> int_mod<N> &;

            /** \fn constexpr auto operator/=(int_mod<N> const rhs) -> int_mod<N> &
                \brief Divides rhs, if invertible modulo N, from element_ and reduces modulo N. Throws std::invalid_argument if rhs is not invertible.
             */
            constexpr auto operator/=(int_mod<N> const rhs)->int_mod<N> &;

            /** \fn constexpr auto operator=(s64 rhs) noexcept -> int_mod<N> &
                \brief Assigns rhs to element_ and reduces modulo N.
             */
            constexpr auto operator=(s64 rhs) noexcept -> int_mod<N> &;

            /** \fn constexpr auto operator+=(s64 rhs) noexcept -> int_mod<N> &
                \brief Adds rhs to element_ and reduces modulo N.
             */
            constexpr auto operator+=(s64 rhs) noexcept -> int_mod<N> &;

            /** \fn constexpr auto operator-=(s64 rhs) noexcept -> int_mod<N> &
                \brief Subtracts rhs from element_ and reduces modulo N.
             */
            constexpr auto operator-=(s64 rhs) noexcept -> int_mod<N> &;

            /** \fn constexpr auto operator*=(s64 rhs) noexcept -> int_mod<N> &
                \brief Multiples rhs to element_ and reduces modulo N.
             */
            constexpr auto operator*=(s64 rhs) noexcept -> int_mod<N> &;

            /** \fn constexpr auto operator/=(s64 rhs) -> int_mod<N> &
                \brief Divides rhs, if invertible modulo N, from element_ and reduces modulo N. Throws std::invalid_argument if rhs is not invertible.
             */
            constexpr auto operator/=(s64 rhs)->int_mod<N> &;


            /** \name Comparison operators */
            // int_mod<N> versions
            /** \fn constexpr auto operator==(int_mod<N> const rhs) const noexcept -> bool
                \brief Compares the values and returns true if equal.
             */
            constexpr auto operator==(int_mod<N> const rhs) const noexcept -> bool;

            /** \fn  constexpr auto operator!=(int_mod<N> const rhs) const noexcept -> bool
                \brief Compares the values and returns false if equal.
             */
            constexpr auto operator!=(int_mod<N> const rhs) const noexcept -> bool;

            // s64 versions
            /** \fn constexpr auto operator==(s64 rhs) const noexcept -> bool
                \brief Compares the values and returns true if equal.
             */
            constexpr auto operator==(s64 rhs) const noexcept -> bool;

            /** \fn constexpr auto operator!=(s64 rhs) const noexcept -> bool
                \brief Compares the values and returns false if equal.
             */
            constexpr auto operator!=(s64 rhs) const noexcept -> bool;
        };

        // Increment/Decrement Operators
        template <s64 N>
        constexpr auto int_mod<N>::operator++() noexcept -> int_mod<N> &
        {
            if( element_ == N - 1 )
            {
                element_ = 0;
            }
            else
            {
                ++element_;
            }

            return *this;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator++(int) noexcept -> int_mod<N>
        {
            int_mod<N> tmp(*this);
            operator++();
            return tmp;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator--() noexcept -> int_mod<N> &
        {
            if( element_ == 0 )
            {
                element_ = N - 1;
            }
            else
            {
                --element_;
            }

            return *this;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator--(int) noexcept -> int_mod<N>
        {
            int_mod<N> tmp(*this);
            operator--();
            return tmp;
        }

        // Unary operators
        template <s64 N>
        constexpr auto int_mod<N>::operator+() const noexcept -> int_mod<N>
        {
            return *this;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator-() const noexcept -> int_mod<N>
        {
            return N - element_;
        }

        // Assignment operators
        // int_mod<N> versions
        template <s64 N>
        constexpr auto int_mod<N>::operator+=(int_mod<N> const rhs) noexcept -> int_mod<N> &
        {
            element_ += rhs.value();
            element_ %= N;

            return *this;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator-=(int_mod<N> const rhs) noexcept -> int_mod<N> &
        {
            if( rhs.value() > element_ )
            {
                element_ += N - rhs.value();
                element_ = impl_details::standard_modulo<N>(element_);
            }
            else
            {
                element_ -= rhs.value();
            }

            return *this;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator*=(int_mod<N> const rhs) noexcept -> int_mod<N> &
        {
            element_ *= rhs.value();
            element_ %= N;

            return *this;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator/=(int_mod<N> const rhs) -> int_mod<N> &
        {
            try
            {
                element_ *= rhs.inverse();
                element_ %= N;
            }
            catch( std::invalid_argument const & )
            {
                throw;
            }

            return *this;
        }

        // s64 versions
        template <s64 N>
        constexpr auto int_mod<N>::operator=(s64 rhs) noexcept -> int_mod<N> &
        {
            element_ = impl_details::standard_modulo<N>(rhs);

            return *this;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator+=(s64 rhs) noexcept -> int_mod<N> &
        {
            element_ += impl_details::standard_modulo<N>(rhs);

            element_ %= N;

            return *this;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator-=(s64 rhs) noexcept -> int_mod<N> &
        {
            rhs = impl_details::standard_modulo<N>(rhs);

            if( rhs > element_ )
            {
                element_ += N - rhs;
                element_ = impl_details::standard_modulo<N>(element_);
            }
            else
            {
                element_ -= rhs;
            }

            return *this;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator*=(s64 rhs) noexcept -> int_mod<N> &
        {
            element_ *= impl_details::standard_modulo<N>(rhs);
            element_ %= N;

            return *this;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator/=(s64 rhs) -> int_mod<N> &
        {
            try
            {
                rhs = impl_details::inverse_of<N>(rhs);
            }
            catch( std::invalid_argument const & )
            {
                throw;
            }

            element_ *= rhs;
            element_ %= N;

            return *this;
        }

        // Comparison operators
        // int_mod<N> versions
        template <s64 N>
        constexpr auto int_mod<N>::operator==(int_mod<N> const rhs) const noexcept -> bool
        {
            return element_ == rhs.value();
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator!=(int_mod<N> const rhs) const noexcept -> bool
        {
            return element_ != rhs.value();
        }

        // s64 versions
        template <s64 N>
        constexpr auto int_mod<N>::operator==(s64 rhs) const noexcept -> bool
        {
            rhs = impl_details::standard_modulo<N>(rhs);

            return element_ == rhs;
        }

        template <s64 N>
        constexpr auto int_mod<N>::operator!=(s64 rhs) const noexcept -> bool
        {
            rhs = impl_details::standard_modulo<N>(rhs);

            return element_ != rhs;
        }

        /** \name Arithmetic operators. */

        // int_mod<N> versions
        /** \fn constexpr auto operator+(int_mod<N> lhs, int_mod<N> rhs) noexcept -> int_mod<N>
            \brief Returns the result of adding two int_mod<N>.
         */
        template <s64 N>
        constexpr auto operator+(int_mod<N> lhs, int_mod<N> rhs) noexcept -> int_mod<N>
        {
            lhs += rhs;
            return lhs;
        }

        /** \fn constexpr auto operator-(int_mod<N> lhs, int_mod<N> rhs) noexcept -> int_mod<N>
            \brief Returns the result of subtracting two int_mod<N>.
         */
        template <s64 N>
        constexpr auto operator-(int_mod<N> lhs, int_mod<N> rhs) noexcept -> int_mod<N>
        {
            if( lhs.value() < rhs.value() )
            {
                rhs -= lhs;
                return N - rhs.value();
            }
            else
            {
                lhs -= rhs;
                return lhs;
            }
        }

        /** \fn constexpr auto operator*(int_mod<N> lhs, int_mod<N> rhs) noexcept -> int_mod<N>
            \brief Returns the result of multiplying two int_mod<N>.
         */
        template <s64 N>
        constexpr auto operator*(int_mod<N> lhs, int_mod<N> rhs) noexcept -> int_mod<N>
        {
            lhs *= rhs;

            return lhs;
        }

        /** \fn constexpr auto operator/(int_mod<N> lhs, int_mod<N> rhs) -> int_mod<N>
            \brief Returns the result of dividing two int_mod<N>. Throws std::invalid_argument if rhs is not invertible.
         */
        template <s64 N>
        constexpr auto operator/(int_mod<N> lhs, int_mod<N> rhs) -> int_mod<N>
        {
            try
            {
                lhs /= rhs.value();
            }
            catch( std::invalid_argument const & )
            {
                throw;
            }

            return lhs;
        }

        // Right-s64 versions
        /** \fn constexpr auto operator+(int_mod<N> lhs, s64 rhs) noexcept -> int_mod<N>
            \brief Returns the result of adding int_mod<N> and s64.
         */
        template <s64 N>
        constexpr auto operator+(int_mod<N> lhs, s64 rhs) noexcept -> int_mod<N>
        {
            lhs += rhs;

            return lhs;
        }

        /** \fn constexpr auto operator-(int_mod<N> lhs, s64 rhs) noexcept -> int_mod<N>
            \brief Returns the result of subtracting int_mod<N> by s64.
         */
        template <s64 N>
        constexpr auto operator-(int_mod<N> lhs, s64 rhs) noexcept -> int_mod<N>
        {
            lhs -= rhs;

            return lhs;
        }

        /** \fn constexpr auto operator*(int_mod<N> lhs, s64 rhs) noexcept -> int_mod<N>
            \brief Returns the result of multiplying int_mod<N> by s64.
         */
        template <s64 N>
        constexpr auto operator*(int_mod<N> lhs, s64 rhs) noexcept -> int_mod<N>
        {
            lhs *= rhs;

            return lhs;
        }

        /** \fn constexpr auto operator/(int_mod<N> lhs, s64 rhs) -> int_mod<N>
            \brief Returns the result of dividing int_mod<N> by s64. Throws std::invalid_argument if rhs is not invertible.
         */
        template <s64 N>
        constexpr auto operator/(int_mod<N> lhs, s64 rhs) -> int_mod<N>
        {
            try
            {
                lhs /= rhs;
            }
            catch( std::invalid_argument const & )
            {
                throw;
            }

            return lhs;
        }

        // Left-s64 versions
        /** \fn constexpr auto operator+(s64 const lhs, int_mod<N> rhs) noexcept -> int_mod<N>
            \brief Returns the result of adding int_mod<N> and s64.
         */
        template <s64 N>
        constexpr auto operator+(s64 const lhs, int_mod<N> rhs) noexcept -> int_mod<N>
        {
            rhs += lhs;

            return rhs;
        }

        /** \fn constexpr auto operator-(s64 const lhs, int_mod<N> rhs) noexcept -> int_mod<N>
            \brief Returns the result of subtracting int_mod<N> by s64.
         */
        template <s64 N>
        constexpr auto operator-(s64 const lhs, int_mod<N> rhs) noexcept -> int_mod<N>
        {
            if( (lhs % N) < rhs.value() )
            {
                rhs -= lhs;
                return N - rhs.value();
            }
            else
            {

                return (lhs % N) - rhs.value();
            }
        }

        /** \fn constexpr auto operator*(s64 const lhs, int_mod<N> rhs) noexcept -> int_mod<N>
            \brief Returns the result of multiplying int_mod<N> by s64.
         */
        template <s64 N>
        constexpr auto operator*(s64 const lhs, int_mod<N> rhs) noexcept -> int_mod<N>
        {
            rhs *= lhs;

            return rhs;
        }

        /** \fn constexpr auto operator/(s64 const lhs, int_mod<N> rhs) -> int_mod<N>
            \brief Returns the result of dividing int_mod<N> by s64. Throws std::invalid_argument if rhs is not invertible.
         */
        template <s64 N>
        constexpr auto operator/(s64 const lhs, int_mod<N> rhs) -> int_mod<N>
        {
            try
            {
                return rhs.inverse() * lhs;
            }
            catch( std::invalid_argument const & )
            {
                throw;
            }
        }

        // I/O operators
        /** \fn constexpr auto operator<<(std::ostream &os, int_mod<N> const &rhs) -> std::ostream &
            \brief Outputs our number in standard form. Returns the ostream object for further output.
         */
        template <s64 N>
        constexpr auto operator<<(std::ostream &os, int_mod<N> const &rhs) -> std::ostream &
        {
            os << rhs.value();
            return os;
        }

        /** \fn constexpr auto operator>>(std::istream &is, int_mod<N> &rhs) -> std::istream &
            \brief Inputs our number in standard form. Returns the istream object for further input.
         */
        template <s64 N>
        constexpr auto operator>>(std::istream &is, int_mod<N> &rhs) -> std::istream &
        {
            s64 tmp;
            is >> tmp;

            rhs = impl_details::standard_modulo<N>(tmp);

            return is;
        }

        // Implementation function definitions.
        namespace impl_details
        {
            constexpr auto gcd(s64 a, s64 b) noexcept -> s64
            {
                if( a < 0 ) a *= -1;
                if( b < 0 ) b *= -1;

                if( b == 0 )
                {
                    return a;
                }

                return gcd(b, a % b);
            }

            constexpr auto euler_phi(s64 N) noexcept -> s64
            {
                s64 res = N;

                for( auto p{ 2 }; p * p <= N; ++p )
                {   // Check all numbers <= sqrt(n)
                    if( N % p == 0 )
                    {   // If we find a factor, it is prime
                        while( N % p == 0 )
                        { // so we divide it out until it's all gone.
                            N /= p;
                        }

                        res -= res / p; // and update res by multiplying by (1-1/p).
                    }
                }

                if( N > 1 )
                {   // If we have any factors > sqrt(n), then there is at most one
                    res -= res / N; // and we update res accordingly.
                }

                return res;
            }

            template<s64 N>
            constexpr auto ipow(s64 const base, s64 const exponent) -> s64
            {
                if( exponent < 0 )
                {
                    throw std::invalid_argument{ "Exponent must be non-negative." };
                }

                if( base == 0 || base == 1 || exponent == 1 )
                {
                    return base;
                }
                else if( exponent == 0 )
                {
                    return 1;
                }

                s64 tmp = ipow<N>(base, exponent / 2);

                if( exponent % 2 == 0 )
                {
                    return standard_modulo<N>(tmp * tmp);
                }
                else
                {   // Circumventing some overflows by modding by N before second multiplication by tmp.
                    return standard_modulo<N>((base * tmp) % N * tmp);
                }
            }

            template <s64 N>
            constexpr auto inverse_of(s64 n) -> s64
            {

                constexpr s64 phi{ euler_phi(N) };

                s64 inv{ 0 };
                s64 d{ gcd(n, N) };

                if( d == 1 )
                {
                    try
                    {
                        inv = ipow<N>(standard_modulo<N>(n) % N, phi - 1);
                    }
                    catch( std::invalid_argument const & )
                    {
                        throw;
                    }

                }
                else
                {
                    throw std::invalid_argument(std::to_string(n) + " is not invertible modulo " + std::to_string(N)
                        + " because gcd(" + std::to_string(n) + ", " + std::to_string(N) + ") = "
                        + std::to_string(d) + ", which is not 1.\n");
                }

                return inv;
            }

            template <s64 N>
            constexpr auto standard_modulo(s64 rhs) -> s64
            {
                rhs %= N;

                if( rhs < 0 )
                {
                    rhs += N;
                }

                return rhs;
            }

        } // namespace impl_details

    } // namespace int_mod

} // namespace math_nerd

/** \mainpage Integers Modulo N
    \section Integers Modulo N
    This project is simply a wrapper for signed 64-bit integers which implements <a href="https://en.wikipedia.org/wiki/Modular_arithmetic">modular arithmetic</a>.

    The main use of this was simply to implement the <a href="../hill_cipher/index.html">Hill Cipher (modulo 97)</a>, which uses another type I made,
    the <a href="../matrix_t/index.html">Minimal Matrix</a>.

    \section gitlab_link GitLab Link
    View the source code at <a href="https://gitlab.com/mathnerd/integers-modulo-n">GitLab</a>.
 */
#endif
