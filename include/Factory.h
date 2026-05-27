#ifndef FACTORY_H
#define FACTORY_H

#include "ISistema.h"

class Factory {
public:
    static ISistema* getSistema();
};

#endif
