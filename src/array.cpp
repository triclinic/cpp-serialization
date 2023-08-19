#include "array.hpp"

namespace ObjectModel
{

    Array::Array(): _type(0), _itemsCount(0) {}

    Array::Array(const std::string& name, uint8_t type, uint8_t targetSize, uint16_t itemsCount):
        Root(name, static_cast<uint8_t>(Wrapper::ARRAY), targetSize * itemsCount + sizeof type + sizeof itemsCount),
        _type(type), _itemsCount(itemsCount) {}

	void Array::pack(SerializationContext& receiver) const
	{
		(void)receiver
                .encode(_wrapper)
                .encode(_nameLength)
                .encode(_name)
                .encode(_type)
                .encode(_itemsCount)
                .encode(context)
                .encode(_size)
        ;
	}

    std::unique_ptr<Array> Array::unpack(const SerializationContext& transmitter)
    {
        std::unique_ptr<Array> result(new Array());
        SerializationView view(transmitter);
        view
            .decode(result->_wrapper)
            .decode(result->_nameLength)
            .decode(result->_name, result->_nameLength)
            .decode(result->_type)
            .decode(result->_itemsCount)
            .decode(result->context, result->_type, result->_itemsCount)
            .decode(result->_size)
        ;
        return result;
    }
}
