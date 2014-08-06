#include <stdio.h>
#include <vector>
#include <random>

using namespace std;

const int AGENT_COUNT = 10;
const int COMMODITY = 50;
const int MAX_QUANTITY = 10;
const int START_SALARY_STANDERD = 40;
const int PAYMENT_AMOUNT = 20;
const int RETIREMENT_AGE = 40;

const int POOR = 1;
const int COMMON = 3;
const int RICH = 1;

std::mt19937 engine;

struct HistoryData
{
	int my_stock;
	int target_stock;
	int conduct;
};

class Agent 
{
public:
	Agent()
	{
		memset(image, 0, sizeof(image));
		for (int i = 0; i < AGENT_COUNT; i++){
			image[i] = 1.0;
		}
		memset(learn, 0, sizeof(learn));
		for (int i = 0; i <= MAX_QUANTITY; i++){
			for (int j = 0; j <= MAX_QUANTITY; j++){
				learn[i][j][0] = 1;
				learn[i][j][1] = 1;
			}
		}
	}
	int init(int id)
	{
		this->id = id;
		age = 0;
		stock = COMMODITY;
		set_hierarchy();
		image[id] = -1000000;
	}

	int get_id()
	{
		return id;
	}
	
	int get_age()
	{
		return age;
	}

	int set_hierarchy()
	{
		double hierarchy[3] = {POOR, COMMON, RICH};
		discrete_distribution<int> dist(hierarchy, hierarchy + 3);
		//start_salary = (dist(engine) + 1) * START_SALARY_STANDERD;
		start_salary = pow(2, (dist(engine) - 1)) * START_SALARY_STANDERD;
	}
	int update()
	{
		printf("%d: Age %d Stock %d\n", id, age, stock);
		if (age <= RETIREMENT_AGE){
			stock += start_salary * pow(1.005, age);
		}
		payment();
		stock -= COMMODITY;
		if (stock < 0){
			die();
		}
		age++;
	}

	int up_image(int index, int amount)
	{
		image[index] += amount;
	}

	int down_image(int index, int amount)
	{
		image[index] -= amount;
	}
	int die()
	{
		int learn_amount = age - RETIREMENT_AGE;

		//image[i] = 1.0;
		printf("%d is die\n", id);
		for (int i = 0; i < history.size(); i++){
			HistoryData hd = history[i];
			learn[hd.my_stock][hd.target_stock][hd.conduct] += learn_amount;
		}
		history = vector<HistoryData>();
		age = 0;
		stock = 0;
		set_hierarchy();
	}

	int decision_conduct(int my_stock, int target_stock)
	{
		discrete_distribution<int> dist(learn[my_stock][target_stock], learn[my_stock][target_stock] + 2);
		printf("%lf %lf\n", learn[my_stock][target_stock][0], learn[my_stock][target_stock][1]);
		return dist(engine);
	}

	int payment();

	int add_stock(int amount)
	{
		stock += amount;
	}

	int quantization_stock()
	{
		if (stock / COMMODITY >= MAX_QUANTITY){
			return MAX_QUANTITY;
		}
		return (stock / COMMODITY);
	}


public:
	int id;
	int start_salary;
	int age;
	int stock;
	double learn[MAX_QUANTITY + 1][MAX_QUANTITY + 1][2];
	vector<HistoryData> history;
	double image[AGENT_COUNT];

};
/*

class Agents
{
public:
	static Agent* get_agent(int index)
	{
		return &agents[index];
	}

	static int init()
	{
		for (int i = 0; i  < AGENT_COUNT; i++){
			agents[i].init(i);
		}
	}


	static int update()
	{
		for (int i = 0; i  < AGENT_COUNT; i++){
			agents[i].update();
		}
	}

	static int up_image(int index, int amount)
	{
		for (int i = 0; i  < AGENT_COUNT; i++){
			agents[i].up_image(index, amount);
		}
	}

	static int down_image(int index, int amount)
	{
		for (int i = 0; i  < AGENT_COUNT; i++){
			agents[i].down_image(index, amount);
		}
	}
private:
	static Agent agents[AGENT_COUNT];
};
*/

Agent agents[AGENT_COUNT];

int Agent::payment()
{
	discrete_distribution<int> dist(image, image + AGENT_COUNT);
	puts("image:");
	for (int i = 0; i < AGENT_COUNT; i++){
		printf("%lf ", image[i]);
	}
	puts("");
	int xi =dist(engine);

	//Agent* target = Agents::get_agent(xi);
	if (xi != id){
		Agent* target = &agents[xi];

		int my_stock = quantization_stock();
		int target_stock = target->quantization_stock();

		HistoryData data;
		data.my_stock = my_stock;
		data.target_stock = target_stock;
		int conduct = decision_conduct(my_stock, target_stock);
		int im;
		printf("conduct %d\n", conduct);
		if (conduct == 0){
			data.conduct = 0;
			im = -1;
			//Agents::down_image(id, 1);
		}
		else {
			target->add_stock(PAYMENT_AMOUNT);
			stock -= PAYMENT_AMOUNT;
			data.conduct = 1;
			im = 1;
			//Agents::up_image(id, 1);
			printf("payment %d -> %d\n", id, target->get_id());
		}
		for (int i = 0; i < AGENT_COUNT; i++){
			agents[i].up_image(id, im);
		}
		history.push_back(data);
	}

	return (0);
}


int main(void)
{
	engine = std::mt19937(10);
	int turn;

	for (int i = 0; i < AGENT_COUNT; i++){
		agents[i].init(i);
	}
	turn = 1;
	while (1){
		puts("");
		puts("");
		puts("");
		printf("trun%d\n", turn);
		if (turn % 1000 == 0){
			int sum_age = 0;
			for (int i = 0; i < AGENT_COUNT; i++){
				printf("%d\n", agents[i].get_age());
				sum_age += agents[i].get_age();
			}
			printf("ageavg=%lf\n", sum_age / double(AGENT_COUNT));
			int n;
			scanf("%d", &n);
		}
		for (int i = 0; i < AGENT_COUNT; i++){
			agents[i].update();
		}
		turn++;
	}
	return (0);
}

