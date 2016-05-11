#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


void find_range(const std::vector<unsigned char>& data, const std::string& delimiter, int& a, int& b, int& c, int& d)
{
    for(int i=0;i<(int)data.size();i++)
    {
        int j=0;
        while(j<(int)delimiter.size()&&data[i+j]==delimiter[j])
            j++;
        if(j>=(int)delimiter.size())
        {
            for(unsigned int k=0;k<4;k++)
            {
                std::string cur;
                cur.resize(20);
                while(data[i]<'0'||data[i]>'9')
                    i++;
                j = i;
                while(i<j+20&&data[i]>='0'&&data[i]<='9')
                {
                    cur[i-j] = data[i];
                    i++;
                }
                std::istringstream is(cur);
                if(!k)
                    is>>a;
                else if(k==1)
                    is>>b;
                else if(k==2)
                    is>>c;
                else
                    is>>d;
            }
            return;
        }
        i+=j;
    }
}

int main()
{
    std::ifstream ifs("letter_of_rec.pdf",std::ios::binary);
    ifs.seekg(0,ifs.end);
    std::vector<unsigned char> read(ifs.tellg());
    ifs.seekg(0,ifs.beg);
    ifs.read((char*)&(read[0]),read.size());

    int a,b,c,d;
    find_range(read,"ByteRange",a,b,c,d);

    std::cout<<a<<" "<<b<<" "<<c<<" "<<d<<std::endl;
    std::ofstream p1("part1.txt",std::ios::binary|std::ios::trunc|std::ios::out);
    std::ofstream p3("part2.txt",std::ios::binary|std::ios::trunc|std::ios::out);
    std::ofstream p2("between.txt",std::ios::binary|std::ios::trunc|std::ios::out);

    for(int i=a;i<b;i++)
        p1<<read[i];
    for(int i=a+b;i<c;i++)
        p2<<read[i];
    for(int i=c;i<c+d;i++)
        p3<<read[i];

    return 0;
}
