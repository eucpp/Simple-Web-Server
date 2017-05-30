#ifndef MACRO_HPP
#define MACRO_HPP

#if defined(PRECISE_GC_SERIAL) || defined(PRECISE_GC_CMS)
    #include <liballocgc/liballocgc.hpp>
#endif

#ifdef SHARED_PTR
    #include <memory>
#endif

#if defined(PRECISE_GC_SERIAL)

    #define ptr_t(T) allocgc::serial::gc_ptr<T>
    #define ptr_in(T) const allocgc::serial::gc_ptr<T>&
    #define ref_t(T) allocgc::serial::gc_ref<T>
    #define pin_t(T) allocgc::serial::gc_pin<T>
    #define pin(ptr) ptr.pin()

    #define raw_ptr(pin_ptr) pin_ptr.get()

    #define ptr_array_t(T) allocgc::serial::gc_ptr<T[]>
    #define pin_array_t(T) allocgc::serial::gc_pin<T[]>

    #define new_(T) allocgc::serial::gc_new<T>()
    #define new_args_(T, ...) allocgc::serial::gc_new<T>(__VA_ARGS__)
    #define new_array_(T, size) allocgc::serial::gc_new<T[]>(size)

    #define delete_(ptr)
    #define set_null(ptr) ptr.reset()
    #define null_ptr(T) allocgc::serial::gc_ptr<T>()

    #define const_pointer_cast_(T, ptr) allocgc::pointers::const_pointer_cast<T>(ptr)
    #define static_pointer_cast_(T, ptr) allocgc::pointers::static_pointer_cast<T>(ptr)
    #define dynamic_pointer_cast_(T, ptr) allocgc::pointers::dynamic_pointer_cast<T>(ptr)
    #define reinterpret_pointer_cast_(T, ptr) allocgc::pointers::reinterpret_pointer_cast<T>(ptr)

    #define const_array_pointer_cast_(T, ptr) allocgc::pointers::const_pointer_cast<T[]>(ptr)
    #define reinterpret_array_pointer_cast_(T, ptr) allocgc::pointers::reinterpret_pointer_cast<T[]>(ptr)

    #define ptr_to(ptr, T, R, Mem) allocgc::pointers::take_interior<T, R, Mem>(ptr)

#elif defined(PRECISE_GC_CMS)

    #define ptr_t(T) allocgc::cms::gc_ptr<T>
    #define ptr_in(T) const allocgc::cms::gc_ptr<T>&
    #define ref_t(T) allocgc::cms::gc_ref<T>
    #define pin_t(T) allocgc::cms::gc_pin<T>
    #define pin(ptr) ptr.pin()

    #define raw_ptr(pin_ptr) pin_ptr.get()

    #define ptr_array_t(T) allocgc::cms::gc_ptr<T[]>
    #define pin_array_t(T) allocgc::cms::gc_pin<T[]>

    #define new_(T) allocgc::cms::gc_new<T>()
    #define new_args_(T, ...) allocgc::cms::gc_new<T>(__VA_ARGS__)
    #define new_array_(T, size) allocgc::cms::gc_new<T[]>(size)

    #define delete_(ptr)
    #define set_null(ptr) ptr.reset()
    #define null_ptr(T) allocgc::cms::gc_ptr<T>()

    #define const_pointer_cast_(T, ptr) allocgc::pointers::const_pointer_cast<T>(ptr)
    #define static_pointer_cast_(T, ptr) allocgc::pointers::static_pointer_cast<T>(ptr)
    #define dynamic_pointer_cast_(T, ptr) allocgc::pointers::dynamic_pointer_cast<T>(ptr)
    #define reinterpret_pointer_cast_(T, ptr) allocgc::pointers::reinterpret_pointer_cast<T>(ptr)

    #define const_array_pointer_cast_(T, ptr) allocgc::pointers::const_pointer_cast<T[]>(ptr)
    #define reinterpret_array_pointer_cast_(T, ptr) allocgc::pointers::reinterpret_pointer_cast<T[]>(ptr)

    #define ptr_to(ptr, T, R, Mem) allocgc::pointers::take_interior<T, R, Mem>(ptr)

#elif defined(SHARED_PTR)

    #define ptr_t(T) std::shared_ptr<T>
    #define ptr_in(T) const std::shared_ptr<T>&
    #define ref_t(T) T&
    #define pin_t(T) T*
    #define pin(ptr) ptr.get()

    #define raw_ptr(pin_ptr) pin_ptr

    #define ptr_array_t(T) std::shared_ptr<T>
    #define pin_array_t(T) T*

    #define new_(T) std::make_shared<T>()
    #define new_args_(T, ...) std::make_shared<T>(__VA_ARGS__)
    #define new_array_(T, size) std::shared_ptr<T>(new std::remove_cv<T>::type[size], std::default_delete<T[]>())

    #define delete_(ptr)
    #define set_null(ptr) ptr.reset()
    #define null_ptr(T) std::shared_ptr<T>()

    template <typename T, typename U>
    std::shared_ptr<T> reinterpret_pointer_cast(const std::shared_ptr<U>& ptr)
    {
        typedef typename std::remove_extent<T>::type V;
        V* casted = reinterpret_cast<V*>(ptr.get());
        return std::shared_ptr<V>(ptr, casted);
    }

    #define static_pointer_cast_(T, ptr) std::static_pointer_cast<T>(ptr)
    #define dynamic_pointer_cast_(T, ptr) std::dynamic_pointer_cast<T>(ptr)
    #define const_pointer_cast_(T, ptr) std::const_pointer_cast<T>(ptr)
    #define reinterpret_pointer_cast_(T, ptr) reinterpret_pointer_cast<T>(ptr)

    #define const_array_pointer_cast_(T, ptr) std::const_pointer_cast<T>(ptr)
    #define reinterpret_array_pointer_cast_(T, ptr) reinterpret_pointer_cast<T>(ptr)

    #define ptr_to(ptr, T, R, Mem) std::shared_ptr<R>(ptr, &(ptr.get()->*Mem))

#endif

#endif //MACRO_HPP
