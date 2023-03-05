#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int** create_matrix(int, int);
void delete_matrix(int**, int);

int** take_input(string, int&, int&);

//-------Swap two columns of the matrix
void swap_cols(int**, int, int, int);

//------Copy base matrix without 0 column------
void copy_matrix(int**, int**, int, int);

//------Create matrix UxM with preferences relatively to certain user-----
int** make_relative_preferences(int**, int, int, int);

int sort_and_count_inversion(int*, int, int);
int merge_and_count_split_inversion(int*, int, int, int);

int** create_inversions_matrix(int**, int, int, int);
void sort_inversions_matrix(int**, int);

//----Create output file and fill it with data--------
void create_output(string, int**, int, int);

int main(int argc, char* argv[]) {
    const string input_name = "input_10_5.txt";
    const string output_name = "ip22_prysiazhnyi_02_output.txt";

    int user_number = 6;//stoi(argv[2], nullptr, 10);
    int amount_of_users, amount_of_films;
    int **user_preferences;
    int **relative_users_preferences;
    int **inversion_matrix;

    user_preferences = take_input(input_name, amount_of_users, amount_of_films);
//    for(int i = 0; i < amount_of_users; i++) {
//        for(int j = 0; j < amount_of_films+1; j++) {
//            cout.width(4);
//            cout<<user_preferences[i][j];
//        }
//        cout<<endl;
//    }
//    cout<<"--------------------------------------"<<endl;
    relative_users_preferences = make_relative_preferences(user_preferences, amount_of_users, amount_of_films, user_number);
//    for(int i = 0; i < amount_of_users; i++) {
//        for(int j = 0; j < amount_of_films; j++) {
//            cout.width(4);
//            cout<<relative_users_preferences[i][j];
//        }
//        cout<<endl;
//    }
//    cout<<"--------------------------------------"<<endl;
    inversion_matrix = create_inversions_matrix(relative_users_preferences, amount_of_users, amount_of_films, user_number);
//    for(int i = 0; i < 2; i++) {
//        for(int j = 0; j < amount_of_users-1; j++) {
//            cout.width(4);
//            cout<<inversion_matrix[i][j];
//        }
//        cout<<endl;
//    }
    sort_inversions_matrix(inversion_matrix, amount_of_users-1);

    create_output(output_name, inversion_matrix, amount_of_users-1, user_number);


    delete_matrix(user_preferences, amount_of_users);
    delete_matrix(relative_users_preferences, amount_of_users);
    delete_matrix(inversion_matrix, 2);
    system("pause");
    return 0;
}

int** create_matrix(int rows, int cols) {
    int** matrix;

    matrix = new int*[rows];
    for(int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    return matrix;
}

void delete_matrix(int** matrix, int rows) {
    for(int i = 0;i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int** take_input(string file_path, int&U, int&M) {
    ifstream input_file(file_path);

    input_file>>U>>M;

    int** matrix = create_matrix(U, M+1);

    for(int i = 0; i < U; i++) {
        for(int j = 0; j < M+1; j++) {
            input_file>>matrix[i][j];
        }
    }

    input_file.close();
    return matrix;
}

void swap_cols(int** matrix, int col1, int col2, int rows) {
    int temp;
    for(int i = 0; i < rows; i++) {
        temp = matrix[i][col1];
        matrix[i][col1] = matrix[i][col2];
        matrix[i][col2] = temp;
    }

}

void copy_matrix(int** dest, int** from, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 1; j <= cols; j++) {
            dest[i][j-1] = from[i][j];
        }
    }
}

int** make_relative_preferences(int** matrix, int U, int M, int user) {
    int **relative_preferences = create_matrix(U, M);
    copy_matrix(relative_preferences, matrix, U, M);

    for(int i = 0; i < M; i++) {
        for(int j = 0; j < M; j++) {
            if(relative_preferences[user-1][i] < relative_preferences[user-1][j]) {
                swap_cols(relative_preferences, i, j, U);
            }
        }
    }

    return relative_preferences;
}

int sort_and_count_inversion(int *array, int left, int right) {
    if (right == left) return 0;

    int middle = (left + right)/2;

    int x = sort_and_count_inversion(array, left, middle);
    int y = sort_and_count_inversion(array, middle+1, right);
    //cout<<middle<<'\t'<<length - middle<<endl;
    int z = merge_and_count_split_inversion(array, left, middle, right);

    return x+y+z;
}

int merge_and_count_split_inversion(int *array, int left, int middle, int right) {
    int *temp = new int[right - left + 1];
    int i = left;
    int j = middle+1;
    int k = left;
    int inversions = 0;

    while(i <= middle && j <= right) {
        if(array[i] < array[j]) {
            temp[k] = array[i];
            k++;
            i++;
        } else {
            temp[k] = array[j];
            k++;
            j++;
            inversions += middle + 1 - i;
        }
    }

    while(i <= middle) {
        temp[k] = array[i];
        k++;
        i++;
    }

    while(j <= right) {
        temp[k] = array[j];
        k++;
        j++;
    }

    for(int l = left; l<= right; l++) {
        array[l] = temp[l];
    }

    delete[] temp;
    return inversions;
}

int** create_inversions_matrix(int** preferences, int U, int M, int x) {
    int** inversions = create_matrix(2, U-1);
    int shift = 0;

    for(int i = 0; i < U; i++) {
           if(i+1==x) {
               shift = 1;
               continue;
           }

           inversions[0][i-shift] = i + 1;
           inversions[1][i-shift] = sort_and_count_inversion(preferences[i], 0, M-1);
    }

    return inversions;
}

void sort_inversions_matrix(int** matrix, int length) {
    int key, j, index;

    for(int i = 1; i < length; i++) {
        index = matrix[0][i];
        key = matrix[1][i];
        j = i-1;

        while(j >= 0 && key < matrix[1][j]) {
            matrix[0][j+1] = matrix[0][j];
            matrix[1][j+1] = matrix[1][j];
            j--;
        }

        matrix[0][j+1] = index;
        matrix[1][j+1] = key;
    }
}

void create_output(string path, int **matrix, int length, int x) {
    ofstream output_file(path);
    output_file<<x<<endl;

    for(int i = 0; i < length; i++) {
        output_file<<matrix[0][i]<<'\t'<<matrix[1][i]<<endl;
    }

    output_file.close();
}
