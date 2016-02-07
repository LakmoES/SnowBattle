#include "Localized.h"
#include "rapidjson\document.h"

Localized* 		 Localized::_instance		= nullptr;
const char*		 Localized::_DEF_FILE_NAME  = "Default.json"; //Название языкового хранилища
const char*		 Localized::_DEF_LANGUAGE   = "en";	     //Язык поумолчанию, если не найден родной язык пользователя
using namespace std;
Localized::Localized()
{
}

Localized::~Localized()
{
}

void Localized::init()
{
	if(_instance == nullptr)
	{
		_instance = new Localized();
	}
	else
	{
		return;
	}

	//Определяем какой используется язык на текущем устройстве
	string currentLang 		 = _instance->_getLanguageCode();
	string langFilePath 	 = "Localized/" + currentLang + "/" + Localized::_DEF_FILE_NAME;

	//Загружаем данные из языкового файла
	ssize_t dataSize  = 0;
	char*   charData  = nullptr;

	//Загружаем информацию из файла
	string stringData = FileUtils::getInstance()->getStringFromFile(langFilePath);

	//Парсим JSON данные, если не удалось распарсить - скорее всего ошибка в синтаксисе JSON
	rapidjson::Document stringsMap;
	stringsMap.Parse<0>(stringData.c_str());

	if( ! stringsMap.IsObject() )
	{
		CCLOG("Language module error!");
		return;
	}

	//Загружаем данные в словарь объекта
	for (auto item = stringsMap.MemberBegin(); item != stringsMap.MemberEnd(); ++item)
	{
		string key	 = item->name.GetString();
		string value     = item->value.GetString();

		_instance->_langDictionary.insert( make_pair(key, value) );
	}
}

void Localized::free()
{
	delete _instance;
	_instance = nullptr;
}

string Localized::getString(const char* key)
{
	//Ищем строку по ключу в словаре
	auto strItem = _langDictionary.find(key);

	//если указатель не на последнем элементе, значит элемент найден
	if(strItem != _langDictionary.end())
	{
		return strItem->second;
	}

	//Если ничего не найдено, возвращаем пустую строку
	return "";
}

string Localized::_getLanguageCode()
{
	//Определяем язык на устройстве
	LanguageType langKey = Application::getInstance()->getCurrentLanguage();
	string languagePrefix;

	//Переводим код языка в строковый эквивалент
	switch(langKey)
	{
		case LanguageType::ENGLISH:
			languagePrefix = "en";
			break;
		case LanguageType::CHINESE:
			languagePrefix = "zh";
			break;
		case LanguageType::FRENCH:
			languagePrefix = "fr";
			break;
		case LanguageType::ITALIAN:
			languagePrefix = "it";
			break;
		case LanguageType::GERMAN:
			languagePrefix = "de";
			break;
		case LanguageType::SPANISH:
			languagePrefix = "es";
			break;
		case LanguageType::UKRAINIAN:
		case LanguageType::RUSSIAN:
			languagePrefix = "ru";
			break;
		case LanguageType::KOREAN:
			languagePrefix = "ko";
			break;
		case LanguageType::JAPANESE:
			languagePrefix = "ja";
			break;
		case LanguageType::HUNGARIAN:
			languagePrefix = "hu";
			break;
		case LanguageType::PORTUGUESE:
			languagePrefix = "pt";
			break;
		case LanguageType::ARABIC:
			languagePrefix = "ar";
			break;
		case LanguageType::NORWEGIAN:
			languagePrefix = "nb";
			break;
		case LanguageType::POLISH:
			languagePrefix = "pl";
			break;
	}

	//Проверяем существует ли языковый файл для текущего языка, если нет то берём код языка поумолчанию
	string langFilePath = "Localized/" + languagePrefix + "/" + Localized::_DEF_FILE_NAME;

	if ( ! FileUtils::getInstance()->isFileExist( langFilePath.c_str() ) || languagePrefix.empty() )
	{
		return Localized::_DEF_LANGUAGE;
	}

	return languagePrefix;
}

Localized* Localized::getInstance()
{
	return _instance;
}

string Localized::getLanguage()
{
	return "Localized/" + this->_getLanguageCode() + "/" + Localized::_DEF_FILE_NAME;
}