#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <opencv2/core/core.hpp>

using namespace std;

typedef struct{
    double vTimestampsRGB;
    string vstrImageFilenamesRGB;
    double vTimestampsD;
    string vstrImageFilenamesD;
    int vResultTxt;
}output;

void ReadImageFile(const string &strFilename, vector<double> &vTimestamps, vector<string> &vstrImageFilenames)
{
    ifstream Filename;
    Filename.open(strFilename.c_str());
    while(!Filename.eof())
    {
        string s;
        getline(Filename,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;

            double t;
            ss >> t;
            vTimestamps.push_back(t);

            string file;
            ss >> file;
            vstrImageFilenames.push_back(file);
        }
    }
}

void associate(vector<double> &vTimestampsRGB, vector<string> &vstrImageFilenamesRGB, vector<double> &vTimestampsD,
               vector<string> &vstrImageFilenamesD, vector<output> &association)
{
    output Auxiliary;
    int last_index_i = -1;
    int last_index_j = -1;
    for (int i = 0; i < vstrImageFilenamesRGB.size(); ++i)
    {
        double min = 1;
        int index_i = -1;
        int index_j = -1;
        for (int j = 0; j < vstrImageFilenamesD.size(); ++j)
        {
            double error = abs(vTimestampsRGB[i]-vTimestampsD[j]);
            if(error < 0.02)
            {
                if (error < min)
                {
                    min = error;
                    index_i = i;
                    index_j = j;
                }
            }
        }
        if (index_i ==-1 || index_j==-1)
            continue;
        if (index_i == last_index_i || index_j == last_index_j)
            continue;
        Auxiliary.vTimestampsRGB = vTimestampsRGB[index_i];
        Auxiliary.vstrImageFilenamesRGB = vstrImageFilenamesRGB[index_i];
        Auxiliary.vResultTxt= index_i+1;
        Auxiliary.vTimestampsD = vTimestampsD[index_j];
        Auxiliary.vstrImageFilenamesD = vstrImageFilenamesD[index_j];
        association.push_back(Auxiliary);
        last_index_i = index_i;
        last_index_j = index_j;
    }
}

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        cerr << endl << "Usage: ./Yoloassociate rgb.txt  depth.txt path_to_txt " << endl;
        return 1;
    }

    vector<string> vstrImageFilenamesRGB;
    vector<string> vstrImageFilenamesD;
    vector<double> vTimestampsRGB;
    vector<double> vTimestampsD;
    ReadImageFile(argv[1],vTimestampsRGB,vstrImageFilenamesRGB);
    ReadImageFile(argv[2],vTimestampsD,vstrImageFilenamesD);

    vector<output> association;
    associate(vTimestampsRGB,vstrImageFilenamesRGB,vTimestampsD,vstrImageFilenamesD,association);

    ofstream inFile;
    inFile.open("associate.txt",ios::trunc);

    for (int i = 0; i <association.size() ; ++i)
    {
        inFile<< fixed;
        inFile << setprecision(6) << association[i].vTimestampsRGB<< " " <<association[i].vstrImageFilenamesRGB<< " " <<
               association[i].vTimestampsD << " " <<association[i].vstrImageFilenamesD << " " <<association[i].vResultTxt << ".txt" << "\n";
    }
    return 0;
}