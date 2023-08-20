#pragma once
#include <stdint.h>
#include <type_traits>


namespace ObjectModel
{

	enum class Wrapper : uint8_t
	{
		PRIMITIVE = 1,
		ARRAY,
		STRING,
		OBJECT
	};

	enum class Type : uint8_t
	{
		I8 = 1,
		I16,
		I32,
		I64,

		FLOAT,
		DOUBLE,

		BOOL
	};

	template<typename ...>
	uint8_t getTypeSize(Type type)
	{
		switch (type)
		{
		case Type::BOOL: return sizeof(bool); break;
		case Type::I8: return sizeof(uint8_t); break;
		case Type::I16: return sizeof(int16_t); break;
		case Type::I32: return sizeof(int32_t); break;
		case Type::I64: return sizeof(int64_t); break;
		case Type::FLOAT: return sizeof(float); break;
		case Type::DOUBLE: return sizeof(double); break;
		}
		return 0;
	}


namespace Util{

    template<typename T, typename _ = void>
    struct selector {
        static const uint8_t value = (sizeof(T) <= 2u) ? (
                                                            (uint8_t)sizeof(T)
                                                         ) : (
                                                            (sizeof(T) == 4u) ? (uint8_t)Type::I32 : (uint8_t)Type::I64
                                                         );
    };

    template<typename T>
    struct selector<
            T,
            std::void_t<
                typename std::enable_if<std::is_floating_point<T>::value || std::is_same<T, bool>::value>::type
            >
            >
    {
        static const uint8_t value = (sizeof(T) < 2u) ? (
                                                            (uint8_t)Type::BOOL
                                                         ) : (
                                                            (sizeof(T) == 4u) ? (uint8_t)Type::FLOAT : (uint8_t)Type::DOUBLE
                                                         );
    };

    template<typename T, typename _ = void>
    struct is_container : std::false_type {};

    //template<typename... Ts>
    //struct is_container_helper {};

    //template<typename T>
    //struct is_container<
    //        T,
    //        std::conditional_t<
    //            false,
    //            is_container_helper<
    //                typename T::value_type,
    //                typename T::size_type,
    //                typename T::allocator_type,
    //                typename T::iterator,
    //                typename T::const_iterator,
    //                decltype(std::declval<T>().size()),
    //                decltype(std::declval<T>().begin()),
    //                decltype(std::declval<T>().end()),
    //                decltype(std::declval<T>().cbegin()),
    //                decltype(std::declval<T>().cend())
    //                >,
    //            void
    //            >
    //        > : public std::true_type {};

    template<typename T>
    struct is_container<
            T,
            std::void_t<
                typename std::enable_if<std::is_arithmetic<typename T::value_type>::value>::type,
                typename T::value_type,
                decltype(std::declval<T>().size()),
                decltype(std::declval<T>().push_back(std::declval<typename T::value_type>())),
                decltype(std::declval<T>().operator[](std::declval<size_t>()))
                >
            > : public std::true_type {};

}

}
