#ifndef ORDEREDDICTIONARYENTRY_H
#define	ORDEREDDICTIONARYENTRY_H

#include "../interfaces/OrderedKey.h"
#include "../interfaces/ICollectible.h"
#include <cstddef>









class OrderedDictionaryEntry {
public:
    OrderedDictionaryEntry(
            OrderedKey *key,
            ICollectible *val,
            OrderedDictionaryEntry *l = NULL,
            OrderedDictionaryEntry *g = NULL
            );
    
    
    OrderedKey *getKey();
    ICollectible *getVal();
    void setVal(ICollectible *i);
    OrderedDictionaryEntry *getLesser();
    void setLesser(OrderedDictionaryEntry *e);
    OrderedDictionaryEntry *getGreater();
    void setGreater(OrderedDictionaryEntry *e);
    
    
    



    OrderedDictionaryEntry *getLeastElement();
    
    



    OrderedDictionaryEntry *getGreatestElement();
    
    


    void deleteInDepth();
    
    


    virtual ~OrderedDictionaryEntry();
private:
    OrderedKey *key;
    ICollectible *val;
    OrderedDictionaryEntry *lesser;
    OrderedDictionaryEntry *greater;
};

#endif	

