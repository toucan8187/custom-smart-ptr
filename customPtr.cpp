#include "customPtr.h"

int main(void) {
    cp::unique_ptr<int> ex1(new int(1));
    cp::unique_ptr<int[2][3]> ex2(new int[1][2][3]{{{ 4, 5, 6 }, { 7, 8, 9 }}});
    cp::unique_ptr<int[]> ex3(new int[3]{1, 2, 3});
    cp::shared_ptr<int> ex4(new int(1));
    cp::shared_ptr<int[2][3]> ex5(new int[1][2][3]{{{ 1, 2, 3 }, { 4, 5, 6 }}});
    cp::shared_ptr<int[]> ex6(new int[3]{1, 2, 3});
    cp::weak_ptr<int> ex7 = ex4;
    cp::weak_ptr<int[2][3]> ex8 = ex5;
    cp::weak_ptr<int[]> ex9 = ex6;

    cp::unique_ptr<int> uptrNULL = cp::make_unique<int>();
    cp::unique_ptr<int> uptr = cp::make_unique<int>(1);
}
