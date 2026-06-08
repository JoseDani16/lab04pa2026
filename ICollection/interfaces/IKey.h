#ifndef _IKEY_H
#define	_IKEY_H

class IKey
{
public:
    


    virtual bool equals(IKey *) const = 0;
    
    virtual ~IKey();
};

#endif	

