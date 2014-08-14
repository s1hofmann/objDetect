#include "evaluate.h"

using namespace std;

int main(int argc, char **argv)
{
    Evaluator test("/home/sim0n/Documents/git/rota/rota/cascade.xml", argv[1]);
    test.evaluate();
    return 0;
}
