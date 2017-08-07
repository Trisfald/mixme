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

#ifndef MIXME_WRAP_BASE_TPP_
#define MIXME_WRAP_BASE_TPP_

namespace mixme
{
    namespace wrap
    {
    	template <typename T>
        base<T>& base<T>::operator=(const base& other)
        { 
        	value_ = other.value_;
        	return *this;
        }
        
        template <typename T>
        base<T>& base<T>::operator=(base&& other) noexcept(std::is_nothrow_move_assignable<T>::value)
        { 
        	value_ = std::move(other.value_);
        	return *this;
        }
        
        template <typename T>
        template <typename U, typename std::enable_if_t<!std::is_base_of<base<T>, std::decay_t<U>>::value>*>
        base<T>& base<T>::operator=(U&& other) 
        { 
        	value_ = std::forward<U>(other);
        	return *this;
        }    
        
        template <typename T>
        void swap(base<T>& lhs, base<T>& rhs)
        {
        	using std::swap;
        	swap(lhs.value(), rhs.value());
        }        
    }
}

#endif
