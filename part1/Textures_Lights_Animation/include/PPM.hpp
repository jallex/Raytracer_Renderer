#ifndef PPM_HPP
#define PPM_HPP

#include <string>
#include <vector>

//Struct to represent a Pixel
struct Pixel{
  int x;
  int y;
  unsigned int r;
  unsigned int g;
  unsigned int b; 
  };

//PPM class
class PPM{
    public:
        //Constructor
        PPM(std::string fileName);
        //Destructor
        ~PPM();
        //Load PPM data from file
        void loadPPM(std::string fileName);
        //Load PPM data into data types
        void loadPPMPixels();
        //Set pixel at specified location to be specified color
        void setPixel(int x, int y, unsigned int R, unsigned int G, unsigned int B);
        //Darken PPM
        void darken();
        //Lighten PPM
        void lighten();
        //Save a P6 PPM
        void saveP6(std::string outputFileName);
        //Save a P3 PPM
        void saveP3(std::string outputFileName);
        //Getter method for pixel data
        inline unsigned int* getPixelData() const { return m_PixelData; }
        // Getter method for Pixel data structure data
        std::vector<std::vector<Pixel>> getPPMPixels() const { return m_PPMPixels; }
        // Getter method for width of image
        inline int getWidth() const { return m_width; }
        // Getter method for height of image
        inline int getHeight() const { return m_height; }
        // Get numbers from a string
        int getNumbersFromLine(std::string line, int index);
        // Create a blank canvas of specified color
        void createBlankCanvas(int r, int g, int b);
        // Draw a circle in the center of canvas of specified color
        void drawCircle(int r, int g, int b);

    private:
        // PPM data loaded from file
        unsigned int* m_PixelData;
        // PPM data in data structure form
        std::vector<std::vector<Pixel>> m_PPMPixels;
        // width of image
        int m_width{0};
        // height of image
        int m_height{0};
        // max color of PPM pixel
        int max{0};
        // min color of PPM pixel
        int min{0};

};

#endif