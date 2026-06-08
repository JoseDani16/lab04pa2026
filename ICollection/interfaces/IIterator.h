#include "ICollectible.h"


#ifndef _IITERATOR_H
#define	_IITERATOR_H



 
class IIterator{
public:
    



    virtual ICollectible *getCurrent() = 0;

    


    virtual bool hasCurrent() = 0;
    
    



    virtual void next() = 0;
        
    virtual ~IIterator();
};

#endif	

