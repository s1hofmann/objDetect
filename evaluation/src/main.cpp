#include "evaluate.h"

#include <exception>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    string cascade;
    string positive;

    bool verbose = false;
    bool show = false;

    for (int i = 1; i < argc; i++)
    {
        if (!(strcmp(argv[i],"-c")) and (i+1 < argc))
        {
            cascade = argv[i+1];
        }
        else if (!(strcmp(argv[i],"-p")) and (i+1 < argc))
        {
            positive = argv[i+1];
        }
        else if (!(strcmp(argv[i],"-v")))
        {
            verbose = true;
        } 
        else if (!(strcmp(argv[i],"-s")))
        {
            show = true;
        }
        else if (!(strcmp(argv[i],"-h")))
        {
        }
    }

    Evaluator test(cascade.c_str(), positive.c_str(), verbose, show);
    test.evaluate();
    return 0;
}
