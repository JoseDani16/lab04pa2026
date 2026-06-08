#include "interfaces/ICollectible.h"
#include "interfaces/OrderedKey.h"






#ifndef COLLECTION_STRING_H
#define	COLLECTION_STRING_H

class String: public ICollectible, public OrderedKey
{
private:
    char *s;
public:
    
    String(const char *s = "");
    
    
    const char *getValue() const;
    
    ComparisonRes compare(OrderedKey *k) const;
    
    virtual ~String();
    
};

#endif	
