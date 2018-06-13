#include "utils.h"

void split_string(const string original_str,vector<string>& vec_strs,const string& split_char)
{
    string::size_type pos_first,pos_second;

    pos_first=0;
    pos_second=original_str.find(split_char);
    vec_strs.push_back(original_str.substr(pos_first,pos_second-pos_first));
    string left_str=original_str.substr(pos_second+1);
    while(left_str.length()>0)
    {
        pos_first=0;
        pos_second=left_str.find(split_char);
        if(pos_second==-1)
        {
            vec_strs.push_back(left_str.substr(pos_first,pos_second));
            break;
        }
        vec_strs.push_back(left_str.substr(pos_first,pos_second));
        left_str=left_str.substr(pos_second+1);
    }
    return;
}
