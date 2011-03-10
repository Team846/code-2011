// Perl must be installed for this to function - http://strawberryperl.com/
#ifndef BUILD_H_
#define BUILD_H_

#include <string>
using namespace std;

class Build
{
public:
    static int GetNumber()
    {
        return NUMBER;
    }

    static string GetTime()
    {
        return TIME;
    }

private:
    const static int NUMBER;
    const static string TIME;
};

const int Build::NUMBER = 1589;
const string Build::TIME = "Wed Mar  9 15:46:25 2011";

#endif

