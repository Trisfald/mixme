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

#ifndef MIXME_WRAP_HISTORY_TPP_
#define MIXME_WRAP_HISTORY_TPP_

#include <utility>
#include <type_traits>
#include <cstring>

namespace mixme
{
    namespace wrap
    {
        namespace detail
        {
            template <typename T,
					typename U,
					typename std::enable_if_t<std::is_copy_assignable<T>::value>* = nullptr>
            void copy_or_move_impl(T& from, U& to, bool constructed)
            {
            	if (constructed)
            	{
            		*reinterpret_cast<T*>(to) = from;
            	}
            	else
            	{
            		new (to) T(from);
            	}
            }

            template <typename T,
					typename U,
					typename std::enable_if_t<!std::is_copy_assignable<T>::value &&
							std::is_move_assignable<T>::value>* = nullptr>
            void copy_or_move_impl(T& from, U& to, bool constructed)
            {
            	if (constructed)
            	{
            		*reinterpret_cast<T*>(to) = std::move(from);
            	}
            	else
            	{
            		new (to) T(std::move(from));
            	}
            }

            template <typename T,
					typename U,
					typename std::enable_if_t<!std::is_copy_assignable<T>::value &&
							!std::is_move_assignable<T>::value>* = nullptr>
            void copy_or_move_impl(T&, U&, bool)
            {
            	static_assert(mixme::detail::signal_error<T>::value,
            		"T must be copy-assignable or move-assignable");
            }

            /** Performs copy assignment or, if missing, move assignment */
            template <typename T, typename U>
            void copy_or_move(T& from, U& to, bool constructed)
            {
            	copy_or_move_impl(from, to, constructed);
            }

            template <typename T, typename std::enable_if_t<std::is_copy_assignable<T>::value>* = nullptr>
            void copy_or_move_impl(T& from, T& to)
            {
            	to = from;
            }

            template <typename T,
					typename std::enable_if_t<!std::is_copy_assignable<T>::value &&
							std::is_move_assignable<T>::value>* = nullptr>
            void copy_or_move_impl(T& from, T& to)
            {
            	to = std::move(from);
            }

            template <typename T,
					typename std::enable_if_t<!std::is_copy_assignable<T>::value &&
							!std::is_move_assignable<T>::value>* = nullptr>
            void copy_or_move_impl(T& from, T& to)
            {
            	static_assert(mixme::detail::signal_error<T>::value,
            		"T must be copy-assignable or move-assignable");
            }

            /** Performs copy assignment or, if missing, move assignment */
            template <typename T>
            void copy_or_move(T& from, T& to)
            {
            	copy_or_move_impl(from, to);
            }
        }

        template <typename T, typename Storage_policy>
        constexpr undoable<T, Storage_policy>::undoable(const undoable& other)
		noexcept(noexcept(Storage_policy::copy_construct))
		: base<T>(other)
        {
        	Storage_policy::copy_construct(other.undo_data_, other.undo_bkp_, undo_data_, undo_bkp_);
        }

        template <typename T, typename Storage_policy>
        constexpr undoable<T, Storage_policy>::undoable(undoable&& other)
		noexcept(noexcept(Storage_policy::move_construct))
		: base<T>(std::move(other))
		{
        	Storage_policy::move_construct(std::move(other.undo_data_),
        			std::move(other.undo_bkp_),
        			undo_data_,
					undo_bkp_);
		}

        template <typename T, typename Storage_policy>
        undoable<T, Storage_policy>::~undoable()
		{
        	Storage_policy::dispose(undo_data_, undo_bkp_);
		}

        template <typename T, typename Storage_policy>
        undoable<T, Storage_policy>& undoable<T, Storage_policy>::operator=(const undoable& other)
        noexcept(noexcept(Storage_policy::copy_assign))
		{
        	base<T>::operator=(other);
        	Storage_policy::copy_assign(other.undo_data_, other.undo_bkp_, undo_data_, undo_bkp_);
        	return *this;
		}

        template <typename T, typename Storage_policy>
        undoable<T, Storage_policy>& undoable<T, Storage_policy>::operator=(undoable&& other)
        noexcept(noexcept(Storage_policy::move_assign))
		{
        	base<T>::operator=(std::move(other));
        	Storage_policy::move_assign(std::move(other.undo_data_),
        			std::move(other.undo_bkp_),
        			undo_data_,
					undo_bkp_);
        	return *this;
		}

        template <typename T, typename Storage_policy>
        template <typename U>
		undoable<T, Storage_policy>& undoable<T, Storage_policy>::operator=(U&& other)
        {
        	base<T>::operator=(std::forward<U>(other));
        	return *this;
        }

        template <typename T, typename Storage_policy>
        bool undoable<T, Storage_policy>::save()
        {
        	const bool will_overwrite = ((max_saves() - saves()) == 0);
        	Storage_policy::store(this->value(), undo_data_, undo_bkp_);
            return !will_overwrite;
        }

        template <typename T, typename Storage_policy>
        bool undoable<T, Storage_policy>::undo()
        {
        	if (!has_save())
        	{
        		return false;
        	}
        	Storage_policy::restore(this->value(), undo_data_, undo_bkp_);
            return true;
        }

        template <typename T, typename Storage_policy>
        constexpr redoable<T, Storage_policy>::redoable(const redoable& other)
		noexcept(noexcept(Storage_policy::copy_construct))
		: undoable<T, Storage_policy>(other)
		{
        	Storage_policy::copy_construct(other.redo_data_, other.redo_bkp_, redo_data_, redo_bkp_);
		}

        template <typename T, typename Storage_policy>
        constexpr redoable<T, Storage_policy>::redoable(redoable&& other)
		noexcept(noexcept(Storage_policy::move_construct))
		: undoable<T, Storage_policy>(std::move(other))
		{
        	Storage_policy::move_construct(std::move(other.redo_data_),
        			std::move(other.redo_bkp_),
        			redo_data_,
					redo_bkp_);
		}

        template <typename T, typename Storage_policy>
        redoable<T, Storage_policy>::~redoable()
        {
        	Storage_policy::dispose(redo_data_, redo_bkp_);
        }

        template <typename T, typename Storage_policy>
        redoable<T, Storage_policy>& redoable<T, Storage_policy>::operator=(const redoable& other)
        noexcept(noexcept(Storage_policy::copy_assign))
		{
        	undoable<T, Storage_policy>::operator=(other);
        	Storage_policy::copy_assign(other.redo_data_, other.redo_bkp_, redo_data_, redo_bkp_);
        	return *this;
		}

        template <typename T, typename Storage_policy>
        redoable<T, Storage_policy>& redoable<T, Storage_policy>::operator=(redoable&& other)
        noexcept(noexcept(Storage_policy::move_assign))
		{
        	undoable<T, Storage_policy>::operator=(std::move(other));
        	Storage_policy::move_assign(std::move(other.redo_data_),
        			std::move(other.redo_bkp_),
        			redo_data_,
					redo_bkp_);
        	return *this;
		}

        template <typename T, typename Storage_policy>
        template <typename U>
		redoable<T, Storage_policy>& redoable<T, Storage_policy>::operator=(U&& other)
        {
        	undoable<T, Storage_policy>::operator=(std::forward<U>(other));
        	return *this;
        }

        template <typename T, typename Storage_policy>
        bool redoable<T, Storage_policy>::undo()
		{
        	if (!this->has_save())
        	{
        		return false;
        	}
        	Storage_policy::store(this->value(), redo_data_, redo_bkp_);
        	undoable<T, Storage_policy>::undo();
        	return true;
		}

        template <typename T, typename Storage_policy>
        bool redoable<T, Storage_policy>::redo()
		{
        	if (!this->has_edit())
        	{
        		return false;
        	}
        	Storage_policy::restore(this->value(), redo_data_, redo_bkp_);
        	return true;
		}

        template <typename T>
    	void single_element_storage<T>::copy_construct(const data_type& src,
    			const bookkeeping_type& src_bkp,
    			data_type& dst,
				bookkeeping_type& dst_bkp) noexcept(std::is_nothrow_copy_constructible<T>::value)
        {
        	if (src_bkp)
        	{
        		new (dst) T(*reinterpret_cast<const T*>(src));
        	}
        	dst_bkp = src_bkp;
        }

        template <typename T>
    	void single_element_storage<T>::move_construct(data_type&& src,
    			bookkeeping_type&& src_bkp,
    			data_type& dst,
				bookkeeping_type& dst_bkp) noexcept(std::is_nothrow_move_constructible<T>::value)
    	{
        	if (src_bkp)
        	{
        		new (dst) T(std::move(*reinterpret_cast<T*>(src)));
        	}
        	dst_bkp = std::move(src_bkp);
    	}

        template <typename T>
    	void single_element_storage<T>::copy_assign(const data_type& src,
    			const bookkeeping_type& src_bkp,
    			data_type& dst,
				bookkeeping_type& dst_bkp)
		noexcept(std::is_nothrow_copy_constructible<T>::value && std::is_nothrow_copy_assignable<T>::value)
        {
        	if (src_bkp)
        	{
        		if (dst_bkp)
        		{
        			*reinterpret_cast<T*>(dst) = *reinterpret_cast<T*>(src);
        		}
        		else
        		{
        			new (dst) T(*reinterpret_cast<const T*>(src));
        		}
        	}
        	else
        	{
        		if (dst_bkp)
        		{
        			reinterpret_cast<T*>(dst)->~T();
        		}
        	}
        	dst_bkp = src_bkp;
        }

        template <typename T>
    	void single_element_storage<T>::move_assign(data_type&& src,
    			bookkeeping_type&& src_bkp,
    			data_type& dst,
				bookkeeping_type& dst_bkp)
		noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value)
    	{
        	if (src_bkp)
        	{
        		if (dst_bkp)
        		{
        			*reinterpret_cast<T*>(dst) = std::move(*reinterpret_cast<T*>(src));
        		}
        		else
        		{
        			new (dst) T(std::move(*reinterpret_cast<const T*>(src)));
        		}
        	}
        	else
        	{
        		if (dst_bkp)
        		{
        			reinterpret_cast<T*>(dst)->~T();
        		}
        	}
        	dst_bkp = std::move(src_bkp);
    	}

        template <typename T>
        void single_element_storage<T>::dispose(data_type& data, bookkeeping_type bkp) noexcept
        {
        	if (bkp)
        	{
        		reinterpret_cast<T*>(data)->~T();
        	}
        }

        template <typename T>
        void single_element_storage<T>::store(T& value, data_type& data, bookkeeping_type& bkp)
        {
			detail::copy_or_move(value, data, bkp);
			bkp = true;
        }

        template <typename T>
        void single_element_storage<T>::restore(T& value, data_type& data, bookkeeping_type& bkp)
        {
        	value = std::move(*reinterpret_cast<T*>(data));
        	reinterpret_cast<T*>(data)->~T();
        	bkp = false;
        }

        template <typename T, std::size_t N>
    	void array_storage<T, N>::copy_construct(const data_type& src,
    			const bookkeeping_type& src_bkp,
    			data_type& dst,
				bookkeeping_type& dst_bkp)
		noexcept(std::is_nothrow_copy_assignable<T>::value)
        {
        	dst = src;
        	dst_bkp = src_bkp;
        }

        template <typename T, std::size_t N>
    	void array_storage<T, N>::move_construct(data_type&& src,
    			bookkeeping_type&& src_bkp,
    			data_type& dst,
				bookkeeping_type& dst_bkp)
		noexcept(std::is_nothrow_move_assignable<T>::value)
    	{
        	dst = std::move(src);
        	dst_bkp = std::move(src_bkp);
    	}

        template <typename T, std::size_t N>
    	void array_storage<T, N>::copy_assign(const data_type& src,
    			const bookkeeping_type& src_bkp,
    			data_type& dst,
				bookkeeping_type& dst_bkp)
		noexcept(std::is_nothrow_copy_assignable<T>::value)
        {
        	copy_construct(src, src_bkp, dst, dst_bkp);
        }

        template <typename T, std::size_t N>
    	void array_storage<T, N>::move_assign(data_type&& src,
    			bookkeeping_type&& src_bkp,
    			data_type& dst,
				bookkeeping_type& dst_bkp)
		noexcept(std::is_nothrow_move_assignable<T>::value)
    	{
        	move_construct(std::move(src), std::move(src_bkp), dst, dst_bkp);
    	}

        template <typename T, std::size_t N>
        void array_storage<T, N>::store(T& value, data_type& data, bookkeeping_type& bkp)
        {
        	detail::copy_or_move(value, data[bkp]);
        	if (bkp < max_size(bkp))
        	{
        		bkp++;
        	}
        }

        template <typename T, std::size_t N>
        void array_storage<T, N>::restore(T& value, data_type& data, bookkeeping_type& bkp)
        {
        	value = std::move(data[bkp - 1]);
        	bkp--;
        }
    }
}

#endif
