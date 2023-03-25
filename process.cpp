#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <math.h>

//#define DEBUG
using namespace std;

int hexStringToInt(string s){
    int sum = 0;
    
    for(int i=2; i<=s.size()-1; i++){
        if('0'<=s[i] && s[i]<='9')
            sum += (s[i] - '0');
        else if('a'<=s[i] && s[i]<='f')
            sum += (s[i] - 'a' + 10);
        else{
            cout<<"invalid hexString: "<<s<<" "<<i<<endl;
        }
        sum *= 16;
    }

    sum /= 16;
    return sum;
}

string intHexString(int x){
    std::ostringstream ss;
    ss << std::hex << x;
    return ss.str();
}

void writeAddressMap(string file_path, map<int, pair<int, int>>& address_map, int time_interval){
    long long t = 0;
    std::ofstream ofs(file_path);
    if (ofs.is_open()){
        while(1){
            int flag = 0;
            
            for(map<int, pair<int, int>>::iterator it=address_map.begin(); it!=address_map.end(); ++it){
                //read
                if(it->second.first >= 1){
                    flag = 1;

                    ofs<<to_string(t);
                    ofs<<" ";
                    ofs<<"R";
                    ofs<<" ";
                    ofs<<"0x";
                    ofs<<intHexString(it->first);
                    ofs<<"\n";
                    it->second.first--;
                    t += time_interval;
                }
                
                //write
                if(it->second.second >= 1){
                    flag = 1;

                    ofs<<to_string(t);
                    ofs<<" ";
                    ofs<<"W";
                    ofs<<" ";
                    ofs<<"0x";
                    ofs<<intHexString(it->first);
                    ofs<<"\n";
                    it->second.second--;
                    t += time_interval;
                }
            }
            if(flag == 0)
                break;
        }
    }else{
        cout<<"open file error\n";
    }
}

int main(int argc, char** argv){
    std::ifstream input_file(argv[1]);
    string output_file(argv[2]);
    int time_interval = 2500; // unit: second
    //int time_interval = atoi(argv[3]); // unit: second

    std::string time_stamp;
    std::string RW;
    std::string address;
    // 不能直接用string类型的地址当键，string会按字典序排序。
    std::map<int, pair<int, int>> address_map;

    int line_num = 0;
    int read_num = 0;
    int write_num = 0;

    if(input_file){
        while(input_file>>time_stamp){
            input_file>>RW>>address;
            line_num++;

            int int_address = hexStringToInt(address);
            if(RW == "R"){
                read_num++;
                address_map[int_address].first++;
            }
            else if(RW == "W"){
                write_num++;
                address_map[int_address].second++;
            }
            else
                cout<<"ERROR OPERATION\n";
        }

        int exp = 0;
        int i = 0;
        int address_group_num = 17; // 0-7, 8, 7-0
        int mid = 8;
        int address_group_size = address_map.size() / 17; 
        for(map<int, pair<int, int>>::iterator it=address_map.begin(); it!=address_map.end(); ++it){
            if((i / address_group_size) <= 8){
                it->second.second = int(pow(2, (i / address_group_size)));
            }else{
                it->second.second = int(pow(2, 8 - ((i / address_group_size) - 8)));
            }
            i++;
        }

        #ifdef DEBUG
        printf("Address read write num: ");
        for(map<int, pair<int, int>>::iterator it=address_map.begin(); it!=address_map.end(); ++it){
            printf("0x%x\tread num: %d\twrite num: %d\n", it->first, it->second.first, it->second.second);
        }
        #endif

        writeAddressMap(output_file, address_map, time_interval);
    }
    return 0;
}