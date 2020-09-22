#pragma once

#include "Model.h"
#include "AssetEnums.h"

enum MACHINES {
	MACHINE_NONE,
	MACHINE_BOILER,
	MACHINE_PIPE,
	MAX_MACHINES
};

class Machine {
public:

	Machine(MACHINES); 

	MACHINES getType() { return type; }
	Model& getModel() { return model; }
	std::string getName() { return getName(type); }
	std::string getDescription() { return getDescription(type); }
	long long getPrice() { return getPrice(type); }

	static std::string getName(MACHINES);
	static std::string getDescription(MACHINES);
	static MODELS getModel(MACHINES);
	static long long getPrice(MACHINES);

private:

	MACHINES type;
	Model model;

	static const std::string MACHINE_NAMES[MAX_MACHINES];
	static const std::string MACHINE_DESC[MAX_MACHINES];
	static const MODELS MACHINE_MODELS[MAX_MACHINES];
	static const long long MACHINE_PRICES[MAX_MACHINES];

};