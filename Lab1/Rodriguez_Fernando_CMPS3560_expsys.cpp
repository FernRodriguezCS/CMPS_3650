#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <limits>

 /*===========================*/
 /*======== Rule =============*/
 // If feature > threshold ? 2 (Virginica) : 1 (Versicolour)
 /*===========================*/

// Function that reads data from a CSV into a 2-D array.
template <size_t rows, size_t cols>
void readCSV(double (&array)[rows][cols], char *filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: failed to open file: " << filename << "\n";
        std::exit(1);
    }

    for (size_t row = 0; row < rows; ++row)
    {
        std::string line;
        std::getline(file, line);
        if (!file.good()) { break; }

        // stringstream lets us treat a string (one CSV row) like a stream
        std::stringstream iss(line);
        for (size_t col = 0; col < cols; ++col)
        {
            std::string val;
            std::getline(iss, val, ',');
            // convert the string "1.8" into a double and store
            std::stringstream convertor(val);
            convertor >> array[row][col];
        }
    }
}

// Function that displays data values
template <size_t rows, size_t cols>
void displayValues(double (&array)[rows][cols])
{
    for (size_t row = 0; row < rows; ++row)
    {
        for (size_t col = 0; col < cols; col++)
        {
            std::cout << array[row][col] << ' ';
        }
        std::cout << std::endl;
    }
}

// predict using the simple threshold rule on a single row
template <size_t cols>
int predict_one(const double (&row)[cols], size_t feature_col, double threshold)
{
    return (row[feature_col] > threshold) ? 2 : 1;
}

// check accuracy of a given threshold for all rows
template <size_t rows, size_t cols>
double evaluate_accuracy(double (&array)[rows][cols],
                         size_t feature_col,
                         size_t label_col,
                         double threshold)
{
    size_t correct = 0;
    for (size_t r = 0; r < rows; ++r)
    {
        int pred = predict_one(array[r], feature_col, threshold);
        int gt   = static_cast<int>(array[r][label_col]);
        if (pred == gt) ++correct;
    }
    return rows ? (100.0 * correct / rows) : 0.0;
}

int main(int argc, char *argv[])
{
    constexpr size_t ROWS = 100;   
    constexpr size_t COLS = 2;     

    double pedalData[ROWS][COLS];
    readCSV(pedalData, (char*)"iris.csv");

    //displayValues(pedalData);

    // column 0 = feature, column 1 = label
    constexpr size_t FEATURE_COL = 0;
    constexpr size_t LABEL_COL   = 1;

    // try the original rule threshold
    double rule_threshold = 1.45;
    size_t correct = 0;
    for (size_t r = 0; r < ROWS; ++r)
    {
        int pred = predict_one(pedalData[r], FEATURE_COL, rule_threshold);
        int gt   = static_cast<int>(pedalData[r][LABEL_COL]);
        if (pred == gt) ++correct;
    }
    double acc = ROWS ? (100.0 * correct / ROWS) : 0.0;
    std::cout << "Accuracy @ " << rule_threshold << " = "
              << acc << "% (" << correct << "/" << ROWS << ")\n\n";

    // grid search for best threshold
    double fmin = std::numeric_limits<double>::infinity();
    double fmax = -std::numeric_limits<double>::infinity();
    for (size_t r = 0; r < ROWS; ++r) {
        double v = pedalData[r][FEATURE_COL];
        if (v < fmin) fmin = v;
        if (v > fmax) fmax = v;
    }

    // sweep thresholds between min and max feature values
    double best_thresh = rule_threshold;
    double best_acc = -1.0;
    const double step = 0.01; // more precise
    for (double t = fmin; t <= fmax; t += step)
    {
        double a = evaluate_accuracy(pedalData, FEATURE_COL, LABEL_COL, t);
        if (a > best_acc) {
            best_acc = a;
            best_thresh = t;
        }
    }

    std::cout << "Best threshold = " << best_thresh << "\n";
    std::cout << "Best accuracy  = " << best_acc << "%\n";

    return 0;
}