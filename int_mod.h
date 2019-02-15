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
            /** \fn constexpr s64 gcd(s64 const a, s64 const b) noexcept
                \brief Computes the greatest common divisor of two integers using the Euclidean algorithm.
             */
            constexpr s64 gcd(s64 const a, s64 const b) noexcept;

            /** \fn constexpr s64 euler_phi(s64 const n) noexcept
                \brief Computes the Euler phi of an integer.
             */
            constexpr s64 euler_phi(s64 const n) noexcept;

            /** \fn constexpr s64 ipow(s64 const base, s64 const exponent, s64 const N)
                \brief Computes base to the power exponent modulo N.
                \details This function uses a divide-and-conquer algorithm which takes advantage of integer division to
                         divide the exponent in half at each level of recursion until an exponent of 0 is reached, for
                         which it returns 1. It then multiplies the results together while taking remainders after division by N.
                         Throws std::invalid_argument if exponent is negative.
             */
            constexpr s64 ipow(s64 const base, s64 const exponent, s64 const N);

            /** \fn s64 inverse_of(s64 const n)
                \brief Computes the inverse of an integer modulo N. Throws std::invalid_argument if not invertible.
                \brief By Euler's theorem, \f$a^{\phi\left(N\right)}\equiv a\,\left(\text{mod}\,N\right)\f$ for \f$\text{gcd}\left(a,N\right)=1,\f$
                       which implies that that \f$a^{\phi\left(N\right)-1}\equiv 1\,\left(\text{mod}\,N\right)\f$ under the same condition.
             */
            template <s64 N>
            s64 inverse_of(s64 const n);

            /** \fn s64 standard_modulo(s64 rhs)
                \brief Returns the standard form of rhs modulo N. The standard form is the integer
                       between 0 and N-1 (inclusive) which is equivalent to rhs modulo N.
             */
            template <s64 N>
            s64 standard_modulo(s64 rhs);

        } // namespace impl_details


        /** \class int_mod<N>
            \brief Wrapper for 64-bit integer for arithmetic modulo N.
         */
        template <s64 N>
        class int_mod
        {
            static_assert(N > 1, "Modulus N of int_mod<N> must be at least 2.");
            static_assert(N <= 1000000000, "Modulus N > 1000000000 of int_mod<N> are currently not supported due to lack of overflow checks.");

            private:
                /** \property s64 element_
                    \brief The integer which will be taken modulo N. Default initializes to 0.
                 */
                s64 element_{ 0 };

            public:
                int_mod() = default;

                int_mod(s64 num) noexcept
                {
                    element_ = impl_details::standard_modulo<N>(num);
                    element_ %= N;
                }

                ~int_mod() = default;

                /** \fn s64 modulus() const noexcept
                    \brief Returns the modulus N.
                 */
                s64 modulus() const noexcept
                {
                    return N;
                }

                /** \fn s64 value() const noexcept
                    \brief Returns the stored value.
                 */
                s64 value() const noexcept
                {
                    return element_;
                }

                /** \fn inverse() const
                    \brief Returns the inverse modulo N of the stored value. Throws std::invalid_argument if not invertible.
                 */
                s64 inverse() const
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

                /** \name Increment/Decrement operators */
                /** \fn int_mod<N> &operator++() noexcept
                    \brief Pre-increments element_ and reduces modulo N.
                 */
                int_mod<N> &operator++() noexcept;

                /** \fn int_mod<N> operator++(int) noexcept
                    \brief Post-increments element_ and reduces modulo N.
                 */
                int_mod<N> operator++(int) noexcept;

                /** \fn int_mod<N> &operator--() noexcept
                    \brief Pre-decrements element_ and reduces modulo N.
                 */
                int_mod<N> &operator--() noexcept;

                /** \fn int_mod<N> operator--(int) noexcept
                    \brief Post-decrements element_ and reduces modulo N.
                 */
                int_mod<N> operator--(int) noexcept;

                /** \name Unary operators */
                /** \fn int_mod<N> operator+() const noexcept
                    \brief Returns the *this.
                 */
                int_mod<N> operator+() const noexcept;

                /** \fn int_mod<N> operator-() const noexcept
                    \brief Returns the additive inverse modulo N.
                 */
                int_mod<N> operator-() const noexcept;

                /** \name Assignment operators */
                /** \fn int_mod<N> &operator=(int_mod<N> const rhs) noexcept
                    \brief Assigns rhs to element_ and reduces modulo N.
                 */
                int_mod<N> &operator=(int_mod<N> const rhs) noexcept;

                /** \fn int_mod<N> &operator+=(int_mod<N> const rhs) noexcept
                    \brief Adds rhs to element_ and reduces modulo N.
                 */
                int_mod<N> &operator+=(int_mod<N> const rhs) noexcept;

                /** \fn int_mod<N> &operator-=(int_mod<N> const rhs) noexcept
                    \brief Subtracts rhs from element_ and reduces modulo N.
                 */
                int_mod<N> &operator-=(int_mod<N> const rhs) noexcept;

                /** \fn int_mod<N> &operator*=(int_mod<N> const rhs) noexcept
                    \brief Multiples rhs to element_ and reduces modulo N.
                 */
                int_mod<N> &operator*=(int_mod<N> const rhs) noexcept;

                /** \fn int_mod<N> &operator/=(int_mod<N> const rhs)
                    \brief Divides rhs, if invertible modulo N, from element_ and reduces modulo N. Throws std::invalid_argument if rhs is not invertible.
                 */
                int_mod<N> &operator/=(int_mod<N> const rhs);

                /** \fn int_mod<N> &operator%=(int_mod<N> const rhs)
                    \brief Finds remainder of element_ when divided by rhs. Throws std::invalid_argument if rhs is zero.
                 */
                int_mod<N> &operator%=(int_mod<N> const rhs);

                /** \fn int_mod<N> &operator=(s64 rhs) noexcept
                    \brief Assigns rhs to element_ and reduces modulo N.
                 */
                int_mod<N> &operator=(s64 rhs) noexcept;

                /** \fn int_mod<N> &operator+=(s64 rhs) noexcept
                    \brief Adds rhs to element_ and reduces modulo N.
                 */
                int_mod<N> &operator+=(s64 rhs) noexcept;

                /** \fn int_mod<N> &operator-=(s64 rhs) noexcept
                    \brief Subtracts rhs from element_ and reduces modulo N.
                 */
                int_mod<N> &operator-=(s64 rhs) noexcept;

                /** \fn int_mod<N> &operator*=(s64 rhs) noexcept
                    \brief Multiples rhs to element_ and reduces modulo N.
                 */
                int_mod<N> &operator*=(s64 rhs) noexcept;

                /** \fn int_mod<N> &operator/=(s64 rhs)
                    \brief Divides rhs, if invertible modulo N, from element_ and reduces modulo N. Throws std::invalid_argument if rhs is not invertible.
                 */
                int_mod<N> &operator/=(s64 rhs);

                /** \fn int_mod<N> &operator%=(s64 rhs)
                    \brief Finds remainder of element_ when divided by rhs. Throws std::invalid_argument if rhs is zero.
                 */
                int_mod<N> &operator%=(s64 rhs);


                /** \name Comparison operators */
                // int_mod<N> versions
                /** \fn bool operator==(int_mod<N> const rhs) const noexcept
                    \brief Compares the values and returns true if equal.
                 */
                bool operator==(int_mod<N> const rhs) const noexcept;

                /** \fn  bool operator!=(int_mod<N> const rhs) const noexcept
                    \brief Compares the values and returns false if equal.
                 */
                bool operator!=(int_mod<N> const rhs) const noexcept;
                
                // s64 versions
                /** \fn bool operator==(s64 rhs) const noexcept
                    \brief Compares the values and returns true if equal.
                 */
                bool operator==(s64 rhs) const noexcept;
                
                /** \fn bool operator!=(s64 rhs) const noexcept
                    \brief Compares the values and returns false if equal.
                 */
                bool operator!=(s64 rhs) const noexcept;

                
        };

        // Increment/Decrement Operators
        template <s64 N>
        int_mod<N> &int_mod<N>::operator++() noexcept
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
        int_mod<N> int_mod<N>::operator++(int) noexcept
        {
            int_mod<N> tmp(*this);
            operator++();
            return tmp;
        }

        template <s64 N>
        int_mod<N> &int_mod<N>::operator--() noexcept
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
        int_mod<N> int_mod<N>::operator--(int) noexcept
        {
            int_mod<N> tmp(*this);
            operator--();
            return tmp;
        }

        // Unary operators
        template <s64 N>
        int_mod<N> int_mod<N>::operator+() const noexcept
        {
            return *this;
        }

        template <s64 N>
        int_mod<N> int_mod<N>::operator-() const noexcept
        {
            return N - element_;
        }

        // Assignment operators
        // int_mod<N> versions
        template<s64 N>
        int_mod<N> &int_mod<N>::operator=(int_mod<N> const rhs) noexcept
        {
            element_ = rhs.value();

            return *this;
        }

        template <s64 N>
        int_mod<N> &int_mod<N>::operator+=(int_mod<N> const rhs) noexcept
        {
            element_ += rhs.value();
            element_ %= N;

            return *this;
        }

        template <s64 N>
        int_mod<N> &int_mod<N>::operator-=(int_mod<N> const rhs) noexcept
        {
            if( rhs.value() > element_ )
            {
                element_ = N - (rhs.value() - element_);
            }
            else
            {
                element_ = element_ - rhs.value();
            }

            return *this;
        }

        template <s64 N>
        int_mod<N> &int_mod<N>::operator*=(int_mod<N> const rhs) noexcept
        {
            element_ *= rhs.value();
            element_ %= N;

            return *this;
        }

        template <s64 N>
        int_mod<N> &int_mod<N>::operator/=(int_mod<N> const rhs)
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

        template <s64 N>
        int_mod<N> &int_mod<N>::operator%=(int_mod<N> const rhs)
        {
            if( rhs.value() != 0 )
            {
                element_ %= rhs.value();
            }
            else
            {
                throw std::invalid_argument("Cannot mod by zero.");
            }

            return *this;
        }

        // s64 versions
        template <s64 N>
        int_mod<N> &int_mod<N>::operator=(s64 rhs) noexcept
        {
            rhs = impl_details::standard_modulo<N>(rhs);
            
            element_ = rhs % N;

            return *this;
        }

        template <s64 N>
        int_mod<N> &int_mod<N>::operator+=(s64 rhs) noexcept
        {
            rhs = impl_details::standard_modulo<N>(rhs);

            element_ += rhs;

            element_ %= N;

            return *this;
        }

        template <s64 N>
        int_mod<N> &int_mod<N>::operator-=(s64 rhs) noexcept
        {
            rhs = impl_details::standard_modulo<N>(rhs);

            if( rhs > element_ )
            {
                element_ = N - (rhs - element_);
            }
            else
            {
                element_ -= rhs;
            }

            return *this;
        }

        template <s64 N>
        int_mod<N> &int_mod<N>::operator*=(s64 rhs) noexcept
        {
            rhs = impl_details::standard_modulo<N>(rhs);

            element_ *= rhs;
            element_ %= N;

            return *this;
        }

        template <s64 N>
        int_mod<N> &int_mod<N>::operator/=(s64 rhs)
        {
            rhs = impl_details::standard_modulo<N>(rhs);

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

        template <s64 N>
        int_mod<N> &int_mod<N>::operator%=(s64 rhs)
        {
            if( rhs != 0 )
            {
                element_ %= rhs;
            }
            else
            {
                throw std::invalid_argument("Cannot mod by zero.");
            }

            return *this;
        }

        // Comparison operators
        // int_mod<N> versions
        template <s64 N>
        bool int_mod<N>::operator==(int_mod<N> const rhs) const noexcept
        {
            return element_ == rhs.value();
        }

        template <s64 N>
        bool int_mod<N>::operator!=(int_mod<N> const rhs) const noexcept
        {
            return element_ != rhs.value();
        }

        // s64 versions
        template <s64 N>
        bool int_mod<N>::operator==(s64 rhs) const noexcept
        {
            rhs = impl_details::standard_modulo<N>(rhs);

            return element_ == rhs;
        }

        template <s64 N>
        bool int_mod<N>::operator!=(s64 rhs) const noexcept
        {
            rhs = impl_details::standard_modulo<N>(rhs);

            return element_ != rhs;
        }

        /** \name Arithmetic operators. */

        // int_mod<N> versions
        /** \fn int_mod<N> operator+(int_mod<N> lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of adding two int_mod<N>.
         */
        template <s64 N>
        int_mod<N> operator+(int_mod<N> lhs, int_mod<N> rhs) noexcept
        {
            lhs += rhs;
            return lhs;
        }

        /** \fn int_mod<N> operator-(int_mod<N> lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of subtracting two int_mod<N>.
         */
        template <s64 N>
        int_mod<N> operator-(int_mod<N> lhs, int_mod<N> rhs) noexcept
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

        /** \fn int_mod<N> operator*(int_mod<N> lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of multiplying two int_mod<N>.
         */
        template <s64 N>
        int_mod<N> operator*(int_mod<N> lhs, int_mod<N> rhs) noexcept
        {
            lhs *= rhs;

            return lhs;
        }

        /** \fn int_mod<N> operator/(int_mod<N> lhs, int_mod<N> rhs)
            \brief Returns the result of dividing two int_mod<N>. Throws std::invalid_argument if rhs is not invertible.
         */
        template <s64 N>
        int_mod<N> operator/(int_mod<N> lhs, int_mod<N> rhs)
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

        /** \fn int_mod<N> operator%(int_mod<N> lhs, int_mod<N> rhs)
            \brief Returns the remainder when dividing two int_mod<N>. Throws std::invalid_argument if rhs is zero.
         */
        template <s64 N>
        int_mod<N> operator%(int_mod<N> lhs, int_mod<N> rhs)
        {
            try
            {
                lhs %= rhs.value();
            }
            catch( std::invalid_argument const & )
            {
                throw;
            }

            return lhs;
        }

        // Right-s64 versions
        /** \fn int_mod<N> operator+(int_mod<N> lhs, s64 rhs) noexcept
            \brief Returns the result of adding int_mod<N> and s64.
         */
        template <s64 N>
        int_mod<N> operator+(int_mod<N> lhs, s64 rhs) noexcept
        {
            lhs += rhs;

            return lhs;
        }

        /** \fn int_mod<N> operator-(int_mod<N> lhs, s64 rhs) noexcept
            \brief Returns the result of subtracting int_mod<N> by s64.
         */
        template <s64 N>
        int_mod<N> operator-(int_mod<N> lhs, s64 rhs) noexcept
        {
            lhs -= rhs;

            return lhs;
        }

        /** \fn int_mod<N> operator*(int_mod<N> lhs, s64 rhs) noexcept
            \brief Returns the result of multiplying int_mod<N> by s64.
         */
        template <s64 N>
        int_mod<N> operator*(int_mod<N> lhs, s64 rhs) noexcept
        {
            lhs *= rhs;

            return lhs;
        }

        /** \fn int_mod<N> operator/(int_mod<N> lhs, s64 rhs)
            \brief Returns the result of dividing int_mod<N> by s64. Throws std::invalid_argument if rhs is not invertible.
         */
        template <s64 N>
        int_mod<N> operator/(int_mod<N> lhs, s64 rhs)
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

        /** \fn int_mod<N> operator%(int_mod<N> lhs, s64 rhs)
            \brief Returns the remainder when dividing int_mod<N> by s64. Throws std::invalid_argument if rhs is zero.
         */
        template <s64 N>
        int_mod<N> operator%(int_mod<N> lhs, s64 rhs)
        {
            try
            {
                lhs %= rhs;
            }
            catch( std::invalid_argument const & )
            {
                throw;
            }

            return lhs;
        }

        // Left-s64 versions
        /** \fn int_mod<N> operator+(s64 const lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of adding int_mod<N> and s64.
         */
        template <s64 N>
        int_mod<N> operator+(s64 const lhs, int_mod<N> rhs) noexcept
        {
            rhs += lhs;

            return rhs;
        }

        /** \fn int_mod<N> operator-(s64 const lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of subtracting int_mod<N> by s64.
         */
        template <s64 N>
        int_mod<N> operator-(s64 const lhs, int_mod<N> rhs) noexcept
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

        /** \fn int_mod<N> operator*(s64 const lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of multiplying int_mod<N> by s64.
         */
        template <s64 N>
        int_mod<N> operator*(s64 const lhs, int_mod<N> rhs) noexcept
        {
            rhs *= lhs;

            return rhs;
        }

        /** \fn int_mod<N> operator/(s64 const lhs, int_mod<N> rhs)
            \brief Returns the result of dividing int_mod<N> by s64. Throws std::invalid_argument if rhs is not invertible.
         */
        template <s64 N>
        int_mod<N> operator/(s64 const lhs, int_mod<N> rhs)
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

        /** \fn int_mod<N> operator%(s64 const lhs, int_mod<N> rhs)
            \brief Returns the remainder when dividing int_mod<N> by s64. Throws std::invalid_argument if rhs is zero.
         */
        template <s64 N>
        int_mod<N> operator%(s64 const lhs, int_mod<N> rhs)
        {
            try
            {
                lhs %= rhs;
            }
            catch( std::invalid_argument const & )
            {
                throw;
            }

            return lhs;
        }

        // I/O operators
        /** \fn std::ostream &operator<<(std::ostream &os, int_mod<N> const &rhs)
            \brief Outputs our number in standard form. Returns the ostream object for further output.
         */
        template <s64 N>
        std::ostream &operator<<(std::ostream &os, int_mod<N> const &rhs)
        {
            os << rhs.value();
            return os;
        }

        /** \fn std::istream &operator>>(std::istream &is, int_mod<N> &rhs)
            \brief Inputs our number in standard form. Returns the istream object for further input.
         */
        template <s64 N>
        std::istream &operator>>(std::istream &is, int_mod<N> &rhs)
        {
            s64 tmp;
            is >> tmp;

            rhs = impl_details::standard_modulo<N>(tmp);

            return is;
        }

        // Implementation function definitions.
        namespace impl_details
        {
            constexpr s64 gcd(s64 const a, s64 const b) noexcept
            {
                if( b == 0 )
                {
                    return a;
                }

                return gcd(b, a%b);
            }

            constexpr s64 euler_phi(s64 const n) noexcept
            {
                s64 res = 1;

                for( auto i = 2; i < n; ++i )
                {
                    if( gcd(i, n) == 1 )
                    {   // phi(n) = # integers k <= n for which gcd(k,n) = 1.
                        ++res;
                    }
                }

                return res;
            }

            constexpr s64 ipow(s64 const base, s64 const exponent, s64 const N)
            {
                if( exponent < 0 )
                {
                    std::invalid_argument("Exponent must be non-negative.");
                }

                if( base == 0 || base == 1 || exponent == 1 )
                {
                    return base;
                }
                else if( exponent == 0 )
                {
                    return 1;
                }

                s64 tmp = ipow(base, exponent / 2, N);

                if( exponent % 2 == 0 )
                {   // Taking modulo N each step to circumvent overflows.
                    return (tmp * tmp) % N;
                }
                else
                {
                    return (base * tmp * tmp) % N;
                }
            }

            template <s64 N>
            s64 inverse_of(s64 const n)
            {
                static s64 phi = 0;

                if( phi == 0 )
                {
                    if constexpr(N < 9901)
                    {   // constexpr works for N < 9901 with MSVC.
                        constexpr s64 phi_const = euler_phi(N);
                        phi = phi_const;
                    }
                    else
                    {   // Otherwise, we must calculate at runtime.
                        phi = euler_phi(N);
                    }
                }

                s64 inv = 0;
                s64 d = gcd(n, N);

                if( d == 1 )
                {
                    try
                    {
                        inv = ipow(n % N, phi - 1, N);
                    }
                    catch( std::invalid_argument const & )
                    {
                        throw;
                    }
                    
                }
                else
                {
                    std::stringstream error_msg;

                    error_msg << n << " is not invertible modulo " << N
                              << " because gcd(" << n << ", " << N << ") = "
                              << d << ", which is not 1.\n";

                    throw std::invalid_argument(error_msg.str());
                }

                return inv;
            }

            template <s64 N>
            s64 standard_modulo(s64 rhs)
            {
                /*
                    For the negative case:
                    rhs / N ~ rhs (integer division)
                    Since rhs < 0, negate:
                    -(rhs / N) ~ -rhs
                    Adding 1 and multiplying by N gives
                    the multiple of N needed to be added
                    to rhs to bring it to standard form.
                 */
                if( rhs < 0 )
                {
                    rhs += (-(rhs / N) + 1)*N;
                }
                else if( rhs > N )
                {   // Positive case is much easier.
                    rhs %= N;
                }

                return rhs;
            }

        } // namespace impl_details

    } // namespace int_mod

} // namespace math_nerd

/** \mainpage Integers Modulo N
    \section gitlab_link GitLab Link
    View the source code at <a href="https://gitlab.com/mathnerd/integers-modulo-n">GitLab</a>.
 */
#endif
