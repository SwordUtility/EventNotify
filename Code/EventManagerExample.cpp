#include <iostream>
#include <map>
#include "EventManager.h"
#include <conio.h>

using namespace std;
using namespace Common;

class Keyboard

{
public:
	bool KeyDown()
	{
		if (getch() == 'k')
			return true;
		return false;
	}
};

class Base
{
public:
	virtual void TriggerSkill() { cout << "11Trigger SkillYes!" << endl; }

	virtual ~Base()
	{
	}
};

class Base2
{
public:
	virtual void SkillExpUp() { cout << "11Skill Exp UpYes!" << endl; }
	virtual void AttackPowerUP() { cout << "Attack Power UpYes!" << endl; }

	virtual ~Base2()
	{
	}
};

class Role : public Base, public Base2
{
public:
	Role(EventManager &en) : ev(en)
	{
	}

	void SkillExpUp() { cout << "Skill Exp UpYes!" << endl; }

	void IsHitMonster() { cout << "Hit The MonsterYes!" << endl; }

	void Resiter()
	{
		ev.Register(EventCode::input_k, [=](EventManager::Function_Arg_Type_const arg)
					{
						if (arg != nullptr)
						{
							auto _a = arg->GetArg<int>(0);
							auto _b = arg->GetArg<int>(1);
							auto _c = arg->GetArg<const char[7]>(2);
							cout << _a << " " << _b << " " << _c << endl;
						}
						this->TriggerSkill();
					});
		ev.Register(EventCode::input_k, [=](EventManager::Function_Arg_Type_const)
					{ this->SkillExpUp(); });
		ev.Register(EventCode::input_k, [=](EventManager::Function_Arg_Type_const)
					{ this->AttackPowerUP(); });
		ev.Register(EventCode::input_k, [=](EventManager::Function_Arg_Type_const)
					{ this->IsHitMonster(); });
	}

	EventHelper ev;
};

void EventManager_Example()
{
	EventManager en;

	{
		Role r(en);
		r.Resiter();

		cout << "Now,I want to handle events here!\n"
			 << endl;

		while (true)
		{
			if (getch() == 'k')
			{
				EventArg arg;
				arg.AddArg(1);
				arg.AddArg(2);
				arg.AddArg("hahaha");
				en.Notify(EventCode::input_k, arg);
				break;
			}
		}
	}
	en.Notify(EventCode::input_k);

}