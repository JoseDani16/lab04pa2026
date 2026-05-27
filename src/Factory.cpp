#include "Factory.h"
#include "Sistema.h"

ISistema* Factory::getSistema() {
    return Sistema::getInstance();
}
