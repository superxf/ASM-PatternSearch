#include "inst_replacer.h"

using namespace std;
vector<vector<string>> f_pc;
vector<Instruction> l_a_s1;
vector<Instruction> l_a_s2;
vector<Instruction> l_a_t_s;
vector<Instruction> l_a_l;
int size1 = 0;
int size2 = 0;
int size3 = 0;
int size4 = 0;
int cnt = 0;
void type_check(Instruction& i1, Instruction& i2, Instruction& i3){
    if(i3.op =="str"){
        if(i1.operand[0] == i2.operand[1] || i1.operand[0] == i2.operand[2]){
            if(i2.operand[0] == i3.operand[0]&& i1.operand.size()==i3.operand.size()){     
                    if(i1.operand[1] == i3.operand[1]){
                        size1++;
                        if(i1.operand.size()==2||(i1.operand.size()==3 && i1.operand[2] == i3.operand[2])){
                            if(i2.operand.size()==3){
                                l_a_s1.emplace_back(i1);
                                l_a_s1.emplace_back(i2);
                                l_a_s1.emplace_back(i3);
                            }
                        }
                    }
                    else{
                        size2++;
                        if(i1.operand.size()==2&&i2.operand[2][0]!='#'&&i2.operand.size()==3){
                            l_a_s2.emplace_back(i1);
                            l_a_s2.emplace_back(i2);
                            l_a_s2.emplace_back(i3);
                        }
                    }
            }
        }
        if(i1.operand[0] == i2.operand[1] || i1.operand[0] == i2.operand[2]){
            if(i2.operand[0] == i3.operand[1]){
                size3++;
                if(i1.operand.size()==i3.operand.size()&&i2.operand.size()==3&&i2.operand[2][0]!='#'&&i1.operand.size()==2){
                        l_a_t_s.emplace_back(i1);
                        l_a_t_s.emplace_back(i2);
                        l_a_t_s.emplace_back(i3);
                }
                    
            }
        }
    }
    else{
        if(i1.operand[0] == i2.operand[1] || i1.operand[0] == i2.operand[2]){
            if(i2.operand[0] == i3.operand[1]){
                size4++;
                if(i1.operand.size()==i3.operand.size()&&i2.operand.size()==3&&i2.operand[2][0]!='#'&&i1.operand.size()==2){
                    l_a_l.emplace_back(i1);
                    l_a_l.emplace_back(i2);
                    l_a_l.emplace_back(i3);
                }
            }
            
        }
    }
    
}

int main(int argc, char *argv[]){
    std::ifstream ifs;
    std::ofstream ofs;
    vector<string> block;
    vector<string> pc;
    vector<string> ins;
    ifs.open(argv[1],std::ios::in) ;
    if (!ifs.is_open()) {
        std::cout << "文件打开失败" << std::endl;
    }
    std::string codeline;
    int line_number = 0;
    int count = 0;
    vector<Search_Result> tmp_res;
    while(getline(ifs, codeline)){
        if(line_number==0){
            line_number++;
            continue;
        } 
        line_number++;
        if(codeline =="" || codeline[codeline.size() - 1] == ':') {
            if(block.size() >= 3){
                Pattern_Search search(block, pc, ins);
                Search_Result res;
                // std::cout<<"aaaa"<<std::endl;
                res = search.find_discontinuous_pattern();
                tmp_res.push_back(res);
                count+=res.ins.size();
            }
            pc.clear();
            block.clear();
            ins.clear();
        }
        else{
            if(codeline.size()>20){
                int pc_end = codeline.find(':');
                int pc_start = pc_end;
                for(; pc_start>=0; pc_start--){
                    if(codeline[pc_start] ==' ')
                        break;
                }
                if(pc_end==-1||pc_start==-1) continue;
                block.emplace_back(codeline.substr(pc_end+12));
                pc.emplace_back(codeline.substr(pc_start+1,pc_end-pc_start-1));
                ins.emplace_back(codeline.substr(pc_end+2,8));
            }
            
        }
    }
    if(block.size() >= 3){
                Pattern_Search search(block, pc, ins);
                Search_Result res;
                res = search.find_discontinuous_pattern();
                tmp_res.push_back(res);
                count+=res.ins.size();
            }
    for(int i = 0; i < tmp_res.size(); i++){
        for(int j = 0; j < tmp_res[i].ins.size(); j+=3){
            type_check(tmp_res[i].ins[j], tmp_res[i].ins[j+1], tmp_res[i].ins[j+2]);
        }
    }
    ifs.close();
    cout<<"padd1"<<endl;
    cout<<"所有满足条件的个数: "<<size1 * 3<<endl;
    cout<<"可替换的个数: "<<l_a_s1.size()<<endl;
    cout<<endl;
    cout<<"padd2"<<endl;
    cout<<"所有满足条件的个数: "<<size2 * 3<<endl;
    cout<<"可替换的个数: "<<l_a_s2.size()<<endl;
    cout<<endl;
    cout<<"plat"<<endl;
    cout<<"所有满足条件的个数: "<<size3 * 3<<endl;
    cout<<"可替换的个数: "<<l_a_t_s.size()<<endl;
    cout<<endl;
    cout<<"pll"<<endl;
    cout<<"所有满足条件的个数: "<<size4 * 3<<endl;
    cout<<"可替换的个数: "<<l_a_l.size()<<endl;
    ofs.open("res.txt", std::ios::out);
    ofs<<"padd1"<<" "<< l_a_s1.size()<<endl;
    for(int i = 0; i < l_a_s1.size(); i++){
        ofs<<l_a_s1[i].pc<<" " <<l_a_s1[i].bin<<" " <<l_a_s1[i].s_code<<endl;
    }
    ofs<<endl;
    ofs<<"padd2"<<" "<< l_a_s2.size()<<endl;
    for(int i = 0; i < l_a_s2.size(); i++){
        ofs<<l_a_s2[i].pc<<" " <<l_a_s2[i].bin<<" " <<l_a_s2[i].s_code<<endl;
    }
    ofs<<endl;
    ofs<<"plats"<<" "<< l_a_t_s.size()<<endl;
    for(int i = 0; i < l_a_t_s.size(); i++){
        ofs<<l_a_t_s[i].pc<<" " <<l_a_t_s[i].bin<<" " <<l_a_t_s[i].s_code<<endl;
    }
    ofs<<endl;
    ofs<<"pll"<<" "<< l_a_l.size()<<endl;
    for(int i = 0; i < l_a_l.size(); i++){
        ofs<<l_a_l[i].pc<<" " <<l_a_l[i].bin<<" " <<l_a_l[i].s_code<<endl;
    }
    ofs.close();
}