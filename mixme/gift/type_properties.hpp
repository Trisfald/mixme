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

#ifndef MIXME_GIFT_TYPE_PROPERTIES_HPP_
#define MIXME_GIFT_TYPE_PROPERTIES_HPP_ 

#include <type_traits>

namespace mixme
{
    namespace gift
    {
        /**
         * Derive from this class to inhibits copy and move operations.
         */
    	struct no_copy_or_move
		{
    		no_copy_or_move() = default;

    		no_copy_or_move(const no_copy_or_move&) = delete;
    		no_copy_or_move(no_copy_or_move&&) = delete;

    		no_copy_or_move& operator=(const no_copy_or_move&) = delete;
        	no_copy_or_move& operator=(no_copy_or_move&&) = delete;
		};

        /**
         * Derive from this class to inhibits move operations.
		 * Note that it'll be still possible to assign or copy initialize from a temporary object.
         */
        struct copy_only
        {
        	copy_only() = default;
            
        	copy_only(const copy_only&) = default;
        	copy_only(copy_only&&) = delete;
            
        	copy_only& operator=(const copy_only&) = default;
        	copy_only& operator=(copy_only&&) = delete;
        };

        /**
         * Derive from this class to inhibits copy operations
         */        
        struct move_only
        {
        	move_only() = default;
            
        	move_only(const move_only&) = delete;
        	move_only(move_only&&) = default;
            
        	move_only& operator=(const move_only&) = delete;
        	move_only& operator=(move_only&&) = default;
        };        
    }
}

#endif
