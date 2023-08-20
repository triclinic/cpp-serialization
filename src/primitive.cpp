#include "primitive.hpp"

namespace ObjectModel
{

    Primitive::Primitive(): _type(0) {}

    Primitive::Primitive(const std::string& name, uint8_t type, uint8_t targetSize):
        Root(name, static_cast<uint8_t>(Wrapper::PRIMITIVE), targetSize + sizeof type),
        _type(type) {}

	void Primitive::pack(SerializationContext& receiver) const
	{
		receiver
                .encode(_wrapper)
                .encode(_nameLength)
                .encode(_name)
                .encode(_type)
                .encode(context)
                .encode(_size)
        ;
	}

    std::unique_ptr<Primitive> Primitive::unpack(const SerializationContext& transmitter)
    {
        std::unique_ptr<Primitive> result(new Primitive());
        SerializationView view(transmitter);
        view
            .decode(result->_wrapper)
            //TODO: make assert on "result->_wrapper"
            .decode(result->_nameLength)
            .decode(result->_name, result->_nameLength)
            .decode(result->_type)
            //TODO: make assert on "result->_type"
            .decode(result->context, result->_type)
            .decode(result->_size)
        ;
        return result;
    }
}
