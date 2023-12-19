#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<random>
#include<cmath>
#include<cstdlib>
#include<cstring>
#include <unordered_map>
#include <ctime>

#define POPULATION_SIZE 1000
#define NUMBER_OF_PARENTS 5
#define GENERATIONS 50

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
void printPopulation(vector<vector<double> > population, int size);

int mutaion_cnt = 0;

int main() {
    srand(time(NULL));
    vector<vector<double> > population = firstPopulation();
    cout << "Enter mutation probability in percent: ";
    int mutationProbability = 20;
//    cin >> mutationProbability;
    for (int i = 0; i < GENERATIONS; i++) {
        cout << "Generation " << i << endl;
        population = selection(population, mutationProbability);
//        printPopulation(population, POPULATION_SIZE);
    }
    return 0;

}

double func(double x, double y) {
    return x*x + y*y - x * y + x + y;
}

void printPopulation(vector<vector<double> > population, int size) {
    for (int i = 0; i < size; i++) {
        cout << "Individual " << i << ": " << population[i][0] << " " << population[i][1] << endl;
    }
}

vector<vector<double> > firstPopulation() {
    vector<vector<double> > population;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        vector<double> individual;
        individual.push_back(((double)rand() / RAND_MAX) * 2000.0 - 1000.0);
        individual.push_back(((double)rand() / RAND_MAX) * 2000.0 - 1000.0);
        population.push_back(individual);
    }
//    printPopulation(population, POPULATION_SIZE);
    return population;
}

string doubleToBinary(double x) {
    if (x < 0) {
        return "-" + doubleToBinary(-x);
    }
    int count = 0;
    string binary = "";
    int integer = (int) x;
    double decimal = x - integer;
    while (integer > 0 && count < 17) {
        binary += to_string(integer % 2);
        integer /= 2;
        count++;
    }
    while (count++ < 16) {
        binary += "0";
    }
    reverse(binary.begin(), binary.end());
    binary += ".";
    count = 0;
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

//vector<double> crossover (vector<double> parent1, vector<double> parent2) {
//    vector<double> child;
//    string parent1XGene = doubleToBinary(parent1[0]);
//    string parent1YGene = doubleToBinary(parent1[1]);
//    string parent2XGene = doubleToBinary(parent2[0]);
//    string parent2YGene = doubleToBinary(parent2[1]);
////    vector<double> child;
//    short crossoverPoint = (rand() % 33 + 5) % 33;
////    short crossoverPoint = 15;
//    string childXGene =
//                parent1XGene.substr(0, crossoverPoint) + parent2XGene.substr(crossoverPoint, parent2XGene.length() - 1);
//    string childYGene =
//                parent1YGene.substr(0, crossoverPoint) + parent2YGene.substr(crossoverPoint, parent2YGene.length() - 1);
////    child.push_back(binaryToDouble(childXGene));
////    child.push_back(binaryToDouble(childYGene));
//    child.push_back(binaryToDouble(childXGene));
//    child.push_back(binaryToDouble(childYGene));
//
//
//    return child;
//}

// EU crossover
vector<double> crossover (vector<double> parent1, vector<double> parent2) {
    vector<double> child;
    double crossoverPoint = ((double)rand() / RAND_MAX);
    double childX = parent1[0] * crossoverPoint + parent2[0] * (1 - crossoverPoint);
    double childY = parent1[1] * crossoverPoint + parent2[1] * (1 - crossoverPoint);
    child.push_back(childX);
    child.push_back(childY);
    return child;
}

double binaryToDouble(string binary) {
    if (binary[0] == '-') {
        return -binaryToDouble(binary.substr(1, binary.length() - 1));
    }
    double decimal = 0;
    int x;
    int ind_of_point = binary.find('.');
    for (int i = 0; i < 16 - ind_of_point; i++) {
        binary = "0" + binary;

    }
    ind_of_point = binary.find('.');
    unsigned length = binary.length();
    binary.erase(ind_of_point + 1, 1);
    int double_dot_check = binary.find('.');
    if (double_dot_check != string::npos) {
        binary.erase(ind_of_point + 1, 1);
    }
    for (int i = 0; i < length; i++) {
        x = binary[i] == '1';
        decimal += x * pow(2, 15 - i - 1);
    }
    return decimal;
}

vector<double> mutation(vector<double> child) {
    string childXGene = doubleToBinary(child[0]);
    string childYGene = doubleToBinary(child[1]);
//    short numberOfMutations = rand() % 32;
    short numberOfMutations = 5;
    for (int i = 0; i < numberOfMutations; i++) {
        short mutationPoint = rand() % 33;
        if (childXGene[mutationPoint] == '0') {
            childXGene[mutationPoint] = '1';
        } else if (childXGene[mutationPoint] == '1') {
            childXGene[mutationPoint] = '0';
        }
        mutationPoint = rand() % 33;
        if (childYGene[mutationPoint] == '0') {
            childYGene[mutationPoint] = '1';
        } else if (childYGene[mutationPoint] == '1') {
            childYGene[mutationPoint] = '0';
        }
    }
    vector<double> newChild;
    newChild.push_back(binaryToDouble(childXGene));
    newChild.push_back(binaryToDouble(childYGene));
    return newChild;
}

vector<vector<double> > selection(vector<vector<double> > population, int mutationProbability) {
    unordered_map<int, vector<double> > fitnessMap;
    vector<double> fitnessVector;
    vector<vector<double> > children;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitnessMap.insert({fitness(population[i]), population[i]});
        fitnessVector.push_back(fitness(population[i]));
    }
    sort(fitnessVector.begin(), fitnessVector.end());
    cout << "Best fitness: " << fitnessVector[0] << endl;
    cout << "Best individual: " << fitnessMap[fitnessVector[0]][0] << " " << fitnessMap[fitnessVector[0]][1] << endl;
    cout << "second best fitness: " << fitnessMap[fitnessVector[1]][0] << " " << fitnessMap[fitnessVector[1]][1] << endl;
    cout << "third best fitness: " << fitnessMap[fitnessVector[2]][0] << " " << fitnessMap[fitnessVector[2]][1] << endl;
    cout << "fourth best fitness: " << fitnessMap[fitnessVector[3]][0] << " " << fitnessMap[fitnessVector[3]][1] << endl;
    cout << "fifth best fitness: " << fitnessMap[fitnessVector[4]][0] << " " << fitnessMap[fitnessVector[4]][1] << endl;
//    vector<vector<double> > parents;
//
//    for (int i = 0; i < NUMBER_OF_PARENTS; i++) {
//        parents.push_back(fitnessMap[fitnessVector[i]]);
//    }

    for (int i = 0; i < POPULATION_SIZE; i += 2) {
        int tournamentSize = 5;
        int index1 = rand() % POPULATION_SIZE;
        int index2 = rand() % POPULATION_SIZE;
        for (int j = 0; j < tournamentSize; j++) {
            int index = rand() % POPULATION_SIZE;
            if (fitness(population[index]) < fitness(population[index1])) {
                index1 = index;
            }
            index = rand() % POPULATION_SIZE;
            if (fitness(population[index]) < fitness(population[index2])) {
                index2 = index;
            }
        }
//        printPopulation(parents, NUMBER_OF_PARENTS);
//        cout << "parents vector: " << endl;
        vector<double> parent1 = population[index1];
//        cout << "parent1: " << parent1[0] << " " << parent1[1] << endl;
        vector<double> parent2 = population[index2];
//        cout << "parent2: " << parent2[0] << " " << parent2[1] << endl;
//        vector<double> child = crossover(parent1, parent2);
//        children.push_back(child);
        children.push_back(crossover(parent1, parent1));
        children.push_back(crossover(parent2, parent1));
    }
    int mutaion_cnt = 0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        double randomInteger = rand() % 100;
//        cout << "randomInteger: " << randomInteger << " mutationProbability: " << mutationProbability << endl;
        if (randomInteger < mutationProbability) {
            mutaion_cnt++;
//            cout << "MUTATION!!!!!!!" << endl;
            children[i] = mutation(children[i]);
        }
        //cout << "Child " << i << ": " << children[i][0] << " " << children[i][1] << endl;
    }
    cout << "cnt_mutation: " << mutaion_cnt << endl;
    return children;
}
