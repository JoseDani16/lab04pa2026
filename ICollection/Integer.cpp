#include <stddef.h>
#include <stdexcept>
#include "Integer.h"

using namespace std;

Integer::Integer(int i):
        val(i)
{
}

int Integer::getValue() const
{
    return val;
}

ComparisonRes Integer::compare(OrderedKey* k) const
{
    Integer *i = dynamic_cast<Integer *>(k);
    if(i == NULL) 
        throw invalid_argument("Invalid key k");
    if(i->val == val)
        return EQUAL;
    else if(val > i->val)
        return GREATER;
    else
        return LESSER;
}
