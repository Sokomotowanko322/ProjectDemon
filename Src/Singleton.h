#pragma once

/// <summary>
/// シングルトンテンプレート
/// </summary>
/// <typeparam name="T">クラス</typeparam>
template<typename T>
class Singleton
{

public:

	/// <summary>
	/// 静的なインスタンスの取得
	/// </summary>
	/// <returns>クラスの参照</returns>
	static inline T& GetInstance()
	{
		static T instansce;
		return instansce;
	}

	/// <summary>
	/// デストラクタ前のリソースの解放
	/// </summary>
	/// <remarks>Dxlib_End()前に解放する必要がある</remarks>
	virtual void Destroy(void) {};

	Singleton(const Singleton& singleton) = delete;
	Singleton& operator=(const Singleton& singleton) = delete;

	Singleton(Singleton&& singleton) = delete;
	Singleton& operator=(Singleton&& singleton) = delete;

protected:

	Singleton() = default;
	virtual ~Singleton() = default;

private:


};

