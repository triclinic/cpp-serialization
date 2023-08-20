#pragma once
#include <type_traits>
#include <vector>
#include <string>
#include <stdint.h>

#include "meta.hpp"

namespace ObjectModel
{

    class SerializationContext{
        std::vector<uint8_t> data;
        friend class SerializationView;
    public:
        SerializationContext(uint8_t size = 0): data(){ data.reserve(size); }

        template<class T>
        typename std::enable_if<
            std::is_integral<T>::value
            && !std::is_same<T, bool>::value,
            SerializationContext&
        >::type encode(T value)
        {
            for (unsigned i = 0; i < sizeof (T); i++)
            {
                data.push_back((uint8_t)(value >> (((sizeof(T) * 8) - 8) - i * 8)));
            }
            return *this;
        }


        template<class T>
        typename std::enable_if<
            std::is_same<T, bool>::value,
            SerializationContext&
        >::type encode(T value)
        {
            data.push_back((value) ? 0x7F : 0x00);
            return *this;
        }


        template<class T>
        typename std::enable_if<
            std::is_floating_point<T>::value,
            SerializationContext&
        >::type encode(T value)
        {
            typedef typename std::conditional<std::is_same<T, float>::value, int32_t, int64_t>::type intfloat_t;
            intfloat_t result = *reinterpret_cast<intfloat_t*>(&value);
            encode<intfloat_t>(result);
            return *this;
        }


        // Container
        template<class T>
        typename std::enable_if<
            Util::is_container<T>::value,
            SerializationContext&
        >::type encode(T value)
        {
            typedef typename T::value_type target_t;
            for (unsigned i = 0; i < value.size(); i++)
            {
                encode<target_t>(value[i]);
            }
            return *this;
        }

        SerializationContext& encode(const std::string& _string);

        SerializationContext& encode(const SerializationContext& other);

        inline const std::vector<uint8_t>& getData() const { return data; }

    };

    class Root;

    class SerializationView{
        const std::vector<uint8_t>& data;
        typename std::vector<uint8_t>::const_iterator position;
    public:
        SerializationView(const SerializationContext& context): data(context.data), position(context.data.cbegin()) {}


        template<class T>
        typename std::enable_if<
            std::is_integral<T>::value,
            SerializationView&
        >::type decode(T& value)
        {
            for (unsigned i = 0; i < sizeof (T); i++)
            {
                value |= (T)(*position++) << (((sizeof(T) * 8) - 8) - (i * 8));
            }
            return *this;
        }

        template<class T>
        typename std::enable_if<
            std::is_floating_point<T>::value,
            SerializationView&
        >::type decode(T& value)
        {
            typedef typename std::conditional<
                std::is_same<T, float>::value,
                int32_t,
                int64_t
                    >::type intfloat_t;

            intfloat_t result = 0;
            decode<intfloat_t>(result);
            value = *reinterpret_cast<T*>(&result);
            return *this;
        }

        // Container
        template<class T>
        typename std::enable_if<
            Util::is_container<T>::value,
            SerializationView&
        >::type decode(T& value, uint16_t itemsCount)
        {
            typedef typename T::value_type target_t;

            for (unsigned i = 0; i < itemsCount; i++)
            {
                target_t temp{};
                decode<target_t>(temp);
                value.push_back(temp);
            }
            (void)value;
            return *this;
        }

        SerializationView& decode(std::string& _string, uint16_t size);

        SerializationView& decode(SerializationContext& other, uint8_t type);
        SerializationView& decode(SerializationContext& other, uint8_t type, uint16_t count);

        inline const std::vector<uint8_t>& getData() const { return data; }

    };
}
