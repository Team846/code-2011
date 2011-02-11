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

const int Build::NUMBER = 569;
const string Build::TIME = "Thu Feb 10 18:42:59 2011";

#endif

