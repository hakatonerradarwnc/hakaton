#include <iostream>  // для ввода вывода
#include <fstream>   // для файлов
#include <sstream>   // для строк
#include <vector>    // для вектора
#include <string>    // для строк
#include <algorithm> // всякие прикольные штуки
#include <chrono>    // для времени

// всего у нас есть 1 секунда = 1000 миллисикунд = 1 000 000 микросекунд,
// одноразовое чтение из файла занимает порядка 300-500 микросекунд
// места где можно и нужно (при возможности) сильно ускориться:
// 1 переписать всё под свой класс заранее заготовив место на этапе компиляции, разметив матрицу совместности как инты, и только веса как дабл
//

void readCSV(std::vector<std::vector<double>> &matrix)
{
   const std::string fname = "input.csv";
   std::ifstream ifs(fname.c_str());
   if (!ifs)
   {
      std::cerr << "No file with name: " << fname << std::endl;
      // return 1;
   }
   std::string line;
   while (ifs.good())
   {
      std::getline(ifs, line);
      std::replace(line.begin(), line.end(), ',', ' ');
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
         catch (const std::exception &e)
         {
         }
      }
      matrix.push_back(temp);
   }
   ifs.close();
};
void printmat(std::vector<std::vector<double>> &matrix) // печать матрицы, стоит около 500 микросекунд вызов
{
   for (const auto &vec : matrix)
   {
      for (const auto &v : vec)
      {
         std::cout << v << " ";
      }
      std::cout << std::endl;
   }
}
void zhadno(std::vector<std::vector<double>> &matrix, std::vector<std::vector<double>> &res) //
{
   std::vector<std::vector<double>> M = matrix; // снимаем копию, чтобы не испортить оригинальные данные
   int m = M[0].size() - 1;                     // количество столбцов, оно же число строчек матрицы совместности. сразу уменьшаем на единицу для работы с индексами
   int n = M.size() - 1;                        // количество строк, последняя содержит веса
   for (int i = 0; i < m; i++)                  // так как матрица совместности квадратная то и обработать нужно только столько строк сколько столбцов
   {
      std::vector<int> sovm;      // вспомогательный вектор для проверки совместности используемых всех траекторных гипотез
      res[i][n] += M[n][i];       // записываем вес в сумму в последний столбец результирующей матрицы
      res[i][i] = 1;              // записываем в результирующую матрицу какую именно т раекторную гипотезу мы выбрали
      M[n][i] = 0;                // удаляем вес в рабочией матрице
      sovm.push_back(i);          // добавляем индекс в вектор для проверок
      for (int j = 0; j < m; j++)
      {
         auto me = std::max_element(M[n].begin(), M[n].end()); // ищем максимальный вес
         int indme = std::distance(M[n].begin(), me);          // ищем индекс максимального веса        
         // сравнить коичество проверяемых тг итоговой суммой этой колонки, если равно, можем продолжать, если нет, что-то не совместно
         int ssum = 0;                         // сумма совместности
         for (int k = 0; k < sovm.size(); k++) // считаем совместность с тем что уже есть
         {
            ssum += M[sovm[k]][indme];
         }

         if ((M[i][indme] == 1)&&(ssum == sovm.size())) // проверяем совместность текущего потока решения
         {
            res[i][n] += *me;      // записываем вес в сумму в последний столбец результирующей матрицы
            res[i][indme] = 1;     // записываем в результирующую матрицу какую именно т раекторную гипотезу мы выбрали
            M[n][indme] = 0;       // удаляем вес в рабочией матрице
            sovm.push_back(indme); // добавляем индекс в вектор для проверок
         }
         else
         {
            M[n][indme] = 0; // удаляем вес в рабочией матрице
         }
         
      }
      sovm.clear();
      M = matrix;
   }
}
void writeCSV(std::vector<std::vector<double>> &matrix)
{
   std::ofstream file("output.csv");

    // Iterate through the two-dimensional vector and write the data to the CSV file
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            file << element << ",";
        }
        file << "\n"; // Add a new line after each row
    }

    // Close the file
    file.close();

    std::cout << "Data has been written to output.csv" << std::endl;
}

void sort_matrix(std::vector<std::vector<double>> &mat) // хотелось бы конечно что-то побыстрее, но я не спал более 30 часов, так что пузырь, сталин сорт к сожалению ухудшит и без того скудную точность
{
    std::vector<double> temp;
    int m = mat[0].size()-1;
    for (int j=0;j<mat.size()-1;j++)
    {
    for (int i = 0; i < mat.size()-1; i++)
    {
            if (mat[i][m] < mat[i + 1][m])
            {
                temp = mat[i];
                mat[i] = mat[i + 1];
                mat[i + 1] = temp;
            }
    }
    }
}

int main()
{
   auto start = std::chrono::steady_clock::now(); // время начала выполгнения

   std::vector<std::vector<double>> matrix;
   readCSV(matrix);
   std::vector<std::vector<double>> result(matrix[0].size(), std::vector<double>(matrix.size(), 0));
   zhadno(matrix, result);
   
   sort_matrix(result);
   if (result.size()>5)
   {
   for (int i=0; i<result.size()-5; i++)
   {
      result[i+5].clear();
   }
   }
   //printmat(result);
   writeCSV(result);


   auto end = std::chrono::steady_clock::now();
   auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
   std::cout << "Время выполнения программы: " << duration << " микросекунд" << std::endl;

   return 0;
}