#pragma once
#include <vector>
#include <map>
#include "../Utility/Geometry.h"

//データ基底クラス
class Data
{
public:
	//このデータが使用可能かどうかを返す
	virtual bool IsAvailable()=0;
	virtual ~Data();
};

class Loader
{
public:
	Loader();
	virtual ~Loader();

	//ロード
	//@param path ファイルパス
	//@param data dataへの参照
	//@retcal ロードの成否
	virtual bool Load(const char* path, Data& data)=0;

	//ロードデータの開放
	//@param path ファイルパス
	//@attention これで指定されたパスに対応するデータは
	//メモリ上からなくなりますので
	// もう一度使用するときにはストレージアクセスが
	//発生します
	virtual void Unload(const char* path)=0;
};

