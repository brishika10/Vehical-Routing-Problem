#include <bits/stdc++.h>
using namespace std;
#define MAX_SIZE 100

int n, k;
float distances[50][50];
float distance_from_depot[50];

int random_number(int l, int r)
{
    int range = (r - l + 1);
    int num = l + rand() % range;
    return num;
}

float random_decimal()
{
    int a = rand() % 1000000;
    float p = (float)a;
    p = p / 1000000.0;
    return p;
}

class Individual
{
public:
    float fitness;
    double calc_fitness();
    Individual mutate_individual(Individual);
    vector<float> chromosome;
    Individual(vector<float> chromosome)
    {
        this->chromosome = chromosome;
        fitness = calc_fitness();
    }
};

vector<Individual> population;

double Individual::calc_fitness()
{
    vector<vector<int>> data(k);
    int length = chromosome.size();
    vector<pair<float, int>> chromo_copy;
    for (int i = 0; i < length; i++)
    {
        chromo_copy.push_back({chromosome[i], i});
    }
    sort(chromo_copy.begin(), chromo_copy.end());
    for (int i = 0; i < length; i++)
    {
        int val = (int)(chromo_copy[i].first);
        data[val].push_back(chromo_copy[i].second);
    }
    vector<float> total_distance(k);
    for (int i = 0; i < k; i++)
    {
        if (data[i].empty())
        {
            continue;
        }
        int total = 0;
        total = distance_from_depot[data[i].front()] + distance_from_depot[data[i].back()];
        int length = data[i].size();
        for (int j = 1; j < length; j++)
        {
            total += distances[data[i][j]][data[i][j - 1]];
        }
        total_distance[i] = total;
    }
    float mean = 0;
    for (int i = 0; i < k; i++)
    {
        mean += total_distance[i];
    }
    mean = mean / (float)length;
    float variance = 0;
    for (int i = 0; i < k; i++)
    {
        variance += pow(total_distance[i] - mean, 2);
    }
    float standard_deviation = variance / (float)(length);
    standard_deviation = sqrt(standard_deviation);
    return standard_deviation;
}

Individual Individual::mutate_individual(Individual parent2)
{
    vector<float> new_chromosome(n);
    for (int i = 0; i < n; i++)
    {
        int p = random_number(0, 100);
        if (p <= 50)
        {
            new_chromosome[i] = chromosome[i];
        }
        else if (p <= 75)
        {
            new_chromosome[i] = parent2.chromosome[i];
        }
        else
        {
            new_chromosome[i] = random_number(0, k - 1) + random_decimal();
        }
    }
    return Individual(new_chromosome);
}

vector<float> generate_chromosome()
{
    vector<float> chromosome(n);
    for (int i = 0; i < n; i++)
    {
        chromosome[i] = random_number(0, k - 1) + random_decimal();
    }
    return chromosome;
}

void create_population()
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        vector<float> chr = generate_chromosome();
        Individual ii = Individual(chr);
        population.push_back(ii);
    }
}

void display_final_configuration(Individual parent)
{
    vector<vector<int>> data(k);
    int length = parent.chromosome.size();
    vector<pair<float, int>> chromo_copy;
    for (int i = 0; i < length; i++)
    {
        chromo_copy.push_back({parent.chromosome[i], i});
    }
    sort(chromo_copy.begin(), chromo_copy.end());
    for (int i = 0; i < length; i++)
    {
        data[(int)chromo_copy[i].first].push_back(chromo_copy[i].second);
    }
    vector<float> total_distance(k);
    for (int i = 0; i < k; i++)
    {
        if (data[i].empty())
        {
            continue;
        }
        int total = 0;
        total = distance_from_depot[data[i].front()] + distance_from_depot[data[i].back()];
        int length = data[i].size();
        for (int j = 1; j < length; j++)
        {
            total += distances[data[i][j]][data[i][j - 1]];
        }
        total_distance[i] = total;
    }
    for (int i = 0; i < k; i++)
    {
        cout << "TRUCK: " << i + 1 << " : ";
        for (auto x : data[i])
        {
            cout << x << " ";
        }
        cout << "             DISTANCE: " << total_distance[i] << endl;
        cout << endl;
    }
    cout << parent.fitness << endl;
}

void init()
{
    cout << "NUMBER OF TRUCKS: ";
    cin >> k;

    cout << "NUMBER OF POINTS: ";
    cin >> n;

    cout << "ADJACENCY MATRIX FOR THE DISTANCES: " << endl;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> distances[i][j];
        }
    }

    cout << "ENTER DISTANCE FROM DEPOT: " << endl;

    for (int i = 0; i < n; i++)
    {
        cin >> distance_from_depot[i];
    }
}

bool operator<(const Individual &a1, const Individual &a2)
{
    return a1.fitness < a2.fitness;
}

int main()
{
    srand((unsigned)time(0));
    init();
    create_population();
    int number_of_generations = 2000;
    vector<Individual> new_population;
    while (number_of_generations--)
    {
        sort(population.begin(), population.end());
        new_population.clear();
        int size1 = (30 * MAX_SIZE) / 100;
        for (int i = 0; i < size1; i++)
        {
            new_population.push_back(population[i]);
        }
        for (int i = size1; i < MAX_SIZE; i++)
        {
            int r1 = random_number(0, size1 - 1);
            Individual parent1 = population[r1];
            int r2 = random_number(size1, MAX_SIZE - 1);
            Individual parent2 = population[r2];
            new_population.push_back(parent1.mutate_individual(parent2));
        }
    }
    display_final_configuration(population[0]);
}