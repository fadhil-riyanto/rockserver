#include <cstdio>
#include <thread>

void somefunc() {
    printf("ret\n");
}

int main() {
    std::thread xyz;


    for(int i = 0; i < 2; i++) {
        xyz = std::thread(somefunc);
        xyz.join();
    } 
    
}