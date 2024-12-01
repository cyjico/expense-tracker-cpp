# expense-tracker-cpp

Final project for CS 1131.

> Develop a command-line expense tracker that allows users to manage and analyze their daily expenses. The project will involve multiple features, file handling, and data analysis.

## Prerequisites for Developing

Ensure the following tools are installed:

- **[clang-format](https://releases.llvm.org/)** (used for code formatting)
- **[clang-tidy](https://releases.llvm.org/)** (used for static code analysis)
- **[cppcheck](https://github.com/danmar/cppcheck)** (used for static code analysis)

## Getting Started (Windows with Ninja)

1. Clone the repository:

    ```sh
    git clone https://github.com/cyjico/expense-tracker-cpp.git
    ```

2. Create the build directory:

    ```sh
    mkdir build 
    cd build
    ```

3. Build the project:

    ```sh
    cmake -G "Ninja" ..
    ninja
    ```

4. Done! To open the program, run:

    ```sh
    ./expense-tracker-cpp.exe
    ```

## Getting Started (Linux with Make)

1. Clone the repository:

    ```sh
    git clone https://github.com/cyjico/expense-tracker-cpp.git
    ```

2. Create the build directory:

    ```sh
    mkdir build 
    cd build
    ```

3. Build the project:

    ```sh
    cmake ..
    make
    ```

4. Done! To open the program, run:

    ```sh
    ./expense-tracker-cpp
    ```

## License

This project is licensed under the GNU General Public License v3.0 --- see the [LICENSE](LICENSE) file for details.
