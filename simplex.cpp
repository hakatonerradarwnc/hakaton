#include <iostream>// для ввода вывода
#include <fstream> // для файлов
#include <sstream> // для строк
#include <vector> // для вектора
#include <string> // для строк
#include <algorithm>
#include <chrono> // для времени

void readCSV(std::vector<std::vector<double>> &values)
{
const std::string fname = "input.csv";
    std::ifstream ifs(fname.c_str());
    if (!ifs)
    {
       std::cerr << "No file with name: " << fname << std::endl;
       //return 1;
    }
    std::string line;
    while (ifs.good())
    {
       std::getline(ifs, line);
       std::replace(line.begin(), line.end(), ',',  ' ');
       std::stringstream ss(line);
       std::string current;
       std::vector<double> temp;
       while (ss >> current)
       {
          try
          {
             const double d = std::stod(current);
             temp.push_back(d);
          }
          catch (const std::exception& e)
          {
          }
       }
       values.push_back(temp);
    }
    ifs.close();
    for (const auto& vec : values)
    {
       for (const auto& v : vec)
       {
          std::cout << v << " ";
       }
       std::cout << std::endl;
    }
};
 
int main()
{
    auto start = std::chrono::steady_clock::now(); // время начала выполгнения

    std::vector<std::vector<double>> matrix;
    readCSV(matrix);
   


    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Время выполнения программы: " << duration << " микро секунд" << std::endl;

   return 0;
}