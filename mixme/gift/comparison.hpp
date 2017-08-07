// Copyright (C) 2017 Andrea Spurio. All rights reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef MIXME_GIFT_COMPARISON_HPP_
#define MIXME_GIFT_COMPARISON_HPP_

namespace mixme
{
    namespace gift
    {
    	namespace comparison
		{
    		/**
    		 * Gifts the equal to operator to a derived class.
    		 *
    		 * T must be the derived class type.
    		 * T must define at least one of these operators: !=, <, >, <=, >=
    		 */
    		template <typename T>
    		struct eq
			{
    			constexpr T& impl() { return *static_cast<T*>(this); }
    			constexpr const T& impl() const { return *static_cast<const T*>(this); }
			};

    		/**
    		 * Gifts the not equal to operator to a derived class.
    		 *
    		 * T must be the derived class type.
    		 * T must define at least one of these operators: ==, <, >, <=, >=
    		 */
    		template <typename T>
    		struct ne
			{
    			constexpr T& impl() { return *static_cast<T*>(this); }
    			constexpr const T& impl() const { return *static_cast<const T*>(this); }
			};

    		/**
    		 * Gifts the less than operator to a derived class.
    		 *
    		 * T must be the derived class type.
    		 * T must define at least one of these operators: >, <=, >=
    		 */
    		template <typename T>
    		struct lt
			{
    			constexpr T& impl() { return *static_cast<T*>(this); }
    			constexpr const T& impl() const { return *static_cast<const T*>(this); }
			};

    		/**
    		 * Gifts the less than or equal to operator to a derived class.
    		 *
    		 * T must be the derived class type.
    		 * T must define at least one of these operators: <, >, >=
    		 */
    		template <typename T>
    		struct le
			{
    			constexpr T& impl() { return *static_cast<T*>(this); }
    			constexpr const T& impl() const { return *static_cast<const T*>(this); }
			};

    		/**
    		 * Gifts the greater than operator to a derived class.
    		 *
    		 * T must be the derived class type.
    		 * T must define at least one of these operators: <, <=, >=
    		 */
    		template <typename T>
    		struct gt
			{
    			constexpr T& impl() { return *static_cast<T*>(this); }
    			constexpr const T& impl() const { return *static_cast<const T*>(this); }
			};

    		/**
    		 * Gifts the greater than or equal to operator to a derived class.
    		 *
    		 * T must be the derived class type.
    		 * T must define at least one of these operators: <, >, <=
    		 */
    		template <typename T>
    		struct ge
			{
    			constexpr T& impl() { return *static_cast<T*>(this); }
    			constexpr const T& impl() const { return *static_cast<const T*>(this); }
			};

    		/**
    		 * Gifts all operators to a derived class.
    		 *
    		 * T must be the derived class type.
    		 * T must define at least one of these operators: <, >, <=, >=
    		 */
    		template <typename T>
    		struct all : public eq<T>, public ne<T>, public lt<T>, public le<T>, public gt<T>, public ge<T>
    		{
    			using eq<T>::impl;
    		};

    		template <typename T>
    		constexpr bool operator==(const eq<T>&, const eq<T>&);

    		template <typename T>
    		constexpr bool operator!=(const ne<T>&, const ne<T>&);

    		template <typename T>
    		constexpr bool operator<(const lt<T>&, const lt<T>&);

    		template <typename T>
    		constexpr bool operator<=(const le<T>&, const le<T>&);

    		template <typename T>
    		constexpr bool operator>(const gt<T>&, const gt<T>&);

    		template <typename T>
    		constexpr bool operator>=(const ge<T>&, const ge<T>&);
		}
	}
}

#include <mixme/gift/impl/comparison.tpp>

#endif
