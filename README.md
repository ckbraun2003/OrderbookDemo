# Orderbook Demo

An educational C++20 orderbook implementation for Windows, demonstrating core order matching mechanics with a focus on performance and clean architecture.

## Features

- **Order Types**: GoodTillCancel (GTC) and Market orders
- **Price-Time Priority Matching**: FIFO execution at each price level
- **Dual-Sided Book**: Full support for buy and sell order management
- **Performance-Oriented**: Efficient data structures and standard library usage
- **Interactive CLI**: Command-line interface for order management and book visualization

## Requirements

- **Operating System**: Windows
- **C++ Compiler**: MSVC (with C++20 support)
- **CMake**: Version 3.8 or higher
- **Dependencies**: C++ Standard Library only

## Building the Project

### Using CMake (Command Line)

```bash
# Clone repository
git clone https://github.com/ckbraun2003/OrderbookDemo.git

# Navigate to directory
cd OrderbookDemo

# Create build directory
mkdir build
cd build

# Configure the project
cmake ..

# Build
cmake --build .

# Run the executable
./Debug/OrderbookDemo.exe  # or ./Release/OrderbookDemo.exe
```

### Using Visual Studio

1. Open the project folder in Visual Studio (File → Open → Folder)
2. Visual Studio will automatically detect the CMake configuration
3. Select your build configuration (Debug/Release)
4. Build the project (Ctrl+Shift+B)
5. Run the executable (F5 or Ctrl+F5)

## Project Structure

```
OrderbookDemo/
├── CMakeLists.txt
├── include/
│   ├── pch.hpp           # Precompiled header
│   ├── Order.hpp
│   ├── Trade.hpp
│   ├── Orderbook.hpp
│   ├── Terminal.hpp
│   └── Enums.hpp
└── src/
    ├── mainWindows.cpp   # Entry point
    ├── Order.cpp
    ├── Trade.cpp
    ├── Orderbook.cpp
    ├── Terminal.cpp
    ├── Enums.cpp
    └── pch.cpp
```

## Usage

Run the executable and interact with the orderbook through the command-line interface. The terminal provides commands for:

- Adding GoodTillCancel and Market orders
- Viewing the current state of the orderbook
- Monitoring matched trades
- Managing order lifecycle

## How It Works

The orderbook maintains separate buy and sell sides, matching incoming orders based on price-time priority:

1. **GoodTillCancel Orders**: Remain in the book until matched or explicitly cancelled
2. **Market Orders**: Execute immediately at the best available price(s)
3. **Matching Engine**: Orders match when buy price ≥ sell price, with priority given to earlier orders at the same price level

## Educational Focus

This project demonstrates:

- Modern C++20 features and idioms
- Efficient orderbook data structure design
- Order matching algorithms used in financial exchanges
- Clean separation of concerns across components
- Performance-conscious implementation patterns

## Notes

- The project uses precompiled headers to improve build times
- MSVC's Edit and Continue is enabled for Debug builds
- All code adheres to C++20 standards without compiler extensions

## Contact
LinkedIn: www.linkedin.com/in/cameron-braun-51b480272