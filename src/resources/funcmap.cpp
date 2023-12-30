#include <resources/funcmap.hpp>

char* getFunctionMap()  {
    #ifdef POSTPROC
        return (char*) &_funcmap;
    #else
        return (char*) 0;
    #endif
}