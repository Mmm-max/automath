#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<random>
#include<cmath>
#include <unordered_map>
#define POPULATION_SIZE 1000
#define NUMBER_OF_PARENTS 100
#define GENERATIONS 1000

using namespace std;

vector<vector<string> > firstPopulation();
vector<double> crossover (vector<double> parent1, vector<double> parent2);
vector<double> mutation(vector<double> child);
vector<double> randomDoubles(int size);
vector<vector<double> > selection(vector<vector<double> > population, int mutationProbability);
string doubleToBinary(double x);
double func(double x, double y);
double binaryToDouble(string binary);
double fitness(vector<double> individual);


int main() {
    vector<vector<string> > population = firstPopulation();
    cout << "Enter mutation probability in percent: ";
    int mutationProbability;
    cin >> mutationProbability;
    for (int i = 0; i < GENERATIONS; i++) {
        population = selection(population, mutationProbability);
    }
    return 0;

}

//function to find the minimum of
double func(double x, double y) {
    return x*x + y*y - x * y + x + y;
}

//first population
vector<vector<string> > firstPopulation() {
    vector<vector<string> > population;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        vector<double> individual;

        double lowerBound = -100;
        double upperBound = 100;

        uniform_real_distribution<double> unif(lowerBound,upperBound);

        default_random_engine re;

        default_random_engine y;

        double randomDoubleX = unif(re);
        double randomDoubleY = unif(y);
        individual.push_back(randomDoubleX);
        individual.push_back(randomDoubleY);


        individual.push_back(rand() % 100);
        individual.push_back(rand() % 100);
        //population.push_back(individual);
    }
    return population;
}

vector<double> randomDoubles(int size) {
    vector<double> randomDoubles;
    double lowerBound = -100;
    double upperBound = 100;

    uniform_real_distribution<double> unif(lowerBound,upperBound);

    default_random_engine re;

    for (int i = 0; i < size; i++) {
        double randomDouble = unif(re);
        printf("%f\n", randomDouble);
        randomDoubles.push_back(randomDouble);
    }
    return randomDoubles;
}

string doubleToBinary(double x) {
    if (x < 0) {
        return "-" + doubleToBinary(-x);
    }
    int count = 0;
    string binary = "";
    int integer = (int) x;
    double decimal = x - integer;
    while (integer > 0 && count < 16) {
        binary += to_string(integer % 2);
        integer /= 2;
        count++;
    }
    reverse(binary.begin(), binary.end());
    binary += ".";
    while (decimal > 0 && count < 16) {
        decimal *= 2;
        int bit = decimal;
        if (bit == 1) {
            decimal -= bit;
            binary += "1";
        } else {
            binary += "0";
        }
        count++;
    }
    if (count < 16) {
        for (int i = 0; i < 15 - count; i++) {
            binary += "0";
        }
    }
    return binary;
}

double fitness(vector<double> individual) {
    double x = individual[0];
    double y = individual[1];
    double dx = 0.0001;
    double dy = 0.0001;
    double dfdx = (func(x + dx, y) - func(x, y)) / dx;
    double dfdy = (func(x, y + dy) - func(x, y)) / dy;
    double grad = sqrt(dfdx * dfdx + dfdy * dfdy);
    return 1 / grad * 100;
}

vector<double> crossover (vector<double> parent1, vector<double> parent2) {
    string parent1XGene = doubleToBinary(parent1[0]);
    string parent1YGene = doubleToBinary(parent1[1]);
    string parent2XGene = doubleToBinary(parent2[0]);
    string parent2YGene = doubleToBinary(parent2[1]);
    vector<double> child;
    short crossoverPoint = rand() % 16;
    string childXGene = parent1XGene.substr(0, crossoverPoint) + parent2XGene.substr(crossoverPoint, 16);
    string childYGene = parent2YGene.substr(0, crossoverPoint) + parent1YGene.substr(crossoverPoint, 16);
    child.push_back(stod(childXGene, nullptr));
    child.push_back(stod(childYGene, nullptr));
    return child;
}

//function to transorm a binary double to decimal double
double binaryToDouble(string binary) {
    double decimal = 0;
    unsigned length = binary.length();
    if (length < 16) {
        for (int i = 0; i < 16 - length; i++) {
            binary += "0";
        }
    }
    for (int i = 0; i < length; i++) {
        if (binary[i] == '.') {
            length--;
        }
    }
    int count = 0;
    int integer = 0;
    double fraction = 0;
    while (binary[count] != '.' && count < 16) {
        count++;
    }
    int copy = count - 1;
    while (copy >= 0) {
        integer += pow(2, copy) * (binary[copy] - '0');
        copy--;
    }
    count++;
    while (count < 16) {
        fraction += pow(2, copy) * (binary[count] - '0');
        count++;
        copy--;
    }
    decimal = integer + fraction;
    cout << decimal << endl;
    return 0;
}

vector<double> mutation(vector<double> child) {
    string childXGene = doubleToBinary(child[0]);
    string childYGene = doubleToBinary(child[1]);
    short numberOfMutations = rand() % 16;
    for (int i = 0; i < numberOfMutations; i++) {
        short mutationPoint = rand() % 16;
        if (childXGene[mutationPoint] == '0') {
            childXGene[mutationPoint] = '1';
        } else {
            childXGene[mutationPoint] = '0';
        }
        mutationPoint = rand() % 16;
        if (childYGene[mutationPoint] == '0') {
            childYGene[mutationPoint] = '1';
        } else {
            childYGene[mutationPoint] = '0';
        }
    }
    return child;
}

//selection with unordered map
vector<vector<double> > selection(vector<vector<double> > population, int mutationProbability) {
    unordered_map<int, vector<double> > fitnessMap;
    vector<double> fitnessVector;
    vector<vector<double> > children;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitnessMap.insert({fitness(population[i]), population[i]});
        fitnessVector.push_back(fitness(population[i]));
    }
    sort(fitnessVector.begin(), fitnessVector.end());

    vector<vector<double> > parents;

    for (int i = 0; i < NUMBER_OF_PARENTS; i++) {
        parents.push_back(fitnessMap[fitnessVector[i]]);
    }

    for (int i = 0; i < POPULATION_SIZE; i++) {
        int index1 = rand() % NUMBER_OF_PARENTS;
        int index2 = rand() % NUMBER_OF_PARENTS;
        vector<double> parent1 = parents[index1];
        vector<double> parent2 = parents[index2];
        vector<double> child = crossover(parent1, parent2);
        children.push_back(child);
    }

    for (int i = 0; i < POPULATION_SIZE; i++) {
        double randomInteger = rand() % 100;
        if (randomInteger < mutationProbability) {
            children[i] = mutation(children[i]);
        }
    }
    return children;
}
