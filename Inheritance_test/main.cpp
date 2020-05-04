#include <conio.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <iterator>

class Dice
{
public:
	int Roll_the_dice(int dice)
	{
		int total = 0;
		for (int i = 0; i < dice; i++)
		{
			total += d6(rng);
		}
		return total;
	};

private:
	std::uniform_int_distribution<int> d6 = std::uniform_int_distribution<int>(1, 6);
	std::mt19937 rng = std::mt19937(std::random_device{}());

};
class Fighter
{
public:
	Fighter() // default constructor
		:
		Fighter("Blah Blah", "unknown", 500, 15, 10) {}; 
	Fighter(const std::string& name, const std::string& gender, int hp, int str, int def)
		:
		name(name),
		gender(gender),
		hp(hp),
		str(str),
		def(def)
	{
		//std::cout << name << " prepares for battle..." << std::endl;
	}
	void attack(Fighter& s, int x)
	{
		int damage = str * 4 + x ;
		if (s.isAlive() == true)
		{
			s.deductedHP(damage);
			//std::cout << name << " does " << damage << " damage to " << s.getName() << std::endl;
		}

	}
	void deductedHP(int damage)
	{
		hp -= damage - def;
	}
	void increaseHP(int x)
	{
		hp = hp + x;
	}
	bool isAlive()
	{
		return hp > 0;
	}
	int getHP()
	{
		return hp;
	}
	const std::string getName() 
	{ 
		return name; 
	}
	const std::string getPronoun()
	{
		if (gender == "male" || gender == "m")
		{
			return "his";
		}
		else if (gender == "female" || gender == "f")
		{
			return "her";
		}
	
		return "their";
	}

protected:
	int hp;
	int str;
	int def;
	std::string gender;
	std::string name;
};
class Healer : public Fighter
{
public:
	Healer(const std::string& name, const std::string& gender, int hp, int str, int def)
		:
		Fighter(name, gender, hp, str, def) {};
	void heal(Fighter f[], int numFighter, Healer h[], int numHealers, int diceRoll)
	{
		int health = int(70 - (str * str) + diceRoll); // 8 and 49/2 = 24.5
		std::cout << getName() << " heals all by... " << health << std::endl;
		for (int i = 0; i < numFighter; i++)
		{
			if (f[i].isAlive() == true)
			{
				f[i].increaseHP(health);
			}
		}
		for (int i = 0; i < numHealers; i++)
		{
			if (h[i].isAlive() == true)
			{
				h[i].increaseHP(health);
			}
		}
		sp = 0;
	}
	void attack(Fighter& s, int x)
	{
		Fighter::attack(s, x);
		if (sp < 10)
		{
			sp += str;
		}
	}
	int getSp()
	{
		return sp;
	}
private:
	int sp = 0;
};
class Boss : public Fighter
{
public:
	Boss(const std::string& name, const std::string& gender, int hp, int str, int def)
		:
		Fighter(name, gender,hp,str, def){};
	void attack(Fighter& s, int x)
	{
		Fighter::attack(s, x);
		if (sp < 10)
		{
			sp += 5;
		}
	}
	void useSuper(Fighter f[], int numFighters, Healer h[], int numHealers, int diceRoll)
	{
		int damage = int(90 + 3 * diceRoll / 2);
		std::cout << getName() << " releases Cerberus, dealing " << damage << " to all!" << std::endl;

		for (int i = 0; i < numFighters; i++)
		{
			if (f[i].isAlive() == true) // check alive
			{
				f[i].deductedHP(damage);
				if (f[i].isAlive() == false) //check death
				{
					std::cout << f[i].getName() << " falls" << std::endl;
				}
			}
		}
		for (int i = 0; i < numHealers; i++)
		{
			if (h[i].isAlive()) // check alive
			{
				h[i].deductedHP(damage);
				if (h[i].isAlive() == false) //check death
				{
					std::cout << h[i].getName() << " falls" << std::endl;
				}
			}
		}
		sp = 0;

	}
	int getSp()
	{
		return sp;
	}
private:
	int sp = 5;
};

int main()
{
	int wins =0;

	for (int i = 0; i < 500; i++)
	{
		Fighter Ares("Ares", "male", 670, 23, 15);
		Fighter Athena("Athena", "f", 680, 20, 20);
		Fighter Hephaestus("Hephaestus", "m", 750, 15, 20);
		Fighter Artemis("Artemis", "m", 640, 30, 12);
		Healer Apollo("Apollo", "m", 600, 7, 12);
		Healer Persephone("Persephone", "f", 590, 4, 10);
		Boss boss("Hades, Lord of the Underworld", "m", 4550, 7, 40);

		const int numFighters = 4;
		const int numHealers = 2;
		int killer = -1;
		int totalChampions = numFighters + numHealers;
		Fighter f[numFighters] = { Ares, Athena, Hephaestus, Artemis };
		Healer h[numHealers] = { Apollo,Persephone };
		Dice d;
		bool Fighting = true;

		while (Fighting == true)
		{
			for (int i = 0; i < numFighters; i++)
			{
				if (f[i].isAlive() == true)
				{
					f[i].attack(boss, d.Roll_the_dice(2));
					if (boss.isAlive() != true)
					{
						killer = i;
						break;
					}
				}
			}
			if (boss.isAlive() == false)
			{
				Fighting = false;
				std::cout << boss.getName() << "falls... and " << f[killer].getName() << " dealt the killing blow!" << std::endl;
				break;
			}

			for (int i = 0; i < numHealers; i++)
			{
				if (h[i].isAlive() == true)
				{
					if (h[i].getSp() >= 12)
					{
						h[i].heal(f, numFighters, h, numHealers, d.Roll_the_dice(3));
					}
					else
					{
						h[i].attack(boss, d.Roll_the_dice(2));
						if (boss.isAlive() != true)
						{
							killer = i;
							break;
						}
					}

				}
			}
			if (boss.isAlive() == false)
			{
				Fighting = false;
				std::cout << boss.getName() << " falls... and " << f[killer].getName() << " dealt the killing blow!" << std::endl;
				break;
			}

			if (boss.getSp() == 10)
			{
				boss.useSuper(f, numFighters, h, numHealers, d.Roll_the_dice(2));
			}
			else
			{
				for (int i = 0; i < numFighters; i++)
				{
					boss.attack(f[i], d.Roll_the_dice(2));
				}
				for (int i = 0; i < numHealers; i++)
				{
					boss.attack(h[i], d.Roll_the_dice(2));
				}
			}

			int championDeaths = 0;
			for (int i = 0; i < numFighters; i++)
			{
				if (f[i].isAlive() == false)
				{
					championDeaths++;
				}
			}
			for (int i = 0; i < numHealers; i++)
			{
				if (h[i].isAlive() == false)
				{
					championDeaths++;
				}
			}
			if (championDeaths == totalChampions)
			{
				Fighting = false;
				wins++;
				std::cout << boss.getName() << " is victorious!" << std::endl;
			}
		}
	}
	std::cout << "Hades, Lord of the Underworld won " << wins << " times" << std::endl;
	std::cout << "The Olympians won " << 500 - wins << " times" << std::endl;
	while (!_kbhit()) {};
	return 0;
}



