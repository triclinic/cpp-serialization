 #include "context.hpp"
 #include "root.hpp"


 namespace ObjectModel
{

        SerializationContext& SerializationContext::encode(const std::string& _string)
        {
            for (unsigned i = 0; i < _string.size(); i++)
            {
                uint8_t ch = static_cast<uint8_t>(_string.c_str()[i]);
                this->encode<uint8_t>(ch ^ 0x5A);
            }
            return *this;
        }

        SerializationContext& SerializationContext::encode(const SerializationContext& other)
        {
            data.insert(data.end(), other.data.cbegin(), other.data.cend());
            return *this;
        }


        SerializationView& SerializationView::decode(std::string& _string, uint16_t size)
        {
            for (unsigned i = 0; i < size; i++)
            {
                uint8_t ch = *position;
                decode<uint8_t>(ch);
                _string.push_back(ch ^ 0x5A);
            }
            return *this;
        }

        SerializationView& SerializationView::decode(SerializationContext& other, uint8_t type)
        {
            uint8_t offset = getTypeSize(static_cast<Type>(type));
            for(unsigned i = 0; i < offset; ++i)
            {
                other.data.push_back(*position++);
            }
            return *this;
        }

        SerializationView& SerializationView::decode(SerializationContext& other, uint8_t type, uint16_t count)
        {
            uint8_t offset = getTypeSize(static_cast<Type>(type));
            for(unsigned i = 0; i < offset * count; ++i)
            {
                other.data.push_back(*position++);
            }
            return *this;
        }
}
