#include <stdio.h>
#include <cuda_runtime.h>

// Simple GPU kernel that prints to console
__global__ void hello() {
    printf("Hello GPU\n");
}

int main() {
    printf("Starting Phase 0: GPU Hello Kernel Test\n");
    printf("Launching kernel...\n");
    
    hello<<<1, 1>>>();
    
    // Ensure GPU work completes
    cudaDeviceSynchronize();
    
    printf("Kernel completed successfully!\n");
    
    return 0;
}
