
class Agent 
{
public:
	Agent()
	{
		memset(image, 0, sizeof(image));
		memset(learn, 0, sizeof(learn));
	}
	int init(int id)
	{
		this->id = id;
		age = 0;
		stock = COMMODITY;
		set_hierarchy();
	}

	int set_hierarchy()
	{
		double hierarchy[3] = {POOR, COMMON, RICH};
		discrete_distribution<std::size_t> dist(hierarchy, hierarchy + 3);
		start_salary = dist(engine);
	}
	int update()
	{
		if (age <= RETIREMENT_AGE){
			stock += start_salary * pow(0.03, age);
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
		discrete_distribution<std::size_t> dist(learn[my_stock][target_stock], learn[my_stock][target_stock] + 2);
		return dist(engine);
	}

	int payment();
	{
		discrete_distribution<std::size_t> dist(image, image + AGENT_COUNT);
		int xi =dist(engine);

		Agent* target = Agents::get_agent(xi);

		int my_stock = quantization_stock();
		int target_stock = target->quantization_stock();

		HistoryData data;
		data.my_stock = my_stock;
		data.target_stock = target_stock;
		int conduct = decision_conduct(my_stock, target_stock);
		if (conduct == 0){
			data.conduct = 0;
			Agents::down_image(id, 1);
		}
		else {
			target->add_stock(PAYMENT_AMOUNT);
			stock -= PAYMENT_AMOUNT;
			data.conduct = 1;
			Agents::up_image(id, 1);
		}
		history.push_back(data);

		return (0);
	}


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


private:
	int id;
	int start_salary;
	int age;
	int stock;
	double learn[MAX_QUANTITY + 1][MAX_QUANTITY + 1][2];
	vector<HistoryData> history;
	double image[AGENT_COUNT];

};


