
#include "primitive.hpp"

namespace ObjectModel
{

    Primitive::Primitive() {}

    Primitive::Primitive(const std::string& name, uint8_t type, uint8_t targetSize):
        Root(name, static_cast<uint8_t>(Wrapper::PRIMITIVE), targetSize + sizeof type),
        _type(type) {}

	void Primitive::pack(SerializationContext& receiver)
	{
		receiver.encode(_wrapper)
                .encode(_nameLength)
                .encode(_name)
                .encode(_type)
                .encode(context)
                .encode(_size);
	}

    Primitive Primitive::unpack(const SerializationContext& cxt)
    {
        Primitive result;
        SerializationView view(cxt, result);
        view.decode(result._wrapper)
            .decode(result._nameLength)
            .decode(result._name)
            .decode(result._type)
            .decode(result.context)
            .decode(result._size);
        return result;
    }
}
