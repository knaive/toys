#include "ksum.h"

int main() {
    Table a, b;
    a.Init();
    b.Init();
    // a.Print();
    // b.Print();
    KSum ksum(a, b);
    ksum.DoMatch();
    a.ShowMatches();
    return 0;
}

