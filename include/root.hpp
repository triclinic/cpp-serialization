#pragma once
#include <string>
#include <vector>
#include <memory>

#include "meta.hpp"
#include "context.hpp"


namespace ObjectModel
{
	class Root
	{
	protected:
        std::string _name;
		uint8_t _wrapper;
		uint8_t _nameLength;
		uint16_t _size;
	    SerializationContext context;

        Root()
            :
			_name(),
			_wrapper(0),
			_nameLength(0),
			_size(0),
			context(0) {}

		Root(const std::string& name, uint8_t wrapper, uint16_t targetSize)
			:
			_name(name),
			_wrapper(wrapper),
			_nameLength(name.size()),
			_size(sizeof _wrapper + sizeof _nameLength + name.size() + sizeof _size + targetSize),
			context(_size) {}
	public:

		inline const std::string& getName() const { return _name; }
		inline uint32_t getSize() const { return _size; }

		virtual void pack(SerializationContext&) const = 0;
		virtual ~Root() = default;

		template<class T>
        friend T root_cast(const Root&);
	};

    template<class T>
	T root_cast(const Root& p)
	{
	    T v{};
        SerializationView view(p.context);
        view.template decode<T>(v);
        return v;
	}
}
