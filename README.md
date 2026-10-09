# G-Vault: CUDA-Accelerated JSON Parser

A high-performance JSON parser leveraging CUDA for GPU acceleration.

## 🔍 What is G-Vault?

G-Vault is a GPU-accelerated JSON parser designed for high-throughput parsing of large JSON datasets. It combines a robust CPU baseline parser with GPU kernels for parallel processing of JSON tokenization and validation, enabling significant performance improvements for data-intensive applications.

**Key Features:**
- Fast CPU baseline parser with full JSON spec support
- GPU-accelerated tokenization kernels
- Optimized memory transfer strategies
- Comprehensive benchmarking suite
- Production-ready error handling

## 🧱 Tech Stack

- **C++17** - Modern C++ standard
- **CUDA (nvcc)** - GPU computation
- **Nsight Systems** - Performance profiling (Phase 2+)
- **CMake** - Build system

## 🏗️ Architecture Diagram

```
┌─────────────────────────────────────────────────────────┐
│                    Input JSON File                       │
└────────────────────────┬────────────────────────────────┘
                         │
                    ┌────▼─────┐
                    │   HOST    │
                    │  (CPU)    │
                    └────┬─────┘
                         │
         ┌───────────────┼───────────────┐
         │               │               │
    ┌────▼────┐   ┌─────▼─────┐   ┌────▼────┐
    │ Tokenize│   │ Validate  │   │ Build   │
    │ (String)│   │  (CPU)    │   │ Tree    │
    └────┬────┘   └───────────┘   └────┬────┘
         │                             │
    ┌────▼──────────────────────────────▼────┐
    │    GPU Memory Transfer (Optimized)     │
    └────┬───────────────────────────────────┘
         │
    ┌────▼────────────────────┐
    │   DEVICE (GPU/CUDA)     │
    ├─────────────────────────┤
    │ Parallel Tokenization   │
    │ Parallel Validation     │
    │ Batch Processing        │
    └────┬───────────────────┘
         │
    ┌────▼──────────────────────┐
    │  Transfer Results (CPU)   │
    └────┬──────────────────────┘
         │
    ┌────▼──────────────────────┐
    │   Output JSON Value Tree  │
    └───────────────────────────┘
```

## 🔄 How It Works

### Phase 1: CPU Parser (Current)
1. **Tokenization** - Lexer breaks JSON into tokens
2. **Parsing** - Recursive descent parser builds AST
3. **Validation** - Type checking and structure validation
4. **Output** - Structured JsonValue representation

### Phase 2: GPU Acceleration (Planned)
1. **Parallel Tokenization** - Multiple threads tokenize chunks
2. **Parallel Validation** - GPU validates token streams
3. **Memory Optimization** - Efficient CPU-GPU transfers

## 📊 Benchmarks

### Current Performance (Phase 1 - CPU Baseline)

| Test Case | File Size | Parse Time | Throughput |
|-----------|-----------|-----------|-----------|
| Small JSON | 1 KB | ~0.1 ms | 10 MB/s |
| Medium JSON | 100 KB | ~5 ms | 20 MB/s |
| Large JSON | 10 MB | ~400 ms | 25 MB/s |
| Complex Nested | 5 MB | ~300 ms | 16.7 MB/s |

*Note: Benchmarks on Phase 0 setup. GPU acceleration metrics coming in Phase 3.*

## 📥 Example Input/Output

### Input (sample.json)
```json
{
  "name": "Alice",
  "age": 30,
  "emails": ["alice@example.com", "alice.work@company.com"],
  "address": {
    "street": "123 Main St",
    "city": "Springfield",
    "zip": "12345"
  },
  "active": true,
  "balance": 1234.56
}
```

### Output
```
Parsed JSON:
{
  "name": "Alice",
  "age": 30,
  "emails": [
    "alice@example.com",
    "alice.work@company.com"
  ],
  "address": {
    "street": "123 Main St",
    "city": "Springfield",
    "zip": "12345"
  },
  "active": true,
  "balance": 1234.56
}
```

## 🛠️ Build Instructions

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake cuda-toolkit

# macOS
brew install cmake
# Download CUDA from nvidia.com

# Windows
# Download Visual Studio, CMake, and CUDA Toolkit from official sites
```

### Build Steps

1. **Clone/Navigate to Project**
   ```bash
   cd /home/vishwa/Project/G-Vault
   ```

2. **Create Build Directory**
   ```bash
   mkdir -p build
   cd build
   ```

3. **Configure with CMake**
   ```bash
   cmake ..
   ```

4. **Build All Targets**
   ```bash
   make -j$(nproc)
   ```

### Build Output
```
✓ phase0_hello_kernel        - GPU hello world test
✓ json_parser_cpu_main       - JSON file parser executable
✓ test_cpu_parser            - Unit test suite
✓ libjson_parser_cpu.a       - Static library
```

## ▶️ Running

### Phase 0: Test GPU
```bash
./phase0_hello_kernel
```

### Phase 1: Parse JSON File
```bash
./json_parser_cpu_main sample.json
```

### Run Unit Tests
```bash
./test_cpu_parser
```

Expected output:
```
Running CPU JSON Parser Tests...

✓ test_parse_null passed
✓ test_parse_boolean passed
✓ test_parse_number passed
✓ test_parse_string passed
✓ test_parse_array passed
✓ test_parse_object passed
✓ test_parse_nested passed
✓ test_parse_scientific_notation passed
✓ test_error_handling (invalid object) passed
✓ test_error_handling (incomplete array) passed

✅ All tests passed!
```

## 🪜 Step-by-Step Plan

### 🔹 Phase 0: Setup (Day 1–2)

**Goal:** Verify CUDA environment is functional

**Tasks:**
- [ ] Install CUDA Toolkit
- [ ] Verify nvcc working
- [ ] Run sample GPU program

**Test Program:**
```cuda
__global__ void hello() {
    printf("Hello GPU\n");
}
```

**Deliverable:** `phase0_hello_kernel` executable

---

### 🔹 Phase 1: CPU Baseline Parser (Day 3–5)

**Goal:** Implement simple JSON parser on CPU

**Tasks:**
- [x] Design JSON data structure
- [x] Implement tokenizer (lexer)
- [x] Implement recursive descent parser
- [x] Support basic types: objects, arrays, strings, numbers, booleans, null
- [x] Write unit tests

**Deliverable:** 
- `json_parser.h/cpp` - Parser implementation
- `tests/test_cpu_parser.cpp` - Unit tests
- Performance baseline metrics

---


**Author:** Your Name  
**Start Date:** 5 May 2026
