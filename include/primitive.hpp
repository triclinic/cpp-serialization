#pragma once

#include <memory>
#include "root.hpp"
#include "meta.hpp"
#include "context.hpp"

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

		void pack(SerializationContext&) const;
		static std::unique_ptr<Primitive> unpack(const SerializationContext& cxt);

        template<typename T>
        typename std::enable_if<std::is_floating_point<T>::value, T>::type
        asFloating()
        {
            T v = 0;
            SerializationView view(context);
            view.template decode<T>(v);
            return v;
        }

        template<class T>
        friend T primitive_cast(const Primitive&);
	};

	template<class T>
	T primitive_cast(const Primitive& p)
	{
	    T v = 0;
        SerializationView view(p.context);
        view.template decode<T>(v);
        return v;
	}
}

