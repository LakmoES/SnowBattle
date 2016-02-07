 #ifndef __LOCALIZED_SIMPLE_H__
#define __LOCALIZED_SIMPLE_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class Localized
{
private:
	Localized();
	~Localized();

protected:
	static const char*	_DEF_FILE_NAME;				//�������� ��������� ���������
	static const char*	_DEF_LANGUAGE;				//���� �����������, ���� �� ������ ������ ���� ������������
	static Localized*	_instance;					//��������� ��� �� ����
	std::map<std::string, std::string>	_langDictionary;			//�������� �������

	std::string				_getLanguageCode();			//����� ������������ �������� ��� Cocos2D � ��������� ����������� �������� ���

public:
	static Localized*	getInstance();				//����� ��� �������������� ������� � ���������� ��������� �������
	static void			init();						//������������� �������
	static void			free();						//�������� �������

	std::string				getString(const char* key);	//����� ��� ��������� ������ �� ������� �� ���������� �����
	std::string				getLanguage();
};

#endif // __LOCALIZED_SIMPLE_H__