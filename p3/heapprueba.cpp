#include <iostream>
#include <algorithm>
#include <vector>

int main() {
    std::vector<int> number = {5, 2, 7, 1, 9};
    int numbers[] = {5, 2, 7, 1, 9};
    int size = sizeof(numbers) / sizeof(numbers[0]);
    std::cout << size<< " size";
    // Create a heap in decreasing order using make_heap and std::greater
    std::make_heap(numbers, numbers + size, std::greater<int>());

    // Sort the heap in decreasing order using sort_heap and std::greater
    std::sort_heap(numbers, numbers + size, std::greater<int>());

    // Print the elements of the heap
    std::cout << "Heap elements: ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}