#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<random>
#include<cmath>
#include<cstdlib>
#include <unordered_map>

#define POPULATION_SIZE 100
#define NUMBER_OF_PARENTS 10
#define GENERATIONS 10

using namespace std;

vector<vector<double> > firstPopulation();
vector<double> crossover (vector<double> parent1, vector<double> parent2);
vector<double> mutation(vector<double> child);
vector<double> randomDoubles(int size);
vector<string> manyDoublesToBinaries (vector<double> doubles);
vector<vector<double> > selection(vector<vector<double> > population, int mutationProbability);
string doubleToBinary(double x);
double func(double x, double y);
double binaryToDouble(string binary);
double fitness(vector<double> individual);

int main() {
    vector<vector<double> > population = firstPopulation();
    cout << "Enter mutation probability in percent: ";
    int mutationProbability;
    cin >> mutationProbability;
    for (int i = 0; i < GENERATIONS; i++) {
        population = selection(population, mutationProbability);
    }
    return 0;

}

double func(double x, double y) {
    return x*x + y*y - x * y + x + y;
}

vector<vector<double> > firstPopulation() {
    vector<vector<double> > population;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        vector<double> individual;
        individual.push_back(((double)rand() / RAND_MAX) * 2000.0 - 1000.0);
        individual.push_back(((double)rand() / RAND_MAX) * 2000.0 - 1000.0);
        population.push_back(individual);
    }
    return population;
}

/*vector<double> randomDoubles(int size) {
    vector<double> randomDoubles;
    double lowerBound = -100;
    double upperBound = 100;

    uniform_real_distribution<double> unif(lowerBound,upperBound);

    default_random_engine re;

    for (int i = 0; i < size; i++) {
        double randomDouble = unif(re);
        randomDoubles.push_back(randomDouble);
    }
    return randomDoubles;
}*/

vector<string> manyDoublesToBinaries (vector<double> doubles) {
    vector<string> binaries;
    for (double i : doubles) {
        binaries.push_back(doubleToBinary(i));
    }
    return binaries;
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
    return grad;
}

vector<double> crossover (vector<double> parent1, vector<double> parent2) {
    string parent1XGene = doubleToBinary(parent1[0]);
    string parent1YGene = doubleToBinary(parent1[1]);
    string parent2XGene = doubleToBinary(parent2[0]);
    string parent2YGene = doubleToBinary(parent2[1]);
    vector<double> child;
    short crossoverPoint = rand() % 16;
    // test
    string childXGene = parent1XGene.substr(0, crossoverPoint) + parent2XGene.substr(crossoverPoint, parent2XGene.length() - crossoverPoint);
    string childYGene = parent2YGene.substr(0, crossoverPoint) + parent1YGene.substr(crossoverPoint, parent2YGene.length() - crossoverPoint);
    child.push_back(binaryToDouble(childXGene));
    child.push_back(binaryToDouble(childYGene));

    cout << "Child x: " << child[0] << endl;
    cout << "Child y: " << child[1] << endl;
    return child;
}

double binaryToDouble(string binary) {
    if (binary[0] == '-') {
        return -binaryToDouble(binary.substr(1, binary.length() - 1));
    }
    double decimal = 0;
    int x;
    unsigned length = binary.length();
    int ind_of_point = binary.find('.');
    binary.erase(ind_of_point, 1);
    for (int i = 0; i < length; i++) {
        x = binary[i] == '1';
        decimal += x * pow(2, ind_of_point - i - 1);
    }
    return decimal;
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

vector<vector<double> > selection(vector<vector<double> > population, int mutationProbability) {
    unordered_map<int, vector<double> > fitnessMap;
    vector<double> fitnessVector;
    vector<vector<double> > children;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitnessMap.insert({fitness(population[i]), population[i]});
        cout << "Individual fitness: " << fitness(population[i]) << endl;
        fitnessVector.push_back(fitness(population[i]));
    }
    sort(fitnessVector.begin(), fitnessVector.end());
    cout << "Best fitness: " << fitnessVector[0] << endl;
    cout << "Best individual: " << fitnessMap[fitnessVector[0]][0] << " " << fitnessMap[fitnessVector[0]][1] << endl;

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
        //cout << "Child " << i << ": " << children[i][0] << " " << children[i][1] << endl;
    }
    return children;
}
