#include "PPM.hpp"
#include <iostream> 
#include <fstream>
#include <string> 
#include <sstream>
#include <vector>

//PPM Constructor 
PPM::PPM(std::string fileName){
    loadPPM(fileName);
    loadPPMPixels();
}

//PPM Destructor, delete memory
PPM::~PPM(){
    delete[] m_PixelData;
}

//Load PPM data from file
void PPM::loadPPM(std::string fileName){
    int index = 0;
    // Read PPM3
    std::ifstream inFile;
    inFile.open(fileName);
    // The number line fo the file that we are reading, excluding comments
    int lineNonComment = 0;
    if(inFile.is_open()){
        std::string currentLine;
        while(getline(inFile, currentLine)){
            if(currentLine.substr(0, 1) != "#"){
                lineNonComment += 1;
                if(lineNonComment == 2){
                    // Holds the dimensions
                    for(int i = 0; i < currentLine.length(); i++){
                        if(currentLine[i] == ' '){
                            m_width = std::atoi(currentLine.substr(0, i).c_str());
                            m_height = std::atoi(currentLine.substr(i+1).c_str());
                            break;
                        }
                    }
                }
                if(lineNonComment == 3){
                    // maximum pixel value
                    max = std::atoi(currentLine.c_str());
                    m_PixelData = new unsigned int[m_width*m_height*3]();
                }
                else if(lineNonComment > 3){
                    // Actual pixel data
                    index = getNumbersFromLine(currentLine, index);
                }
            }
        }
    }
    inFile.close();
}

//Get numbers from a string 
int PPM::getNumbersFromLine(std::string line, int index){
    int temp_index = index;
    std::stringstream ss;    
    ss << line;

    std::string temp_str;
    int num;
    while (!ss.eof()) {
        ss >> temp_str;
        if (std::stringstream(temp_str) >> num)
            m_PixelData[temp_index] = num;
        temp_str = "";
        temp_index += 1;
    }
    return temp_index;
}

//Load PPM pixels into a vector of pixels data structure
void PPM::loadPPMPixels(){
    std::vector<Pixel> row;
    int temp_index = 0;
    int pixel_index = 0;
    int x = 0;
    int y = 0;
    for(unsigned int i = 0; i < getWidth()*getHeight()*3; i++){
        temp_index += 1;
        if(temp_index == 3){
            unsigned int r = m_PixelData[i - 2];
            unsigned int g = m_PixelData[i - 1];
            unsigned int b = m_PixelData[i];
            temp_index = 0;

            //Set pixels 
            Pixel pixel;
            pixel.r = r;
            pixel.g = g;
            pixel.b = b;
            pixel.x = x;
            pixel.y = y;

            //update x and y pixel position
            if(x == getWidth() - 1){
                m_PPMPixels.push_back(row);
                x = 0;
                y += 1;
                row.clear();
            }
            else {
                row.push_back(pixel);
                x += 1;
            }
        }
    }
}

//Set pixel at location to be specified color
void PPM::setPixel(int x, int y, unsigned int R, unsigned int G, unsigned int B){
    if(R < min){
        R = min;
    }
    if(R > max){
        R = max;
    }
    if(G < min){
        G = min;
    }
    if(G > max){
        G = max;
    }
    if(B < min){
        B = min;
    }
    if(B > max){
        B = max;
    }
    m_PPMPixels[x][y].r = R;
    m_PPMPixels[x][y].g = G;
    m_PPMPixels[x][y].b = B;
}

//Darken the PPM image
void PPM::darken(){
    for(unsigned int i = 0; i < getWidth(); i++){
        for(unsigned int j = 0; j < getHeight(); j++){
            unsigned int r = m_PPMPixels[i][j].r;
            unsigned int g = m_PPMPixels[i][j].g;
            unsigned int b = m_PPMPixels[i][j].b;
            setPixel(i, j, r/2.0, g/2.0, b/2.0);
        }
    }
}

//Lighten the PPM image
void PPM::lighten(){
    for(unsigned int i = 0; i < getWidth(); i++){
        for(unsigned int j = 0; j < getHeight(); j++){
            unsigned int r = m_PPMPixels[i][j].r;
            unsigned int g = m_PPMPixels[i][j].g;
            unsigned int b = m_PPMPixels[i][j].b;
            setPixel(i, j, r*2.0, g*2.0, b*2.0);
        }
    }
}

//Save PPM image out as a P3 PPM
void PPM::saveP3(std::string outputFileName){
    std::ofstream outFile; 
    outFile.open(outputFileName);
    outFile << "P3" << std::endl;
    outFile << m_width << " " << m_height << std::endl;
    outFile << max << std::endl;
    for(unsigned int i = 0; i < getWidth(); i++){
        for(unsigned int j = 0; j < getHeight(); j++){
            unsigned int r = static_cast<unsigned int>(m_PPMPixels[i][j].r);
            unsigned int g = static_cast<unsigned int>(m_PPMPixels[i][j].g);
            unsigned int b = static_cast<unsigned int>(m_PPMPixels[i][j].b);
            outFile << r << std::endl;
            outFile << g << std::endl;
            outFile << b << std::endl;
        }
    }
    outFile.close();
}

//Save the PPM image out as a P6 PPM
void PPM::saveP6(std::string outputFileName){
    std::ofstream outFile; 
    outFile.open(outputFileName);
    outFile << "P6" << std::endl;
    outFile << m_width << " " << m_height << std::endl;
    outFile << max << std::endl;
    for(unsigned int i = 0; i < getWidth(); i++){
        for(unsigned int j = 0; j < getHeight(); j++){
            unsigned char r = static_cast<unsigned char>(m_PPMPixels[i][j].r);
            unsigned char g = static_cast<unsigned char>(m_PPMPixels[i][j].g);
            unsigned char b = static_cast<unsigned char>(m_PPMPixels[i][j].b);
            outFile << r;
            outFile << g;
            outFile << b;
        }
    }
    outFile.close();
}

//Create a blank canvas
void PPM::createBlankCanvas(int r, int g, int b){
    for(unsigned int x = 0; x < m_width; x++ ) {
       for(unsigned int y = 0; y < m_height; y++ ) {
        setPixel(x, y, r, g, b);
        }
    }
}

// Draw a circle 
void PPM::drawCircle(int r, int g, int b){
    for(unsigned int x = 0; x < m_width; x++ ) {
        for(unsigned int y = 0; y < m_height; y++ ) {
            int centerX = 254;
            int centerY = 254;
            int radius = 50;
            if ((x-centerX)*(x-centerX) + (y-centerY)*(y-centerY) < radius*radius){
                setPixel(x, y, r, g, b);
            }
        }
    }
}
