#ifndef OPONGL_DEBUG_HPP
#define OPONGL_DEBUG_HPP

#ifndef NDEBUG
    #include <cstdio>
    #define DEBUG_FPRINTF(...) std::fprintf(__VA_ARGS__)
#else
    #define DEBUG_FPRINTF(...)
#endif

#endif