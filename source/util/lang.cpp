#include <iostream>
#include <switch.h>
#include <cstring>
#include <sstream>
#include <filesystem>
#include "util/lang.hpp"
#include "util/config.hpp"

namespace Language {
	json lang;

	void Load() {
		//https://switchbrew.org/wiki/Settings_services#LanguageCode
		//Get language from the switch system settings.
		SetLanguage ourLang;
		u64 lcode = 0;
		setInitialize();
		setGetSystemLanguage(&lcode);
		setMakeLanguage(lcode, &ourLang);
		setExit();
		int syslang = (int)ourLang;
		/*
		0 Japanese
		1 AmericanEnglish
		2 French
		3 German
		4 Italian
		5 Spanish
		6 Chinese
		7 Korean
		8 Dutch
		9 Portuguese
		10 Russian
		11 Taiwanese
		12 BritishEnglish
		13 CanadianFrench
		14 LatinAmericanSpanish
		15 [4.0.0+] SimplifiedChinese
		16 [4.0.0+] TraditionalChinese
		17 [10.1.0+] BrazilianPortuguese
		*/
		//Get language int from config file
		std::ifstream ifs;
		std::string languagePath;
		int langInt = inst::config::languageSetting;
		if (std::filesystem::exists(inst::config::appDir + "/lang/custom.json")) {
			languagePath = (inst::config::appDir + "/lang/custom.json");
		}
		else {
			switch (langInt) {
			case 0:
				languagePath = "romfs:/lang/en.json";
				break;
			case 1:
				languagePath = "romfs:/lang/jp.json";
				break;
			case 2:
				languagePath = "romfs:/lang/fr.json";
				break;
			case 3:
				languagePath = "romfs:/lang/de.json";
				break;
			case 4:
				languagePath = "romfs:/lang/it.json";
				break;
			case 5:
				languagePath = "romfs:/lang/ru.json";
				break;
			case 6:
				languagePath = "romfs:/lang/es.json";
				break;
			case 7:
				languagePath = "romfs:/lang/tw.json";
				break;
			case 8:
				switch (syslang) {
					case 0:
						languagePath = "romfs:/lang/jp.json";
						break;
					case 1:
						languagePath = "romfs:/lang/en.json";
						break;
					case 2:
						languagePath = "romfs:/lang/fr.json";
						break;
					case 3:
						languagePath = "romfs:/lang/de.json";
						break;
					case 4:
						languagePath = "romfs:/lang/it.json";
						break;
					case 5:
						languagePath = "romfs:/lang/es.json";
						break;
					case 6:
						languagePath = "romfs:/lang/tw.json";
						break;
					case 10:
						languagePath = "romfs:/lang/ru.json";
						break;
					case 11:
						languagePath = "romfs:/lang/tw.json";
						break;
					case 12:
						languagePath = "romfs:/lang/en.json";
						break;
					case 16:
						languagePath = "romfs:/lang/tw.json";
						break;
					default:
						languagePath = "romfs:/lang/en.json";
						break;
				}
				break;
			default:
				languagePath = "romfs:/lang/en.json";
				break;
			}
		}
		if (std::filesystem::exists(languagePath)) ifs = std::ifstream(languagePath);
		else ifs = std::ifstream("romfs:/lang/en.json");
		if (!ifs.good()) {
			std::cout << "[FAILED TO LOAD LANGUAGE FILE]" << std::endl;
			return;
		}
		lang = json::parse(ifs);
		ifs.close();
	}

	std::string LanguageEntry(std::string key) {
		json j = GetRelativeJson(lang, key);
		if (j == nullptr) {
			return "didn't find: " + key;
		}
		return j.get<std::string>();
	}

	std::string GetRandomMsg() {
		json j = Language::GetRelativeJson(lang, "inst.finished");
		srand(time(NULL));
		return(j[rand() % j.size()]);
	}
}