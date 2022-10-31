#include <iostream>
#include <vector>
#include <functional>
#include <random>

using namespace std;

random_device rd;
mt19937 mt_generator(rd());

vector<vector<int>> create_pop(int size) {
    vector<vector<int>> pop;

    for (int i = 0; i < size; ++i) {
        vector<int> element;

        for (int j = 0; j < size ; ++j) {
            uniform_int_distribution<int> uni(0, 1);
            element.push_back(uni(mt_generator));
        }
        pop.push_back(element);
    }

    return pop;
}

vector<double> decipher(vector<int> chromosome) {
    double x = 0;
    double y = 0;

    for (int i = 0; i < chromosome.size() / 2; i++) {
        x = x * 2 + chromosome[i];
    }

    for (int i = chromosome.size() / 2; i < chromosome.size(); i++) {
        y = y * 2 + chromosome[i];
    }

    x = x / pow(2.0, (chromosome.size() / 2 - 4)) - 8;
    y = y / pow(2.0, (chromosome.size() / 2 - 4)) - 8;
    vector<double> position = {x, y};
    return position;
}

auto holder = [](vector<double> v) {
    double x = v[0];
    double y = v[1];
    return -abs(sin(x)*cos(y)*exp(abs(1-(sqrt(pow((x),2)+pow((y),2))/M_PI))));
};

double fitness(vector<int> chromosome) {
    return 1.0 / (1.0 + abs(holder(decipher(chromosome))));
}

int main() {
    vector<vector<int>> population = create_pop(100 + (22631 % 10) * 2);

    for (auto &chromosome: population) {
        auto deciphered = decipher(chromosome);
        cout << "x = " << deciphered.at(0) <<  ", y = " << deciphered.at(1) << endl << "Max: " <<fitness(chromosome) << endl << endl;
    }
}