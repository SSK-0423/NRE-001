#pragma once
/// <summary>
/// �V���O���g���N���X
/// </summary>
/// <typeparam name="T">�V���O���g��������N���X��</typeparam>
template <typename T>
class Singleton
{
protected:
	Singleton() {}
	virtual ~Singleton() {}
	Singleton(const Singleton& inst) {}
	Singleton& operator=(const Singleton& inst){}
public:
	/// <summary>
	/// �N���X�̃V���O���g���C���X�^���X�擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static T& Instance()
	{
		static T inst;
		return inst;
	}
};