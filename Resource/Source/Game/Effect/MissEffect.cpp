#include "MissEffect.h"
#include "FileSystem.h"
#include "SoundLoader.h"
#include "DataBase.h"

PopupMissEffect::PopupMissEffect(const Vector2Int& pos, Camera& camera, bool cameraActive, bool critical)
	: PopupText("MISS!", pos, camera, cameraActive, critical)
{
	auto& dataBase = DataBase::Instance();
	auto efkPath = dataBase.GetBattleEffectData(BattleEffectType::miss).seName;
	SoundL.PlaySE(efkPath.c_str());
}