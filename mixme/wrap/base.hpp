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

#ifndef MIXME_WRAP_BASE_HPP_
#define MIXME_WRAP_BASE_HPP_

#include <utility>
#include <type_traits>

namespace mixme
{
    namespace wrap
    {
        template <typename T>
        class base
        {
        public:
            using value_type = T;
            
            constexpr base() = default;
            
            constexpr base(const base& other) : value_(other.value_) {}
            
            constexpr base(base&& other) noexcept(std::is_nothrow_move_constructible<T>::value)
            : value_(std::move(other.value_)) {}

            template <typename U, typename std::enable_if_t<!std::is_base_of<base, std::decay_t<U>>::value>* = nullptr>
            constexpr base(U&& value) : value_(std::forward<U>(value)) {}

            template <typename... Args,
				typename std::enable_if_t<(sizeof...(Args) > 1)>* = nullptr>
            constexpr base(Args&&... args) : value_(std::forward<Args>(args)...) {}
            
            base& operator=(const base&);
            
            base& operator=(base&&) noexcept(std::is_nothrow_move_assignable<T>::value);
            
            template <typename U, typename std::enable_if_t<!std::is_base_of<base, std::decay_t<U>>::value>* = nullptr>
            base& operator=(U&&);
            
            constexpr T* operator->() { return &value_; }
            
            constexpr const T* operator->() const { return &value_; }
            
            constexpr T& operator*() & { return value_; }
            
            constexpr const T& operator*() const & { return value_; }
            
            constexpr T&& operator*() && { return std::move(value_); }
            
            constexpr const T&& operator*() const && { return std::move(value_); }
            
            constexpr T& value() noexcept { return value_; }
            
            constexpr const T& value() const noexcept { return value_; }
        private:
            T value_;
        };

        template <typename T, typename U>
        constexpr bool operator==(const base<T>& lhs, const base<U>& rhs) { return lhs.value() == rhs.value(); }

        template <typename T>
        constexpr bool operator==(const base<T>& lhs, const T& rhs) { return lhs.value() == rhs; }

        template <typename T>
        constexpr bool operator==(const T& lhs, const base<T>& rhs) { return lhs == rhs.value(); }

        template <typename T, typename U>
		constexpr bool operator!=(const base<T>& lhs, const base<U>& rhs) { return lhs.value() != rhs.value(); }

        template <typename T>
        constexpr bool operator!=(const base<T>& lhs, const T& rhs) { return lhs.value() != rhs; }

        template <typename T>
        constexpr bool operator!=(const T& lhs, const base<T>& rhs) { return lhs != rhs.value(); }

        template <typename T, typename U>
		constexpr bool operator<(const base<T>& lhs, const base<U>& rhs) { return lhs.value() < rhs.value(); }

        template <typename T>
        constexpr bool operator<(const base<T>& lhs, const T& rhs) { return lhs.value() < rhs; }

        template <typename T>
		constexpr bool operator<(const T& lhs, const base<T>& rhs) { return lhs < rhs.value(); }

        template <typename T, typename U>
		constexpr bool operator<=(const base<T>& lhs, const base<U>& rhs) { return lhs.value() <= rhs.value(); }

        template <typename T>
        constexpr bool operator<=(const base<T>& lhs, const T& rhs) { return lhs.value() <= rhs; }

        template <typename T>
        constexpr bool operator<=(const T& lhs, const base<T>& rhs) { return lhs <= rhs.value(); }

        template <typename T, typename U>
		constexpr bool operator>(const base<T>& lhs, const base<U>& rhs) { return lhs.value() > rhs.value(); }

        template <typename T>
        constexpr bool operator>(const base<T>& lhs, const T& rhs) { return lhs.value() > rhs; }

        template <typename T>
        constexpr bool operator>(const T& lhs, const base<T>& rhs) { return lhs > rhs.value(); }

        template <typename T, typename U>
		constexpr bool operator>=(const base<T>& lhs, const base<U>& rhs) { return lhs.value() >= rhs.value(); }

        template <typename T>
        constexpr bool operator>=(const base<T>& lhs, const T& rhs) { return lhs.value() >= rhs; }

        template <typename T>
		constexpr bool operator>=(const T& lhs, const base<T>& rhs) { return lhs >= rhs.value(); }

        template <typename T>
        void swap(base<T>& lhs, base<T>& rhs);
    }
}

#include <mixme/wrap/impl/base.tpp>

#endif
