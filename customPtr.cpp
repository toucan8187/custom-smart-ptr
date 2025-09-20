#include "customPtr.h"

int main(void) {
    unique_ptr<int> ex1(new int(1));
    unique_ptr<int[2][3]> ex2(new int[1][2][3]{{{ 4, 5, 6 }, { 7, 8, 9 }}});
    unique_ptr<int[]> ex3(new int[3]{1, 2, 3});
    shared_ptr<int> ex4(new int(1));
    shared_ptr<int[2][3]> ex5(new int[1][2][3]{{{ 1, 2, 3 }, { 4, 5, 6 }}});
    shared_ptr<int[]> ex6(new int[3]{1, 2, 3});
    weak_ptr<int> ex7 = ex4;
    weak_ptr<int[2][3]> ex8 = ex5;
    weak_ptr<int[]> ex9 = ex6;
}