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

#ifndef MIXME_DETAIL_CHECK_COMPARISON_HPP_
#define MIXME_DETAIL_CHECK_COMPARISON_HPP_

#include <type_traits>

namespace mixme
{
	namespace detail
	{
		namespace check
		{
			namespace comparison
			{
				struct No {};

				template <typename T, typename U>
				No operator==(const T&, const U&);

				/**
				 * Checks whether T has an equal to operator declared
				 */
				template <typename T, typename U = T>
				struct eq : std::integral_constant<bool,
						!std::is_same<decltype(*(T*)(nullptr) == *(U*)(nullptr)), No>::value> {};

				template <typename T, typename U>
				No operator!=(const T&, const U&);

				/**
				 * Checks whether T has a not equal to operator declared
				 */
				template <typename T, typename U = T>
				struct ne : std::integral_constant<bool,
						!std::is_same<decltype(*(T*)(nullptr) != *(U*)(nullptr)), No>::value> {};

				template <typename T, typename U>
				No operator<(const T&, const U&);

				/**
				 * Checks whether T has a less than operator declared
				 */
				template <typename T, typename U = T>
				struct lt : std::integral_constant<bool,
						!std::is_same<decltype(*(T*)(nullptr) < *(U*)(nullptr)), No>::value> {};

				template <typename T, typename U>
				No operator>(const T&, const U&);

				/**
				 * Checks whether T has a greater than operator declared
				 */
				template <typename T, typename U = T>
				struct gt : std::integral_constant<bool,
						!std::is_same<decltype(*(T*)(nullptr) > *(U*)(nullptr)), No>::value> {};

				template <typename T, typename U>
				No operator<=(const T&, const U&);

				/**
				 * Checks whether T has a less than or equal to operator declared
				 */
				template <typename T, typename U = T>
				struct le : std::integral_constant<bool,
						!std::is_same<decltype(*(T*)(nullptr) <= *(U*)(nullptr)), No>::value> {};

				template <typename T, typename U>
				No operator>=(const T&, const U&);

				/**
				 * Checks whether T has a greater than or equal to operator declared
				 */
				template <typename T, typename U = T>
				struct ge : std::integral_constant<bool,
						!std::is_same<decltype(*(T*)(nullptr) >= *(U*)(nullptr)), No>::value> {};
			}

			namespace comparison_lax // check comparisons keeping track of inheritance
			{
				template <typename T, typename Y>
				struct eq_impl
				{
				    template <typename U, typename V>
				    static auto test(U*) -> decltype(std::declval<U>() == std::declval<V>());

				    template <typename, typename>
				    static auto test(...) -> std::false_type;

				    static constexpr auto value = std::is_same<bool, decltype(test<T, Y>(0))>::value;
				};

				/**
				 * Checks whether T has an equal to operator declared
				 */
				template <typename T, typename U = T>
				struct eq : std::integral_constant<bool, eq_impl<T, T>::value> {};

				template <typename T, typename Y>
				struct ne_impl
				{
				    template <typename U, typename V>
				    static auto test(U*) -> decltype(std::declval<U>() != std::declval<V>());

				    template <typename, typename>
				    static auto test(...) -> std::false_type;

				    static constexpr auto value = std::is_same<bool, decltype(test<T, Y>(0))>::value;
				};

				/**
				 * Checks whether T has a not equal to operator declared
				 */
				template <typename T, typename U = T>
				struct ne : std::integral_constant<bool, ne_impl<T, T>::value> {};

				template <typename T, typename Y>
				struct lt_impl
				{
				    template <typename U, typename V>
				    static auto test(U*) -> decltype(std::declval<U>() < std::declval<V>());

				    template <typename, typename>
				    static auto test(...) -> std::false_type;

				    static constexpr auto value = std::is_same<bool, decltype(test<T, Y>(0))>::value;
				};

				/**
				 * Checks whether T has a less than operator declared
				 */
				template <typename T, typename U = T>
				struct lt : std::integral_constant<bool, lt_impl<T, T>::value> {};

				template <typename T, typename Y>
				struct gt_impl
				{
				    template <typename U, typename V>
				    static auto test(U*) -> decltype(std::declval<U>() > std::declval<V>());

				    template <typename, typename>
				    static auto test(...) -> std::false_type;

				    static constexpr auto value = std::is_same<bool, decltype(test<T, Y>(0))>::value;
				};

				/**
				 * Checks whether T has a greater than operator declared
				 */
				template <typename T, typename U = T>
				struct gt : std::integral_constant<bool, gt_impl<T, T>::value> {};

				template <typename T, typename Y>
				struct le_impl
				{
				    template <typename U, typename V>
				    static auto test(U*) -> decltype(std::declval<U>() <= std::declval<V>());

				    template <typename, typename>
				    static auto test(...) -> std::false_type;

				    static constexpr auto value = std::is_same<bool, decltype(test<T, Y>(0))>::value;
				};

				/**
				 * Checks whether T has a less than or equal to operator declared
				 */
				template <typename T, typename U = T>
				struct le : std::integral_constant<bool, le_impl<T, T>::value> {};

				template <typename T, typename Y>
				struct ge_impl
				{
				    template <typename U, typename V>
				    static auto test(U*) -> decltype(std::declval<U>() >= std::declval<V>());

				    template <typename, typename>
				    static auto test(...) -> std::false_type;

				    static constexpr auto value = std::is_same<bool, decltype(test<T, Y>(0))>::value;
				};

				/**
				 * Checks whether T has a greater than or equal to operator declared
				 */
				template <typename T, typename U = T>
				struct ge : std::integral_constant<bool, ge_impl<T, T>::value> {};
			}
		}
	}
}

#endif
