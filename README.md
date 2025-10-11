# SP - Modern C++ Data Structures Library

A modern C++ implementation of standard library containers and utilities, with focus on performance, modern C++ features, and special optimizations like SSO (Small String Optimization) and SVO (Small Vector Optimization).

## Overview

SP (Smart Primitives) is a comprehensive reimplementation of the C++ Standard Template Library (STL) with these key features:

- 💡 Modern C++26 implementation
- 🚀 Performance-focused design
- 🛡️ Strong exception guarantees
- 🧪 Comprehensive unit tests
- 📦 Zero external dependencies
- 🔍 Transparent implementation
- 📊 Benchmarking suite

## Features

### Core Components

#### Smart Pointers
- `sp::owning_ptr`: Unique ownership semantic
- `sp::shared_ptr`: Shared ownership with reference counting

#### Containers
- `sp::vector`: Dynamic array with SVO optimization
- `sp::string`: String implementation with SSO
- `sp::list`: Singly linked list
- `sp::doubly_list`: Doubly linked list
- `sp::array`: Static array with optimization for small sizes
- `sp::bounded_channel`: Fixed-size thread-safe channel

#### Associative Containers
- `sp::map`: Ordered map implementation
- `sp::set`: Ordered set container
- `sp::unordered_map`: Hash table implementation
- `sp::unordered_set`: Hash set implementation
- `sp::new_hash_set`: Optimized hash set with linear probing

#### Concurrency Utilities
- `sp::latch`: Synchronization primitive (C++20 std::latch implementation)
- `sp::spinlock`: Low-latency spinning mutex
- `sp::mpscllqueue`: Lock-free Multi-Producer Single-Consumer queue

#### Memory Management
- `sp::MemPool`: Memory pool for efficient allocation
- `sp::ObjectPool`: Type-safe object pooling

### Design Philosophy

1. **Modern C++ First**
   - Extensive use of C++26 features
   - RAII and value semantics
   - Move semantics optimization

2. **Performance Focused**
   - Small buffer optimization where applicable
   - Cache-friendly data structures
   - Optimized memory allocation patterns

3. **Safety and Correctness**
   - Strong exception guarantees
   - Comprehensive testing
   - Static analysis compliance

## Implementation Status

| Component | Status | Features |
|-----------|--------|----------|
| `owning_ptr` | ✅ | Unique ownership, RAII |
| `shared_ptr` | ✅ | Reference counting, thread-safe |
| `vector` | 🚧 | Dynamic array, SVO planned |
| `string` | 🚧 | SSO planned |
| `list` | 🚧 | Singly linked list |
| `doubly_list` | 🚧 | Doubly linked list |
| `map` | 📝 | Red-black tree based |
| `set` | 📝 | Ordered set |
| `unordered_map` | 📝 | Hash table based |
| `unordered_set` | 📝 | Hash set implementation |
| `array` | 📝 | Static array, small size optimization |

Legend: ✅ Complete | 🚧 In Progress | 📝 Planned

## Building and Testing

```bash
# Build the library
mkdir build && cd build
cmake ..
make

# Run tests
ctest
```

## Performance Benchmarks

Each component comes with detailed performance benchmarks comparing against standard library implementations:

```bash
# Run all benchmarks
./run_benchmarks

# Run specific component benchmark
./run_benchmarks --component vector
```

## Contributing

Contributions are welcome! Please read through our contributing guidelines before submitting pull requests.

### Guidelines
1. All code must be Modern C++ (C++26)
2. Must include unit tests
3. Must include performance benchmarks
4. Must maintain or improve exception guarantees
5. Must document any deviations from STL behavior

## Experimental Projects

The repository includes several experimental projects demonstrating real-world applications:

### Trading & Finance
- Order Book Implementation: High-performance order matching engine
- Multi-threaded Odd-Even Counter: Concurrency patterns demonstration

### Networking
- Server Implementation: High-performance network server using the SP library
- Memory-mapped Files: Fast file I/O implementation

## Future Projects

The SP library serves as a foundation for several planned projects:
1. High-performance trading engine
2. Ray tracing engine
3. Physics engine for game development

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by the C++ Standard Template Library
- Performance techniques from Andrei Alexandrescu's Modern C++ Design
- Hash table implementation inspired by [UCSD's course materials](https://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16.html)
