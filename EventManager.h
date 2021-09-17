#include <iostream>
#include <vector>
#include <conio.h>
#include <map>
#include <functional>

using namespace std;

enum EventCode
{
	none = 0,
	input_k,
	num,
};

class EventManager
{
public:
	using CallbackMap = std::map<unsigned int, std::function<void()>>;
	using CallBackCodeMap = std::map<EventCode, CallbackMap>;
	EventManager()
	{
	}
	virtual ~EventManager()
	{
		
	}
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;

	unsigned int Register(EventCode eCode, std::function<void()> pCallBack)
	{
		unsigned int id = IDCreater();
		CallbackMap& mpCallback = m_mpEvent[eCode];
		mpCallback.insert(std::make_pair(id, pCallBack));
		return id;
	}
	void UnRegister(unsigned int nRegisterCode)
	{
		for (auto eventIt : m_mpEvent)
		{
			for (auto funcIt : eventIt.second)
			{
				if (funcIt.first == nRegisterCode)
				{
					eventIt.second.erase(funcIt.first);
					return;
				}
			}
		}
	}

	void UnRegister(EventCode eCode, unsigned int nRegisterCode)
	{
		auto fIt = m_mpEvent.find(eCode);
		if (fIt != m_mpEvent.end())
		{
			fIt->second.erase(nRegisterCode);
		}
	}

	void Notify(EventCode eCode)
	{
		auto fIt = m_mpEvent.find(eCode);
		if (fIt != m_mpEvent.end())
		{
			for (auto pFuncIt : fIt->second)
			{
				pFuncIt.second();
			}
		}
	}
private:
	unsigned int IDCreater()
	{
		return m_nIdCounter++;
	}
private:
	CallBackCodeMap m_mpEvent;
	unsigned int m_nIdCounter = 0;
};

class EventHelper
{
public:
	EventHelper(EventManager& tEventManager) : m_tEventManager(tEventManager)
	{
	}
	EventHelper(const EventHelper&) = delete;
	EventHelper& operator=(const EventHelper&) = delete;
	~EventHelper()
	{
		for (auto it : m_mpEventList)
		{
			for (auto code : it.second)
			{
				m_tEventManager.UnRegister(it.first, code);
			}
		}
		m_mpEventList.clear();
	}

	unsigned int Register(EventCode eCode, std::function<void()> pCallBack)
	{
		unsigned int id = m_tEventManager.Register(eCode, pCallBack);
		m_mpEventList[eCode].push_back(id);
		return id;
	}

	void UnRegister(EventCode eCode)
	{
		auto fIt = m_mpEventList.find(eCode);
		if (fIt != m_mpEventList.end())
		{
			for (auto code : fIt->second)
			{
				m_tEventManager.UnRegister(eCode, code);
			}
			fIt->second.clear();
		}
	}
public:
	EventManager& m_tEventManager;
private:
	std::map<EventCode, std::vector<unsigned int>> m_mpEventList;
};


class Keyboard

{
public:

	bool KeyDown()
	{
		if (getch() == 'k')return true;
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


class Role :public Base, public Base2

{
public:
	Role(EventManager& en) :ev(en)
	{

	}

	void SkillExpUp() { cout << "Skill Exp UpYes!" << endl; }


	void IsHitMonster() { cout << "Hit The MonsterYes!" << endl; }

	void Resiter()
	{
		ev.Register(EventCode::input_k, [=](){
			this->TriggerSkill();
		});
		ev.Register(EventCode::input_k, [=]() {
			this->SkillExpUp();
					});
		ev.Register(EventCode::input_k, [=]() {
			this->AttackPowerUP();
					});
		ev.Register(EventCode::input_k, [=]() {
			this->IsHitMonster();
					});
	}

	EventHelper ev;



};

int main()
{
	EventManager en;

	{
		Role r(en);
		r.Resiter();

		cout << "Now,I want to handle events here!\n" << endl;

		while (true)
		{
			if (getch() == 'k')
			{
				en.Notify(EventCode::input_k);
				break;
			}
		}
	}
	en.Notify(EventCode::input_k);


	return 0;

}