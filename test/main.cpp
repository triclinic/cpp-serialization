#include "serialization.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	using ObjectModel::Primitive;
	using ObjectModel::Root;
	using ObjectModel::Type;
	using ObjectModel::SerializationContext;

	double foo = 231.456;
    std::unique_ptr<Root> p = Primitive::create("xval", Type::DOUBLE, foo);
    SerializationContext cxt, cxt2;
    p->pack(cxt);

    const std::vector<uint8_t>& data = cxt.getData();
    for(uint8_t v: data)
    {
        std::cout << (unsigned)v << " ";
    }

    std::cout << std::endl;
    Primitive p2 = Primitive::unpack(cxt);
    p2.pack(cxt2);

    const std::vector<uint8_t>& data2 = cxt2.getData();
    for(uint8_t v: data2)
    {
        std::cout << (unsigned)v << " ";
    }

	(void)argc;
	(void)argv;
	return 0;
}
