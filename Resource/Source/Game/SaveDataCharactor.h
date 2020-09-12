#pragma once
#include "Status.h"
#include "CharactorType.h"

struct SaveDataCharactor
{
	CharactorType charType;
	Status status;
	SaveDataCharactor() :charType(0), status(Status()) {};
	SaveDataCharactor(const CharactorType ct, const Status& st) :charType(ct), status(st) {};
};