#pragma once

#include <memory>
#include "root.hpp"
#include "meta.hpp"
#include "context.hpp"

namespace ObjectModel
{
	class Array : public Root
	{
	private:
		uint8_t _type;
		uint16_t _itemsCount;
	private:
	    Array(const std::string&, uint8_t, uint8_t, uint16_t);
	    Array(void);
	public:
		template<typename T>
		static std::unique_ptr<Array> create(const std::string& name, T value)
		{
			std::unique_ptr<Array> p(new Array(name, Util::selector<typename T::value_type>::value, (uint8_t)sizeof(typename T::value_type), (uint16_t)value.size() ) );
			p->context.template encode<T>(value);

			return p;
		}

		void pack(SerializationContext&) const;
		static std::unique_ptr<Array> unpack(const SerializationContext& cxt);

        template<class T>
        friend T array_cast(const Array&);
	};

	template<class T>
	T array_cast(const Array& p)
	{
	    T v{};
        SerializationView view(p.context);
        view.template decode<T>(v, p._itemsCount);
        return v;
	}
}

