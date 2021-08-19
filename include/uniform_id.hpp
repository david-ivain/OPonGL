#ifndef OPONGL_UNIFORM_ID_HPP
#define OPONGL_UNIFORM_ID_HPP

#include <string>

enum class UniformID
{
    Aspect,
    Transform,
};

std::string uniformIdString(UniformID id);

#endif