#include <functional>
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
using mojafunkcja_t = std::function<double(double,double)>;
int main(int argc, char **argv) {
  using namespace std;
  map<string, mojafunkcja_t> formatery;
  formatery["sin"] = [](int a, int b) { return sin(a); };
  formatery["add"] = [](int a, int b) { return a + b; };
  formatery["mod"] = [](int a, int b) { return a % b; };
  try {
    vector<string> argumenty(argv, argv + argc);
    auto selected_f = argumenty.at(1);
    auto funkcja = formatery.at(selected_f);
    cout << "wynik: " << funkcja(atoi(argv[2]),atoi(argv[3]));
  } catch (std::out_of_range aor) {
    cout << "podaj argument. Dostępne to: ";
    for (auto [k, v] : formatery) cout << " " << k << endl;
  }
  return 0;
}