#pragma once
#include <iostream>
#include <map>
using namespace std;

class PropertiesConfigReader
{
public:
	PropertiesConfigReader(void);
	int getIntProperty(string key);
	double getDoubleProperty(string key);
	string getStringProperty(string key);
	void setIntProperty(string key, int value);
	bool getBoolProperty(string key);
	string ReplaceString(string subject, const string& search,const string& replace);
	bool readProperties(string path);
	bool writeProperties(string path);
	string fixPath(string path);
	~PropertiesConfigReader(void);
	
	string separator;
private:
	map<string,int> properties_int;
	map<string,string> properties_string;
	map<string,bool> properties_bool;
	map<string,double> properties_double;
	
};

