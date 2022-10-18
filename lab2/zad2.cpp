#include <iostream>
#include <functional>
#include <vector>
#include <random>
#include <string>
#include <math.h>
#include <map>
using namespace std;

//Booth function 
double booth(double x, double y) {
    return pow((x + 2*y - 7),2) + pow((2*x+y-5),2);
}

//Matyas function
double matyas(double x, double y){
    return 0.26*(pow((x),2) + pow((y),2)) - 0.48*x*y;
}

//Himmelblau's function
double himmel(double x, double y){
    return pow((pow((x),2) + y - 11),2) + pow((x + pow((y),2) - 7),2);
}

vector<double> get_result(function<double(double,double)> f, double low, double high, int repeats){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distr(low, high);
    vector<double> best_result = {distr(gen), distr(gen)};
    double result = f(best_result.at(0),best_result.at(1));
    for(int i = 0; i < repeats; i++){
        vector<double> args = {distr(gen), distr(gen)};
        double new_result = f(args.at(0),args.at(1));
        if (new_result < result){
            result = new_result;
            best_result = args;
        }
    }
    return best_result;
}
typedef double (*FnPtr)(double, double);
int main(int argc, char **argv) {
    vector<string> argument(argv, argv + argc);
    string function_name = argument.at(1);
    double low = atof(argv[2]);
    double high = atof(argv[3]);
    map<string, FnPtr> myMap;
    myMap["booth"] = booth;
    myMap["matyas"] = matyas;
    myMap["himmel"] = himmel;
    vector<double> output = get_result(myMap[function_name], low, high, 10000000);
    cout << "x = " << output[0] << ", y = " << output[1] << endl;
    return 0;
}