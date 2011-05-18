// Perl must be installed for this to function - http://strawberryperl.com/
#ifndef BUILD_H_
#define BUILD_H_

#include <string>

class Build
{
public:
    static int GetNumber()
    {
        return NUMBER;
    }

    static std::string GetTime()
    {
        return TIME;
    }

private:
    const static int NUMBER;
    const static std::string TIME;
};


#endif

