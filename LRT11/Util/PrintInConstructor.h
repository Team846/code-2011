#ifndef PRINT_IN_CONSTRUCTOR_H_
#define PRINT_IN_CONSTRUCTOR_H_


//Allows messages to be printed during the construction of objects. -dg
//Use as:   func_constructor(): print_brain("Constructing Brain"), brain(args) ... { }
class PrintInConstructor
{
public:
    PrintInConstructor(char* message)
    {
        puts(message);
    }
    ~PrintInConstructor() {};
};
#endif //PRINT_IN_CONSTRUCTOR_H_
