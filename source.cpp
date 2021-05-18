#include "game.h" //.h file (to get ability to make objects)
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <thread>
#include <vector>
#include <string>
using namespace std ;

/* This is our 'client file'. It will be used to create an instance of the class'
 * It will also determine what, how and when we use it 
 * To run in terminal, type 'c++ game.cpp source.cpp' */

string returnCommand()
{ 
	string value ;
	cout << "< Command: " ; cin >> value ;
	cout << endl ;
	return value ;
}

string returnData()
{ 
	string value ;
	cout << "< data: " ; cin >> value ;
	cout << endl ;
	return value ;
}

void examineRoom(Player player, Shop shop, vector<Battleroom> dungeons, Exit exit)
{
	int x = player.getX() ; int y = player.getY() ;
	
	if(x == 0) {dungeons[1].examineRoom() ;}
	else if(x == 1)
	{
		switch(y)
		{
			case 0 : dungeons[2].examineRoom() ; break ;
			case 1 : shop.examineRoom() ; break ;
			case 2 : dungeons[0].examineRoom() ; break ;
			case 3 : dungeons[4].examineRoom() ; break ;
			case 4 : exit.examineRoom() ; break ;
		}
	}
	else if(x == 2 && y == 1) {dungeons[3].examineRoom() ;}
}

void examineName(Player player, Shop shop, vector<Battleroom> dungeons, Exit exit)
{
	int x = player.getX() ; int y = player.getY() ;
	cout << "You are in " ;
	
	if(x == 0) {cout << dungeons[1].getName() ;}
	else if(x == 1)
	{
		switch(y)
		{
			case 0 : cout << dungeons[2].getName() ; break ;
			case 1 : cout << shop.getName() ; break ;
			case 2 : cout << dungeons[0].getName() ; break ;
			case 3 : cout << dungeons[4].getName() ; break ;
			case 4 : cout << exit.getName() ; break ;
		}
	}
	else if(x == 2 && y == 1) {cout << dungeons[3].getName() ;}
	cout << endl ;
}

void examinePosition(Player player, mapPrinter map)
{
	map.whereOnMap(player.getX(), player.getY()) ;
	cout << endl ;
	player.examinePosition() ;
}

void examineItem(Player player, string data, Item item, vector<Weapon> swords)
{
	int fake_x = player.getX() ; int fake_y = player.getY() ;
	if(item.getDescriptionFromMap(data) != "" && item.getDescriptionFromMap(data) != " ")
	{
		for(int i = 0 ; i < swords.size() ; i+=1)
		{
			if(swords[i].getName() == data)
			{
				if(fake_x == 1 && fake_y == 1 && player.isItemInInventory(item.getIntFromMap(data)))
				{
					cout << "Would you like to examine your copy of the sword or the store version?" << endl ;
					cout << "(type mine or store)" << endl ;
					string command = returnCommand() ;
					if(command == "store") 
					{
						if(data == "stone-sword" || data == "gold-sword") { cout << "Durability: 4" << endl; }
						else if(data == "iron-sword" || data == "diamond-sword") { cout << "Durability: 6" << endl; }
					}
					else {cout << item.getDescriptionFromMap(data) << endl ;
						cout << "Durability: " << swords[i].getDurability() << endl ;}
				}
				else { cout << item.getDescriptionFromMap(data) << endl ;
					cout << "Durability: " << swords[i].getDurability() << endl ;}
				cout << "Power: " << swords[i].getPower() << endl ;
			}
		}
	}
	else { cout << "That isn't a valid item" << endl ;}
}

bool validateRoom(Player player, vector<Shop> shop, vector<Battleroom> dungeons, vector<Exit> exit, string direction)
{
	int fake_x = player.getX() ; int fake_y = player.getY() ;
	
	if(direction == "north") {fake_y  += 1 ;}
	else if(direction == "south") {fake_y  -= 1 ;}
	else if(direction == "east")  {fake_x  += 1 ;}
	else if(direction == "west")  {fake_x  -= 1 ;}
	else {return false ;}
	
	for(auto it = begin(shop); it != shop.end() ; it = next(it))
	{
		if(it->getX() == fake_x && it->getY() == fake_y) {return true;}
	}
	
	for(auto it = begin(dungeons); it != dungeons.end() ; it = next(it))
	{
		if(it->getX() == fake_x && it->getY() == fake_y) {return true;}
	}
	
	for(auto it = begin(exit); it != exit.end() ; it = next(it))
	{
		if(it->getX() == fake_x && it->getY() == fake_y) {return true;}
	}
	
	return false ;
}

bool validatePrecense(Player player, vector<Shop> shop, vector <Battleroom> dungeons, vector <Exit> exit, string direction)
{
	int fake_x = player.getX() ; int fake_y = player.getY() ;

	//get the room to be desired's location (tick)
	if(direction == "north") {fake_y  += 1 ;}
	if(direction == "south") {fake_y  -= 1 ;}
	if(direction == "east")  {fake_x  += 1 ;}
	if(direction == "west")  {fake_x  -= 1 ;}
	
	//a bunch of if statements determining if it's predeccesor has been defeated
	if((shop[0].getX() == fake_x) && (shop[0].getY() == fake_y)) {return true ;}
	if((dungeons[0].getX() == fake_x) && (dungeons[0].getY() == fake_y)) {return true ;}
	if((dungeons[1].getX() == fake_x) && (dungeons[1].getY() == fake_y)) 
	{
		if(dungeons[0].getDescription() == "Within lies a Mischevous Goblin") {return false ;}
		else {return true ;}
	}
	if((dungeons[2].getX() == fake_x) && (dungeons[2].getY() == fake_y)) 
	{
		if(dungeons[1].getDescription() == "Within lies an Achromantula") {return false ;}
		else {return true ;}
	}
	if((dungeons[3].getX() == fake_x) && (dungeons[3].getY() == fake_y)) 
	{
		if(dungeons[2].getDescription() == "Within lies a Wearwolf") {return false ;}
		else {return true ;}
	}
	if((dungeons[4].getX() == fake_x) && (dungeons[4].getY() == fake_y)) 
	{
		if(dungeons[3].getDescription() == "Within lies a Dementor") {return false ;}
		else {return true ;}
	}
	if((exit[0].getX() == fake_x) && (exit[0].getY() == fake_y)) 
	{
		if(dungeons[4].getDescription() == "Within lies a Basilisk") {return false ;}
		else {return true ;}
	}
}

bool isCommandValid(vector<string> commands, string command)
{
	for(auto it = begin(commands) ; it != end(commands) ; it = next(it))
	{
		if(command == *it) {return true ;}
	}
	return false;
}

bool gameWon(Player player)
{
	if(player.getX() == 1 && player.getY() == 4) {return true ;}
	return false ;
}

vector <int> fightingScenario(Player player, Battleroom room, Monster monsters, Weapon weapon)
{
	int playerHealth = player.getHealth() ;
	int playerPower = weapon.getPower() ;
	int durability = weapon.getDurability() ;
	
	int monsterPower = monsters.getPower() ;
	int monsterHealth = monsters.getHealth() ;
	
	cout << "Current " << monsters.getName() << " health: " << monsterHealth << endl ;
	cout << "Current player health: " << playerHealth << endl ;
	cout << "Current player's weapon durability: " << durability << endl ;
	cout << endl ;
	
	while(playerHealth > 0 && monsterHealth > 0)
	{
		cout << "Player strikes " << monsters.getName() << endl ;
		cout << "Current player's weapon durability: " << durability << endl ;
		cout << endl ;
		monsters.changeHealth(playerPower) ;
		monsterHealth = monsters.getHealth() ;
		cout << "Current " << monsters.getName() << "'s health: " << monsterHealth << endl ;
		
		if(playerHealth > 0 && monsterHealth > 0)
		{
			cout << monsters.getName() << " strikes Player" << endl ;
			player.changeHealth(-monsterPower) ;
			durability -= 1 ;
			playerHealth = player.getHealth() ;
			cout << "Current player health: " << playerHealth << endl ;
			sleep(2); 
			if(durability == 0) {playerPower = 0 ;}
		cout << endl ;
		}
	}
	if(playerHealth <= 0) {cout << "You died! Better luck next time!" << endl ; exit(0) ; }
	if(monsterHealth <= 0) 
	{
		cout << "You beat the " << room.getMonsterWithin() << "!" << endl ;
		return {durability, playerHealth};
	}
}

int examineMonster(Player player, vector <Monster> monsters, string name)
{
	int fake_x = player.getX() ; int fake_y = player.getY() ;
	if(monsters[0].getX() == fake_x && monsters[0].getY() == fake_y)
	{
		if(name == "Mischevous Goblin" || name == "Goblin" || name == "goblin") 
		{
			if(monsters[3].getHealth() == 0)
			{cout << "The " << monsters[3].getName() << " is dead!" << endl ;}
			else {
			cout << monsters[0].getDescription() << endl ;
			cout << "Health: " << monsters[0].getHealth() << endl ;
			cout << "Power: " << monsters[0].getPower() << endl ;}
			return 0;
		}
	}
	if(monsters[1].getX() == fake_x && monsters[1].getY() == fake_y)
	{
		if(name == "Achromantula" || name == "achromantula") 
		{
			if(monsters[1].getHealth() == 0)
			{cout << "The " << monsters[1].getName() << " is dead!" << endl ;}
			else {
			cout << monsters[1].getDescription() << endl ;
			cout << "Health: " << monsters[1].getHealth() << endl ;
			cout << "Power: " << monsters[1].getPower() << endl ;
			}
			return 0;
		}
	}
	if(monsters[2].getX() == fake_x && monsters[2].getY() == fake_y)
	{
		if(name == "Wearwolf" || name == "wearwolf") 
		{
			if(monsters[2].getHealth() == 0)
			{cout << "The " << monsters[2].getName() << " is dead!" << endl ;}
			else {
			cout << monsters[2].getDescription() << endl ;
			cout << "Health: " << monsters[2].getHealth() << endl ;
			cout << "Power: " << monsters[2].getPower() << endl ;
			}
			return 0;
		}
	}
	else if(monsters[3].getX() == fake_x && monsters[3].getY() == fake_y)
	{
		if(name == "Dementor" || name == "dementor") 
		{
			if(monsters[3].getHealth() == 0)
			{cout << "The " << monsters[3].getName() << " is dead!" << endl ;}
			else{
				cout << monsters[3].getDescription() << endl ;
				cout << "Health: " << monsters[3].getHealth() << endl ;
				cout << "Power: " << monsters[3].getPower() << endl ;
			}
			return 0;
		}
	}
	if(monsters[4].getX() == fake_x && monsters[4].getY() == fake_y)
	{
		if(name == "Basilisk" || name == "basilisk")
		{
			if(monsters[4].getHealth() == 0)
			{cout << "The " << monsters[4].getName() << " is dead!" << endl ;}
			else {
			cout << monsters[4].getDescription() << endl ;
			cout << "Health: " << monsters[4].getHealth() << endl ;
			cout << "Power: " << monsters[4].getPower() << endl ;}
			return 0;
		}
	}
	else { cout << "You can't examine what's not there!" << endl ;}
	return -1 ;
}

int main()
{
	//make all changes to objects here
	Player player(1, 1, 15, "Salih", "This is you!") ;
	
	//define weapons
	Item item ;//for examining items etc.
	Weapon stoneSword(1, 1, "stone-sword", "Most basic sword", 4, 1) ;
	Weapon ironSword(1, 1, "iron-sword", "More durable than the stone-sword", 6, 1) ;
	Weapon goldSword(1, 1, "gold-sword", "Same durability as the stone-sword but packs a bigger punch", 4, 2) ;
	Weapon diamondSword(1, 1, "diamond-sword", "Like the gold-sword, but durable! The ultimate sword", 6, 2) ;
	
	//define monsters
	//Monster::Monster(int inputted_x, int inputted_y, int inputted_health, string inputted_name, string inputted_description, int inputted_power, int inputted_itemToInventory)
	Stonemason stonemason(1, 1, "Stonemason", "This is your provider for weapons") ;
	Monster goblin(1, 2, 2, "Mischevous Goblin", "'Tis pesky little Goblin, best get rid of him", 2, -3) ; //to drop jewels
	Monster achromantula(0, 1, 3, "Achromantula", "'Tis an enormous spider, best get rid of him", 2, 10) ;
	Monster wearwolf(1, 0, 4, "Wearwolf", "'Tis an wearwolf, kill him or get bit", 3, -6) ; 
	Monster dementor(2, 1, 6, "Dementor", "'Tis an Dementor. Try not to feel cold", 3, 11) ; 	
	Monster basilisk(1, 3, 7, "Basilisk", "'Tis an Basilisk; whatever you do, don't stare her in the eye", 4, -7) ; 	
	
	//define rooms
	Shop shop(1, 1, "The Stonemason's store", "To your north is Dungeon #1, to your east is Dungeon #4, to your south is Dungeon #3 & to your south is Dungeon #2") ; 
	Battleroom dungeon1(1, 2, "Dungeon #1", "Within lies a Mischevous Goblin", "Mischevous Goblin") ;
	Battleroom dungeon2(0, 1, "Dungeon #2", "Within lies an Achromantula", "Achromantula") ;
	Battleroom dungeon3(1, 0, "Dungeon #3", "Within lies a Wearwolf", "Wearwolf") ;
	Battleroom dungeon4(2, 1, "Dungeon #4", "Within lies a Dementor", "Dementor") ;
	Battleroom dungeon5(1, 3, "Dungeon #5", "Within lies a Basilisk", "Basilisk") ;
	Exit exit(1, 4, "Exit", "You've made it! Within there's a prize, just for you ;)") ;
	mapPrinter map ;
	
	cout << "Welcome to Salih's Dungeon Escape Game!" << endl ;
	cout << "(type 'help' for a list of possible commands and examples of their usage)" << endl ;
	sleep(3) ;
	cout << endl ;
	examineName(player, shop, {dungeon1, dungeon2, dungeon3, dungeon4, dungeon5}, exit) ;
	examineRoom(player, shop, {dungeon1, dungeon2, dungeon3, dungeon4, dungeon5}, exit) ; cout << endl ;
	map.whereOnMap(player.getX(), player.getY()) ;
	cout << endl ;
	
	while(!gameWon(player))
	{
		//cout << player.getX() << ", " << player.getY() << endl ;
		
		string command = returnCommand() ;
		
		if(!isCommandValid(player.getCommands(), command)) {cout << "Invalid command!" << endl ; continue ;}

		if(command == "help") {player.help() ; continue ;}
		
		if(command == "quit") {cout << "Thanks for playing!" << endl ; return 0 ;}
		   
		if(command == "clear") {cout << "\033[2J\033[1;1H" << endl ; continue ;}
		
		if(command == "fight")
		{
			int fake_x = player.getX() ; int fake_y = player.getY() ;
			
			if(fake_x==1 && fake_y==2) 
			{
				if(dungeon1.getDescription() != "Within lies a Mischevous Goblin") {cout << "You've already fought and killed this monster!" << endl ; continue ;}
			}
			if(fake_x==0 && fake_y==1) 
			{
				if(dungeon2.getDescription() != "Within lies an Achromantula") {cout << "You've already fought and killed this monster!" << endl ; continue ;}
			}
			if(fake_x==1 && fake_y==0) 
			{
				if(dungeon3.getDescription() != "Within lies a Wearwolf") {cout << "You've already fought and killed this monster!" << endl ; continue ;}
			}
			if(fake_x==2 && fake_y==1) 
			{
				if(dungeon4.getDescription() != "Within lies a Dementor") {cout << "You've already fought and killed this monster!" << endl ; continue ;}
			}
			if(fake_x==1 && fake_y==3) 
			{
				if(dungeon5.getDescription() != "Within lies a Basilisk") {cout << "You've already fought and killed this monster!" << endl ; continue ;}
			}
			
			vector<int> array1 ;
			if(player.getEquippedItem() == 1 || player.getEquippedItem() == 2 || player.getEquippedItem() == 3 || player.getEquippedItem() == 4)
			{
				if(player.getEquippedItem()==1)
				{
					if(fake_x==1&&fake_y==2) {
						array1 = fightingScenario(player, dungeon1, goblin, stoneSword);
						dungeon1.changeDescription() ;
						if(goblin.dropItem() < 0)
						{
							cout << "The goblin died and deposited " << goblin.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(goblin.dropItem() * -1) ;
						}
						else {
							cout << "The goblin died and dropped " << item.getNameFromMap(goblin.dropItem()) << " in the room!" << endl ;
							int itemID = goblin.dropItem() ;
							dungeon1.addItems(itemID) ;
						}
						goblin.setHealth(0) ;
					}
					else if(fake_x==0&&fake_y==1) {
						array1 = fightingScenario(player, dungeon2, achromantula, stoneSword);
						dungeon2.changeDescription() ; 
						if(achromantula.dropItem() < 0)
						{
							cout << "The achromantula died and deposited " << achromantula.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(achromantula.dropItem() * -1) ;
						}
						else {
							cout << "The achromantula died and dropped " << item.getNameFromMap(achromantula.dropItem()) << " in the room!" << endl ;
							int itemID = achromantula.dropItem() ;
							dungeon2.addItems(itemID) ;
						}
						achromantula.setHealth(0) ;
					}
					else if(fake_x==1&&fake_y==0) {
						array1 = fightingScenario(player, dungeon3, wearwolf, stoneSword);
						dungeon3.changeDescription() ;
						if(wearwolf.dropItem() < 0)
						{
							cout << "The wearwolf died and deposited " << wearwolf.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(wearwolf.dropItem() * -1) ;
						}
						else {
							cout << "The wearwolf died and dropped " << item.getNameFromMap(wearwolf.dropItem()) << " in the room!" << endl ;
							int itemID = wearwolf.dropItem() ;
							dungeon3.addItems(itemID) ;
						}
						wearwolf.setHealth(0) ;
					}
					else if(fake_x==2&&fake_y==1) {
						array1 = fightingScenario(player, dungeon4, dementor, stoneSword);
						dungeon4.changeDescription() ; 
						if(dementor.dropItem() < 0)
						{
							cout << "The dementor died and deposited " << dementor.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(dementor.dropItem() * -1) ;
						}
						else {
							cout << "The dementor died and dropped " << item.getNameFromMap(dementor.dropItem()) << " in the room!" << endl ;
							int itemID = dementor.dropItem() ;
							dungeon4.addItems(itemID) ;
						}
						dementor.setHealth(0) ;
					}
					else if(fake_x==1&&fake_y==3) {
						array1 = fightingScenario(player, dungeon5, basilisk, stoneSword);
						dungeon5.changeDescription() ; 
						if(basilisk.dropItem() < 0)
						{
							cout << "The basilisk died and deposited " << basilisk.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(basilisk.dropItem() * -1) ;
						}
						else {
							cout << "The basilisk died and dropped " << item.getNameFromMap(basilisk.dropItem()) << " in the room!" << endl ;
							int itemID = basilisk.dropItem() ;
							dungeon5.addItems(itemID) ;
						}
						basilisk.setHealth(0) ;
					}
					else {cout << "There isn't anyone to fight here!" << endl ; continue ;}
				}
				if(player.getEquippedItem()==2)
				{
					if(fake_x==1&&fake_y==2) {
						array1 = fightingScenario(player, dungeon1, goblin, ironSword);
						dungeon1.changeDescription() ;
						if(goblin.dropItem() < 0)
						{
							cout << "The goblin died and deposited " << goblin.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(goblin.dropItem() * -1) ;
						}
						else {
							cout << "The goblin died and dropped " << item.getNameFromMap(goblin.dropItem()) << " in the room!" << endl ;
							int itemID = goblin.dropItem() ;
							dungeon1.addItems(itemID) ;
						}
						goblin.setHealth(0);
					}
					else if(fake_x==0&&fake_y==1) {
						array1 = fightingScenario(player, dungeon2, achromantula, ironSword);
						dungeon2.changeDescription() ; 
						if(achromantula.dropItem() < 0)
						{
							cout << "The achromantula died and deposited " << achromantula.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(achromantula.dropItem() * -1) ;
						}
						else {
							cout << "The achromantula died and dropped " << item.getNameFromMap(achromantula.dropItem()) << " in the room!" << endl ;
							int itemID = achromantula.dropItem() ;
							dungeon2.addItems(itemID) ;
						}
						achromantula.setHealth(0) ;
					}
					else if(fake_x==1&&fake_y==0) {
						array1 = fightingScenario(player, dungeon3, wearwolf, ironSword);
						dungeon3.changeDescription() ;
						if(wearwolf.dropItem() < 0)
						{
							cout << "The wearwolf died and deposited " << wearwolf.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(wearwolf.dropItem() * -1) ;
						}
						else {
							cout << "The wearwolf died and dropped " << item.getNameFromMap(wearwolf.dropItem()) << " in the room!" << endl ;
							int itemID = wearwolf.dropItem() ;
							dungeon3.addItems(itemID) ;
						}
						wearwolf.setHealth(0) ;
					}
					else if(fake_x==2&&fake_y==1) {
						array1 = fightingScenario(player, dungeon4, dementor, ironSword);
						dungeon4.changeDescription() ;
						if(dementor.dropItem() < 0)
						{
							cout << "The dementor died and deposited " << dementor.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(dementor.dropItem() * -1) ;
						}
						else {
							cout << "The dementor died and dropped " << item.getNameFromMap(dementor.dropItem()) << " in the room!" << endl ;
							int itemID = dementor.dropItem() ;
							dungeon4.addItems(itemID) ;
						}
						dementor.setHealth(0) ;
					}
					else if(fake_x==1&&fake_y==3) {
						array1 = fightingScenario(player, dungeon5, basilisk, ironSword);
						dungeon5.changeDescription() ; 
						if(basilisk.dropItem() < 0)
						{
							cout << "The basilisk died and deposited " << basilisk.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(basilisk.dropItem() * -1) ;
						}
						else {
							cout << "The basilisk died and dropped " << item.getNameFromMap(basilisk.dropItem()) << " in the room!" << endl ;
							dungeon5.addItems(basilisk.dropItem()) ;
						}
						basilisk.setHealth(0) ;
					}
					else {cout << "There isn't anyone to fight here!" << endl ; continue ;}
				}
				if(player.getEquippedItem()==3)
				{
					if(fake_x==1&&fake_y==2) {
						array1 = fightingScenario(player, dungeon1, goblin, goldSword);
						dungeon1.changeDescription() ;
						if(goblin.dropItem() < 0)
						{
							cout << "The goblin died and deposited " << goblin.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(goblin.dropItem() * -1) ;
						}
						else {
							cout << "The goblin died and dropped " << item.getNameFromMap(goblin.dropItem()) << " in the room!" << endl ;
							int itemID = goblin.dropItem() ;
							dungeon1.addItems(itemID) ;
						}
						goblin.setHealth(0) ;
					}
					else if(fake_x==0&&fake_y==1) {
						array1 = fightingScenario(player, dungeon2, achromantula, goldSword);
						dungeon2.changeDescription() ;
						if(achromantula.dropItem() < 0)
						{
							cout << "The achromantula died and deposited " << achromantula.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(achromantula.dropItem() * -1) ;
						}
						else {
							cout << "The achromantula died and dropped " << item.getNameFromMap(achromantula.dropItem()) << " in the room!" << endl ;
							dungeon2.addItems(achromantula.dropItem()) ;
						}
						achromantula.setHealth(0) ;
					}
					else if(fake_x==1&&fake_y==0) {
						array1 = fightingScenario(player, dungeon3, wearwolf, goldSword);
						dungeon3.changeDescription() ;
						if(wearwolf.dropItem() < 0)
						{
							cout << "The wearwolf died and deposited " << wearwolf.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(wearwolf.dropItem() * -1) ;
						}
						else {
							cout << "The wearwolf died and dropped " << item.getNameFromMap(wearwolf.dropItem()) << " in the room!" << endl ;
							int itemID = wearwolf.dropItem() ;
							dungeon3.addItems(itemID) ;
						}
						wearwolf.setHealth(0) ;
					}
					else if(fake_x==2&&fake_y==1) {
						array1 = fightingScenario(player, dungeon4, dementor, goldSword);
						dungeon4.changeDescription() ;
						if(dementor.dropItem() < 0)
						{
							cout << "The dementor died and deposited " << dementor.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(dementor.dropItem() * -1) ;
						}
						else {
							cout << "The dementor died and dropped " << item.getNameFromMap(dementor.dropItem()) << " in the room!" << endl ;
							int itemID = dementor.dropItem() ;
							dungeon4.addItems(itemID) ;
						}
						dementor.setHealth(0);
					}
					else if(fake_x==1&&fake_y==3) {
						array1 = fightingScenario(player, dungeon5, basilisk, goldSword);
						dungeon5.changeDescription() ; 
						if(basilisk.dropItem() < 0)
						{
							cout << "The basilisk died and deposited " << basilisk.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(basilisk.dropItem() * -1) ;
						}
						else {
							cout << "The basilisk died and dropped " << item.getNameFromMap(basilisk.dropItem()) << " in the room!" << endl ;
							dungeon5.addItems(basilisk.dropItem()) ;
						}
						basilisk.setHealth(0) ;
					}
					else {cout << "There isn't anyone to fight here!" << endl ; continue ;}
				}
				if(player.getEquippedItem()==4)
				{
					if(fake_x==1&&fake_y==2) {
						array1 = fightingScenario(player, dungeon1, goblin, diamondSword);
						dungeon1.changeDescription() ; 
						if(goblin.dropItem() < 0)
						{
							cout << "The goblin died and deposited " << goblin.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(goblin.dropItem() * -1) ;
						}
						else {
							cout << "The goblin died and dropped " << item.getNameFromMap(goblin.dropItem()) << " in the room!" << endl ;
							int itemID = goblin.dropItem() ;
							dungeon1.addItems(itemID) ;
						}
						goblin.setHealth(0) ;
					}
					else if(fake_x==0&&fake_y==1) {
						array1 = fightingScenario(player, dungeon2, achromantula, diamondSword);
						dungeon2.changeDescription() ;
						if(achromantula.dropItem() < 0)
						{
							cout << "The achromantula died and deposited " << achromantula.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(achromantula.dropItem() * -1) ;
						}
						else {
							cout << "The achromantula died and dropped " << item.getNameFromMap(achromantula.dropItem()) << " in the room!" << endl ;
							int itemID = achromantula.dropItem() ;
							dungeon2.addItems(itemID) ;
						}
						achromantula.setHealth(0) ;
					}
					else if(fake_x==1&&fake_y==0) {
						array1 = fightingScenario(player, dungeon3, wearwolf, diamondSword);
						dungeon3.changeDescription() ; 
						if(wearwolf.dropItem() < 0)
						{
							cout << "The wearwolf died and deposited " << wearwolf.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(wearwolf.dropItem() * -1) ;
						}
						else {
							cout << "The wearwolf died and dropped " << item.getNameFromMap(wearwolf.dropItem()) << " in the room!" << endl ;
							int itemID = wearwolf.dropItem() ;
							dungeon3.addItems(itemID) ;
						}
						wearwolf.setHealth(0) ;
					}
					else if(fake_x==2&&fake_y==1) {
						array1 = fightingScenario(player, dungeon4, dementor, diamondSword);
						dungeon4.changeDescription() ; 
						if(dementor.dropItem() < 0)
						{
							cout << "The dementor died and deposited " << dementor.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(dementor.dropItem() * -1) ;
						}
						else {
							cout << "The dementor died and dropped " << item.getNameFromMap(dementor.dropItem()) << " in the room!" << endl ;
							int itemID = dementor.dropItem() ;
							dungeon4.addItems(itemID) ;
						}
						dementor.setHealth(0) ;
					}
					else if(fake_x==1&&fake_y==3) {
						array1 = fightingScenario(player, dungeon5, basilisk, diamondSword);
						dungeon5.changeDescription() ; 
						if(basilisk.dropItem() < 0)
						{
							cout << "The basilisk died and deposited " << basilisk.dropItem() * -1 << " jewels to you!" << endl;
							player.changeJewels(basilisk.dropItem() * -1) ;
						}
						else {
							cout << "The basilisk died and dropped " << item.getNameFromMap(basilisk.dropItem()) << " in the room!" << endl ;
							dungeon5.addItems(basilisk.dropItem()) ;
						}
						basilisk.setHealth(0) ;
					}
					else {cout << "There isn't anyone to fight here!" << endl ; continue ;}
				}
			}
			else {cout << "You don't even have an item to fight with!" << endl ; continue ;}
			player.changeHealth(array1[1] * -1 ) ;
			
			int thing = array1[0] ;
			
			if(player.getEquippedItem()==1)
			{
				stoneSword.setDurability(thing) ;
				if(stoneSword.getDurability() <= 0)
				{
					player.equipWeapon(-1) ;
					player.removeItems(1) ;
					cout << "Your sword broke!" << endl ;
					stoneSword.setDurability(3) ;
				}
			}
			if(player.getEquippedItem()==2)
			{
				ironSword.setDurability(thing) ;
				if(ironSword.getDurability() <= 0)
				{
					player.equipWeapon(-1) ;
					player.removeItems(2) ;
					cout << "Your sword broke!" << endl ;
					ironSword.setDurability(5) ;				
				}
			}
			if(player.getEquippedItem()==3)
			{
				goldSword.setDurability(thing) ;
				if(goldSword.getDurability() <= 0)
				{
					player.equipWeapon(-1) ;
					player.removeItems(3) ;
					cout << "Your sword broke!" << endl ;
					goldSword.setDurability(3) ;
				}
			}
			if(player.getEquippedItem()==4)
			{
				diamondSword.setDurability(thing) ;
				if(diamondSword.getDurability() <= 0)
				{
					player.equipWeapon(-1) ;
					player.removeItems(4) ;
					cout << "Your sword broke!" << endl ;
					diamondSword.setDurability(5) ;
				}		
			}
					
			if(fake_x==1 && fake_y==2) 
			{
				dungeon1.changeDescription();
				//dungeon1.addItems(goblin.dropItem()) ;
			}
			if(fake_x==0 && fake_y==1) 
			{
				dungeon2.changeDescription();
				//dungeon2.addItems(achromantula.dropItem()) ;
			}
			if(fake_x==1 && fake_y==0) 
			{
				dungeon3.changeDescription();
				//dungeon3.addItems(wearwolf.dropItem()) ;
			}
			if(fake_x==2 && fake_y==1) 
			{
				dungeon4.changeDescription();
				//dungeon4.addItems(dementor.dropItem()) ;
			}
			if(fake_x==1 && fake_y==3) 
			{
				dungeon5.changeDescription();
				//dungeon5.addItems(basilisk.dropItem()) ;
			}
			
			continue ;
		}

		string data = returnData() ;
		if(command == "use")
		{
			int itemID = item.getIntFromMap(data) ;
			if(player.isItemInInventory(itemID))
			{
				if(data == "apple")
				{
					player.removeItems(itemID) ;
					cout << "Heath regenerated by 4!" << endl ;
					player.changeHealth(4) ;
				}
				else if(data == "banana")
				{
					player.removeItems(itemID) ;
					cout << "Heath regenerated by 6!" << endl ;
					player.changeHealth(6) ;
				}
				else if(data == "pear")
				{
					player.removeItems(itemID) ;
					cout << "Heath regenerated by 8!" << endl ;
					player.changeHealth(8) ;
				}
				else {
					cout << "You can't use an item you don't have!" << endl ; 
				}
			}
			else {cout << "You can't use that!" << endl ;}
			continue ;
		}
		if(command == "get")
		{
			if(data == "banana" || data == "pear") 
			{
				int fake_x = player.getX() ;
				int fake_y = player.getY() ;
				int itemID1 = item.getIntFromMap(data) ;
				if(fake_x == 0 && fake_y == 1) {dungeon2.removeItems(itemID1) ; player.addToInventory(itemID1) ;}
				else if(fake_x == 2 && fake_y == 1) {dungeon4.removeItems(itemID1) ; player.addToInventory(itemID1) ;}
				else {cout << "This room has no items in it!" << endl ;}
			}
			else {cout << "You can't get that!" << endl ;}
		}
		if(command == "examine")
		{
			if(data == "room")
			{
				examineRoom(player, shop, {dungeon1, dungeon2, dungeon3, dungeon4, dungeon5}, exit) ;
				continue ;
			}
			else if(data == "inventory")
			{
				player.examineInventory() ;
				continue ;
			}
			else if(data == "health")
			{
				player.seeHealth() ;
			}
			else if(data == "Achromantula" || data == "achromantula" || data == "Mischevous Goblin" || data == "goblin" || data == "Dementor" || data == "dementor"|| data == "Wearwolf" || data == "wearwolf"|| data == "Basilisk" || data == "basilisk")
			{
				examineMonster(player, {goblin, achromantula, wearwolf, dementor, basilisk}, data) ;
			}
			else if(data == "jewels")
			{
				cout << "Current jewel count: " << player.getJewels() << endl ;
			}
			else if(data == "position")
			{
				examinePosition(player, map) ;
			}
			else if(data == "apple" || data == "banana" || data == "pear")
			{
				cout << item.getDescriptionFromMap(data) << endl ;
			}
			else
			{
				int fake_x = player.getX() ; int fake_y = player.getY() ;
				if(fake_x == 1 && fake_y == 1) {
					examineItem(player, data, item, {stoneSword, ironSword, goldSword, diamondSword}) ;}
				else{
					if(player.isItemInInventory(item.getIntFromMap(data))) 
					{
						examineItem(player, data, item, {stoneSword, ironSword, goldSword, diamondSword}) ;	
					}
					else { cout << "You can't examine an item you don't have!" << endl ;}
				}
			}
		}
		if(command == "buy")
		{
			if(player.getX() == 1 && player.getY() == 1)
			{
				int weapon = stonemason.sell(data, player.getJewels()) ;
				if(weapon == 500) 
				{
					if(player.getEquippedItem() == 1) {stoneSword.setDurability(3) ;}
					else if(player.getEquippedItem() == 2) {ironSword.setDurability(5) ;}
					else if(player.getEquippedItem() == 3) {goldSword.setDurability(3) ;}
					else if(player.getEquippedItem() == 4) {diamondSword.setDurability(5) ;}
				}
				else if(weapon != -1)
				{
					player.addToInventory(weapon) ;
				}
			}
			else { cout << "You aren't in the shop!" << endl ; continue ;}
		}
		
		if(command == "equip")
		{
			if(player.isItemInInventory(item.getIntFromMap(data))) 
			{
				if(player.getEquippedItem() == 1 || player.getEquippedItem() == 2 || player.getEquippedItem() == 3 || player.getEquippedItem() == 4)
				{
					cout << "Replacing your currently equipped item deletes it! Are you sure you want to do this?" << endl ;
					cout << "(type yes or no)" << endl ;
					command = returnCommand() ;
					if(command == "yes")
					{
						player.removeItems(player.getEquippedItem()) ; 
						player.equipWeapon(item.getIntFromMap(data)) ;
					}
					else {continue ;}
				}
				else
				{
					player.equipWeapon(item.getIntFromMap(data)) ;
				}
			}
			else { cout << "You can't equip yourself with an item you don't have!" << endl ;}
			
		}
		if(command == "move")
		{
			if(data == "north" || data == "south" || data == "east" || data == "west")
			{
				bool isRoomValid = validateRoom(player, {shop}, {dungeon1, dungeon2, dungeon3, dungeon4, dungeon5}, {exit}, data) ;
				if(isRoomValid)
				{
					bool isOldMonsterDead = validatePrecense(player, {shop}, {dungeon1, dungeon2, dungeon3, dungeon4, dungeon5}, {exit}, data) ;
					if(isOldMonsterDead)
					{
							if(data == "north") { player.setY(player.getY() + 1) ;}
							else if(data == "south") { player.setY(player.getY() - 1) ;}
							else if(data == "east")  { player.setX(player.getX() + 1) ;}
							else if(data == "west")  { player.setX(player.getX() - 1) ;}
							examineName(player, shop, {dungeon1, dungeon2, dungeon3, dungeon4, dungeon5}, exit) ;
							examineRoom(player, shop, {dungeon1, dungeon2, dungeon3, dungeon4, dungeon5}, exit) ;
							map.whereOnMap(player.getX(), player.getY()) ;
							cout << endl ;
					}
					else {cout << "You need to defeat the previous monster!" << endl ;}
				}
				else {cout << "You can't move there!" << endl ;}
			}
			else {cout << "That isn't a valid direction!" << endl ;}
		}
	cout << endl ;
	}
	cout << "Well done, you've won the game!" << endl ;
	cout << "(as a reward, here's a little parting gift...)" << endl ;
	sleep(3); 
	cout << "\033[2J\033[1;1H" << endl ;
	sleep(1) ;
	exit.examineRoom() ;
	//
	return 0 ;
}