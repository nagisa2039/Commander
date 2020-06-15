#include "AnkerCalculation.h"

AnkerCalculation::AnkerCalculation()
{
	_funcs[static_cast<size_t>(Anker::leftup)] = [](const Vector2Int& drawPos, const Size& size) {return drawPos; };
	_funcs[static_cast<size_t>(Anker::leftcenter)] = [](const Vector2Int& drawPos, const Size& size) {return drawPos + Vector2Int(0, -size.h / 2); };
	_funcs[static_cast<size_t>(Anker::leftdown)] = [](const Vector2Int& drawPos, const Size& size) {return drawPos + Vector2Int(0, -size.h); };
	_funcs[static_cast<size_t>(Anker::centerup)] = [](const Vector2Int& drawPos, const Size& size) {return drawPos + Vector2Int(-size.w / 2, 0); };
	_funcs[static_cast<size_t>(Anker::center)] = [](const Vector2Int& drawPos, const Size& size) {return drawPos + Vector2Int(-size.w / 2, -size.h / 2); };
	_funcs[static_cast<size_t>(Anker::centerdown)] = [](const Vector2Int& drawPos, const Size& size) {return drawPos + Vector2Int(-size.w / 2, -size.h); };
	_funcs[static_cast<size_t>(Anker::rightup)] = [](const Vector2Int& drawPos, const Size& size) {return drawPos + Vector2Int(-size.w, 0); };
	_funcs[static_cast<size_t>(Anker::rightcenter)] = [](const Vector2Int& drawPos, const Size& size) {return drawPos + Vector2Int(-size.w, -size.h / 2); };
	_funcs[static_cast<size_t>(Anker::rightdown)] = [](const Vector2Int& drawPos, const Size& size) {return drawPos + Vector2Int(-size.w, -size.h); };
}

AnkerCalculation::~AnkerCalculation()
{
}

Vector2Int AnkerCalculation::GetDrawPos(const Vector2Int& drawPos, const Size& size, const Anker anker)const
{
	return _funcs.at(static_cast<size_t>(anker))(drawPos, size);
}
