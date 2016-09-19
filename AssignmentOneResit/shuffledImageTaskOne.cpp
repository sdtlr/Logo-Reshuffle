#include"matrixHeader.h"
#include<iostream>
#include<Windows.h>
#include<cmath>
#include<fstream>
#include<string>

using namespace std;

void shuffledLogo();
void whereIsWally();
void shuffledLogoBinary();
string getCurrentDirectoryOnWindows();
double* readTXT(char* fileName, int sizeR, int sizeC);
void writePGM(char* fileName, Matrix& toWrite, int Q);

int main() // The main function is called and the user is given an option to start the image restoration or to exit the program.
{
	int startOrExit;

	cout << "Do you wish to commence the image restoration..." << endl ; // Outputs are displayed to the user to provide instruction.
	cout << "'1' to continue, '2' to exit....." << endl ;
	cout << "Answer: ";

	std::cin >> startOrExit; // Takes the users input and stores it in the variable startOrExit.
	cout << endl;

	switch (startOrExit) // Switch statement checks each case, if a case matches the user input the function is run or the program is exited.
	{
	case 1:
		cout << "Unshuffling the logo..." << endl;
		shuffledLogo();
		break;
	case 2:
		cout << "Thank you, Goodbye..." << endl;
		break;
	default:
		cout << "That is not a recognised input... please try again." << endl;
		break;
	}

	Sleep(500); // Sleep means user will see thank you message before console is closed.

	return 0; // Exits the program.
}

void shuffledLogo() // Unshuffle the logo stored in the .txt files, and save a completed version.
{
	// Const Declarations
	const int imageWidth = 512; // Full image is 512 x 512 pixels, this is defined in the imageWidth and imageHeight variables. This data should never change, it is therefore a constant variable.
	const int imageHeight = 512; // The image is made up of 256 blocks in a 16 x 16 grid.
	const int blockHeight = 32; // Each indiviual block is 32 x 32 pixels, this is defined in the blockWidth and blockHeight variables. This data should never change, it is therefore a constant variable.
	const int blockWidth = 32;
	// Matrix Declarations
	Matrix logoUnshuffled(imageHeight, imageWidth); // Create a matrix 512 x 512, using image height and width constants.

	// Scores used when calculating the sum of squared differences.
	double SSDScore = 0.0; // Init variable to store the current score.
	double SSDBestScore = 0.0; // Init variable to store best score, lower scores are better.
	int bestBlockStartX = 0; // Init variable to store the starting x co-ords of the best block.
	int bestBlockStartY = 0; // Init variable to store the starting y co-ords of the best block.

	// Shuffled Image
	char* imageDir = "\\Images\\Stock\\logo_shuffled.txt"; // Sets the directory of the .txt file containing the shuffled logo.
	double* imageTxt = readTXT(imageDir, imageHeight, imageWidth); // Uses the readTXT function to read the values stored in the text file and store them.
	Matrix logoShuffled(imageHeight, imageWidth, imageTxt);	// Populates the matrix with the values sotred in imageTxt.
	// Noisey Image
	imageDir = "\\Images\\Stock\\logo_with_noise.txt"; // Sets the directory of the .txt file containing the noisey logo.
	imageTxt = readTXT(imageDir, imageHeight, imageWidth); // Uses the readTXT function to read the values stored in the text file and store them.
	Matrix logoWithNoise(imageHeight, imageWidth, imageTxt); // Populates the matrix with the values sotred in imageTxt.

	// SSD Calculation
	for (int x = 0; x < imageHeight; x += blockHeight){ // For loop to loop through the blocks in the matrix vertically.
		for (int y = 0; y < imageWidth; y += blockWidth){ // For loop to loop through the blocks in the matrix horizontally.
			SSDBestScore = 100000000000000.0; // This variable is give a large number so that the first result that is calculated is always lower, and therefore stored in the SSDBestScore variable.

			Matrix blockNoisey(blockHeight, blockWidth); // Create a matrix 32 x 32, using block height and width constants. This matrix will hold blocks of the noisey logo.
			blockNoisey = logoWithNoise.getBlock(x, (x + blockWidth), y, (y + blockHeight)); // Populates the matrix. Gets a block from the matrix logoWithNoise.	

			for (int xx = 0; xx< imageHeight; xx += blockHeight){ // Cycles through the matrix on the x axis.
				for (int yy = 0; yy < imageWidth; yy += blockWidth){ // Cycles through the matrix on the y axis.
					Matrix subShuffled(blockHeight, blockWidth); // Create a matrix 32 x 32, using block height and width constants. This matrix will hold blocks of the shuffled logo.
					subShuffled = logoShuffled.getBlock(xx, xx + blockHeight, yy, yy + blockWidth);	// Populates the matrix with a block from the logoShuffled matrix.

					// Calculating the sum of squared differences.
					Matrix SSD(blockHeight, blockWidth); // Create a matrix 32 x 32, using block height and width constants.
					SSD = (blockNoisey - subShuffled); // Find the difference between the the nosey block and the shuffled block.
					SSD = SSD*SSD; // Multiply the difference by iteslefl to find the squared value.
					SSDScore = SSD.sum(); // Sum the squared values and store them in the sum of squared diffferences score variable.

					if (SSDScore < SSDBestScore){ // If SSD score is less than the SSD Best score, the scores must be replaced, as the best score is the lowest score.	
						SSDBestScore = SSDScore; // Replace the current SSD best score with the new SSD score.
						bestBlockStartX = xx; // Sets the best block x coords, so the block can be found.
						bestBlockStartY = yy; // Sets the best block y coords, so the block can be found.
					}
				}
			}
			int a = x;
			int b = y;
			for (int i = bestBlockStartX; i < (bestBlockStartX + blockHeight); i++){ // Inserting the best blocks into the final image (For loop to cycle through image.)
				for (int j = bestBlockStartY; j < (bestBlockStartY + blockWidth); j++){ // Looping throught the Y axis.
					double temp = logoShuffled.get(i, j); // Creates a temporary duoble to hold the suffled blocks.
					logoUnshuffled.set(a, b, temp); // Sets the block in the logo unshuffled matrix..
					b++; // Increments b for looping.
				}
				a++; // Incrememnts a for looping.
				b = y;
			}

		}
	}

	//Creates the pgm file so the result can be viewed.
	imageDir = "\\Images\\Processed\\sortedImage.pgm"; // Sets the image direcotry.
	cout << endl << endl << endl << "Final (unshuffled) image has been saved" << endl << "finalImage.pgm saved to: "; // Prints out user message to notify that the image has been saved.
	writePGM(imageDir, logoUnshuffled, 255); // Uses the writePGM function to save the matrix to an image file.

}


// Reading, wrirting and direcotry functions...


double* readTXT(char* fileName, int sizeR, int sizeC)
{
	double* input_data = new double[sizeR*sizeC];	//Double to store values contained in the text file
	int i = 0;
	string filePathStr2 = getCurrentDirectoryOnWindows() + fileName;
	char* filePath = new char[250];
	filePath = _strdup(filePathStr2.c_str());

	ifstream currentFile(filePath);

	if (currentFile.is_open())
	{
		while (currentFile.good()) //Populate input_data
		{
			if (i>sizeR*sizeC - 1) break;
			currentFile >> *(input_data + i);
			i++;
		}
		currentFile.close();
	}


	return input_data;
	delete[] input_data;

}
void writePGM(char* fileName, Matrix& toWrite, int Q)
{
	int x = toWrite.getM();
	int y = toWrite.getN();

	unsigned char *image;
	ofstream myfile;

	image = (unsigned char *) new unsigned char[x*y];



	// convert the integer values to unsigned char
	for (int i = 0; i<x; i++)
	{
		for (int j = 0; j<y; j++)
		{
			image[i*y + j] = (unsigned char)toWrite.get(i, j);
		}
	}

	string filePathStr2 = getCurrentDirectoryOnWindows() + fileName;
	char* filePath = new char[250];
	filePath = _strdup(filePathStr2.c_str());

	myfile.open(filePath, ios::out | ios::binary | ios::trunc);

	if (!myfile)
	{
		cout << "Can't open file: " << filePath << endl;
		exit(1);
	}

	//Necessary for PGM files, the length and width of image, etc
	myfile << "P5" << endl;
	myfile << y << " " << x << endl;
	myfile << Q << endl;

	myfile.write(reinterpret_cast<char *>(image), (x*y)*sizeof(unsigned char)); //Create image

	if (myfile.fail())
	{
		cout << endl << "Can't write image " << filePath << endl;
		exit(0);
	}
	else
	{
		cout << filePath;
	}

	myfile.close();

	delete[] image;
	delete[] filePath;
}
string getCurrentDirectoryOnWindows()
{
	const unsigned long maxDir = 260;
	char currentDir[maxDir];
	GetCurrentDirectory(maxDir, currentDir);
	return string(currentDir);
}
