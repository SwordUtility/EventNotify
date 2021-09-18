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

class EventArgBase
{
};
template<class Type>
class EATemplate : public EventArgBase
{
public:
	EATemplate(const Type& v) :value(v) {}
	const Type& value;
};

class EventArg
{
public:
	using CallBackArgType = std::vector<EventArgBase*>;
	~EventArg()
	{
		for (auto ptr : m_arrArg)
		{
			delete ptr;
		}
		m_arrArg.clear();
	}
	CallBackArgType m_arrArg;
	template<typename T>
	void AddArg(const T& arg)
	{
		m_arrArg.push_back(new EATemplate<T>(arg));
	}
	template<typename T>
	const T& GetArg(int nIndex) const
	{
		const EATemplate<T>* _value = static_cast<const EATemplate<T>*>(m_arrArg[nIndex]);
		return _value->value;
	}
};

class EventManager
{
public:
	using Function_Arg_Type_const = typename const EventArg*;
	using Function_Type = typename std::function<void(Function_Arg_Type_const)>;
protected:
	using Function_Arg_Type = typename EventArg*;
	using IDFucntion_Map = typename std::map<unsigned int, Function_Type>;
	using EventCode_Map = typename std::map<EventCode, IDFucntion_Map>;

public:
	EventManager()
	{
	}
	virtual ~EventManager()
	{

	}
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;

	unsigned int Register(EventCode eCode, Function_Type pCallBack)
	{
		unsigned int id = IDCreater();
		IDFucntion_Map& mpCallback = m_mpEvent[eCode];
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

	void Notify(EventCode eCode, Function_Arg_Type_const arg = nullptr)
	{
		auto fIt = m_mpEvent.find(eCode);
		if (fIt != m_mpEvent.end())
		{
			for (auto pFuncIt : fIt->second)
			{
				pFuncIt.second(arg);
			}
		}
	}

	void Notify(EventCode eCode, const EventArg& arg)
	{
		Notify(eCode, (EventManager::Function_Arg_Type_const)&arg.m_arrArg);
	}

private:
	unsigned int IDCreater()
	{
		return m_nIdCounter++;
	}
private:
	EventCode_Map m_mpEvent;
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

	unsigned int Register(EventCode eCode, EventManager::Function_Type pCallBack)
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
		ev.Register(EventCode::input_k, [=](EventManager::Function_Arg_Type_const arg) {
			if (arg != nullptr)
			{
				auto _a = arg->GetArg<int>(0);
				auto _b = arg->GetArg<int>(1);
				auto _c = arg->GetArg<const char[7]>(2);
				cout << _a << " " << _b << " " << _c << endl;
			}
			this->TriggerSkill();
					});
		ev.Register(EventCode::input_k, [=](EventManager::Function_Arg_Type_const) {
			this->SkillExpUp();
					});
		ev.Register(EventCode::input_k, [=](EventManager::Function_Arg_Type_const) {
			this->AttackPowerUP();
					});
		ev.Register(EventCode::input_k, [=](EventManager::Function_Arg_Type_const) {
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


	return 0;

}