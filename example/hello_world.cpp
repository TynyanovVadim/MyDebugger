#include <iostream>
#include <string>

void print(std::string_view s);

int main() {
    print("Hello, World!");
    print("Hello, Proger");
    
    return 0;
}

void print(std::string_view s) {
    std::cout << s << "\n"; 
}