#include "sharedptr.h"

int main(void) {
    sPtr<int> one;
    sPtr<int> two(new int(1));
    sPtr<int> three = new int(42);
    sPtr<int> four = std::move(three);
    sPtr<int> five = four;
    sPtr<int> six(five);
    sPtr<int> seven(std::move(two));
}