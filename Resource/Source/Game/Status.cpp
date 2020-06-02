#include "Status.h"
#include <algorithm>
#include "Application.h"
#include "DataBase.h"

using namespace std;

int Status::GetDamage(const Status& target)const
{
	auto rate = Application::Instance().GetDataBase().GetAttributeRate(attributeId, target.attributeId);
	return max( static_cast<int>((power - (magicAttack ? target.magic_defense : target.defense)) * rate) - target.defenseCorrection, 0);
}

int Status::GetRecover()
{
	return power;
}

int Status::GetHitRate() const
{
	return 100 + skill * 2;
}

int Status::GetHit(const Status& target) const
{
	return min(100, max(0, GetHitRate() - target.GetAvoidance()));
}

int Status::GetCriticalRate() const
{
	return min(100, max(0, skill / 5));
}

int Status::GetCritical(const Status& target) const
{
	return min(100, max(0, skill / 5));
}

int Status::GetDifense() const
{
	return this->defense + this->defenseCorrection;
}

int Status::GetMagicDifense() const
{
	return this->magic_defense + this->defenseCorrection;
}

int Status::GetAvoidance() const
{
	return GetAttackSpeed() * 2 + this->avoidanceCorrection;
}

bool Status::CheckPursuit(const Status& target) const
{
	return GetAttackSpeed() - target.GetAttackSpeed() >= 4;
}

void Status::AddStatus(Status addStatus)
{
	level += addStatus.level;
	health += addStatus.health;
	power += addStatus.power;
	defense += addStatus.defense;
	magic_defense += addStatus.magic_defense;
	skill += addStatus.skill;
	speed += addStatus.speed;
}

int Status::GetAttackSpeed()const
{
	return this->speed;
}
