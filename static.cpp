/*
    输入地址按0x40间隔递增（64字节）
*/
#include <iostream>
#include <fstream>
#include <map>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <math.h>

using namespace std;

bool myCmp(const pair<string, pair<int,int>>& p1,const pair<string, pair<int,int>>& p2)
{
	return (p1.second.first + p1.second.second) < (p2.second.first + p2.second.second);
}

void writeDistribution(int write_num, int*distribution, int exp){   
    for(int i=0; i<=exp; i++){
        if(i==0){
            if(write_num == 1){
                distribution[i]++;
                break;
            }
        }else{
            if(write_num <= pow(2, i)){
                distribution[i]++;
                break;
            }
        }
    }
}

int main(int argc, char** argv){
    std::ifstream input_file(argv[1]);
    std::string time_stamp;
    std::string RW;
    std::string address;

    std::map<std::string, pair<int, int>> address_map; // first read, second write
    int line_num = 0;
    int read_num = 0;
    int write_num = 0;
    int write_num_0 = 0;
    int read_num_0 = 0;
    int non_duplicate_address_count = 0;
    int duplicate_address_count = 0;
    int read_non_duplicate_address_count = 0;
    int read_duplicate_address_count = 0;
    int write_non_duplicate_address_count = 0;
    int write_duplicate_address_count = 0;

    int exp = 10;
    int* write_distribution = new int[exp+1];

    if(input_file){
        while(input_file>>time_stamp){
            input_file>>RW>>address;
            line_num++;

            if(RW == "R"){
                read_num++;
                address_map[address].first++;
            }
            else if(RW == "W"){
                write_num++;
                address_map[address].second++;
            }
            else
                cout<<"ERROR OPERATION\n";
        }  

        vector<pair<string, pair<int, int>>> arr;
        for (map<string, pair<int, int>>::iterator it=address_map.begin();it!=address_map.end();++it){
            arr.push_back(make_pair(it->first, make_pair(it->second.first, it->second.second)));

            if(it->second.first + it->second.second == 1)
                non_duplicate_address_count++;
            else
                duplicate_address_count++;
            
            if(it->second.first == 1)
                read_non_duplicate_address_count++;
            else if(it->second.first > 1)
                read_duplicate_address_count++;
            else if(it->second.first <= 0)
                read_num_0++;
            else{
                cout<<"read num Should not reach here\n";
                exit(-1);
            }

            if(it->second.second == 1)
                write_non_duplicate_address_count++;
            else if(it->second.second > 1)
                write_duplicate_address_count++; 
            else if(it->second.second <= 0)
                write_num_0++;
            else{
                cout<<"write num Should not reach here\n";
                exit(-1);
            }

            writeDistribution(it->second.second, write_distribution, exp);
        }            

        sort(arr.begin(), arr.end(), myCmp);

        for (vector<pair<string, pair<int,int>>>::iterator it=arr.begin();it!=arr.end();++it)
            cout<<"Address:"<<it->first<<'\t'<<"Read: "<<it->second.first<<"\t"<<"Write: "<<
            it->second.second<<endl;
        
        cout<<"\n--- static ---\n";
        std::cout<<"Trace count: "<<line_num<<std::endl;
        std::cout<<"address count: "<<address_map.size()<<std::endl;
        cout<<"\n--- ---\n";
        std::cout<<"non-duplicate address count: "<<non_duplicate_address_count<<std::endl;
        std::cout<<"duplicate address count: "<<duplicate_address_count<<std::endl;
        std::cout<<"Read non-duplicate address count: "<<read_non_duplicate_address_count<<std::endl;
        std::cout<<"Read duplicate address count: "<<read_duplicate_address_count<<std::endl;
        std::cout<<"Write non-duplicate address count: "<<write_non_duplicate_address_count<<std::endl;
        std::cout<<"Write duplicate address count: "<<write_duplicate_address_count<<std::endl;
        cout<<"\n--- ---\n";
        std::cout<<"Write num: "<<write_num<<std::endl;
        std::cout<<"Read num: "<<read_num<<std::endl;
        cout<<"\n--- Write distribution---\n";
        printf("Write num = %d: %d\n", 0, write_num_0);
        for(int i=0; i<=exp; i++)
            if(i==0)
                printf("Write num = %d: %d\n", int(pow(2, i)), write_distribution[i]);
            else    
                printf("Write num (%d %d]: %d\n", int(pow(2, i-1)), int(pow(2, i)), write_distribution[i]);
    }
}