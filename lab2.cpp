#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

void display_matrix(string info, vector <vector <double>> matrix){

  double eps = 0.001;

  cout << info << "\n";

  for (int i = 0; i < matrix.size(); i++){

    for ( int j = 0; j < matrix.size() + 1; j++ ){

      if ( j == matrix.size()){

        cout << "|" << "\t";
        
      }

      if ( matrix[i][j] == -0){

        matrix[i][j] = abs(matrix[i][j]);
        
      }

      cout << fixed << setprecision(3) << matrix[i][j] << "\t";
      
    }

    cout << "\n";
    
  }
  
}

void display_vector(string info, vector <double> vector){

  cout << info << "\n";

  for ( int i = 0; i < vector.size(); i++ ){

    cout << "x" << i << " = " << vector[i] << "\n";
    
  }
  
}

vector <double> get_vector_B(vector <vector <double>> matrix){

  vector <double> vector_B;

  for ( int i = 0; i < matrix.size(); i++ ){

    vector_B.push_back(matrix[i][matrix.size()]);
    
  }

  return vector_B;
  
}

vector <double> discrepancy(vector <double> vector, ::vector <double> matrix){

  for ( int i = 0; i < vector.size(); i++ ){

    matrix[i] = vector[i] - matrix[i];
    
  }

  return matrix;
  
}

double determinant(vector <vector <double>> matrix){

  double det = 1;

  for ( int i = 0; i < matrix.size(); i++ ){

    det *= matrix[i][i];
    
  }

  return det;
  
}

vector <vector <double>> transpose(vector <vector <double>> matrix){

  for ( int i = 0; i < matrix.size(); i++ ){

    for ( int j = 0; j < matrix.size(); j++ ){
    
      double tmp = matrix[i][j];

      matrix[i][j] = matrix[j][i];

      matrix[j][i] = tmp;
      
    }
    
  }  

  return matrix;
  
}

bool check_for_same_lines(vector <vector <double>> matrix){

  for ( int i = 0; i < matrix.size(); i++ ){

    for ( int j = 0; j < matrix.size(); j++ ){

      if ( i == j) continue;

      vector <double> row1 = matrix[i];

      vector <double> row2 = matrix[j];

      row1.pop_back();

      row2.pop_back();

      if ( row1 == row2 ){

        return true;
        
      }
      
    }
    
  }

  matrix = transpose(matrix);

  for ( int i = 0; i < matrix.size(); i++ ){

    for ( int j = 0; j < matrix.size(); j++ ){

      if ( i == j) continue;

      vector <double> row1 = matrix[i];

      vector <double> row2 = matrix[j];

      row1.pop_back();

      row2.pop_back();

      if ( row1 == row2 ){

        return true;
        
      }
      
    }

  }

  return false;
  
}

vector <vector <double>> sub(vector <vector <double>> matrix, int row_one, int row_two, double mn = 1){

  for ( int i = 0; i < matrix.size() + 1; i++ ){

    matrix[row_one][i] -= matrix[row_two][i] * mn;
    
  }

  return matrix;
  
}

vector <double> matrix_mul(vector <vector <double>> matrix, ::vector <double> vector){

  ::vector <double> result;

  for ( int i = 0; i < matrix.size(); i++ ){

    double s = 0;

    for ( int j = 0; j < vector.size(); j++ ){

      s += vector[j] * matrix[i][j];
      
    }

    result.push_back(s);
    
  }

  return result;
  
}

vector <double> divide_row(vector <double> row, double d){

  for ( int i = 0; i < row.size(); i++){

    row[i] /= d;
    
  }

  return row;
  
}

vector <vector <double>> read_matrix_from_file(ifstream & file){

  if (file.is_open()){

    int count = 0;
    
    float temp;

    while (!file.eof()){
      
      file >> temp;
      
      count++;
    
      }

    file.seekg(0, ios::beg);
    
    file.clear();

    int count_space = 0;
    
    char symbol;
    
    while (!file.eof()){
      
      file.get(symbol);
      
      if (symbol == ' ') count_space++;
      
      if (symbol == '\n') break;
      
    }

    file.seekg(0, ios::beg);
    
    file.clear();

    int n = count / (count_space + 1);
    
    int m = count_space + 1;
    
    vector <vector <double>> x;

    vector <double> row;
    
    for (int i = 0; i<n; i++) x.push_back(row);

    for (int i = 0; i < n; i++){
    
      for (int j = 0; j < m; j++){

        double curr_number;
      
        file >> curr_number;

        x[i].push_back(curr_number);

        }

      }

    return x;

    file.close();
    
  }
    
  else{

    cout << "Файл не найден.";
    
    }
  
}

vector <vector <double>> straight_path(vector <vector <double>> matrix){

  for ( int i = 0; i < matrix.size(); i++ ){

    matrix[i] = divide_row(matrix[i], matrix[i][i]);

    if ( i != matrix.size()){
    
      for ( int j = i + 1; j < matrix.size(); j++ ){

        matrix = sub(matrix, j, i, matrix[j][i]);

          }
      
        }
    
      }

    return matrix;

  }

vector <vector <double>> reverse_path(vector <vector <double>> matrix){

  vector <double> vec = get_vector_B(matrix);

  for ( int i = matrix.size() - 1; i > 0; i-- ){

    for ( int j = i - 1; j > -1; j-- ){

      matrix = sub(matrix, j, i, matrix[j][i]);
      
    }
    
  }

  return matrix;
  
  }

vector <vector <double>> gauss(vector <vector <double>> matrix){

  matrix = straight_path(matrix);

  matrix = reverse_path(matrix);

  return matrix;
  
}

int main(){

  ifstream file;

  file.open("matrix.txt");

  vector <vector <double>> clean_matrix = read_matrix_from_file(file);

  display_matrix("Исходная матрица", clean_matrix);

  if ( check_for_same_lines(clean_matrix)){

    cout << "\nМатрица вырожденная. Определитель равен нулю\n";

    return 1;
    
  }

  vector <vector <double>> matrix = clean_matrix;

  matrix = straight_path(matrix);

  display_matrix("Треугольная матрица", matrix);

  double det = determinant(matrix);

  cout << "Определитель: " << det << "\n";

  matrix = reverse_path(matrix);

  display_matrix("DIAGONAL", matrix);

  vector <double> x = get_vector_B(matrix);

  display_vector("\nРешение системы", x);

  vector <double> b = get_vector_B(clean_matrix);

  display_vector("Вектор B:", b);

  vector <double> res = matrix_mul(clean_matrix, x);

  display_vector("Результат перемножения матрицы A на вектор решения X:", res);

  vector <double> dis = discrepancy(res, b);

  display_vector("Невязка:", dis);
  
}