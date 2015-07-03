#include "PropertiesConfigReader.h"

#include "Markup.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

PropertiesConfigReader::PropertiesConfigReader(void)
{
	#ifdef _WIN32
		 separator = string("\\");
	#else
		 separator = string("/");
	#endif
}

string PropertiesConfigReader::ReplaceString(string subject, const string& search,const string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

string PropertiesConfigReader::fixPath(string path){
	return PropertiesConfigReader::ReplaceString(path,string("#separator#"),separator);
}

bool PropertiesConfigReader::readProperties(string path){
	CMarkup xml;

	if(!xml.Load( path )){
		throw string(xml.GetError());
	}
	xml.FindElem();
	xml.IntoElem();

	xml.FindElem("int");
	xml.IntoElem();
	int tmp_int;
	while(xml.FindElem()){
		istringstream iss(xml.GetData());
		iss >> tmp_int;
		properties_int[xml.GetTagName()] = tmp_int;
	}
	xml.OutOfElem();

	xml.FindElem("bool");
	xml.IntoElem();
	bool tmp_bool;
	while(xml.FindElem()){
		istringstream iss(xml.GetData());
		iss >> std::boolalpha >> tmp_bool;
		properties_bool[xml.GetTagName()] = tmp_bool;
	}
	xml.OutOfElem();

	xml.FindElem("string");
	xml.IntoElem();
	while(xml.FindElem()){
		properties_string[string(xml.GetTagName())] = string(xml.GetData());
	}
	xml.OutOfElem();

	xml.FindElem("double");
	xml.IntoElem();
	double tmp_double;
	while(xml.FindElem()){
		istringstream iss(xml.GetData());
		iss >> tmp_double;
		properties_double[xml.GetTagName()] = tmp_double;
	}
	xml.OutOfElem();
	return true;
}

bool PropertiesConfigReader::writeProperties(string path){
/*	CFG_File config;
	int result = CFG_OpenFile(path.c_str(), &config );

	if(result == CFG_ERROR )
		return false;

	for (map<string,int>::iterator it=properties_int.begin(); it!=properties_int.end(); ++it)
		CFG_WriteInt(it->first.c_str(),it->second);

	for (map<string,bool>::iterator it=properties_bool.begin(); it!=properties_bool.end(); ++it)
		CFG_WriteBool(it->first.c_str(),it->second);

	for (map<string,string>::iterator it=properties_string.begin(); it!=properties_string.end(); ++it)
		CFG_WriteText(it->first.c_str(),it->second.c_str());

	for (map<string,double>::iterator it=properties_double.begin(); it!=properties_double.end(); ++it)
		CFG_WriteFloat(it->first.c_str(),it->second);

	CFG_SaveFile(path.c_str(), CFG_SORT_ORIGINAL, CFG_SPACE_ENTRIES | CFG_NO_COMMENTS);
	CFG_CloseFile(0);*/
	return true;
}

bool PropertiesConfigReader::getBoolProperty(string key){
	return properties_bool[key];
}

int PropertiesConfigReader::getIntProperty(string key){
	return properties_int[key];
}

void PropertiesConfigReader::setIntProperty(string key, int value){
	properties_int[key] = value;
}

double PropertiesConfigReader::getDoubleProperty(string key){
	return properties_double[key];
}

string PropertiesConfigReader::getStringProperty(string key){
	return fixPath(properties_string[key]);
}


PropertiesConfigReader::~PropertiesConfigReader(void)
{
}
