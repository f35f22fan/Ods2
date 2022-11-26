#include "Property.hpp"

#include "../Prefix.hpp"

namespace ods::ndff {

bool Property::is(ods::Prefix *prefix) const {
	return uri_id == prefix->id();
}

}
