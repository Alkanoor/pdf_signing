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

std::string find_subject_and_save_certifs(const std::string& path, const std::string& delimiter)
{
    std::ifstream ifs(path.c_str(),std::ios::binary);
    std::string buf, curPath = "tmp/";
    std::vector<std::string> toWrite;
    std::string ret;
    while(std::getline(ifs,buf))
    {
        unsigned int i = 0;
        while(i<delimiter.size()&&i<buf.size()&&buf[i]==delimiter[i])
            i++;
        if(i>=delimiter.size())
        {
            std::ofstream ofs(curPath.c_str(),std::ios::trunc|std::ios::out|std::ios::binary);
            for(unsigned j=0;j<toWrite.size();j++)
                ofs<<toWrite[j]<<std::endl;
            toWrite.clear();
            int found = -1;
            for(unsigned int j=0;j+1<buf.size()&&found<0;j++)
                if((buf[j]=='C'||buf[j]=='c')&&(buf[j+1]=='N'||buf[j+1]=='n'))
                    found = j+3;
            if(found<0)
            {
                std::cout<<"Warning, no CN detected"<<std::endl;
                curPath = "noCN.pem";
            }
            else
            {
                curPath = "tmp/";
                while(found<buf.size()&&buf[found]!='/')
                {
                    curPath.push_back(buf[found]);
                    found++;
                }
                curPath += ".pem";
                ret += curPath+" ";
            }
        }
        toWrite.push_back(buf);
    }
    std::ofstream ofs(curPath.c_str(),std::ios::trunc|std::ios::out|std::ios::binary);
    for(unsigned j=0;j<toWrite.size();j++)
        ofs<<toWrite[j]<<std::endl;
    return ret;
}

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        std::cerr<<"Usage : "<<argv[0]<<" <pdf file> [CA file]"<<std::endl;
        return 1;
    }
    std::string ca_file;
    if(argc>2)
        ca_file = std::string(argv[2]);

    std::ifstream ifs(argv[1],std::ios::binary);
    ifs.seekg(0,ifs.end);
    std::vector<unsigned char> read(ifs.tellg());
    ifs.seekg(0,ifs.beg);
    ifs.read((char*)&(read[0]),read.size());

    int a,b,c,d;
    find_range(read,"ByteRange",a,b,c,d);

    mkdir("tmp",0700);

    std::cout<<a<<" "<<b<<" "<<c<<" "<<d<<std::endl;
    std::ofstream p1("tmp/extern.txt",std::ios::binary|std::ios::trunc|std::ios::out);
    std::ofstream p2("tmp/between.txt",std::ios::binary|std::ios::trunc|std::ios::out);

    for(int i=a;i<b;i++)
        p1<<read[i];
    for(int i=a+b;i<c;i++)
        p2<<read[i];
    for(int i=c;i<c+d;i++)
        p1<<read[i];

    p1.close();
    p2.close();

    system("xxd -p -r < tmp/between.txt > tmp/signature.tmp");
    system("openssl pkcs7 -in tmp/signature.tmp -inform der -print_certs > tmp/certif.txt");

    std::string signers = find_subject_and_save_certifs("tmp/certif.txt","subject");
    std::cout<<signers<<std::endl;

    if(ca_file.size()>0)
        system(std::string("openssl smime -verify -in tmp/signature.tmp -inform der -content tmp/extern.txt -CAfile "+ca_file+" -signer "+signers+" > tmp/out.pdf").c_str());
    else
        system(std::string("openssl smime -verify -in tmp/signature.tmp -inform der -content tmp/extern.txt -signer "+signers+" > tmp/out.pdf").c_str());

    return 0;
}
