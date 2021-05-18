#include <vector>
#include <array>
#include <string>
#include <map>
using namespace std ;

class Being {
	protected:
		int x, y ;
		int health ;
		string name, description ;
	public:
		int getX() ; int getY() ;
		string getDescription() ;
		string getName() ;
		int getHealth() ;
		virtual void changeHealth(int) = 0 ;
};

class Stonemason : public Being {
	protected:
		static vector<int>weapons ;
		static map<string, int> mapWeaponsToPrice ;
		static map<int, int> mapWeaponsIDToPrice ;
	public:
		Stonemason(int, int, string, string);
		void changeHealth(int) ;
		void setHealth(int) ;
		static void examineInventory() ;
		static int getPriceFromName(string) ;
		static int getPriceFromInt(int) ;
		int sell(string, int) ; //confirm & place weapon in inventory is exchange for jewels
};

class Monster : public Being {
	protected:
		int power ; //will affect person differently
		int inventory[1] ; //item will either be a jewel or a potion to boost health, item will be dropped
	public:
		Monster(int, int, int, string, string, int, int) ;
		int getPower() ; //returns the damage they inflict per strike
		void changeHealth(int) ; //based on the weapon's power, the health they lose will differ
		int dropItem() ;
		void setHealth(int);
};

class Player : public Being {	
	protected:
		static vector<int>inventory ;
		static int weaponEquipped[1] ;
		static vector<string> commands ;
		static vector<string> commandDescriptions ;
		static int jewels ;
	public:
		Player(int, int, int, string, string) ;
		void equipWeapon(int) ;
		void examinePosition() ;
		void removeItems(int) ;
		void seeweaponEquipped() ;
		vector<string> getCommands() ;
		int getEquippedItem() ;
		void setLocation(int, int) ;
		void setX(int) ; void setY(int) ;
		void changeHealth(int); //based off monster, will take away players health differently
		static void addToInventory(int) ;
		static bool isItemInInventory(int) ;
		static void help() ;
		void seeHealth() ;
		static void examineInventory() ;
		static void changeJewels(int) ;
		static int getJewels() ;
};

class Item {
	protected:
		int x, y ; //placed in rooms
		string name, description ; 
		static map<int, string> mapIntToName ;
		static map<string, int> mapNameToInt ;
		static map<string, string> mapNameToDescription ;
	public:
		void setMaps(vector<int>, vector<string>, vector<string>) ;
		static string getNameFromMap(int) ;
		static int getIntFromMap(string) ;
		static string getDescriptionFromMap(string) ;
		string getName() ;
		string getDescription() ;
};

class Weapon : public Item {
	protected:
		int durability ; //how many strikes the weapon can be used for
		int power ; //the damage the weapon has per strike
	public:
		Weapon(int, int, string, string, int, int) ;
		int getPower() ; 
		void setDurability(int) ; 
		int getDurability() ;
		void decreaseDurability(int) ;
};

class Jewel : public Item {
	protected:
		static map<string, int> mapJewelToValue ;
	public:
		static int getValueFromJewel(string) ;
};

//Complete rooms & it's child classes
//such as battle rooms (where monsters are placed) 
//or stores (where stonemasons operate)

class Room {
	protected:
		int x, y ;
		string name, description ;
	public:
		string getName() ;
		int getX() ; int getY() ;
		virtual void examineRoom()=0 ; //
		void printName() ;
		void printDescription() ;
		//void printLocation(int, int) ;
};

class Battleroom : public Room {
	protected:
		string monsterWithin ; 
		vector<int> items ; //dropped when monster dies
	public:
		Battleroom(int, int, string, string, string) ;
		void changeDescription() ;
		string getMonsterWithin() ;
		void addItems(int) ;
		void removeItems(int) ;
		void examineRoom() ; 
		string getDescription() ;
};

class Shop : public Room {
	protected:
		vector<int> items ;
	public:
		Shop(int, int, string, string) ;
		void removeItems(int) ;
		void addItems(int) ;
		void examineRoom() ; //make call to shopkeeper's examineInventory() 
};

class Exit : public Room {
	public:
		Exit(int, int, string, string) ;
		void examineRoom() ;
};

class mapPrinter {
	public:
		void whereOnMap(int, int) ;
};
