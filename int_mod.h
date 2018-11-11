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
#ifndef INT_MOD_H
#define INT_MOD_H

/** \file int_mod.h
    \brief std::uint64_t wrapper for arithmetic modulo N.
 */
#include <array>
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
        /** \name Unsigned 64-bit integer
         */
        using u64 = std::uint64_t;

        /** \class int_mod<N>
            \brief Wrapper for 64-bit unsigned integer for arithmetic modulo N.
         */
        template <u64 N>
        class int_mod
        {
            static_assert(N > 1, "Modulus N of int_mod<N> must be at least 2.");

            private:
                /** \property u64 element_
                    \brief The unsigned integer which will be taken modulo N. Default initializes to 0.
                 */
                u64 element_{ 0 };

            public:
                constexpr int_mod() = default;

                constexpr int_mod(u64 num) noexcept
                {
                    element_ = num;
                    element_ %= N;
                }

                ~int_mod() = default;

                /** \fn constexpr u64 modulus() const noexcept
                    \brief Returns the modulus N.
                 */
                constexpr u64 modulus() const noexcept
                {
                    return N;
                }

                /** \fn constexpr u64 value() const noexcept
                    \brief Returns the stored value.
                 */
                constexpr u64 value() const noexcept
                {
                    return element_;
                }

                /** \fn constexpr inverse() const
                    \brief Returns the inverse modulo N of the stored value. Throws std::invalid_argument if not invertible.
                 */
                constexpr u64 inverse() const
                {
                    u64 inv; 
                    try
                    {
                        inv = impl_details::inverse_of<N>(element_);
                    }
                    catch( std::invalid_argument const & )
                    {
                        throw;
                    }
                    return impl_details::inverse_of<N>(element_);
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

                /** \fn int_mod<N> &operator=(u64 const rhs) noexcept
                    \brief Assigns rhs to element_ and reduces modulo N.
                 */
                int_mod<N> &operator=(u64 const rhs) noexcept;

                /** \fn int_mod<N> &operator+=(u64 const rhs) noexcept
                    \brief Adds rhs to element_ and reduces modulo N.
                 */
                int_mod<N> &operator+=(u64 const rhs) noexcept;

                /** \fn int_mod<N> &operator-=(u64 const rhs) noexcept
                    \brief Subtracts rhs from element_ and reduces modulo N.
                 */
                int_mod<N> &operator-=(u64 const rhs) noexcept;

                /** \fn int_mod<N> &operator*=(u64 const rhs) noexcept
                    \brief Multiples rhs to element_ and reduces modulo N.
                 */
                int_mod<N> &operator*=(u64 const rhs) noexcept;

                /** \fn int_mod<N> &operator/=(u64 const rhs)
                    \brief Divides rhs, if invertible modulo N, from element_ and reduces modulo N. Throws std::invalid_argument if rhs is not invertible.
                 */
                int_mod<N> &operator/=(u64 const rhs);

                /** \fn int_mod<N> &operator%=(u64 const rhs)
                    \brief Finds remainder of element_ when divided by rhs. Throws std::invalid_argument if rhs is zero.
                 */
                int_mod<N> &operator%=(u64 const rhs);


                /** \name Comparison operators */
                // int_mod<N> versions
                /** \fn constexpr bool operator==(int_mod<N> const rhs) const noexcept
                    \brief Compares the values and returns true if equal.
                 */
                constexpr bool operator==(int_mod<N> const rhs) const noexcept;

                /** \fn constexpr bool operator!=(int_mod<N> const rhs) const noexcept
                    \brief Compares the values and returns false if equal.
                 */
                constexpr bool operator!=(int_mod<N> const rhs) const noexcept;
                
                // u64 versions
                /** \fn constexpr bool operator==(u64 const rhs) const noexcept
                    \brief Compares the values and returns true if equal.
                 */
                constexpr bool operator==(u64 const rhs) const noexcept;
                
                /** \fn constexpr bool operator!=(u64 const rhs) const noexcept
                    \brief Compares the values and returns false if equal.
                 */
                constexpr bool operator!=(u64 const rhs) const noexcept;

                /** \name Arithmetic operators */
                // int_mod<N> versions
                /** \fn constexpr constexpr friend int_mod<N> operator+(int_mod<N> lhs, int_mod<N> const rhs) noexcept
                    \brief Returns the result of adding two int_mod<N>.
                 */
                template <u64 N>
                constexpr friend int_mod<N> operator+(int_mod<N> lhs, int_mod<N> const rhs) noexcept;

                /** \fn constexpr friend int_mod<N> operator-(int_mod<N> lhs, int_mod<N> const rhs) noexcept
                    \brief Returns the result of subtracting two int_mod<N>.
                 */
                template <u64 N>
                constexpr friend int_mod<N> operator-(int_mod<N> lhs, int_mod<N> const rhs) noexcept;

                /** \fn constexpr friend int_mod<N> operator*(int_mod<N> lhs, int_mod<N> const rhs) noexcept
                    \brief Returns the result of multiplying two int_mod<N>.
                 */
                template <u64 N>
                constexpr friend int_mod<N> operator*(int_mod<N> lhs, int_mod<N> const rhs) noexcept;

                /** \fn constexpr friend int_mod<N> operator/(int_mod<N> lhs, int_mod<N> const rhs)
                    \brief Returns the result of dividing two int_mod<N>. Throws std::invalid_argument if rhs is not invertible.
                 */
                template <u64 N>
                friend int_mod<N> operator/(int_mod<N> lhs, int_mod<N> const rhs);

                /** \fn constexpr friend int_mod<N> operator%(int_mod<N> lhs, int_mod<N> const rhs)
                    \brief Returns the remainder when dividing two int_mod<N>. Throws std::invalid_argument if rhs is zero.
                 */
                template <u64 N>
                friend int_mod<N> operator%(int_mod<N> lhs, int_mod<N> const rhs);
                
                // Right-u64 versions
                /** \fn constexpr constexpr friend int_mod<N> operator+(int_mod<N> lhs, u64 const rhs) noexcept
                    \brief Returns the result of adding int_mod<N> and u64.
                 */
                template <u64 N>
                constexpr friend int_mod<N> operator+(int_mod<N> lhs, u64 const rhs) noexcept;

                /** \fn constexpr constexpr friend int_mod<N> operator-(int_mod<N> lhs, u64 const rhs) noexcept
                    \brief Returns the result of subtracting int_mod<N> by u64.
                 */
                template <u64 N>
                constexpr friend int_mod<N> operator-(int_mod<N> lhs, u64 const rhs) noexcept;

                /** \fn constexpr constexpr friend int_mod<N> operator*(int_mod<N> lhs, u64 const rhs) noexcept
                    \brief Returns the result of multiplying int_mod<N> by u64.
                 */
                template <u64 N>
                constexpr friend int_mod<N> operator*(int_mod<N> lhs, u64 const rhs) noexcept;

                /** \fn constexpr friend int_mod<N> operator/(int_mod<N> lhs, u64 const rhs)
                    \brief Returns the result of dividing int_mod<N> by u64. Throws std::invalid_argument if rhs is not invertible.
                 */
                template <u64 N>
                friend int_mod<N> operator/(int_mod<N> lhs, u64 const rhs);

                /** \fn constexpr friend int_mod<N> operator%(int_mod<N> lhs, u64 const rhs)
                    \brief Returns the remainder when dividing int_mod<N> by u64. Throws std::invalid_argument if rhs is zero.
                 */
                template <u64 N>
                friend int_mod<N> operator%(int_mod<N> lhs, u64 const rhs);

                // Left-u64 versions
                /** \fn constexpr constexpr friend int_mod<N> operator+(u64 const lhs, int_mod<N> rhs) noexcept
                \brief Returns the result of adding int_mod<N> and u64.
                */
                template <u64 N>
                constexpr friend int_mod<N> operator+(u64 const lhs, int_mod<N> rhs) noexcept;

                /** \fn constexpr constexpr friend int_mod<N> operator-(u64 const lhs, int_mod<N> rhs) noexcept
                \brief Returns the result of subtracting int_mod<N> by u64.
                */
                template <u64 N>
                constexpr friend int_mod<N> operator-(u64 const lhs, int_mod<N> rhs) noexcept;

                /** \fn constexpr constexpr friend int_mod<N> operator*(u64 const lhs, int_mod<N> rhs) noexcept
                \brief Returns the result of multiplying int_mod<N> by u64.
                */
                template <u64 N>
                constexpr friend int_mod<N> operator*(u64 const lhs, int_mod<N> rhs) noexcept;

                /** \fn constexpr friend int_mod<N> operator/(u64 const lhs, int_mod<N> rhs)
                \brief Returns the result of dividing int_mod<N> by u64. Throws std::invalid_argument if rhs is not invertible.
                */
                template <u64 N>
                friend int_mod<N> operator/(u64 const lhs, int_mod<N> rhs);

                /** \fn constexpr friend int_mod<N> operator%(u64 const lhs, int_mod<N> rhs)
                \brief Returns the remainder when dividing int_mod<N> by u64. Throws std::invalid_argument if rhs is zero.
                */
                template <u64 N>
                friend int_mod<N> operator%(u64 const lhs, int_mod<N> rhs);

                /** \name I/O operators */
                /** \fn friend std::ostream &operator<<(std::ostream &os, int_mod<N> const &rhs)
                    \brief Outputs the value stored in rhs to the output stream. Returns reference to the output stream for further output.
                 */
                template <u64 N>
                friend std::ostream &operator<<(std::ostream &os, int_mod<N> const &rhs);

                /** \fn friend std::istream &operator>>(std::istream &is, int_mod<N> &rhs)
                    \brief Outputs the value stored in rhs to the output stream. Returns reference to the output stream for further output.
                 */
                template <u64 N>
                friend std::istream &operator>>(std::istream &is, int_mod<N> &rhs);
        };

        // Increment/Decrement Operators
        template <u64 N>
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

        template <u64 N>
        int_mod<N> int_mod<N>::operator++(int) noexcept
        {
            int_mod<N> tmp(*this);
            operator++();
            return tmp;
        }

        template <u64 N>
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

        template <u64 N>
        int_mod<N> int_mod<N>::operator--(int) noexcept
        {
            int_mod<N> tmp(*this);
            operator--();
            return tmp;
        }

        // Unary operators
        template <u64 N>
        int_mod<N> int_mod<N>::operator+() const noexcept
        {
            return *this;
        }

        template <u64 N>
        int_mod<N> int_mod<N>::operator-() const noexcept
        {
            return N - element_;
        }

        // Assignment operators
        // int_mod<N> versions
        template<u64 N>
        int_mod<N> &int_mod<N>::operator=(int_mod<N> const rhs) noexcept
        {
            element_ = rhs.value();

            return *this;
        }

        template <u64 N>
        int_mod<N> &int_mod<N>::operator+=(int_mod<N> const rhs) noexcept
        {
            element_ += rhs.value();
            element_ %= N;

            return *this;
        }

        template <u64 N>
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

        template <u64 N>
        int_mod<N> &int_mod<N>::operator*=(int_mod<N> const rhs) noexcept
        {
            element_ *= rhs.value();
            element_ %= N;

            return *this;
        }

        template <u64 N>
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

        template <u64 N>
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

        // u64 versions
        template <u64 N>
        int_mod<N> &int_mod<N>::operator=(u64 const rhs) noexcept
        {
            element_ = rhs;
            element_ %= N;

            return *this;
        }

        template <u64 N>
        int_mod<N> &int_mod<N>::operator+=(u64 const rhs) noexcept
        {
            element_ += rhs;
            element_ %= N;

            return *this;
        }

        template <u64 N>
        int_mod<N> &int_mod<N>::operator-=(u64 const rhs) noexcept
        {
            if( rhs > element_ )
            {
                element_ = N - (rhs - element_);
            }

            return *this;
        }

        template <u64 N>
        int_mod<N> &int_mod<N>::operator*=(u64 const rhs) noexcept
        {
            element_ *= rhs;
            element_ %= N;

            return *this;
        }

        template <u64 N>
        int_mod<N> &int_mod<N>::operator/=(u64 const rhs)
        {
            try
            {
                element_ *= impl_details::inverse_of<N>(rhs);
                element_ %= N;
            }
            catch( std::invalid_argument const & )
            {
                throw;
            }

            return *this;
        }

        template <u64 N>
        int_mod<N> &int_mod<N>::operator%=(u64 const rhs)
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
        template <u64 N>
        constexpr bool int_mod<N>::operator==(int_mod<N> const rhs) const noexcept
        {
            return element_ == rhs.value();
        }

        template <u64 N>
        constexpr bool int_mod<N>::operator!=(int_mod<N> const rhs) const noexcept
        {
            return element_ != rhs.value();
        }

        // u64 versions
        template <u64 N>
        constexpr bool int_mod<N>::operator==(u64 const rhs) const noexcept
        {
            return element_ == rhs;
        }

        template <u64 N>
        constexpr bool int_mod<N>::operator!=(u64 const rhs) const noexcept
        {
            return element_ != rhs;
        }

        /** \name Arithmetic operators. */
        // int_mod<N> versions
        /** \fn constexpr constexpr friend int_mod<N> operator+(int_mod<N> lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of adding two int_mod<N>.
         */
        template <u64 N>
        constexpr int_mod<N> operator+(int_mod<N> lhs, int_mod<N> rhs) noexcept
        {
            lhs += rhs;
            return lhs;
        }

        /** \fn constexpr friend int_mod<N> operator-(int_mod<N> lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of subtracting two int_mod<N>.
         */
        template <u64 N>
        constexpr int_mod<N> operator-(int_mod<N> lhs, int_mod<N> rhs) noexcept
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

        /** \fn constexpr friend int_mod<N> operator*(int_mod<N> lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of multiplying two int_mod<N>.
         */
        template <u64 N>
        constexpr int_mod<N> operator*(int_mod<N> lhs, int_mod<N> rhs) noexcept
        {
            lhs *= rhs;

            return lhs;
        }

        /** \fn constexpr friend int_mod<N> operator/(int_mod<N> lhs, int_mod<N> rhs)
            \brief Returns the result of dividing two int_mod<N>. Throws std::invalid_argument if rhs is not invertible.
         */
        template <u64 N>
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

        /** \fn constexpr friend int_mod<N> operator%(int_mod<N> lhs, int_mod<N> rhs)
            \brief Returns the remainder when dividing two int_mod<N>. Throws std::invalid_argument if rhs is zero.
         */
        template <u64 N>
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

        // Right-u64 versions
        /** \fn constexpr constexpr friend int_mod<N> operator+(int_mod<N> lhs, u64 const rhs) noexcept
            \brief Returns the result of adding int_mod<N> and u64.
         */
        template <u64 N>
        constexpr int_mod<N> operator+(int_mod<N> lhs, u64 const rhs) noexcept
        {
            lhs += rhs;

            return lhs;
        }

        /** \fn constexpr constexpr friend int_mod<N> operator-(int_mod<N> lhs, u64 const rhs) noexcept
            \brief Returns the result of subtracting int_mod<N> by u64.
         */
        template <u64 N>
        constexpr int_mod<N> operator-(int_mod<N> lhs, u64 const rhs) noexcept
        {
            if( lhs.value() < (rhs % N) )
            {

                return (rhs % N) - lhs.value();
            }
            else
            {
                lhs -= rhs;
                return lhs;
            }
        }

        /** \fn constexpr constexpr friend int_mod<N> operator*(int_mod<N> lhs, u64 const rhs) noexcept
            \brief Returns the result of multiplying int_mod<N> by u64.
         */
        template <u64 N>
        constexpr int_mod<N> operator*(int_mod<N> lhs, u64 const rhs) noexcept
        {
            lhs *= rhs;

            return lhs;
        }

        /** \fn constexpr friend int_mod<N> operator/(int_mod<N> lhs, u64 const rhs)
            \brief Returns the result of dividing int_mod<N> by u64. Throws std::invalid_argument if rhs is not invertible.
         */
        template <u64 N>
        int_mod<N> operator/(int_mod<N> lhs, u64 const rhs)
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

        /** \fn constexpr friend int_mod<N> operator%(int_mod<N> lhs, u64 const rhs)
            \brief Returns the remainder when dividing int_mod<N> by u64. Throws std::invalid_argument if rhs is zero.
         */
        template <u64 N>
        int_mod<N> operator%(int_mod<N> lhs, u64 const rhs)
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

        // Left-u64 versions
        /** \fn constexpr constexpr friend int_mod<N> operator+(u64 const lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of adding int_mod<N> and u64.
         */
        template <u64 N>
        constexpr int_mod<N> operator+(u64 const lhs, int_mod<N> rhs) noexcept
        {
            rhs += lhs;

            return rhs;
        }

        /** \fn constexpr constexpr friend int_mod<N> operator-(u64 const lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of subtracting int_mod<N> by u64.
         */
        template <u64 N>
        constexpr int_mod<N> operator-(u64 const lhs, int_mod<N> rhs) noexcept
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

        /** \fn constexpr constexpr friend int_mod<N> operator*(u64 const lhs, int_mod<N> rhs) noexcept
            \brief Returns the result of multiplying int_mod<N> by u64.
         */
        template <u64 N>
        constexpr int_mod<N> operator*(u64 const lhs, int_mod<N> rhs) noexcept
        {
            rhs *= lhs;

            return rhs;
        }

        /** \fn constexpr friend int_mod<N> operator/(u64 const lhs, int_mod<N> rhs)
            \brief Returns the result of dividing int_mod<N> by u64. Throws std::invalid_argument if rhs is not invertible.
         */
        template <u64 N>
        int_mod<N> operator/(u64 const lhs, int_mod<N> rhs)
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

        /** \fn constexpr friend int_mod<N> operator%(u64 const lhs, int_mod<N> rhs)
            \brief Returns the remainder when dividing int_mod<N> by u64. Throws std::invalid_argument if rhs is zero.
         */
        template <u64 N>
        int_mod<N> operator%(u64 const lhs, int_mod<N> rhs)
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
        template <u64 N>
        std::ostream &operator<<(std::ostream &os, int_mod<N> const &rhs)
        {
            os << rhs.element_;
            return os;
        }

        template <u64 N>
        std::istream &operator>>(std::istream &is, int_mod<N> &rhs)
        {
            is >> rhs.element_;
            num.element_ %= N;
            return is;
        }

        // Implementation Details
        /** \namespace math_nerd::int_mod::impl_details
            \brief Contains implementation details.
         */
        namespace impl_details
        {
            /** \fn constexpr u64 gcd(u64 const a, u64 const b) noexcept
                \brief Computes the greatest common divisor of two integers using the Euclidean algorithm.
             */
            constexpr u64 gcd(u64 const a, u64 const b) noexcept
            {
                if( b == 0 )
                {
                    return a;
                }

                return gcd(b, a%b);
            }

            /** \fn constexpr u64 euler_phi(u64 const n) noexcept
                \brief Computes the Euler phi of an integer.
             */
            constexpr u64 euler_phi(u64 const n) noexcept
            {
                u64 res = 1;

                for( auto i = 2u; i < n; ++i )
                {
                    if( gcd(i, n) == 1 )
                    {
                        ++res;
                    }
                }

                return res;
            }

            /** \fn constexpr u64 ipow(u64 const a, u64 const b, u64 const N)
                \brief Computes a to the power b modulo N.
                \details This is a divide-and-conquer algorithm which takes advantage of integer division to divide the exponent in
                         half at each level of recursion until an exponent of 0 is reached, for which it returns 1.
                         It then multiplies the results together while taking remainders after division by N.
             */
            constexpr u64 ipow(u64 const a, u64 const b, u64 const N)
            {
                if( b == 0 )
                {
                    return 1;
                }

                u64 tmp = ipow(a, b / 2, N);

                if( b % 2 == 0 )
                {
                    return (tmp * tmp) % N;
                }
                else
                {
                    return (a * tmp * tmp) % N;
                }
            }

            /** \fn constexpr u64 inverse_of(u64 const n) noexcept
                \brief Computes the inverse of an integer modulo N. Throws std::invalid_argument if not invertible.
                \brief By Euler's theorem, \f$a^{\phi\left(N\right)}\equiv a\,\left(\text{mod}\,N\right)\f$ for \f$\text{gcd}\left(a,N\right)=1,\f$
                       which implies that that \f$a^{\phi\left(N\right)-1}\equiv 1\,\left(\text{mod}\,N\right)\f$ under the same condition.
             */
            template <u64 N>
            constexpr u64 inverse_of(u64 const n)
            {
                static u64 phi = 0;

                if( phi == 0 )
                {
                    if constexpr(N < 9901)
                    {   // constexpr works for N < 9901 with MSVC.
                        constexpr static u64 phi_const = impl_details::euler_phi(N);
                        phi = phi_const;
                    }
                    else
                    {   // Otherwise, we must calculate at runtime.
                        phi = impl_details::euler_phi(N);
                    }
                }

                u64 inv = 0;
                u64 d = gcd(n, N);

                if( d == 1 )
                {
                    inv = impl_details::ipow(n, phi - 1, N);
                }
                else
                {
                    std::stringstream error_stream;

                    error_stream << n << " is not invertible modulo " << N
                                 << " because gcd(" << n << ", " << N << ") = "
                                 << d << ", which is not 1.\n";

                    std::string error_msg = error_stream.str();

                    throw std::invalid_argument(error_msg);
                }

                return inv;
            }

        } // namespace impl_details

    } // namespace int_mod

} // namespace math_nerd

/** \mainpage Integers Modulo N
    \section gitlab_link GitLab Link
    View the source code at <a href="https://gitlab.com/mathnerd/integers-modulo-n">GitLab</a>.
 */
#endif
