#include <vector> //only to be used to contain dispenser states and nothing else
#include <iostream>
#include <string>

using namespace std;

enum state { Out_Of_Poptarts, No_Credit, Has_Credit, Dispenses_Poptart };
enum stateParameter { No_Of_Poptarts, Credit, Cost_Of_Poptart };
enum bases {Plain = 1, Spicy = 2, Chocolate = 4, Coconut = 8, Fruity = 16};
enum fillings 
{
	F_Chocolate = 32, F_Banana = 64, F_Strawberry = 128, 
	F_Raspberry = 256, F_Apple = 512, F_Blackberry = 1024, 
	F_Maple = 2048, F_Marshmellow = 4096, F_Cheese = 8192,
	F_CheeseAndHam = 16384, F_Caramel = 32768, F_Vanilla = 65536
};

class StateContext;

class State
{
protected:

	StateContext* CurrentContext;

public:

	State(StateContext* Context)
	{
		CurrentContext = Context;
	}
	virtual ~State(void)
	{
	}
};

class StateContext
{
protected:

	State* CurrentState = nullptr;
	int stateIndex = 0;
	vector<State*> availableStates;
	vector<int> stateParameters;

public:

	virtual ~StateContext()
	{
		for (int index = 0; index < this->availableStates.size(); index++) delete this->availableStates[index];
		this->availableStates.clear();
		this->stateParameters.clear();
	}

	virtual void setState(state newState)
	{
		this->CurrentState = availableStates[newState];
		this->stateIndex = newState;
	}

	virtual int getStateIndex(void)
	{
		return this->stateIndex;
	}

	virtual void setStateParam(stateParameter SP, int value)
	{
		this->stateParameters[SP] = value;
	}

	virtual int getStateParam(stateParameter SP)
	{
		return this->stateParameters[SP];
	}
};

class Transition
{
public:

	virtual bool insertMoney(int) { cout << "Error!" << endl; return false; }
	virtual bool makeSelection(int) { cout << "Error!" << endl; return false; }

	virtual bool moneyRejected(void) { cout << "Error!" << endl; return false; }

	virtual bool addPoptart(int) { cout << "Error!" << endl; return false; }

	virtual bool dispense(void) { cout << "Error!" << endl; return false; }

};

class Product
{
protected:
	string product_description;
	int itemCost = 0;
public:
	virtual void consume(void)
	{

	}
	virtual int cost(void) { return this->itemCost; }
	virtual string description(void) { return product_description; }
	virtual Product* ReturnHighestCostItem(void)
	{
		return nullptr;
	}
};

class Base : public Product
{
public:
	Base(string productDesc, int itemCost)
	{
		this->product_description = productDesc;
		this->itemCost = itemCost;
	}
	int cost(void);
	string description(void);
	void consume();
	Product* ReturnHighestCostItem(void) { return nullptr; }
};

int Base::cost(void)
{
	return this->itemCost;
}

string Base::description(void)
{
	return this->product_description;
}

void Base::consume()
{
	cout << "Consuming " << product_description << "filled with: " << endl;
}

class Filling : public Product
{
protected:
	Product* base;
public:
	Filling(string productDesc, int itemCost)
	{
		this->product_description = productDesc;
		this->itemCost = itemCost;
	}
	virtual void fillProduct(Product* NewBase);
	virtual int cost(void);
	virtual string description(void);
	void consume();
	Product* ReturnHighestCostItem(void) {return nullptr;}
};

void Filling::fillProduct(Product* NewBase)
{
	base = NewBase;
}

int Filling::cost(void)
{
	return itemCost + base->cost();
}

string Filling::description(void)
{
	return this->product_description;
}

void Filling::consume()
{
	cout << base->description() + ", " + this->product_description;
}

class PoptartState : public State, public Transition
{
public:
	PoptartState(StateContext* context) : State(context)
	{}
};

class OutOfPoptarts : public PoptartState
{
public:
	OutOfPoptarts(StateContext* context) : PoptartState(context) {}
	bool moneyRejected();
	bool addPoptart(int);
};

bool OutOfPoptarts::moneyRejected(void)
{
	cout << "Ejecting!" << endl;
	this->CurrentContext->setState(Out_Of_Poptarts);
	return true;
}

bool OutOfPoptarts::addPoptart(int number)
{
	this->CurrentContext->setStateParam(No_Of_Poptarts, number);
	this->CurrentContext->setState(No_Credit);
	return true;
}

class NoCredit : public PoptartState
{
public:
	NoCredit(StateContext* context) : PoptartState(context)
	{}
	bool insertMoney(int);
};

bool NoCredit::insertMoney(int money)
{
	cout << "You inserted: " << money << endl;
	this->CurrentContext->setStateParam(Credit, money);
	cout << " Total: " << money << endl;
	this->CurrentContext->setState(Has_Credit);
	return true;
}



class HasCredit : public PoptartState
{
protected:
	Product* product = nullptr;
public:
	HasCredit(StateContext* context) : PoptartState(context)
	{}
	bool makeSelection(int);
	bool moneyRejected();
	bool insertMoney(int);
};


bool HasCredit::moneyRejected()
{
	this->CurrentContext->setState(Has_Credit);
	return true;
}

bool HasCredit::insertMoney(int money)
{
	cout << "You inserted: " << money << endl;
	int oldMoney = this->CurrentContext->getStateParam(Credit);
	this->CurrentContext->setStateParam(Credit, oldMoney + money);
	cout << " Total: " << oldMoney + money << endl;
	return true;
}

class DispensesPoptart : public PoptartState
{
protected:
	Product* product = nullptr;
public:
	DispensesPoptart(StateContext* context) : PoptartState(context)
	{}
	bool dispense(void);
};


class Poptart_Dispenser : public StateContext, public Transition
{
	friend class DispensesPoptart;
	friend class HasCredit;
private:
	Product* DispensedItem = nullptr;
	bool itemDispensed = false;
	bool itemRetrived = false;
	PoptartState* currentPoptartState = nullptr;
	//bool itemDispensed = false;
public:
	Poptart_Dispenser(int inventory_count);

	~Poptart_Dispenser(void);

	bool insertMoney(int money);

	bool makeSelection(int option);

	bool moneyRejected(void);

	bool addPoptart(int number);

	bool dispense(void);

	Product* getProduct(void);

	virtual void setStateParam(stateParameter SP, int value);

	virtual int getStateParam(stateParameter SP);
};


Poptart_Dispenser::Poptart_Dispenser(int inventory_count)
{
	this->availableStates.push_back(new OutOfPoptarts(this));
	this->availableStates.push_back(new NoCredit(this));
	this->availableStates.push_back(new HasCredit(this));
	this->availableStates.push_back(new DispensesPoptart(this));
	this->stateParameters.push_back(0); //Number of Poptarts
	this->stateParameters.push_back(0); //Available Credit
	this->stateParameters.push_back(0); //Cost of Poptarts

	this->setState(Out_Of_Poptarts);
	if (inventory_count > 0)
	{
		this->addPoptart(inventory_count);
	}
}

Poptart_Dispenser::~Poptart_Dispenser()
{
	if (!this->itemRetrived)
	{
		delete this->DispensedItem;
	}
}

bool Poptart_Dispenser::insertMoney(int money)
{
	currentPoptartState = (PoptartState*)this->CurrentState;
	return this->currentPoptartState->insertMoney(money);
}

bool Poptart_Dispenser::makeSelection(int option)
{
	currentPoptartState = (PoptartState*)this->CurrentState;
	return this->currentPoptartState->makeSelection(option);
}


bool Poptart_Dispenser::moneyRejected()
{
	currentPoptartState = (PoptartState*)this->CurrentState;
	return this->currentPoptartState->moneyRejected();
}

bool Poptart_Dispenser::addPoptart(int number)
{
	currentPoptartState = (PoptartState*)this->CurrentState;
	return this->currentPoptartState->addPoptart(number);
}

bool Poptart_Dispenser::dispense()
{
	currentPoptartState = (PoptartState*)this->CurrentState;
	itemDispensed = this->currentPoptartState->dispense();
	return itemDispensed;
}

Product* Poptart_Dispenser::getProduct()
{
	if (this->itemDispensed)
	{
		this->itemDispensed = false;
		this->itemRetrived = true;
		return this->DispensedItem;
	}

	cout << "Item is not dispensed!" << endl;
	return nullptr;
}

void Poptart_Dispenser::setStateParam(stateParameter SP, int value)
{
	this->stateParameters[SP] = value;
}

int Poptart_Dispenser::getStateParam(stateParameter SP)
{
	return this->stateParameters[SP];
}

bool HasCredit::makeSelection(int option)
{
	if (product != nullptr)
	{
		delete product;
	}
	switch (option)
	{
		case Plain:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, 100);
			product = new Base("Plain", this->CurrentContext->getStateParam(Cost_Of_Poptart));
			break;
		case Spicy:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, 150);
			product = new Base("Spicy", this->CurrentContext->getStateParam(Cost_Of_Poptart));
			break;
		case Chocolate:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, 200);
			product = new Base("Chocolate", this->CurrentContext->getStateParam(Cost_Of_Poptart));
			break;
		case Coconut:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, 200);
			product = new Base("Coconut", this->CurrentContext->getStateParam(Cost_Of_Poptart));
			break;
		case Fruity:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, 200);
			product = new Base("Fruity", this->CurrentContext->getStateParam(Cost_Of_Poptart));
			break;
		default:
			cout << "No base with option code: " << option << endl;
			product = nullptr;
			return false;
	}
	int baseCost = product->cost();
	
	switch (option)
	{
		case F_Chocolate:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 20);
			product = new Filling("Chocolate", 20);
		case F_Banana:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 50);
			product = new Filling("Banana", 50);
		case F_Strawberry:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 50);
			product = new Filling("Strawberry", 50);
		case F_Raspberry:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 50);
			product = new Filling("Raspberry", 50);
		case F_Apple:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 50);
			product = new Filling("Apple", 50);
		case F_Blackberry:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 50);
			product = new Filling("Blackberry", 50);
		case F_Maple:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 100);
			product = new Filling("Maple", 100);
		case F_Marshmellow:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 20);
			product = new Filling("Marshmellow", 20);
		case F_Cheese:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 70);
			product = new Filling("Cheese", 70);
		case F_CheeseAndHam:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 100);
			product = new Filling("Cheese and ham", 100);
		case F_Caramel:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 20);
			product = new Filling("Caramel", 20);
		case F_Vanilla:
			this->CurrentContext->setStateParam(Cost_Of_Poptart, baseCost + 50);
			product = new Filling("Vanilla", 50);
	default:
		break;
	}
	
	this->CurrentContext->setState(Dispenses_Poptart);
	return true;
}

bool DispensesPoptart::dispense()
{
	int money = this->CurrentContext->getStateParam(Credit);
	int numberOfPoptarts = this->CurrentContext->getStateParam(No_Of_Poptarts);
	int cost = product->cost();

	if (numberOfPoptarts > 0 && money > 0)
	{
		this->CurrentContext->setState(Has_Credit);
		return false;
	}
	else if (numberOfPoptarts > 0 && money == 0)
	{
		this->CurrentContext->setStateParam(Credit, 0);
		this->CurrentContext->setState(No_Credit);
		return false;
	}
	else if (numberOfPoptarts == 0)
	{
		this->CurrentContext->setStateParam(No_Of_Poptarts, 0);
		this->CurrentContext->setState(Out_Of_Poptarts);
		return false;
	}

	if (money > cost)
	{
		//cout << "Dispensing " << Poptart_Dispenser::DispensedItem->description << endl;
		this->CurrentContext->setStateParam(Credit, money - cost);
		return true;
	}
}

void main()
{
	Poptart_Dispenser pop(1);

	pop.getProduct();

	cin.get();
}



