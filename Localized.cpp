#include "Localized.h"
#include "rapidjson\document.h"

Localized* 		 Localized::_instance		= nullptr;
const char*		 Localized::_DEF_FILE_NAME  = "Default.json"; //�������� ��������� ���������
const char*		 Localized::_DEF_LANGUAGE   = "en";	     //���� �����������, ���� �� ������ ������ ���� ������������
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

	//���������� ����� ������������ ���� �� ������� ����������
	string currentLang 		 = _instance->_getLanguageCode();
	string langFilePath 	 = "Localized/" + currentLang + "/" + Localized::_DEF_FILE_NAME;

	//��������� ������ �� ��������� �����
	ssize_t dataSize  = 0;
	char*   charData  = nullptr;

	//��������� ���������� �� �����
	string stringData = FileUtils::getInstance()->getStringFromFile(langFilePath);

	//������ JSON ������, ���� �� ������� ���������� - ������ ����� ������ � ���������� JSON
	rapidjson::Document stringsMap;
	stringsMap.Parse<0>(stringData.c_str());

	if( ! stringsMap.IsObject() )
	{
		CCLOG("Language module error!");
		return;
	}

	//��������� ������ � ������� �������
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
	//���� ������ �� ����� � �������
	auto strItem = _langDictionary.find(key);

	//���� ��������� �� �� ��������� ��������, ������ ������� ������
	if(strItem != _langDictionary.end())
	{
		return strItem->second;
	}

	//���� ������ �� �������, ���������� ������ ������
	return "";
}

string Localized::_getLanguageCode()
{
	//���������� ���� �� ����������
	LanguageType langKey = Application::getInstance()->getCurrentLanguage();
	string languagePrefix;

	//��������� ��� ����� � ��������� ����������
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

	//��������� ���������� �� �������� ���� ��� �������� �����, ���� ��� �� ���� ��� ����� �����������
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