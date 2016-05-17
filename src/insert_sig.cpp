#include <sys/stat.h>
#include <cstdlib>
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
            int numberHexa = 0;
            for(int l=b;l<c;l++)
                if(data[l]>='0'&&data[l]<='9')
                    numberHexa++;
                else if(data[l]>='A'&&data[l]<='F')
                    numberHexa++;
                else if(data[l]>='a'&&data[l]<='f')
                    numberHexa++;
            if(4*numberHexa>3*(c-b))
                return;
        }
        i+=j;
    }
}

int main(int argc, char* argv[])
{
    if(argc<4)
    {
        std::cerr<<"Usage : "<<argv[0]<<" <pdf file> <content file> <output file>"<<std::endl;
        return 1;
    }

    std::ifstream pdf(argv[1],std::ios::binary);
    pdf.seekg(0,pdf.end);
    std::vector<unsigned char> read(pdf.tellg());
    pdf.seekg(0,pdf.beg);
    pdf.read((char*)&(read[0]),read.size());

    int a,b,c,d;
    find_range(read,"ByteRange",a,b,c,d);

    std::ifstream content_file(argv[2],std::ios::binary);
    content_file.seekg(0,content_file.end);
    std::vector<unsigned char> content(content_file.tellg());
    content_file.seekg(0,content_file.beg);
    content_file.read((char*)&(content[0]),content.size());

    std::vector<unsigned char> content_until_superior, content_after_superior;
    unsigned int i=0;
    for(;i<content.size()&&content[i]!='>';i++)
        content_until_superior.push_back(content[i]);
    for(;i<content.size();i++)
        content_after_superior.push_back(content[i]);

    std::ofstream out(argv[3],std::ios::binary|std::ios::trunc|std::ios::out);

    int n_beg=0, n_zero=0, n_end=0; //n_beg+n_zero+n_end = c-a-b
    if(c<=content_until_superior.size()+a+b)
        n_beg = c-a-b;
    else
    {
        n_beg = content_until_superior.size();
        if(c<=n_beg+content_after_superior.size()+a+b)
            n_end = c-a-b-n_beg;
        else
        {
            n_end = content_after_superior.size();
            n_zero = c-a-b-n_beg-n_end;
        }
    }

    std::cout<<n_beg<<" "<<n_zero<<" "<<n_end<<" "<<c<<" "<<c+d<<std::endl;
    for(int i=a;i<b;i++)
        out<<read[i];
    for(int i=0;i<n_beg;i++)
        out<<content_until_superior[i];
    for(int i=0;i<n_zero;i++)
        out<<'0';
    for(int i=0;i<n_end;i++)
        out<<content_after_superior[i];
    for(int i=b;i<read.size();i++)
        out<<read[i];

    out.close();

    return 0;
}
