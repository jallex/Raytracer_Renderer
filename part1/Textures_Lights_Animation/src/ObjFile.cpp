#include "ObjFile.hpp"
#include <iostream> 
#include <fstream>
#include <string> 
#include <sstream>
#include <vector>

// OBJ requirement
// Constructor loads a filename with the .Obj extension
ObjFile::ObjFile(std::string fileName){
    std::ifstream inFile;
    //Open file
    inFile.open(fileName);

   //store normal data
   std::vector<float> normalData;

    //Get number of vertices and faces
    if(inFile.is_open()){
        std::string line;
        while(getline(inFile, line)){
        if(line.substr(0, 2) == "vn"){ // found normal data
            //Find numbers in line using stringstream
            std::stringstream s; 
            s << line;
            std::string temp_s;
            float temp_i;
            while(!s.eof()){
                s >> temp_s;
                if(std::stringstream(temp_s) >> temp_i) {
                    normalData.push_back(temp_i);
                }
                temp_s = ""; 
            }
        }
           else if(line.substr(0, 1) == "v"){ // found vertex data
            //Find numbers in line using stringstream
            std::stringstream s; 
            s << line;
            std::string temp_s;
            float temp_i;
            while(!s.eof()){
                s >> temp_s;
                if(std::stringstream(temp_s) >> temp_i) {
                    m_verticeData.push_back(temp_i);
                }
                temp_s = ""; 
            }
        }
        else if(line.substr(0, 1) == "f"){ //Found face data
            //Find numbers in line using stringstream
            std::stringstream s; 
            s << line;
            std::string temp_s;
            int temp_i;
            while(!s.eof()){
                s >> temp_s;
                if(std::stringstream(temp_s) >> temp_i) {
                    //subtract one because objs are 1-indexed, and we want 0-indexed data.
                    m_indices.push_back(temp_i - 1);
                }
                temp_s = ""; 
            }
          }
        }
    }
  //Close file
  inFile.close();
}