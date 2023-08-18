#pragma once


#include <memory>
#include "root.hpp"
#include "serialization.hpp"
#include "meta.hpp"

namespace ObjectModel
{
	class Primitive : public Root
	{
	private:
		uint8_t _type;
	private:
	    Primitive(const std::string&, uint8_t, uint8_t);
	    Primitive(void);
	public:
		template<typename T>
		static std::unique_ptr<Primitive> create(const std::string& name, Type type, T value)
		{
			std::unique_ptr<Primitive> p(new Primitive(name, (uint8_t)type, (uint8_t)sizeof(T)));
			p->context.template encode<T>(value);

			return p;
		}

		void pack(SerializationContext&);
		static Primitive unpack(const SerializationContext& cxt);

	};
}
