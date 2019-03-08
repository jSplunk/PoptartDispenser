#include <vector> //only to be used to contain dispenser states and nothing else
#include <iostream>
#include <string>

/*
*
*	Work in progress, not working so far
*
*/

using namespace std;

//Enumerations used for state and state context tracking. Also used for opt-codes for bases and fillings in the Poptart_Dispenser makeSelection function

enum state { Out_Of_Poptarts, No_Credit, Has_Credit, Dispenses_Poptart };
enum stateParameter { No_Of_Poptarts, Credit, Cost_Of_Poptart };
enum bases { Plain = 1, Spicy = 2, Chocolate = 4, Coconut = 8, Fruity = 16 };
enum fillings
{
	F_Chocolate = 32, F_Banana = 64, F_Strawberry = 128,
	F_Raspberry = 256, F_Apple = 512, F_Blackberry = 1024,
	F_Maple = 2048, F_Marshmellow = 4096, F_Cheese = 8192,
	F_CheeseAndHam = 16384, F_Caramel = 32768, F_Vanilla = 65536
};

//Using an abstract State Machine, which allows for the current bidirectional composite relationship between State and StateContext
#pragma region StateMachine-Declaration
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
#pragma endregion StateMachine-Declaration


//Transition is used as an interface for all transistions that will occur during the lifespan of the Poptart_Dispenser
#pragma region Transition-Declaration
class Transition
{
public:

	virtual bool insertMoney(int) { cout << "Error!" << endl; return false; }
	virtual bool makeSelection(int) { cout << "Error!" << endl; return false; }

	virtual bool moneyRejected(void) { cout << "Error!" << endl; return false; }

	virtual bool addPoptart(int) { cout << "Error!" << endl; return false; }

	virtual bool dispense(void) { cout << "Error!" << endl; return false; }

};
#pragma endregion Transition-Declaration

/*
*	Product will act as an interface for bases and filling for the different Poptarts,
*	and treating the product system as non-modifyable code we can apply the Decorator pattern to
*	the existing interface to add-on new bases or fillings
*/

#pragma region Product-Declaration
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

#pragma endregion Product-Declaration

//Base will inherit from product and will act as an interface for all bases furthur created

#pragma region Base-Declaration
class Base : public Product
{
public:
	int cost(void);
	string description(void);
	void consume();
	Product* ReturnHighestCostItem(void)
	{
		return this;
	}
};

//Diffrent base class implementations

class PlainBase : public Base
{
public:
	PlainBase()
	{
		this->itemCost = 50;
		this->product_description = "Plain base";
	}
};

class SpicyBase : public Base
{
public:
	SpicyBase()
	{
		this->itemCost = 50;
		this->product_description = "Spicy base";
	}
};

class ChocolateBase : public Base
{
public:
	ChocolateBase()
	{
		this->itemCost = 50;
		this->product_description = "Chocolate base";
	}
};

class CoconutBase : public Base
{
public:
	CoconutBase()
	{
		this->itemCost = 50;
		this->product_description = "Coconut base";
	}
};

class FruityBase : public Base
{
public:
	FruityBase()
	{
		this->itemCost = 50;
		this->product_description = "Fruity base";
	}
};
#pragma endregion Base-Declaration

//Filling acts as an interface for furthur created filling, adding onto existing base product

#pragma region Filling-Declaration
class Filling : public Product
{
protected:
	Product* base;
public:
	virtual void fillProduct(Product* NewBase);
	virtual int cost(void);
	virtual string description(void);
	void consume();
	Product* ReturnHighestCostItem(void)
	{
		return this->itemCost > base->ReturnHighestCostItem()->cost() ? this : base->ReturnHighestCostItem();
	}
};


//Different filling class implementations

class ChocolateFilling : public Filling
{
public:
	ChocolateFilling()
	{
		this->itemCost = 20;
		this->product_description = "Chocolate filling";
	}
};

class BananaFilling : public Filling
{
public:
	BananaFilling()
	{
		this->itemCost = 50;
		this->product_description = "Banana filling";
	}
};

class StrawberryFilling : public Filling
{
public:
	StrawberryFilling()
	{
		this->itemCost = 50;
		this->product_description = "Strawberry filling";
	}
};

class RaspberryFilling : public Filling
{
public:
	RaspberryFilling()
	{
		this->itemCost = 50;
		this->product_description = "Raspberry filling";
	}
};


class AppleFilling : public Filling
{
public:
	AppleFilling()
	{
		this->itemCost = 50;
		this->product_description = "Apple filling";
	}
};

class BlackberryFilling : public Filling
{
public:
	BlackberryFilling()
	{
		this->itemCost = 50;
		this->product_description = "Blackberry filling";
	}
};

class MapleFilling : public Filling
{
public:
	MapleFilling()
	{
		this->itemCost = 100;
		this->product_description = "Maple filling";
	}
};

class MarshmellowFilling : public Filling
{
public:
	MarshmellowFilling()
	{
		this->itemCost = 20;
		this->product_description = "Marshmellow filling";
	}
};

class CheeseFilling : public Filling
{
public:
	CheeseFilling()
	{
		this->itemCost = 70;
		this->product_description = "Cheese filling";
	}
};

class CheeseAndHamFilling : public Filling
{
public:
	CheeseAndHamFilling()
	{
		this->itemCost = 100;
		this->product_description = "Chocolate filling";
	}
};

class CaramelFilling : public Filling
{
public:
	CaramelFilling()
	{
		this->itemCost = 20;
		this->product_description = "CheeseAndHam filling";
	}
};

class VanillaFilling : public Filling
{
public:
	VanillaFilling()
	{
		this->itemCost = 50;
		this->product_description = "Vanilla filling";
	}
};
#pragma endregion Filling-Declaration



#pragma region PoptartStates-Declaration
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

class NoCredit : public PoptartState
{
public:
	NoCredit(StateContext* context) : PoptartState(context)
	{}
	bool insertMoney(int);
};

class HasCredit : public PoptartState
{
protected:
	Product* product = nullptr;
	Filling* fill = nullptr;
public:
	HasCredit(StateContext* context) : PoptartState(context)
	{}
	bool makeSelection(int);
	bool moneyRejected();
	bool insertMoney(int);
};

class DispensesPoptart : public PoptartState
{
protected:
	Product* product = nullptr;
public:
	DispensesPoptart(StateContext* context) : PoptartState(context)
	{}
	bool dispense(void);
};
#pragma endregion PoptartStates-Declaration


#pragma region PoptartDispenser-Declaration
class Poptart_Dispenser : public StateContext, public Transition
{
	friend class HasCredit;
	friend class DispensesPoptart;
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
#pragma endregion PoptartDispenser-Declaration

#pragma region Base-Implementation
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
	cout << "Consuming " << product_description << endl;
}
#pragma endregion Base-Implementation

#pragma region Filling-Implementation
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
	return base->description() + ", " + this->product_description;
}

void Filling::consume()
{
	cout << "Consuming " << description() << endl;
}
#pragma endregion Filling-Implementation


#pragma region PoptartStates-Implementation
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

bool NoCredit::insertMoney(int money)
{
	cout << "You inserted: " << money << endl;
	this->CurrentContext->setStateParam(Credit, money);
	cout << " Total: " << money << endl;
	this->CurrentContext->setState(Has_Credit);
	return true;
}

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

bool HasCredit::makeSelection(int option)
{

	unsigned int baseOption = option;

	baseOption = Plain | Spicy | Chocolate | Coconut | Fruity | F_Chocolate
		| F_Banana | F_Strawberry | F_Raspberry | F_Apple | F_Blackberry | F_Maple
		| F_Marshmellow | F_Cheese | F_CheeseAndHam | F_Caramel | F_Vanilla;

	baseOption |= option;

	if ((option & Plain) == Plain)
	{
		product = new PlainBase();
		this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
		option -= Plain;
	}
	else if ((option & Spicy) == Spicy)
	{
		product = new SpicyBase();
		this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
		option -= Spicy;
	}
	else if ((option & Chocolate) == Chocolate)
	{
		product = new ChocolateBase();
		this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
		option -= Chocolate;
	}
	else if ((option & Coconut) == Coconut)
	{
		product = new CoconutBase();
		this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
		option -= Coconut;
	}
	else if ((option & Fruity) == Fruity)
	{
		product = new FruityBase();
		this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
		option -= Fruity;
	}
	else
	{
		cout << "You can't have multiple bases!" << endl;
		return false;
	}

	cout << "Selected " << ((Base*)(product))->description() << endl;

	while (option)
	{
		if ((option & F_Chocolate) == F_Chocolate)
		{
			fill = new ChocolateFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Chocolate;
		}
		else if ((option & F_Banana) == F_Banana)
		{
			fill = new BananaFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Banana;
		}
		else if ((option & F_Strawberry) == F_Strawberry)
		{
			fill = new StrawberryFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Strawberry;
		}
		else if ((option & F_Raspberry) == F_Raspberry)
		{
			fill = new RaspberryFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Raspberry;
		}
		else if ((option & F_Apple) == F_Apple)
		{
			fill = new AppleFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Apple;
		}
		else if ((option & F_Blackberry) == F_Blackberry)
		{
			fill = new BlackberryFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Blackberry;
		}
		else if ((option & F_Maple) == F_Maple)
		{
			fill = new MapleFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Maple;
		}
		else if ((option & F_Marshmellow) == F_Marshmellow)
		{
			fill = new MarshmellowFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Marshmellow;
		}
		else if ((option & F_Cheese) == F_Cheese)
		{
			fill = new CheeseFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Cheese;
		}
		else if ((option & F_CheeseAndHam) == F_CheeseAndHam)
		{
			fill = new CheeseAndHamFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_CheeseAndHam;
		}
		else if ((option & F_Caramel) == F_Caramel)
		{
			fill = new CaramelFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Caramel;
		}
		else if ((option & F_Vanilla) == F_Vanilla)
		{
			fill = new VanillaFilling();
			fill->fillProduct(product);
			product = fill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
			baseOption &= ~option;
			option -= F_Vanilla;
		}
	}
	if (fill != nullptr) cout << "Selected " << ((Filling*)(product))->description() << endl;
	((Poptart_Dispenser*)(CurrentContext))->DispensedItem = product;
	this->CurrentContext->setState(Dispenses_Poptart);
	return true;
}

bool DispensesPoptart::dispense()
{
	int money = this->CurrentContext->getStateParam(Credit);
	int numberOfPoptarts = this->CurrentContext->getStateParam(No_Of_Poptarts);
	int cost = ((Poptart_Dispenser*)(CurrentContext))->DispensedItem->cost();

	if (numberOfPoptarts > 0 && money > 0)
	{
		((Poptart_Dispenser*)(CurrentContext))->itemDispensed = true;
		cout << "Dispensing " << (((Poptart_Dispenser*)(CurrentContext))->DispensedItem)->description() << endl;
		this->CurrentContext->setStateParam(Credit, money - cost);
		this->CurrentContext->setStateParam(No_Of_Poptarts, numberOfPoptarts - 1);
		this->CurrentContext->setState(Has_Credit);
		return true;
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
}

#pragma endregion PoptartStates-Implementation

#pragma region PoptartDispenser-Implementation
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
	return this->currentPoptartState->dispense();
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
#pragma region PoptartDispenser-Implementation




void main()
{
	Poptart_Dispenser pop(1);

	pop.insertMoney(200);
	pop.insertMoney(200);


	pop.makeSelection(F_Chocolate + F_Cheese + F_Apple + F_Banana + Fruity);

	pop.dispense();

	Product* p = pop.getProduct();

	p->consume();
	Product* HighCost = p->ReturnHighestCostItem();

	cout << HighCost->description() << endl;

	cin.get();
}



