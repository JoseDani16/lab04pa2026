#include "IIterator.h"
#ifndef _ICOLLECTION_H
#define	_ICOLLECTION_H




class ICollection
{
public:
    


    virtual void add(ICollectible *) = 0;
    
    


    virtual void remove(ICollectible *) = 0;
    
    
    


 
    virtual bool member(ICollectible *) const = 0;
    
    


    virtual bool isEmpty() const = 0;
    
    


    virtual int getSize() const = 0;
    
    


    virtual IIterator *getIterator() = 0;
    
    virtual ~ICollection();
};

#endif	

