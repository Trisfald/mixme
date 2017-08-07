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

#ifndef MIXME_GIFT_COMPARISON_TPP_
#define MIXME_GIFT_COMPARISON_TPP_

#include <mixme/detail/types.hpp>
#include <mixme/detail/check/comparison.hpp>
#include <type_traits>

namespace mixme
{
    namespace gift
    {
    	namespace comparison
		{
			namespace detail
			{
				template <typename T, bool Check>
				struct eq_impl_ge
				{
					constexpr static bool valid = true;
					constexpr bool operator()(const T& lhs, const T& rhs) { return lhs >= rhs && rhs >= lhs; }
				};

				template <typename T>
				struct eq_impl_ge<T, false>
				{
					constexpr static bool valid = false;
					static_assert(mixme::detail::signal_error<T>::value,
							"Can't provide operator ==. "
							"Class must have at least one of these operators: !=, <, >, <=, >=.");
				};

				template <typename T, bool Check>
				struct eq_impl_le
				{
					constexpr static bool valid = true;
					constexpr bool operator()(const T& lhs, const T& rhs) { return lhs <= rhs && rhs <= lhs; }
				};

				template <typename T>
				struct eq_impl_le<T, false>
				{
					constexpr static bool valid = eq_impl_ge<T, mixme::detail::check::comparison::ge<T>{}>::valid;
					constexpr bool operator()(const T& lhs, const T& rhs)
					{
						return eq_impl_ge<T, mixme::detail::check::comparison::ge<T>{}>{}(lhs, rhs);
					}
				};

				template <typename T, bool Check>
				struct eq_impl_gt
				{
					constexpr static bool valid = true;
					constexpr bool operator()(const T& lhs, const T& rhs) { return !(lhs > rhs) && !(rhs > lhs); }
				};

				template <typename T>
				struct eq_impl_gt<T, false>
				{
					constexpr static bool valid = eq_impl_le<T, mixme::detail::check::comparison::le<T>{}>::valid;
					constexpr bool operator()(const T& lhs, const T& rhs)
					{
						return eq_impl_le<T, mixme::detail::check::comparison::le<T>{}>{}(lhs, rhs);
					}
				};

				template <typename T, bool Check>
				struct eq_impl_lt
				{
					constexpr static bool valid = true;
					constexpr bool operator()(const T& lhs, const T& rhs) { return !(lhs < rhs) && !(rhs < lhs); }
				};

				template <typename T>
				struct eq_impl_lt<T, false>
				{
					constexpr static bool valid = eq_impl_gt<T, mixme::detail::check::comparison::gt<T>{}>::valid;
					constexpr bool operator()(const T& lhs, const T& rhs)
					{
						return eq_impl_gt<T, mixme::detail::check::comparison::gt<T>{}>{}(lhs, rhs);
					}
				};

				template <typename T, bool Check>
				struct eq_impl_ne
				{
					constexpr static bool valid = true;
					constexpr bool operator()(const T& lhs, const T& rhs) { return !(lhs != rhs); }
				};

				template <typename T>
				struct eq_impl_ne<T, false>
				{
					constexpr static bool valid = eq_impl_lt<T, mixme::detail::check::comparison::lt<T>{}>::valid;
					constexpr bool operator()(const T& lhs, const T& rhs)
					{
						return eq_impl_lt<T, mixme::detail::check::comparison::lt<T>{}>{}(lhs, rhs);
					}
				};

				template <typename T, bool Check>
				struct lt_impl_ge
				{
					constexpr static bool valid = true;
					constexpr bool operator()(const T& lhs, const T& rhs) { return rhs >= lhs; }
				};

				template <typename T>
				struct lt_impl_ge<T, false>
				{
					constexpr static bool valid = false;
					static_assert(mixme::detail::signal_error<T>::value,
							"Can't provide operator <. Class must have at least one of these operators: >, <=, >=.");
				};

				template <typename T, bool Check>
				struct lt_impl_le
				{
					constexpr static bool valid = true;
					constexpr bool operator()(const T& lhs, const T& rhs) { return lhs <= rhs && !(rhs <= lhs); }
				};

				template <typename T>
				struct lt_impl_le<T, false>
				{
					constexpr static bool valid = lt_impl_ge<T, mixme::detail::check::comparison::ge<T>{}>::valid;
					constexpr bool operator()(const T& lhs, const T& rhs)
					{
						return lt_impl_ge<T, mixme::detail::check::comparison::ge<T>{}>{}(lhs, rhs);
					}
				};

				template <typename T, bool Check>
				struct lt_impl_gt
				{
					constexpr static bool valid = true;
					constexpr bool operator()(const T& lhs, const T& rhs) { return rhs > lhs; }
				};

				template <typename T>
				struct lt_impl_gt<T, false>
				{
					constexpr static bool valid = lt_impl_le<T, mixme::detail::check::comparison::le<T>{}>::valid;
					constexpr bool operator()(const T& lhs, const T& rhs)
					{
						return lt_impl_le<T, mixme::detail::check::comparison::le<T>{}>{}(lhs, rhs);
					}
				};

				template <typename T, template <typename, bool> class Gen, template <typename...> class Comp_check>
				constexpr bool valid = Gen<T, Comp_check<T>{}>::valid;

				template <typename T,
						template <typename, bool> class Gen,
						template <typename...> class Comp_check,
						bool C>
				struct lazy_valid_selector
				{
					constexpr static bool value = true;
				};

				template <typename T, template <typename, bool> class Gen, template <typename...> class Comp_check>
				struct lazy_valid_selector<T, Gen, Comp_check, false>
				{
					constexpr static bool value = valid<T, Gen, Comp_check>;
				};

	    		namespace ne_ext
				{
					template <typename T>
					constexpr bool operator==(const T& lhs, const T& rhs)
					{
						return detail::eq_impl_ne<T, mixme::detail::check::comparison::ne<T>{}>{}(
								static_cast<const ne<T>&>(lhs).impl(),
								static_cast<const ne<T>&>(rhs).impl());
					}
				}

	    		namespace le_ext
				{
					template <typename T>
					constexpr bool operator<(const T& lhs, const T& rhs)
					{
						return detail::lt_impl_gt<T, mixme::detail::check::comparison::gt<T>{}>{}(
								static_cast<const le<T>&>(lhs).impl(),
								static_cast<const le<T>&>(rhs).impl());
					}
				}

	    		namespace gt_ext
				{
					template <typename T>
					constexpr bool operator<(const T& lhs, const T& rhs)
					{
						return detail::lt_impl_gt<T, mixme::detail::check::comparison::gt<T>{}>{}(
								static_cast<const gt<T>&>(lhs).impl(),
								static_cast<const gt<T>&>(rhs).impl());
					}
				}

	    		namespace ge_ext
				{
					template <typename T>
					constexpr bool operator<(const T& lhs, const T& rhs)
					{
						return detail::lt_impl_gt<T, mixme::detail::check::comparison::gt<T>{}>{}(
								static_cast<const ge<T>&>(lhs).impl(),
								static_cast<const ge<T>&>(rhs).impl());
					}
				}
			}

    		template <typename T>
    		constexpr bool operator==(const eq<T>& lhs, const eq<T>& rhs)
			{
    			return detail::eq_impl_ne<T, mixme::detail::check::comparison::ne<T>{}>{}(lhs.impl(), rhs.impl());
			}

    		template <typename T>
    		constexpr bool operator!=(const ne<T>& lhs, const ne<T>& rhs)
			{
    			using namespace detail::ne_ext;
    			static_assert(detail::lazy_valid_selector<T,
    							detail::eq_impl_ne,
								mixme::detail::check::comparison::ne,
								mixme::detail::check::comparison::eq<T>{}>::value,
    					"Can't provide operator !=. Generation of operator == failed.");
    			return !(lhs.impl() == rhs.impl());
			}

    		template <typename T>
    		constexpr bool operator<(const lt<T>& lhs, const lt<T>& rhs)
			{
    			return detail::lt_impl_gt<T, mixme::detail::check::comparison::gt<T>{}>{}(lhs.impl(), rhs.impl());
			}

    		template <typename T>
    		constexpr bool operator<=(const le<T>& lhs, const le<T>& rhs)
			{
    			using namespace detail::le_ext;
    			static_assert(detail::lazy_valid_selector<T,
    							detail::lt_impl_gt,
								mixme::detail::check::comparison::gt,
								mixme::detail::check::comparison::lt<T>{}>::value,
    					"Can't provide operator <=. Generation of operator < failed.");
    			return lhs.impl() < rhs.impl() || (!(lhs.impl() < rhs.impl()) && !(rhs.impl() < lhs.impl()));
			}

    		template <typename T>
    		constexpr bool operator>(const gt<T>& lhs, const gt<T>& rhs)
			{
    			using namespace detail::gt_ext;
    			static_assert(detail::lazy_valid_selector<T,
    							detail::lt_impl_gt,
								mixme::detail::check::comparison::gt,
								mixme::detail::check::comparison::lt<T>{}>::value,
    					"Can't provide operator >. Generation of operator < failed.");
    			return rhs.impl() < lhs.impl();
			}

    		template <typename T>
    		constexpr bool operator>=(const ge<T>& lhs, const ge<T>& rhs)
			{
    			using namespace detail::ge_ext;
    			static_assert(detail::lazy_valid_selector<T,
    							detail::lt_impl_gt,
								mixme::detail::check::comparison::gt,
								mixme::detail::check::comparison::lt<T>{}>::value,
    					"Can't provide operator >=. Generation of operator < failed.");
    			return rhs.impl() < lhs.impl() || (!(rhs.impl() < lhs.impl()) && !(lhs.impl() < rhs.impl()));
			}
        }        
    }
}

#endif
