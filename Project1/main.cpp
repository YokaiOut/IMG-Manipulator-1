//main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <algorithm>
#include <vector>
#include <array>
#include <cmath>
#include "Image.h"

using namespace std;

// global variables
vector<Image*> ImageHolder;
string FileName;
string SaveName;
//end of global variables 


// Load class 
class Load
{
public:
	Load()
	{
		cout << "Load Constructor called" << endl;
	}

	void FirstLoad()
	{
		cout << "" << endl;
		cout << " But first we need to load an image into the application" << endl;
		cout << " please enter the file path now" << endl;
		cout << " Example 'Images/Original_Image/IMG_1.ppm' " << endl;
		cout << "" << endl;

		cin >> FileName;

		cout << "" << endl;

		ImageHolder.push_back(new Image());//creates the images we need to gain the pixels of the images

		*ImageHolder[0] = readPPM();

	}

	void MenuLoad() // this is a sub menu for the load section of the application
	{
		bool LoadRunning = true;

		while (LoadRunning == true)
		{
			cout << "" << endl;
			cout << "You have selected to load an Image" << endl;
			cout << "Is that correct?" << endl;
			cout << "*****Please note that loading an image will overide the previous image*****" << endl;
			cout << "" << endl;

			string Choice1;

			cin >> Choice1;

			if (Choice1 == "Yes" || Choice1 == "yes" || Choice1 == "y" || Choice1 == "Y")
			{
				cout << "" << endl;
				cout << " we need to load an image into the application" << endl;
				cout << " please enter the file path now" << endl;
				cout << " Example 'Images/Original_Image/IMG_1.ppm' " << endl;
				cout << "" << endl;

				cin >> FileName;

				*ImageHolder[0] = readPPM();
				break;

			}
			else
			{
				if (Choice1 == "No" || Choice1 == "no" || Choice1 == "N" || Choice1 == "n" )
				{
					break;
				}
			}
		}

	}

	Image readPPM() //this is the function to read the imaages and puts them into the ImageHolder[0] object 
	{
		//Remove this cout to prevent multiple outputs
		std::cout << "Attempting to open file from " << FileName << std::endl;
		std::ifstream is;
		is.open(FileName, std::ios::binary);
		Image src;
		try {
			if (is.fail()) {
				throw("Can't open the input file - is it named correctly/is it in the right directory?");
			}

			std::cout << "Reading image ..." << std::endl;

			std::string header;
			int w, h, b;
			is >> header;
			if (strcmp(header.c_str(), "P6") != 0) throw("Can't read the input file - is it in binary format (Has P6 in the header)?");
			is >> w >> h >> b;
			src.w = w;
			src.h = h;
			//std::cout << w << " " << h << std::endl;
			try
			{
				src.pixels = new Image::Rgb[w * h]; // this is throw an exception if bad_alloc 
				is.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data 
				unsigned char pix[3]; // read each pixel one by one and convert bytes to floats 
				for (int i = 0; i < w * h; ++i) {
					is.read(reinterpret_cast<char *>(pix), 3);
					src.pixels[i].r = pix[0] / 255.f;
					src.pixels[i].g = pix[1] / 255.f;
					src.pixels[i].b = pix[2] / 255.f;
				}
				is.close();
				std::cout << "Image read" << std::endl;
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "bad_alloc caugh: " << ba.what() << '\n';
			}
		}
		catch (const char *err) {
			fprintf(stderr, "%s\n", err);
			is.close();
		}
		return src;
	}
};

// Save class
class Save
{
public:

	Save()
	{
		cout << "Save Constructor called" << endl;
	}

	void MenuSave()
	{
		bool LoadRunning = true;

		while (LoadRunning == true)
		{
			cout << "" << endl;
			cout << "You have selected to load an Image" << endl;
			cout << "Is that correct?" << endl;
			cout << "*****Please note that loading an image will overide the previous image*****" << endl;
			cout << "" << endl;

			string Choice1;

			cin >> Choice1;

			if (Choice1 == "Yes" || Choice1 == "yes" || Choice1 == "y" || Choice1 == "Y")
			{
				cout << "" << endl;
				cout << " we need to load an image into the application" << endl;
				cout << " please enter the file path now" << endl;
				cout << " Example 'Images/Original_Image/IMG_1.ppm' " << endl;
				cout << "" << endl;

				cin >> SaveName;

				writePPM();

				break;
			}
			else
			{
				if (Choice1 == "No" || Choice1 == "no" || Choice1 == "N" || Choice1 == "n")
				{
					break;
				}
			}
		}
	}

	//Write data out to a ppm file
	//Constructs the header as above
	void writePPM()
	{
		//std::cout << filename << std::endl;
		std::cout << "Writing image ..." << std::endl;
		if (ImageHolder[0]->w == 0 || ImageHolder[0]->h == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
		std::ofstream ofs;
		try {
			ofs.open(SaveName, std::ios::binary); // need to spec. binary mode for Windows users 
			if (ofs.fail()) throw("Can't open output file");
			ofs << "P6\n" << ImageHolder[0]->w << " " << ImageHolder[0]->h << "\n255\n";
			//std::cout << "P6\n" << ImageHolder[0].w << " " << ImageHolder[0].h << "\n255\n";
			unsigned char r, g, b;
			// loop over each pixel in the image, clamp and convert to byte format
			for (int i = 0; i < ImageHolder[0]->w * ImageHolder[0]->h; ++i) {
				r = static_cast<unsigned char>(std::min(1.f, ImageHolder[0]->pixels[i].r) * 255);
				g = static_cast<unsigned char>(std::min(1.f, ImageHolder[0]->pixels[i].g) * 255);
				b = static_cast<unsigned char>(std::min(1.f, ImageHolder[0]->pixels[i].b) * 255);
				ofs << r << g << b;
			}
			ofs.close();
			//Confirm image write
			std::cout << "Image written" << std::endl;
		}
		catch (const char *err) {
			fprintf(stderr, "%s\n", err);
			ofs.close();
		}
	}

};

// Colour Change Class
class ColourChange
{
public:

	int Red;
	int Green;
	int Blue;

	ColourChange()
	{
		cout << "ColourChange Constructor called" << endl;
	}

	void MenuColourChange()
	{
		bool LoadRunning = true;

		while (LoadRunning == true)
		{
			cout << "" << endl;
			cout << "You have selected to load an Image" << endl;
			cout << "Is that correct?" << endl;
			cout << "*****Please note that loading an image will overide the previous image*****" << endl;
			cout << "" << endl;

			string Choice1;

			cin >> Choice1;

			if (Choice1 == "Yes" || Choice1 == "yes" || Choice1 == "y" || Choice1 == "Y")
			{

				cout << "" << endl;
				cout << "Enter a Red Value please" << endl;
				cout << "" << endl;

				cin >> Red;

				cout << "" << endl;
				cout << "Enter a Green Value please" << endl;
				cout << "" << endl;

				cin >> Green;

				cout << "" << endl;
				cout << "Enter a Blue Value please" << endl;
				cout << "" << endl;

				cin >> Blue;

				Change();

			}
			else
			{
				if (Choice1 == "No" || Choice1 == "no" || Choice1 == "N" || Choice1 == "n")
				{
					break;
				}
			}
		}
	}

	void Change()
	{
		cout << "" << endl;
		cout << "Current RGB values: " << endl;
		cout << "" << endl;
		cout << "	~ Red " << ImageHolder[0]->pixels->r * 255 << endl;
		cout << "	~ Green " << ImageHolder[0]->pixels->g * 255 << endl;
		cout << "	~ Blue " << ImageHolder[0]->pixels->b * 255 << endl;
		cout << "" << endl;

		ImageHolder[0]->pixels->r = (((ImageHolder[0]->pixels->r * 255) + Red) / 255);
		ImageHolder[0]->pixels->g = (((ImageHolder[0]->pixels->g * 255) + Green) / 255);
		ImageHolder[0]->pixels->b = (((ImageHolder[0]->pixels->b * 255) + Blue) / 255);

		if (ImageHolder[0]->pixels->r * 255 > 255)
		{
			ImageHolder[0]->pixels->r = 255;
		}
		else
		{
			if (ImageHolder[0]->pixels->r * 255 < 0)
			{
				ImageHolder[0]->pixels->r = 0;
			}
		}

		if (ImageHolder[0]->pixels->g * 255 > 255)
		{
			ImageHolder[0]->pixels->g = 255;
		}
		else
		{
			if (ImageHolder[0]->pixels->g * 255 < 0)
			{
				ImageHolder[0]->pixels->g = 0;
			}
		}

		if (ImageHolder[0]->pixels->b * 255 > 255)
		{
			ImageHolder[0]->pixels->b = 255;
		}
		else
		{
			if (ImageHolder[0]->pixels->b * 255 < 0)
			{
				ImageHolder[0]->pixels->b = 0;
			}
		}


		cout << "New RGB values: " << endl;
		cout << "" << endl;
		cout << "	~ Red " << ImageHolder[0]->pixels->r * 255 << endl;
		cout << "	~ Green " << ImageHolder[0]->pixels->g * 255<< endl;
		cout << "	~ Blue " << ImageHolder[0]->pixels->b * 255 << endl;
		cout << "" << endl;

	}

};

// Brightness class
class Brightness
{

};

// Crop Class
class Crop
{

};

// Clean Up Class
class Clean
{
public:
	Clean()
	{
		cout << "Clean Constructor called" << endl;
	}

	void CleanUp()
	{
		for (int i = 0; i < 1; i++)
		{
			delete ImageHolder[0]->pixels;
			ImageHolder[0]->pixels = NULL;
		}
	}

};

int main()
{
	Load LoadClass;
	Save SaveClass;
	ColourChange ColourChangeClass;
	Brightness BrightnessClass;
	Crop CropClass;
	Clean CleanClass;

	cout << "" << endl;
	cout << "Hello and welcome to the Image manipulator Program" << endl;
	cout << "" << endl;
	cout << "In this program you can change: " << endl;
	cout << "	~ The Colour of the Image" << endl;
	cout << "	~ The Brightness of the Image" << endl;
	cout << "	~ You Crop the Image" << endl;

	LoadClass.FirstLoad();

	bool Running = true; // this variable is to keep the game running and will allow me to allow the user to end the program when they want.

	while (Running == true)// keeps the user coming back to the menu allowing them to use the application better.
	{
		cout << "" << endl;
		cout << "Menu:" << endl;
		cout << "" << endl;
		cout << "	~ Press 1 to Load a new Image" << endl;
		cout << "	~ Press 2 to Save the Image" << endl;
		cout << "	~ Press 3 to change the colour of the Image" << endl;
		cout << "	~ Press 4 to change the brightness of the Image" << endl;
		cout << "	~ Press 5 to crop the Image" << endl;
		cout << "	~ Press 6 to leave the Application" << endl;

		//cout << "	~ Press " << endl;

		cout << "" << endl;

		int Choice2;

		cin >> Choice2;

		cout << "" << endl;

		if (Choice2 == 1)
		{
			LoadClass.MenuLoad();
		}
		else
		{
			if (Choice2 == 2)
			{
				SaveClass.MenuSave();
			}
			else
			{
				if (Choice2 == 3)
				{
					ColourChangeClass.MenuColourChange();
				}
				else
				{
					if (Choice2 == 4)
					{
						//BrightnessClass
					}
					else
					{
						if (Choice2 == 5)
						{
							//CropClass
						}
						else
						{
							if (Choice2 == 6)
							{
								cout << "Thank you for using the application, press any button to close the application" << endl;
								Running = false;
							}
						}
					}
				}
			}
		}
	}

	cout << "GoodBye" << endl;

	int wait;
	cin >> wait;
}
