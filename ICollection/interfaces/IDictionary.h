#ifndef IDICTIONARY_H
#define	IDICTIONARY_H

#include "IKey.h"
#include "ICollectible.h"
#include "IIterator.h"

class IDictionary {
public:
    




    
    virtual void add(IKey *k, ICollectible *val) = 0;
    
    



    virtual bool member(IKey *k)  const= 0;
    
    



    virtual void remove(IKey *k) = 0;
    
    




    virtual ICollectible *find(IKey *k) const = 0;
    
    



    virtual bool isEmpty() const = 0;
    
    



    virtual int getSize() const = 0;
    
    



    virtual IIterator *getIterator() = 0;
    
    
    virtual ~IDictionary();
};

#endif	

