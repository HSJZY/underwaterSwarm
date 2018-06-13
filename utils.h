#ifndef UTILS_H
#define UTILS_H

#include<string>
#include<iostream>
#include<vector>
#include"global_setting.h"
using namespace std;

void split_string(const string original_str,vector<string>& vec_strs,const string& split_char);
vector<vector<vector<float> > > parse_agents_position(string global_infomation);
#endif // UTILS_H
