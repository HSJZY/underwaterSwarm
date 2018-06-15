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


//这代码写完不想看第二遍，去tm的，能用就行，WTF！！！
vector<vector<vector<float> > > parse_agents_position(string global_infomation)
{
    vector<string> vec_info_markers;

    vector<vector<vector<float> > > agents_position;

    split_string(global_infomation,vec_info_markers,".");
    for(int i=0;i<vec_info_markers.size();i++)
    {
        string marker_i_info=vec_info_markers[i];//这里得到的结果是string 类型的marker_i:(1,2,3),(4,5,6)
        vector<string> vec_marker_i;
        split_string(marker_i_info,vec_marker_i,":");//这里得到的结果vector类型的[[marker_i],[(1,2,3),(4,5,6)]]
        marker_i_info=vec_marker_i[1];
        vec_marker_i.clear();
        if(marker_i_info.find("(",1)!=-1)
            split_string(marker_i_info,vec_marker_i,";");//这里得到的结果vector类型的[[(1,2,3)],[(4,5,6)]...]
        else
            vec_marker_i.push_back(marker_i_info);

        vector<vector<float> > marker_i_record;
        for(int i=0;i<vec_marker_i.size();i++)
        {
            string marker_i_n=vec_marker_i[i];
            if(marker_i_n.substr(0,1)=="(")
                marker_i_n=vec_marker_i[i].substr(1,vec_marker_i[i].length()-2);//这里得到的string类型的1,2,3或None
            else
                marker_i_n=vec_marker_i[i];

            if(marker_i_n=="None")
            {
                vector<float> marker_i_null;
                marker_i_record.push_back(marker_i_null);
                continue;
            }
            vector<string> str_marker_i_record;
            vector<float> num_marker_i_record;
            split_string(marker_i_n,str_marker_i_record,",");
            for(int j=0;j<str_marker_i_record.size();j++)
            {
                num_marker_i_record.push_back(atof(str_marker_i_record[j].c_str()));//这里得到的string类型的[[1],[2],[3]]
            }
            marker_i_record.push_back(num_marker_i_record);
        }
        agents_position.push_back(marker_i_record);
    }
    return agents_position;
}


