#pragma once
#include <string>
#include "Geometry.h"

struct WeaponData
{
	std::string name;
	uint8_t power;
	uint8_t hit;
	uint8_t critical;
	uint8_t weight;
	Range range;
	bool magicAttack;
	bool heal;
	std::string attribute;
	std::string iconPath;

	WeaponData() : name(""), power(0), hit(0), critical(0), weight(0), range(0, 0), magicAttack(false), heal(false), attribute(""), iconPath("") {};
	WeaponData(const std::string na, const uint8_t pw, const uint8_t ht, const uint8_t ct, const uint8_t we,
		const Range& ra, const bool ma, const bool he, const std::string& atribute, const std::string& icon)
		: name(na), power(pw), hit(ht), critical(ct), weight(we), range(ra), magicAttack(ma), heal(he), attribute(atribute), iconPath(icon) {};
};