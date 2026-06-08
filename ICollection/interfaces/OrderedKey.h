#ifndef ORDEREDKEY_H
#define	ORDEREDKEY_H

#include "IKey.h"


enum ComparisonRes { LESSER, EQUAL, GREATER };




class OrderedKey: public IKey {
public:
    






    virtual ComparisonRes compare(OrderedKey *k) const = 0 ;
    
    




    bool equals(IKey *k) const;
    
    virtual ~OrderedKey();
};

#endif	

