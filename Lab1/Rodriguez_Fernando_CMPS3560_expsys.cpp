#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

// Function that reads data from a CSV into a 2-D array.
template <size_t rows, size_t cols>
void readCSV(double (&array)[rows][cols], char *filename)
{
    std::ifstream file(filename);
    for (size_t row = 0; row < rows; ++row)
    {
        std::string line;
        std::getline(file, line);
        if (!file.good())
            break;
        std::stringstream iss(line);
        for (size_t col = 0; col < cols; ++col)
        {
            std::string val;
            std::getline(iss, val, ',');
            std::stringstream convertor(val);
            convertor >> array[row][col];
        }
    }
}

// Function that displays data values
template <size_t rows, size_t cols>
void displayValues(double (&array)[rows][cols])
{
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; col++)
        {
            std::cout << array[row][col] << ' ';
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[])
{
    // Your code here
    // Use readCSV to create data array
    // displayValues
    // for loop which classifies each element in array using if statement
    // compare predicted class with groundtruth.
    return 0;
}
