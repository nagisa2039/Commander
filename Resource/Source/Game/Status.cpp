#include "Status.h"
#include <algorithm>
#include "Application.h"
#include "DataBase.h"

using namespace std;

int BattleStatus::GetPower() const
{
	return (weaponTypeData.magicAttack ? status.magic_power : status.power) + weaponData.power;
}

int BattleStatus::GetDamage(const BattleStatus& target)const
{
	const auto& dataBase = Application::Instance().GetDataBase();
	auto rate = dataBase.GetAttributeRate(weaponTypeData.attribute, dataBase.GetWeaponTypeData(target.status.weaponId).attribute);
	return max( static_cast<int>((GetPower() - (weaponTypeData.magicAttack ? target.status.magic_defense : target.status.defense))  * rate) - target.defenseCorrection, 0);
}

int BattleStatus::GetRecover()
{
	return status.magic_power/2 + weaponData.power;
}

int BattleStatus::GetHitRate() const
{
	return status.skill + weaponData.hit;
}

int BattleStatus::GetHit(const BattleStatus& target) const
{
	return min(100, max(0, GetHitRate() - target.GetAvoidance()));
}

int BattleStatus::GetCriticalRate() const
{
	return (status.skill + status.luck)/2 + weaponData.critical;
}

int BattleStatus::GetCritical(const BattleStatus& target) const
{
	return min(100, max(0, GetCriticalRate() - target.status.luck));
}

int BattleStatus::GetDifense() const
{
	return status.defense + this->defenseCorrection;
}

int BattleStatus::GetMagicDifense() const
{
	return status.magic_defense + this->defenseCorrection;
}

int BattleStatus::GetAvoidance() const
{
	return GetAttackSpeed() + this->avoidanceCorrection;
}

bool BattleStatus::CheckHeal() const
{
	return weaponTypeData.heal;
}

bool BattleStatus::CheckMagicAttack() const
{
	return weaponTypeData.magicAttack;
}

bool BattleStatus::CheckPursuit(const BattleStatus& target) const
{
	return GetAttackSpeed() - target.GetAttackSpeed() >= 4;
}

int BattleStatus::GetAttackSpeed()const
{
	return status.speed - (weaponData.weight - status.power / 5);
}

void Status::AddStatus(const Status& addStatus)
{
	level			+= addStatus.level;
	health			+= addStatus.health;
	power			+= addStatus.power;
	defense			+= addStatus.defense;
	magic_defense	+= addStatus.magic_defense;
	skill			+= addStatus.skill;
	speed			+= addStatus.speed;
}
