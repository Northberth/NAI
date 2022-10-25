#include <iostream>
#include <functional>
#include <vector>
#include <random>
#include <string>
#include <math.h>
#include <map>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Booth function
double booth(double x, double y){
    return pow((x + 2 * y - 7), 2) + pow((2 * x + y - 5), 2);
}

// Matyas function
double matyas(double x, double y){
    return 0.26 * (pow((x), 2) + pow((y), 2)) - 0.48 * x * y;
}

// Himmelblau's function
double himmel(double x, double y){
    return pow((pow((x), 2) + y - 11), 2) + pow((x + pow((y), 2) - 7), 2);
}

//Hölder table function
double holder(double x, double y){
    return -abs(sin(x)*cos(y)*exp(abs(1-(sqrt(pow((x),2)+pow((y),2))/M_PI))));
}

vector<double> full_review_method(function<double(double, double)> f, double low, double high, int repeats){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distr(low, high);
    vector<double> best_result = {distr(gen), distr(gen)};
    double result = f(best_result.at(0), best_result.at(1));
    for (int i = 0; i < repeats; i++){
        vector<double> args = {distr(gen), distr(gen)};
        double new_result = f(args.at(0), args.at(1));
        if (new_result < result){
            result = new_result;
            best_result = args;
        }
    }
    return best_result;
}

vector<double> next_hill_position(double x, double y){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distance(-0.005, 0.005);
    x = x + distance(gen);
    y = y + distance(gen);
    return {x, y};
}

vector<double> hill_climbing_algorithm(function<double(double, double)> f, double low, double high, int repeats){
    random_device rd;  
    mt19937 gen(rd());
    uniform_real_distribution<double> distr(low, high);
    vector<double> best_result = {distr(gen), distr(gen)};
    double result = f(best_result.at(0), best_result.at(1));
    for (int i = 0; i < repeats; i++){
        vector<double> args = next_hill_position(best_result.at(0), best_result.at(1));
        if (args[0] > high or args[0] < low or args[1] > high or args[1] < low){
            continue;
        }
        double new_result = f(args.at(0), args.at(1));
        if (new_result < result){
            result = new_result;
            best_result = args;
        }
    }
    return best_result;
}

vector<double> next_position_generation(double x, double y){
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> distance;
    x = x + 0.01 * distance(gen);
    y = y + 0.01 * distance(gen);
    return {x, y};
}

vector<double> annealing_algorithm(function<double(double, double)> f, double low, double high, int repeats){
    random_device rd; 
    mt19937 gen(rd());
    uniform_real_distribution<double> distr(low, high);
    vector<double> best_result = {distr(gen), distr(gen)};
    vector<double> args = {distr(gen), distr(gen)};
    double result = f(best_result.at(0), best_result.at(1));
    vector<vector<double>> visited_points = {};
    for (int i = 0; i < repeats; i++){
        vector<double> new_position = next_position_generation(best_result.at(0), best_result.at(1));
        double new_result = f(new_position.at(0),new_position.at(1));
        if (new_result < result){
            result = new_result;
            best_result = new_position;
        }
        else{
            uniform_real_distribution<double> rand(0, 1);
            double diff_result = new_result - result;
            if (diff_result < 0){
                diff_result = diff_result * -1;
            }
            double Tk = 10000.0 / i;
            if (rand(gen) < exp(-1 * diff_result) / Tk){
                best_result = new_position;
            }
        }
    }
    for (int j = 0; j < repeats; j++){
        vector<double> args = {distr(gen), distr(gen)};
        double new_result = f(args.at(0), args.at(1));
        if (new_result < result){
            result = new_result;
            best_result = args;
        }
    }
    return best_result;
}
typedef double (*FnPtr)(double, double);
int main(int argc, char **argv){
    vector<string> argument(argv, argv + argc);
    string function_name = argument.at(1);
    double low = atof(argv[2]);
    double high = atof(argv[3]);
    map<string, FnPtr> myMap;
    myMap["booth"] = booth;
    myMap["matyas"] = matyas;
    myMap["himmel"] = himmel;
    myMap["holder"] = holder;
    
    auto start = high_resolution_clock::now();
    cout << "Metoda pelnego przegladu:" << endl;
    vector<double> output = full_review_method(myMap[function_name], low, high, 1000000);
    auto stop = high_resolution_clock::now();
    auto duration = stop - start;
    cout << "x = " << output[0] << ", y = " << output[1] << ", Czas wykonania: " << duration/chrono::milliseconds(1000)<< "s" << endl;
    
    start = high_resolution_clock::now();
    cout << "Algorytm wspinaczkowy:" << endl;
    output = hill_climbing_algorithm(myMap[function_name], low, high, 1000000);
    stop = high_resolution_clock::now();
    duration = stop - start;
    cout << "x = " << output[0] << ", y = " << output[1] << ", Czas wykonania: " << duration/chrono::milliseconds(1000)<< "s" << endl;
    
    start = high_resolution_clock::now();
    cout << "Algorytm wyzarzania:" << endl;
    output = annealing_algorithm(myMap[function_name], low, high, 1000000);
    stop = high_resolution_clock::now();
    duration = stop - start;
    cout << "x = " << output[0] << ", y = " << output[1] << ", Czas wykonania: " << duration/chrono::milliseconds(1000)<< "s" << endl;
    return 0;
}