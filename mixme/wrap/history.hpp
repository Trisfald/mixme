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

#ifndef MIXME_WRAP_HISTORY_HPP_
#define MIXME_WRAP_HISTORY_HPP_

#include <utility>
#include <cstddef>
#include <array>
#include <mixme/detail/types.hpp>
#include <mixme/wrap/base.hpp>

namespace mixme
{
    namespace wrap
    {
    	template <typename T>
    	struct single_element_storage;

    	/**
    	 * Wraps a class giving it the possibility of saving and restoring its state, undoing all modifications
    	 */
        template <typename T, typename Storage_policy = single_element_storage<T>>
        class undoable : public base<T>, protected Storage_policy
        {
        public:
        	using base<T>::base;

        	using value_type = typename base<T>::value_type;

        	constexpr undoable() = default;

            constexpr undoable(const undoable&) noexcept(noexcept(Storage_policy::copy_construct));

            constexpr undoable(undoable&&) noexcept(noexcept(Storage_policy::move_construct));

            ~undoable();

            undoable& operator=(const undoable&) noexcept(noexcept(Storage_policy::copy_assign));

            undoable& operator=(undoable&&) noexcept(noexcept(Storage_policy::move_assign));

            template <typename U>
            undoable& operator=(U&&);

            /** 
             * Saves the current state. It may overwrite one of the current saved states.
             *
             * @returns False if the operation overwrote a previous saved state
             */
            bool save();
            
            /**
             * @returns Whether there's a valid saved state, that a call to undo will restore
             */
            bool has_save() const { return Storage_policy::has_data(undo_bkp_); }
            
            /**
             * @returns The maximum number of storable save states
             */
            std::size_t max_saves() const { return Storage_policy::max_size(undo_bkp_); }

            /**
             * @returns The current number of stored save states
             */
            std::size_t saves() const { return Storage_policy::size(undo_bkp_); }

            /**
             * Restores the saved state, if present
             *
             * @returns True if a saved state has been restored
             */
            bool undo();
        private:
            typename Storage_policy::data_type undo_data_;
            typename Storage_policy::bookkeeping_type undo_bkp_ = typename Storage_policy::bookkeeping_type();
        };

    	/**
    	 * Wraps a class giving it the possibility of saving and restoring its state, undoing all modifications.
    	 * In addition, modifications can be reapplied with the redo operation.
    	 */
        template <typename T, typename Storage_policy = single_element_storage<T>>
        class redoable : public undoable<T, Storage_policy>
        {
        public:
        	using undoable<T, Storage_policy>::undoable;

        	using value_type = typename undoable<T>::value_type;

        	constexpr redoable() = default;

            constexpr redoable(const redoable&) noexcept(noexcept(Storage_policy::copy_construct));

            constexpr redoable(redoable&&) noexcept(noexcept(Storage_policy::move_construct));

            ~redoable();

            redoable& operator=(const redoable&) noexcept(noexcept(Storage_policy::copy_assign));

            redoable& operator=(redoable&&) noexcept(noexcept(Storage_policy::move_assign));

            template <typename U>
            redoable& operator=(U&&);

            /**
             * Restores the saved state, if present
             *
             * @returns True if a saved state has been restored
             */
            bool undo();

            /**
             * Reapplies all modifications lost with the last undo()
             *
             * @returns True if a saved state has been restored
             */
            bool redo();

            /**
             * @returns Whether there's a stored edit state, that a call to redo will restore
             */
            bool has_edit() const { return Storage_policy::has_data(redo_bkp_); }

            /**
             * @returns The maximum number of storable edit states
             */
            std::size_t max_edits() const { return Storage_policy::max_size(redo_bkp_); }

            /**
             * @returns The current number of stored edit states
             */
            std::size_t edits() const { return Storage_policy::size(redo_bkp_); }
        private:
            typename Storage_policy::data_type redo_data_;
            typename Storage_policy::bookkeeping_type redo_bkp_ = false;
        };

    	/**
    	 * Storage consisting in a single element buffer
    	 */
        template <typename T>
    	struct single_element_storage
		{
		protected:
        	using data_type = unsigned char [sizeof(T)];
        	using bookkeeping_type = bool;

        	static bool has_data(bookkeeping_type bkp) { return bkp; }

        	static std::size_t max_size(bookkeeping_type bkp) { return 1; }

        	static std::size_t size(bookkeeping_type bkp) { return (bkp) ? 1 : 0; }

        	static void copy_construct(const data_type& src,
        			const bookkeeping_type& src_bkp,
        			data_type& dst,
					bookkeeping_type& dst_bkp) noexcept(std::is_nothrow_copy_constructible<T>::value);

        	static void move_construct(data_type&& src,
        			bookkeeping_type&& src_bkp,
        			data_type& dst,
					bookkeeping_type& dst_bkp) noexcept(std::is_nothrow_move_constructible<T>::value);

        	static void copy_assign(const data_type& src,
        			const bookkeeping_type& src_bkp,
        			data_type& dst,
					bookkeeping_type& dst_bkp)
        	noexcept(std::is_nothrow_copy_constructible<T>::value && std::is_nothrow_copy_assignable<T>::value);

        	static void move_assign(data_type&& src,
        			bookkeeping_type&& src_bkp,
        			data_type& dst,
					bookkeeping_type& dst_bkp)
        	noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value);

        	static void dispose(data_type&, bookkeeping_type) noexcept;

        	static void store(T&, data_type&, bookkeeping_type&);

        	static void restore(T&, data_type&, bookkeeping_type&);
		};

		/**
		 * Storage consisting in an underlying array
		 */
		template <typename T, std::size_t N>
		struct array_storage
		{
		protected:
			using data_type = std::array<T, N>;
			using bookkeeping_type = std::size_t;

			static bool has_data(bookkeeping_type bkp) { return bkp > 0; }

			static std::size_t max_size(bookkeeping_type bkp) { return N; }

			static std::size_t size(bookkeeping_type bkp) { return bkp; }

        	static void copy_construct(const data_type& src,
        			const bookkeeping_type& src_bkp,
        			data_type& dst,
					bookkeeping_type& dst_bkp)
        	noexcept(std::is_nothrow_copy_assignable<T>::value);

        	static void move_construct(data_type&& src,
        			bookkeeping_type&& src_bkp,
        			data_type& dst,
					bookkeeping_type& dst_bkp)
        	noexcept(std::is_nothrow_move_assignable<T>::value);

        	static void copy_assign(const data_type& src,
        			const bookkeeping_type& src_bkp,
        			data_type& dst,
					bookkeeping_type& dst_bkp)
        	noexcept(std::is_nothrow_copy_assignable<T>::value);

        	static void move_assign(data_type&& src,
        			bookkeeping_type&& src_bkp,
        			data_type& dst,
					bookkeeping_type& dst_bkp)
        	noexcept(std::is_nothrow_move_assignable<T>::value);

        	static void dispose(data_type&, bookkeeping_type) noexcept {}

			static void store(T&, data_type&, bookkeeping_type&);

			static void restore(T&, data_type&, bookkeeping_type&);
		};
    }
}

#include <mixme/wrap/impl/history.tpp>

#endif
