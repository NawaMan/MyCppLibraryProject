# MyCppLibraryProject

A simple C++ library that provides a Fibonacci number calculator function.

## Features

- Fast and efficient Fibonacci number calculation
- Well-tested implementation
- Modern C++20 codebase
- Cross-platform support (Windows, Linux, macOS)

## Requirements

### Build System
- CMake 3.10 or later
- C++20 compatible compiler (e.g., GCC 10+, Clang 11+, MSVC 19.29+)

### Required Dependencies
- None

### Optional Dependencies
- Google Test 1.11 or later (for testing)

### Ubuntu 24.04 Installation
```bash
# Install build tools
sudo apt-get update
sudo apt-get install -y cmake g++

# No external dependencies required

# Install optional dependencies (for testing)
sudo apt-get install -y libgtest-dev
```

## Installation

### From Source
```bash
# Clone the repository
git clone https://github.com/yourusername/MyCppLibraryProject.git
cd MyCppLibraryProject

# Build and test locally
./build-locally.sh
```

### Using Package Manager
Download the appropriate package for your system from the releases:
- DEB package (Debian/Ubuntu)
- RPM package (Red Hat/Fedora)
- ZIP archive (Windows)
- TGZ archive (Unix-like systems)

## Usage
```cpp
#include "fibonacci.hpp"

int main() {
    // Calculate the 10th Fibonacci number
    int result = mycpplibrary::fibonacci(10);
    // result = 55
    
    return 0;
}
```

## Building

### Local Development
```bash
# Build, test, and generate packages
./build-locally.sh

# Clean build artifacts
./build-clean.sh
```

### Multi-Platform Build
```bash
# Build for all supported platforms
./build-all-platforms.sh
```



## Testing
The library comes with comprehensive test suites:
```bash
# Run all tests
./build-locally.sh

# View coverage report
open coverage_report/index.html
```

## Documentation
- [API Documentation](docs/api/README.md)
- [Build Scripts](docs/scripts/build-locally.md)
- [Task Documentation](docs/tasks/)

## Contributing
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Developer on Linux

### Prerequisites
- Linux
- CMake
- Git
- sed
- Boost
- make
- ctest

```bash
sudo apt-get install -y build-essential cmake git sed libgtest-dev libboost-all-dev
```

### Setup
1. Clone the repository
2. Build the Docker image
3. Run the Docker container

### Ensure proper line ends before commit

Run : find . -path "./.git" -prune -o -type f ! -name "*.ps1" -exec sed -i 's/\r$//' {} +

## Developing on Windows

### Prerequisites
- Windows 10 or later
- WSL (Windows Subsystem for Linux)
- Docker
- CMake
- Git
- sed
- Boost
- make
- ctest

### Setup
1. Install WSL
2. Install Docker
3. Clone the repository
4. Build the Docker image
5. Run the Docker container

### Ensure proper line ends before commit

Run : find . -path "./.git" -prune -o -type f ! -name "*.ps1" -exec sed -i 's/\r$//' {} +

## Developer on MacOS

### Prerequisites
- Windows 10 or later
- WSL (Windows Subsystem for Linux)
- Docker
- CMake
- Git
- sed
- Boost
- Clang
- make
- ctest

### Ensure proper line ends before commit

Run : find . -path "./.git" -prune -o -type f ! -name "*.ps1" -exec sed -i 's/\r$//' {} +

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Based on the structure of the SimpleString project by NawaMan