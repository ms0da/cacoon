
#include "serializable.h"

using cacoon::comms::serializable;
using cacoon::comms::exception::could_not_deserialize;

could_not_deserialize::could_not_deserialize(const std::string& what)
:std::exception(what.c_str()) {
}

void serializable::throw_could_not_deserialize() {
    throw could_not_deserialize("Could not deserialize object");
}
