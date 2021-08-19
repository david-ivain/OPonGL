#include "uniform_id.hpp"

std::string uniformIdString(UniformID id)
{
    switch (id)
    {
    case UniformID::Aspect:
        return "aspect";
        break;
    case UniformID::Transform:
        return "transform";
        break;
    default:
        return "";
        break;
    }
}