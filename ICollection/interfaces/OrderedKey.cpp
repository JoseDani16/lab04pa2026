#include <stdexcept>

#include "OrderedKey.h"

using namespace std;

bool OrderedKey::equals(IKey* k) const
{
    OrderedKey *key = dynamic_cast<OrderedKey *>(k);
    if(key == NULL)
        throw invalid_argument("Se esperaba un OrderedKey");
    return compare(key) == EQUAL;
}

OrderedKey::~OrderedKey() {
}

