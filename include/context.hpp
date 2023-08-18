#pragma once
#include <type_traits>


namespace ObjectModel
{

    class SerializationContext{
        std::vector<uint8_t> data;
        friend class SerializationView;
    public:
        SerializationContext(uint8_t size = 0): data(){ data.reserve(size); }

        template<class T>
        typename std::enable_if<std::is_integral<T>::value && !std::is_floating_point<T>::value, SerializationContext&>::type
        encode(T value)
        {
            for (unsigned i = 0; i < sizeof (T); i++)
            {
                data.push_back((uint8_t)(value >> (((sizeof(T) * 8) - 8) - i * 8)));
            }
            return *this;
        }

        template<class T>
        typename std::enable_if<std::is_floating_point<T>::value, SerializationContext&>::type
        encode(T value)
        {
            typedef typename std::conditional<std::is_same<T, float>::value, int32_t, int64_t>::type intfloat_t;
            intfloat_t result = *reinterpret_cast<intfloat_t*>(&value);
            encode<intfloat_t>(result);
            return *this;
        }

        SerializationContext& encode(const std::string& _string)
        {
            for (unsigned i = 0; i < _string.size(); i++)
            {
                uint8_t ch = static_cast<uint8_t>(_string.c_str()[i]);
                this->encode<uint8_t>(ch);
            }
            return *this;
        }

        inline SerializationContext& encode(const SerializationContext& other)
        {
            data.insert(data.end(), other.data.cbegin(), other.data.cend());
            return *this;
        }

        inline const std::vector<uint8_t>& getData() const
        {
            return data;
        }
    };

    class Root;

    class SerializationView{
        const std::vector<uint8_t>& data;
        const Root& result;
        typename std::vector<uint8_t>::const_iterator position;
    public:
        SerializationView(const SerializationContext& context, const Root& result): data(context.data), result(result), position(context.data.cbegin()) {}

        template<class T>
        typename std::enable_if<std::is_integral<T>::value && !std::is_floating_point<T>::value, SerializationView&>::type
        decode(T& value)
        {
            for (unsigned i = 0; i < sizeof (T); i++)
            {
                value |= (T)(*position++) << (((sizeof(T) * 8) - 8) - (i * 8));
            }
            return *this;
        }

        template<class T>
        typename std::enable_if<std::is_floating_point<T>::value, SerializationView&>::type
        decode(T& value)
        {
            typedef typename std::conditional<std::is_same<T, float>::value, int32_t, int64_t>::type intfloat_t;
            intfloat_t result = *reinterpret_cast<intfloat_t*>(position);
            value = decode<intfloat_t>(result);
            position += sizeof(intfloat_t);
            (void)value;
            return *this;
        }

        SerializationView& decode(std::string& _string)
        {
//            for (unsigned i = 0; i < result.getName().size(); i++)
//            {
//                _string.push_back((char)decode<uint8_t>(position));
//
//                //uint8_t ch = static_cast<uint8_t>(_string.c_str()[i]);
//                //this->encode<uint8_t>(ch);
//            }
            (void)_string;
            return *this;
        }

        inline SerializationView& decode(SerializationContext& other)
        {
            //data.insert(data.end(), other.data.cbegin(), other.data.cend());
            (void)other;
            return *this;
        }

        inline const std::vector<uint8_t>& getData() const
        {
            return data;
        }
    };
}
