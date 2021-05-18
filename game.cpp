#include "game.h" //.h file (to get declarations)
#include <vector>
#include <array>
#include <iostream>
#include <algorithm> //access overloaded function to erase vector values
#include <iterator>
#include <string>
#include <map>
#include <string>
#include <sstream> //to add variables to strings
using namespace std ;

/* This is where we will implement the functions / variables from our .h file 
 * This will be one of our .cpp files (our 'implement' file)
 * We will have to compile both .cpp files simultaneously to run the program */

//Being

int Being::getX() 
{
	return x ;
}

int Being::getY() 
{
	return y ;
}

int Being::getHealth()
{
	return health ;
}

string Being::getName()
{
	return name ;
}

string Being::getDescription()
{
	return description ;
}

//Stonemason

vector<int> Stonemason::weapons = {1,2,3,4} ;
map<string, int> Stonemason::mapWeaponsToPrice = {{"stone-sword",2}, {"iron-sword",4}, {"gold-sword",6}, {"diamond-sword",7}} ;
map<int, int> Stonemason::mapWeaponsIDToPrice = {{1,2}, {2,4}, {3, 6}, {4,7}} ;

Stonemason::Stonemason(int inputted_x, int inputted_y, string inputted_name, string inputted_description)
{
	this->x=inputted_x ; 
	this->y=inputted_x ;
	health = -1 ;
	this->name=inputted_name ; 
	this->description=inputted_description ;
	weapons = {1,2,3,4} ;
	vector<string>weaponsName = {"stone-sword", "iron-sword", "gold-sword", "diamond-sword"} ;
	vector<int>price = {2, 4, 6, 7} ;
	
	for(auto it = begin(weapons) ; it != end(weapons) ; it = next(it))
	{
		for(auto ti = begin(price) ; ti != end(price) ; ti = next(ti))
		{
			mapWeaponsIDToPrice.insert(pair<int,int>(*it,*ti));
			it = next(it) ;
		}
		break ;
	}
	
	for(auto ti = begin(weaponsName) ; ti != end(weaponsName) ; ti = next(ti))
	{
		for(auto it = begin(weapons) ; it != end(weapons) ; it = next(it))
		{
			mapWeaponsToPrice.insert(pair<string,int>(*ti, *it));
			it = next(it) ;
		}
		break ;
	}	
}

void Stonemason::changeHealth(int valueToDecreaseBy)
{
	health -= valueToDecreaseBy ;
}

void Stonemason::examineInventory()
{
	cout << "The following items are for sale: " << endl ;
	for(vector<int>::iterator it = begin(weapons); it != end(weapons); it = next(it)) 
	{
		if(it == prev(end(weapons))) { cout <<"\t" << Item::getNameFromMap(*it) << ", costing " << Stonemason::getPriceFromInt(*it) << " jewels." << endl ;}
		else {cout <<"\t" << Item::getNameFromMap(*it) << ", costing " << Stonemason::getPriceFromInt(*it) << " jewels," << endl ;}
	}
}

int Stonemason::getPriceFromName(string weaponName)
{
	return mapWeaponsToPrice[weaponName] ;
}

int Stonemason::getPriceFromInt(int weaponID)
{
	return mapWeaponsIDToPrice[weaponID] ;
}

int Stonemason::sell(string weaponChoice, int playerJewelCount)
{
	if(playerJewelCount==0) {cout << "You're broke, you don't even have a single jewel!" << endl ; return -1 ; }
	
	int weaponChoiceInt = Item::getIntFromMap(weaponChoice) ; //get desired weapon's ID
	for(vector<int>::iterator it = begin(weapons); it != end(weapons); it = next(it)) 
	{
		if(*it == weaponChoiceInt) 
		{
			if(playerJewelCount >= getPriceFromInt(*it)) 
			{
				if(Player::isItemInInventory(weaponChoiceInt))
				{
					cout << "We will replenish the sword you currently have, as it is the same type" << endl ;
					return 100 ;
				}
				cout << "You have the funds, I'll go make you the tool..." << endl ;
				cout << "(hint, check your inventory)" << endl ;
				Player::changeJewels(-getPriceFromInt(*it)) ;
				return weaponChoiceInt ;
			}
			else
			{
				cout << "You can't afford that!" << endl ;
				cout << "(it costs " << getPriceFromInt(*it) << " & you only have " << Player::getJewels() << ")" << endl ;
				return -1 ;
			}
		}
	}
	cout << "I don't even make that item!" << endl ;
	return -1 ;
}

//Monster

Monster::Monster(int inputted_x, int inputted_y, int inputted_health, string inputted_name, string inputted_description, int inputted_power, int inputted_itemToInventory)
{
	this->x=inputted_x ;
	this->y=inputted_y ;
	this->health=inputted_health ;
	this->name=inputted_name ; 
	this->description=inputted_description ;
	this->power=inputted_power ;
	this->inventory[0]=inputted_itemToInventory ;
}

int Monster::getPower()
{
	return power ;
}

void Monster::changeHealth(int weaponDamage)
{
	health-=weaponDamage ;
}

void Monster::setHealth(int inputted_health) 
{
	health=inputted_health ;
}

int Monster::dropItem()
{
	return inventory[0] ;
}

//Player

vector<int> Player::inventory = {} ;
vector<string> Player::commands = {} ;
vector<string> Player::commandDescriptions = {} ;
int Player::weaponEquipped[1] ;
int Player::jewels = 0 ;

Player::Player(int inputted_x, int inputted_y, int inputted_health, string inputted_name, string inputted_description)
{
	x=inputted_x ;
	y=inputted_y ;
	health=inputted_health ;
	name=inputted_name ; 
	description=inputted_description ;
	inventory = {0} ;
	commands = {"help","move", "examine", "quit", "clear", "buy", "equip", "fight", "use", "get"} ;
	commandDescriptions = {"Player can see availible instructions (as you are now!).\nExample: help",
						   "Player can move in a given direction.\nExample: move west",
						   "Player can examine their inventory, position on the map, jewel count, health, the contents of a room (such as to see if there's monsters within) or an individual item.\nExample: examine inventory, examine position, examine room, examine health, examine jewels, examine <item-name-here>",
						   "Player can quit game.\nExample: quit",
						   "Player can clear screen.\nExample: clear",
						   "Player can purchase weapon.\nExample buy stone-sword",
						   "Player can equip themselves with a select weapon to fight monster.\nExample: equip stone-sword",
						   "Player can fight an enemy in his room (should there be one!).\nExample: fight",
						   "Player can use select items to regenerate their health in respective mannerisms.\nExample: use apple",
						   "Player can store select items within a room into their inventory.\nExample: get item"} ;
	jewels = 5 ;
}

void Player::equipWeapon(int weapon)
{
	for(auto it = begin(inventory) ; it != end(inventory) ; it = next(it))
	{
		if(*it == weapon) {weaponEquipped[0] = weapon ;	cout << "Item equipped!" << endl ;}
	}
	if(weaponEquipped[0] != weapon) {cout << "You can't equip yourself with a weapon you don't have!" << endl ;}
}

void Player::seeweaponEquipped()
{
	if(weaponEquipped[0] == 2) {cout << "You are currently equipped with an " << weaponEquipped[0] << endl ;}
	else {cout << "You are currently equipped with a " << weaponEquipped[0] << endl ;}
}

int Player::getEquippedItem()
{
	return weaponEquipped[0] ;
}

vector<string> Player::getCommands()
{
	return commands ;
}

void Player::setLocation(int inputted_x, int inputted_y)
{
	this->x=inputted_x ;
	this->y=inputted_y ;
}

void Player::setX(int inputted_x)
{
	this->x=inputted_x ;
}

void Player::setY(int inputted_y)
{
	this->y=inputted_y ;
}

void Player::changeHealth(int valueToChange)
{
	health += valueToChange ;
}

int Player::getJewels()
{
	return jewels ;
}

void Player::changeJewels(int valueToChange)
{
	jewels += valueToChange ;
}

void Player::addToInventory(int itemID)
{
	inventory.push_back(itemID) ;
}

void Player::examinePosition()
{
	if(x==1 && y == 1)
	{
		cout << "To your north is Dungeon #1,\nTo your east is Dungeon #4,\nTo your south is Dungeon #3,\nTo your west is Dungeon #2" << endl ;
	}
	if(x==1 && y == 2)
	{
		cout << "To your north is Dungeon #5,\nTo your south is the shop" << endl ;
	}
	if(x==1 && y == 3)
	{
		cout << "To your north is the exit,\nTo your south is Dungeon #1" << endl ;
	}
	if(x==1 && y == 4)
	{
		cout << "To your south is Dungeon #5" << endl ;
	}
	if(x==1 && y == 0)
	{
		cout << "To your north is the Shop" << endl ;
	}
	if(x==0 && y == 1)
	{
		cout << "To your east is the Shop" << endl ;
	}
	if(x==2 && y == 1)
	{
		cout << "To your west is the Shop" << endl ;
	}
}

void Player::help() 
{
	cout << "Valid commands: "<< endl;
	for(vector<string>::iterator it = begin(commands); it != end(commands); it = next(it)) 
	{
		for(vector<string>::iterator itz = begin(commandDescriptions); itz != end(commandDescriptions); itz = next(itz)) 
		{
			cout << *it << " - " << *itz << endl ;
			cout << endl ;
			it = next(it) ;
		}
		break ;
	}
}

bool Player::isItemInInventory(int itemID)
{
	for(vector<int>::iterator it = begin(inventory); it != end(inventory); it = next(it)) 
	{
		if(*it == itemID) {return true ;}
	}
	return false ;
}

void Player::examineInventory()
{
	if(inventory.empty()) {cout << "You have no items in your inventory!" << endl;}
	else {
		cout << "The following items are in your inventory: ";
		for(vector<int>::iterator it = begin(inventory); it != end(inventory); it = next(it)) 
		{
			if(it == prev(end(inventory))) { cout << Item::getNameFromMap(*it) << endl ;}
			else {cout << Item::getNameFromMap(*it) << ", " ;}
		}
	}
}

void Player::seeHealth()
{
	cout << "Current health: " << health << endl ;
}

void Player::removeItems(int itemID)
{
	inventory.erase(remove(inventory.begin(), inventory.end(), itemID), inventory.end());
}

//Item

map<int, string> Item::mapIntToName = {{0, "apple"},{1, "stone-sword"}, {2, "iron-sword"}, {3, "gold-sword"}, {4, "diamond-sword"},{10, "banana"}, {11, "pear"}} ;
map<string, int> Item::mapNameToInt = {{"apple", 0},{"stone-sword", 1},{"iron-sword", 2},{"gold-sword", 3},{"diamond-sword", 4},{"banana", 10}, {"pear", 11}} ;
map<string, string> Item::mapNameToDescription = {{"apple", "Eating this will boost your health by 4"}, {"stone-sword", "Most basic sword"}, {"iron-sword", "More durable than the stone sword"}, {"gold-sword", "Same durability as the stone sword but packs a bigger punch"}, {"diamond-sword", "Like the gold sword, but durable! The ultimate sword"}, {"banana", "Regenerates your health by 6!"}, {"pear", "Eating this regenerates your healthy by 8!"}} ;

void Item::setMaps(vector<int> ids, vector<string> names, vector<string> descriptions)
{
	mapIntToName = {{0, "apple"},{1, "stone-sword"}, {2, "iron-sword"}, {3, "gold-sword"}, {4, "diamond-sword"},{10, "banana"},{11, "pear"}} ;
	mapNameToInt = {{"apple", 0},{"stone-sword", 1},{"iron-sword", 2},{"gold-sword", 3},{"diamond-sword", 4}, {"banana", 10}, {"pear", 11}} ;
	mapNameToDescription = {{"apple", "Eating this will boost your health by 4"}, {"stone-sword", "Most basic sword"}, {"iron-sword", "More durable than the stone sword"}, {"gold-sword", "Same durability as the stone sword but packs a bigger punch"}, {"diamond-sword", "Like the gold sword, but durable! The ultimate sword"}, {"banana", "Regenerates your health by 6!"}, {"pear", "Eating this regenerates your healthy by 8!"}} ;
}

string Item::getNameFromMap(int itemID)
{
	return mapIntToName[itemID] ;
}

int Item::getIntFromMap(string itemName)
{
	return mapNameToInt[itemName] ;
}

string Item::getDescriptionFromMap(string itemName)
{
	return mapNameToDescription[itemName] ;
}

string Item::getName()
{
	return name ;
}

string Item::getDescription()
{
	return description ;
}

//Weapon

Weapon::Weapon(int inputted_x, int inputted_y, string inputted_name, string inputted_description, int inputted_durability, int inputted_power)
{
	this->x=inputted_x ;
	this->y=inputted_y ;
	this->name=inputted_name ;
	this->description=inputted_description ;
	this->durability=inputted_durability ;
	this->power=inputted_power ;
}

int Weapon::getPower()
{
	return power ;
}

int Weapon::getDurability()
{
	return durability ;
}

void Weapon::decreaseDurability(int valueToChange) 
{
	durability -= valueToChange ;
}

void Weapon::setDurability(int valueToChange) 
{
	durability = valueToChange ;
}


//Jewel

map<string, int> mapValueToJewel = {} ;

int Jewel::getValueFromJewel(string jewelName)
{
	return mapValueToJewel[jewelName] ;
}

// Room

// void Room::printLocation(int inputted_x, int inputted_y)
// {
// 	if(inputted_x == 1)
// 	{
// 		cout <<"		 ____         " << endl;
// 		cout <<"        |_  _|        " << endl;
// 		cout <<"         _||_         " << endl;
// 		cout <<"        |_  _|        " << endl;
// 		cout <<"         _||_         " << endl;
// 		cout <<"        |_  _|        " << endl;
// 		cout <<"  ____   _||_   ____  " << endl;
// 		cout <<" |____|=|_  _|=|____| " << endl;
// 		cout <<"         _||_         " << endl;
// 		cout <<"        |____|        " << endl;
// 	}
// }
 
void Room::printDescription()
{
	cout << description << endl ;
}

void Room::printName()
{
	cout << name << endl ;
}

string Room::getName()
{
	return name ;
}

int Room::getX() 
{
	return x;
}

int Room::getY() 
{
	return y;
}

//Battleroom

Battleroom::Battleroom(int inputted_x, int inputted_y, string inputted_name, string inputted_description, string inputted_monster)
{
	this->x=inputted_x ;
	this->y=inputted_y ;
	this->name=inputted_name ;
	this->description=inputted_description ;
	this->monsterWithin=inputted_monster ;
}

void Battleroom::changeDescription() 
{
	description = "The " + monsterWithin + " is dead! You slayed the beast" ;
}

void Battleroom::addItems(int itemID)
{
	items.push_back(itemID) ;
}

void Battleroom::removeItems(int itemID)
{
	if(items.empty()) {cout << "There are no items in the room!" << endl ;}
	else
	{
//		cout << "E0" << endl ;
// 		for(vector<int>::iterator it = begin(items) ; it != end(items) ; it = next(it))
// 		{
// 			cout << "E1" << endl ;
// // 			if(*it == itemID) 
// // 			{
// // 				cout << "E2" << endl ;
// // 				items.erase(remove(items.begin(), items.end(), itemID), items.end());
// // 			}
// // 			
// 		}
		vector<int>::iterator it = std::find(items.begin(), items.end(), itemID);
		if (it != items.end()) { cout << Item::getNameFromMap(itemID) << " is now in your inventory!" << endl ; items.erase(remove(items.begin(), items.end(), itemID), items.end());}
		else {cout << "That item isn't in the room!" << endl ;}
	}
}

void Battleroom::examineRoom()
{
	if(items.empty())
	{
		cout << "There are no items to pick up! " ;
		string temp_str = "The " + monsterWithin + " is dead! You slayed the beast" ;
		if(description == temp_str)
		{
			cout << "The monster is dead! His corpse lays on the floor & nothing else" << endl ; ;
		}
		else { cout << "Maybe kill the " << monsterWithin << " in front of you and you'll get something..." ;}
	}
	else
	{
		cout << "The monster is dead! His corpse lays on the floor, as well as the following items: " ;
		for(vector<int>::iterator it = begin(items); it != end(items); it = next(it)) 
		{
			cout << Item::getNameFromMap(*it) ;
		}
	}
	cout << endl ;
}

string Battleroom::getMonsterWithin()
{
	return monsterWithin ;
}

string Battleroom::getDescription() { return description ; }

//Shop

Shop::Shop(int inputted_x, int inputted_y, string inputted_name, string inputted_description)
{
	this->x=inputted_x ;
	this->y=inputted_y ;
	this->name=inputted_name ;
	this->description=inputted_description ;
}

void Shop::examineRoom()
{
	Stonemason::examineInventory() ;
}

void Shop::addItems(int itemID)
{
	items.push_back(itemID) ;
}


void Shop::removeItems(int itemID)
{
	if(items.empty()) {cout << "There are no items in the room!" << endl ;}
	else
	{
		for(int i = 0 ; i < items.size() ; i++)
		{
			if(items[i] == itemID) 
			{
				items.erase(remove(items.begin(), items.end(), itemID), items.end());
			}
		}
		cout << "That item isn't in the room!" << endl ;
	}
}

//Exit

Exit::Exit(int inputted_x, int inputted_y, string inputted_name, string inputted_description)
{
	this->x=inputted_x ;
	this->y=inputted_y ;
	this->name=inputted_name ;
	this->description=inputted_description ;
}

void Exit::examineRoom()
{
	cout << "                                                              " << endl ;
	cout << "    Y                                                         " << endl ;
	cout << "  .-^-.                                                       " << endl ;
	cout << " /     \      .- ~ ~ -.       le golden snake                 " << endl ;
	cout << "()     ()    /   _ _   `.                     _ _ _           " << endl ;
	cout << " \_   _/    /  /     \   \                . ~  _ _  ~ .       " << endl ;
	cout << "   | |     /  /       \   \             .' .~       ~-. `.    " << endl ;
	cout << "   | |    /  /         )   )           /  /             `.`.  " << endl ;
	cout << "   \ \_ _/  /         /   /           /  /                `'  " << endl ;
	cout << "    \_ _ _.'         /   /           (  (                     " << endl ;
	cout << "                    /   /             \  \                    " << endl ;
	cout << "                   /   /               \  \                   " << endl ;
	cout << "                  /   /                 )  )                  " << endl ;
	cout << "                 (   (                 /  /                   " << endl ;
	cout << "                  `.  `.             .'  /                    " << endl ;
	cout << "                    `.   ~ - - - - ~   .'                     " << endl ;
	cout << "                       ~ . _ _ _ _ . ~                        " << endl ;
	cout << "                                                              " << endl ;
}

void mapPrinter::whereOnMap(int x, int y)
{
	if (x == 0)
	{
		if (y == 1)
		{
			std::cout <<"        ____        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"\033[31m"<<" ____"<<"\033[0m"<<"   _||_   ____"  << std::endl;
			std::cout <<"\033[31m"<<"|____|"<<"\033[0m"<<"=|_  _|=|____|" << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |____|       " << std::endl;
		}
	}
	else if (x == 1)
	{
		if (y == 0)
		{
			cout <<"        ____        " << endl;
			cout <<"       |_  _|       " << endl;
			cout <<"        _||_        " << endl;
			cout <<"       |_  _|       " << endl;
			cout <<"        _||_        " << endl;
			cout <<"       |_  _|       " << endl;
			cout <<" ____   _||_   ____ " << endl;
			cout <<"|____|=|_  _|=|____|" << endl;
			cout <<"\033[31m"<<"        _"<<"\033[0m"<<"||"<<"\033[31m"<<"_"         << endl;
			cout <<"\033[31m"<<"       |____|"        <<"\033[0m"<< endl;
		}
		else if (y == 1)
		{
			std::cout <<"        ____        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<" ____   "<<"\033[31m"<<"_"<<"\033[0m"<<"||"<<"\033[31m"<<"_"<<"\033[0m"<<"   ____"  << std::endl;
			std::cout <<"|____|="<<"\033[31m"<<"|_  _|"<<"\033[0m"<<"=|____|" << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |____|       " << std::endl;
		}
		else if (y ==2)
		{
			std::cout <<"        ____        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"\033[31m"<<"        _"<<"\033[0m"<<"||"<<"\033[31m"<<"_        " << std::endl;
			std::cout <<"\033[31m"<<"       |_  _|       " <<"\033[0m"<< std::endl;
			std::cout <<" ____   _||_   ____ " << std::endl;
			std::cout <<"|____|=|_  _|=|____|" << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |____|       " << std::endl;
		}
		else if (y == 3)
		{
			std::cout <<"        ____        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"\033[31m"<<"        _"<<"\033[0m"<<"||"<<"\033[31m"<<"_        " << std::endl;
			std::cout <<"\033[31m"<<"       |_  _|       " <<"\033[0m"<< std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<" ____   _||_   ____ " << std::endl;
			std::cout <<"|____|=|_  _|=|____|" << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |____|       " << std::endl;
		}
		else if (y == 4)
		{
			std::cout <<"\033[31m"<<"        ____        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"\033[0m"<<"        _||_        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<" ____   _||_   ____ " << std::endl;
			std::cout <<"|____|=|_  _|=|____|" << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |____|       " << std::endl;
		}
	}
	else if (x == 2)
	{
		if (y == 1)
		{
			std::cout <<"        ____        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |_  _|       " << std::endl;
			std::cout <<" ____   _||_   "<<"\033[31m"<<"____"  <<"\033[0m"<< std::endl;
			std::cout <<"|____|=|_  _|="<<"\033[31m"<<"|____|" <<"\033[0m"<<std::endl;
			std::cout <<"        _||_        " << std::endl;
			std::cout <<"       |____|       " << std::endl; 
		}
	}
	else 
	{
		std::cout <<"        ____        " << std::endl;
		std::cout <<"       |_  _|       " << std::endl;
		std::cout <<"        _||_        " << std::endl;
		std::cout <<"       |_  _|       " << std::endl;
		std::cout <<"        _||_        " << std::endl;
		std::cout <<"       |_  _|       " << std::endl;
		std::cout <<" ____   _||_   ____ " << std::endl;
		std::cout <<"|____|=|_  _|=|____|" << std::endl;
		std::cout <<"        _||_        " << std::endl;
		std::cout <<"       |____|       " << std::endl;
	}
}


