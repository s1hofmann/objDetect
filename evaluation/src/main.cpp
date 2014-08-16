#include "evaluate.h"

using namespace std;

int main(int argc, char **argv)
{
    Evaluator test(argv[1], argv[2], true);
    test.evaluate();
    return 0;
}
