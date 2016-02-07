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
	static const char*	_DEF_FILE_NAME;				//Ќазвание €зыкового хранилища
	static const char*	_DEF_LANGUAGE;				//язык поумолчанию, если не найден родной €зык пользовател€
	static Localized*	_instance;					//”казатель сам на себ€
	std::map<std::string, std::string>	_langDictionary;			//языковый словарь

	std::string				_getLanguageCode();			//ћетод конвертирует €зыковый код Cocos2D в строковый стандартный €зыковый код

public:
	static Localized*	getInstance();				//ћетод дл€ предоставлени€ доступа к одиночному €зыковому объекту
	static void			init();						//»нициализаци€ объекта
	static void			free();						//”даление объекта

	std::string				getString(const char* key);	//ћетод дл€ получени€ строки из словар€ по строковому ключу
	std::string				getLanguage();
};

#endif // __LOCALIZED_SIMPLE_H__