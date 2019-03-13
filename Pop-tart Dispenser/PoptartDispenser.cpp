#include <vector> //only to be used to contain dispenser states and nothing else
#include <iostream>
#include <string>

using namespace std;

//Enumerations used for state and state context tracking. Also used for opt-codes for bases and fillings in the Poptart_Dispenser makeSelection function

enum state { Out_Of_Poptarts, No_Credit, Has_Credit, Dispenses_Poptart };
enum stateParameter { No_Of_Poptarts, Credit, Cost_Of_Poptart };
enum bases { Plain = 0x00001, Spicy = 0x00002, Chocolate = 0x00004, Coconut = 0x00008, Fruity = 0x00010 };
enum fillings
{
	F_Chocolate = 0x00020, F_Banana = 0x00040, F_Strawberry = 0x00080,
	F_Raspberry = 0x00100, F_Apple = 0x00200, F_Blackberry = 0x00400,
	F_Maple = 0x00800, F_Marshmellow = 0x01000, F_Cheese = 0x02000,
	F_CheeseAndHam = 0x04000, F_Caramel = 0x08000, F_Vanilla = 0x10000
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

	virtual bool insertMoney(int) { cout << "Error!" << endl; return false; } //Default option for the insertMoney function, in the Transistion interface
	virtual bool makeSelection(int) { cout << "Error!" << endl; return false; } //Default option for the makeSelection function, in the Transistion interface

	virtual bool moneyRejected(void) { cout << "Error!" << endl; return false; } //Default option for the moneyRejected function, in the Transistion interface

	virtual bool addPoptart(int) { cout << "Error!" << endl; return false; } //Default option for the addPoptart function, in the Transistion interface

	virtual bool dispense(void) { cout << "Error!" << endl; return false; } //Default option for the dispense function, in the Transistion interface

};
#pragma endregion Transition-Declaration

/*
*	Product will act as an polymorphic interface for bases and filling for the different Poptarts,
*	and treating the product system as non-modifyable code we can apply the Decorator pattern to
*	the existing interface to add-on new bases or fillings
*/

#pragma region Product-Declaration
class Product
{
protected:
	string product_description; //Description to describe what the product is
	int itemCost = 0; //Net-cost of the product
public:
	virtual void consume(void)
	{

	}

	//Polymorphic getters for the product's attributes
	virtual int cost(void) { return this->itemCost; }
	virtual string description(void) { return product_description; }
	virtual Product* ReturnHighestCostItem(void)
	{
		return nullptr; //No need for implementation in the Product interface, since it will be implemented in the Base and Filling classes
	}
};

#pragma endregion Product-Declaration

//Base inherits from product and will act as an interface for all bases furthur created, while using the Decorator pattern

#pragma region Base-Declaration
class Base : public Product
{
public:
	int cost(void);
	string description(void);
	void consume();
	Product* ReturnHighestCostItem(void)
	{
		return this; //There is only one base, so we can return a 'this' poniter from the Base class
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

//Filling acts as an interface for furthur created filling, adding onto existing base product while using the Decorator pattern

#pragma region Filling-Declaration
class Filling : public Product
{
private:
	Product* base; //Storing a product pointer so we can use fillProduct, and attach a base to the filling
public:
	virtual void fillProduct(Product* NewBase);
	virtual int cost(void);
	virtual string description(void);
	void consume();
	Product* ReturnHighestCostItem(void)
	{
		// Comparing the cost of the current filling compared to the base cost, and returns the highest valued product
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


/*
*
*	The Poptart Dispenser is using different states to determine it's current state in the Poptart Dispenser.
*	The diffrent states have thier own implementations of the transition states, and will act differently
*	depending on which state the Poptart Dispenser is in.
*
*
*	The context has to be continously passed through for it to utilizing all the properties of that class
*/
#pragma region PoptartStates-Declaration

// The poptart state interface, used by the diffrent state classes in the Poptart Dispenser
class PoptartState : public State, public Transition
{
public:
	PoptartState(StateContext* context) : State(context)
	{}
};
 
//Different poptart state classes declarations
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
private:
	Product* product = nullptr; //Used in the makeSelection function to assign the selection of the poptart
public:
	HasCredit(StateContext* context) : PoptartState(context)
	{}
	bool makeSelection(int);
	bool moneyRejected();
	bool insertMoney(int);
};

class DispensesPoptart : public PoptartState
{
public:
	DispensesPoptart(StateContext* context) : PoptartState(context)
	{}
	bool dispense(void);
};
#pragma endregion PoptartStates-Declaration

/*
*
*	Poptart dispenser is the class that will encapsulate all the states and act as the public interface
*	towards the main function.
*
*/
#pragma region PoptartDispenser-Declaration
class Poptart_Dispenser : public StateContext, public Transition
{
	//Using friend classes to be able to access the private product pointer and the private itemDispensed boolean, for assignment in those classes
	friend class HasCredit;
	friend class DispensesPoptart;
private:
	PoptartState* currentPoptartState = nullptr;
	Product* DispensedItem = nullptr;
	bool itemDispensed = false;
	bool itemRetrived = false;
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

//Base class implementations, getters and the consume function (which can be used for consuming the product).
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
	cout << "Consuming " << description() << endl;
}
#pragma endregion Base-Implementation


//Filling class implementations for supplying a new base to the filling, getters for cost and description and consumtion of the product
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


//The out of poptart state class implementation for money rejected, adding poptarts and inserting money
#pragma region PoptartStates-Implementation
bool OutOfPoptarts::moneyRejected(void)
{
	cout << "Ejecting!" << endl;
	this->CurrentContext->setStateParam(Credit, 0);
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

	Product* tmpFill = nullptr;

	while (option)
	{
		if ((option & F_Chocolate) == F_Chocolate)
		{
			tmpFill = new ChocolateFilling;
			baseOption &= ~option;
			option -= F_Chocolate;
		}
		else if ((option & F_Banana) == F_Banana)
		{
			tmpFill = new BananaFilling;
			baseOption &= ~option;
			option -= F_Banana;
		}
		else if ((option & F_Strawberry) == F_Strawberry)
		{
			tmpFill = new StrawberryFilling;
			baseOption &= ~option;
			option -= F_Strawberry;
		}
		else if ((option & F_Raspberry) == F_Raspberry)
		{
			tmpFill = new RaspberryFilling;
			baseOption &= ~option;
			option -= F_Raspberry;
		}
		else if ((option & F_Apple) == F_Apple)
		{
			tmpFill = new AppleFilling;
			baseOption &= ~option;
			option -= F_Apple;
		}
		else if ((option & F_Blackberry) == F_Blackberry)
		{
			tmpFill = new BlackberryFilling;
			baseOption &= ~option;
			option -= F_Blackberry;
		}
		else if ((option & F_Maple) == F_Maple)
		{
			tmpFill = new MapleFilling;
			baseOption &= ~option;
			option -= F_Maple;
		}
		else if ((option & F_Marshmellow) == F_Marshmellow)
		{
			tmpFill = new MarshmellowFilling;
			baseOption &= ~option;
			option -= F_Marshmellow;
		}
		else if ((option & F_Cheese) == F_Cheese)
		{
			tmpFill = new CheeseFilling;
			baseOption &= ~option;
			option -= F_Cheese;
		}
		else if ((option & F_CheeseAndHam) == F_CheeseAndHam)
		{
			tmpFill = new CheeseAndHamFilling;
			baseOption &= ~option;
			option -= F_CheeseAndHam;
		}
		else if ((option & F_Caramel) == F_Caramel)
		{
			tmpFill = new CaramelFilling;
			baseOption &= ~option;
			option -= F_Caramel;
		}
		else if ((option & F_Vanilla) == F_Vanilla)
		{
			tmpFill = new VanillaFilling;
			baseOption &= ~option;
			option -= F_Vanilla;
		}

		if (tmpFill != nullptr)
		{
			((Filling*)(tmpFill))->fillProduct(product);
			product = tmpFill;
			this->CurrentContext->setStateParam(Cost_Of_Poptart, product->cost());
		}
	}
	if (product != nullptr) cout << "Selected " << ((Filling*)(product))->description() << endl;
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



