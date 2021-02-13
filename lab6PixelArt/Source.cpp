#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <SFML/Graphics.hpp>
#include <iostream>
#include<fstream>
#include <stdlib.h> 
#include <time.h> 
#include <vector>
using namespace std;
using namespace sf;
const int zero = 0,one=1,two=2,three=3;
const int colms = 48, rows = 60;
const int gSquareSize = 25;
int index = 0,r=0,c=0,i=0;

const Color colorArray[] = { Color(103, 103, 103),Color::Blue,Color::Red,Color::Cyan,Color::Green };
RectangleShape gridPeace;
RectangleShape panel;
int grid[rows][colms] = {0};
RectangleShape gridDisplay[rows][colms];

vector<RectangleShape> mainPanels;
vector<RectangleShape> panels;
Vertex aLine[two];


void set_rectange(int width, int height, int x_poss, int y_poss) {//set's a Rectange properties
	panel.setPosition(x_poss, y_poss);
	panel.setSize(Vector2f(width, height));
	panel.setFillColor(colorArray[zero]);
}
void set_gridPeace(int x_poss, int y_poss) {//set's a Rectange properties
	gridPeace.setPosition(x_poss, y_poss);
	gridPeace.setSize(Vector2f(gSquareSize, gSquareSize));
	gridPeace.setFillColor(colorArray[zero]);
	gridPeace.setOutlineThickness(1.f);
	gridPeace.setOutlineColor(Color::Black);

}
void set_line(int s_poss_x, int s_poss_y, int e_poss_x, int e_poss_y) {//set's a lines properties
	aLine[zero] = Vertex(Vector2f(s_poss_x, s_poss_y));
	aLine[one] = Vertex(Vector2f(e_poss_x, e_poss_y));
	aLine[zero].color = Color(169, 169, 169);
	aLine[one].color = Color(169, 169, 169);
}
void saveToFile() {
	ofstream myFile("save.txt");
	if (myFile.is_open()) {

		for (int r = zero;r < rows;r++) {
			for (int c = zero;c < colms;c++) {
				if (grid[r][c]!=0) {
					myFile << grid[r][c] << ','<< r <<","<< c << ','<<'\n';
				}
			}
		}
		cout << "Saved\n";
	}
	else {
		cout << "unable to load save file\n";
	}
	
}
void loadFile() {
	string colorIndex="",row="",column="";
	
	ifstream myFile("save.txt"); 
		if(myFile.is_open()){

			while (getline(myFile, colorIndex,',')) {
					getline(myFile, row, ',');
					getline(myFile, column, ',');
					grid[stoi(row)][stoi(column)]= stoi(colorIndex);
					getline(myFile, colorIndex);
				


			}
	}
}

void createGrid(int xposs=400) {
	int pSize = 0;
	int yposs = 0;
	int temp = 0;
	xposs -= gSquareSize;

	for (r = 0;r < rows;r++) {
		yposs += pSize;
		temp = xposs;
		for (c = 0;c < colms;c++) {
			set_gridPeace(temp += gSquareSize, yposs);
			gridDisplay[r][c] = gridPeace;
		}
		pSize = gSquareSize;
	}

}


int main()
{
	int width = 2000, height = 1500;
	int colorSpaceW = width / 5, colorSpaceH = height / 4;//w=400 h=375
	int colorSpaceX = width - colorSpaceW;
	int leftPanelW = colorSpaceW;
	int gridW = width - (colorSpaceW + leftPanelW);//1200
	int mousePx = 0, mousePy = 0;
	int lPanelBlocksW = leftPanelW / 3;
	bool switched = false;
	const int numOfColourButtons = 4,numOfTools=4;
	const string tools[] = { "Rubber","Pencil","Picker","Rectangle"};


	//create Window

	RenderWindow window(VideoMode(width, height), "SFML Sample Code Lab 7");
	
	RectangleShape saveBtn,loadBtn;
	int num = 0;
	Font font;
	//arial.ttf must be in the same fodler as this code
	if (!font.loadFromFile("arial.ttf"))
	{
		// error...
	}

	Text saveText;
	saveText.setFont(font);
	saveText.setString(" Save ");
	saveText.setCharacterSize(24);
	saveText.setFillColor(Color::White);
	
	RectangleShape colourButtons[numOfColourButtons];

	for (int i = 0; i < numOfColourButtons; i++)
	{
		colourButtons[i].setSize(Vector2f(gSquareSize, gSquareSize));//set's acual collor buttons
		colourButtons[i].setPosition(colorSpaceX+ gSquareSize +(i* gSquareSize), gSquareSize);
		colourButtons[i].setFillColor(colorArray[i+1]);
	}

	set_rectange(colorSpaceW, colorSpaceH, colorSpaceX,zero);//draw color space
	mainPanels.push_back(panel);



	set_rectange(leftPanelW, height, zero, zero);//draw left panel
	mainPanels.push_back(panel);


	Texture pencilTexture;
	if (!pencilTexture.loadFromFile("pen.png"))//-----------need class array loop...
	{
		cout << "Pen image not able to load \n";
		return 0;
	}
	Sprite pencilButton;
	pencilButton.setTexture(pencilTexture);
	pencilButton.setPosition(leftPanelW / two + 5, gSquareSize);
	pencilButton.scale(0.25, 0.25);


	Texture rubberTexture;
	rubberTexture.loadFromFile("rubber.png");
	Sprite rubberButton;
	rubberButton.setTexture(rubberTexture);
	rubberButton.setPosition(gSquareSize * 2, gSquareSize);
	rubberButton.scale(0.25, 0.25);


	Texture pickerTexture;
	pickerTexture.loadFromFile("picker.png");
	Sprite pickerButton;
	pickerButton.setTexture(pickerTexture);
	pickerButton.setPosition(gSquareSize * 2, leftPanelW/two);
	pickerButton.scale(0.42, 0.42);

	Texture rectangleTexture;
	rectangleTexture.loadFromFile("rectangle.png");
	Sprite rectangleBtn;
	rectangleBtn.setTexture(rectangleTexture);
	rectangleBtn.setPosition(leftPanelW / two+10, (leftPanelW/two)-10);
	rectangleBtn.scale(0.074, 0.08);


	int possX = gSquareSize*2,possY= gSquareSize;
	for (int index = 1;index <= numOfTools; index++) {//create left panel small blocks
		if (index%two==one) {
			set_rectange(lPanelBlocksW, lPanelBlocksW, possX, possY);
			possX += lPanelBlocksW + gSquareSize;
		}
		else {
			set_rectange(lPanelBlocksW, lPanelBlocksW, possX, possY);
			possX -= lPanelBlocksW + gSquareSize;
			possY += lPanelBlocksW + gSquareSize;
		}
		panel.setFillColor(colorArray[zero]);
		panel.setOutlineThickness(2);//set color maybe later
		panels.push_back(panel);
	}

	Time timePerFrame = seconds(0.5f / 60.0f);
	Time timeSinceLastUpdate = Time::Zero;
	Clock clock;
	clock.restart();

	int currColorInteger=-1;
	int curToolIndex = -1;
	int gridRow = 0, gridColumn = 0;
	int rectangeX = 0, rectangeY = 0;
	string selectedTool="";
	multimap<int, int>rectXYmap;
	createGrid();
	while (window.isOpen())
	{
		 gridRow = 0, gridColumn = 0;

		// check if the close window button is clicked on.
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		timeSinceLastUpdate += clock.restart();

		if (timeSinceLastUpdate > timePerFrame)
		{
			if (Mouse::isButtonPressed(Mouse::Left)) {

				mousePx = Mouse::getPosition(window).x;
				mousePy = Mouse::getPosition(window).y;

				if (mainPanels[zero].getLocalBounds().contains(mousePx, mousePy)) {//check if left panel clicked on
					for (int i = 0;i < numOfTools;i++) {
						if (panels[i].getGlobalBounds().contains(mousePx, mousePy)) {
							selectedTool = tools[i];//gets selected tool keyword
							panels[i].setFillColor(Color::Blue);
							if (curToolIndex != -1 && i != curToolIndex) {
								panels[curToolIndex].setFillColor(colorArray[0]);

							}
							window.display();
							curToolIndex = i;
							break;
						}
					}
				}
				else if (saveBtn.getGlobalBounds().contains(mousePx,mousePy)) {
					saveToFile();
				}
				else if (loadBtn.getGlobalBounds().contains(mousePx, mousePy)) {
					loadFile();
				}

			}
			if (selectedTool == "Pencil" || selectedTool=="Rectangle") {

				if (Mouse::isButtonPressed(Mouse::Left))// if button clicked
				{
					mousePx = Mouse::getPosition(window).x;
					mousePy = Mouse::getPosition(window).y;
					if (switched) {//check if swtiched and set color to null(so it does not draw)
						currColorInteger = -1;
						switched = false;
					}
					for (int i = 0; i < numOfColourButtons; i++)//check number of blocks
					{
						if (colourButtons[i].getGlobalBounds().contains(mousePx, mousePy))
						{
							currColorInteger = i + 1;
							//colorSelected = true;
							break;

						}

					}

				}
			}
			else if (selectedTool == "Rubber") 
			{
				//currColor = colorArray[0];//dont change
				currColorInteger = 0;
				switched = true;
			}
			else if (selectedTool == "Picker")
			{
				if (switched) {//check if swtiched and set color to null(so it does not draw)
					currColorInteger = -1;
					switched = false;
				}
				if (Mouse::isButtonPressed(Mouse::Left))// if button clicked
				{
					mousePx = Mouse::getPosition(window).x;
					mousePy = Mouse::getPosition(window).y;

					if ((mousePx > leftPanelW && mousePx < colorSpaceX) && (mousePy > zero && mousePy < height)) {
							gridColumn = (mousePx - leftPanelW) / gSquareSize;
							gridRow = mousePy / gSquareSize;
							currColorInteger= grid[gridRow][gridColumn];
							selectedTool = "Pencil";
							curToolIndex = one;
							panels[one].setFillColor(Color::Blue);
							panels[two].setFillColor(colorArray[zero]);
							window.display();
					}

				}
			}
			if (selectedTool=="Rectangle")
			{
				if (switched) {//check if swtiched and set color to null(so it does not draw)
					currColorInteger = -1;
					switched = false;
				}
				rectXYmap.clear();//-------------------------continue here
				bool set = false;
				bool first=true;
				bool drawn = false;
				int orgColumn = 0, orgRow = 0;
				int newPossX=0, newPossY = 0;
				int prevColumn = 0, prevRow = 0;
				while (Mouse::isButtonPressed(Mouse::Left) && currColorInteger > -1)// while it is selected
				{
					mousePx = Mouse::getPosition(window).x;
					mousePy = Mouse::getPosition(window).y;
					if ((mousePx > leftPanelW && mousePx < colorSpaceX) && (mousePy > zero && mousePy < height)) {
						if (set == false) {
							orgColumn = (mousePx - leftPanelW) / gSquareSize;//original x column
							orgRow = (mousePy / gSquareSize); // original y column
							set = true;
						}
						prevColumn = gridColumn;
						prevRow = gridRow;

						gridColumn = (mousePx - leftPanelW) / gSquareSize;
						gridRow = mousePy / gSquareSize;
						cout << "Cur C,R: " << gridColumn << " " << gridRow << endl;//--------------------wrong They both the same;
						cout << "Prev C,R: " << prevColumn << " " << prevRow << endl;

						if (prevColumn == gridColumn && prevRow == gridRow) {
								rectXYmap.clear();
								drawn = true;
						}
						else {
							for (auto i = rectXYmap.begin();i != rectXYmap.end();i++) {

								gridDisplay[i->first][i->second].setFillColor(Color::Black);
								window.draw(gridDisplay[i->first][i->second]);
							}
							window.display();
							drawn = false;
						}
						newPossX = leftPanelW + gridColumn * gSquareSize;//calculate next column coppision
						newPossY = gridRow * gSquareSize;

						if (first) {
							set_gridPeace(leftPanelW + gridColumn * gSquareSize, gridRow* gSquareSize);//set Square at this location
							grid[gridRow][gridColumn] = currColorInteger;
							window.draw(gridPeace);
							window.display();
							first = false;
						}
						else if ((orgColumn != gridColumn || orgRow != gridRow)&& !drawn) {

							//set_gridPeace(newPossX, newPossY);//set Square at this location
							//grid[gridRow][gridColumn] = currColorInteger;
							//window.draw(gridPeace);
							//window.display();

							int cAcross = (orgColumn - gridColumn);//gives you how many columns need till 0 (back to the start)
							int rDown = (orgRow - gridRow);// gives you how many rows needed till 0(b to start)(use abs()back to 0)
							int temX=0;
							int temY=0;
							int inC = 1;
							int decRow = 1;
							if (cAcross<=0 && rDown<=0) {
								index = 0;
								for (int c = orgColumn;c <= gridColumn;c++) {
								
										//temX -= gSquareSize;
										//set_gridPeace(temX, newPossY);//set Square at this location
										//gridPeace.setFillColor(colorArray[currColorInteger]);
										//grid[gridRow][gridColumn - decColumn++] = currColorInteger;
										//gridDisplay[gridRow][gridColumn - decColumn++] = gridPeace;
										//gridColumn -= decColumn++;

										gridDisplay[orgRow][c].setFillColor(colorArray[currColorInteger]);
										window.draw(gridDisplay[orgRow][c]);

										//temY = gridColumn + inC++;
										gridDisplay[gridRow][c].setFillColor(colorArray[currColorInteger]);
										window.draw(gridDisplay[gridRow][c]);
										
										rectXYmap.insert({ orgRow,c });
										rectXYmap.insert({ gridRow,c });		
								}
								for (int r = orgRow;r <= gridRow; r++) {
										//temY -= gSquareSize;
										//set_gridPeace(newPossX, temY);//set Square at this location
										//gridPeace.setFillColor(colorArray[currColorInteger]);
										//grid[gridRow - decRow++][gridColumn] = currColorInteger;
										//window.draw(gridPeace);

										//temX = gridRow - decRow++;
										gridDisplay[r][orgColumn].setFillColor(colorArray[currColorInteger]);
										window.draw(gridDisplay[r][orgColumn]);
									
										gridDisplay[r][gridColumn].setFillColor(colorArray[currColorInteger]);
										window.draw(gridDisplay[r][gridColumn]);


										rectXYmap.insert({ r,orgColumn });
										rectXYmap.insert({ r,gridColumn });

									//rDown++;
									//cAcross++;

								}
								window.display();
								
							}
							cout << cAcross<<" "<< rDown<<'\n';


						}

					}


					
				}


			}
			if ((selectedTool == "Pencil" || selectedTool == "Rubber"))//when selected draw
			{
				
				while (Mouse::isButtonPressed(Mouse::Left))
				{
					mousePx = Mouse::getPosition(window).x;
					mousePy = Mouse::getPosition(window).y;
					if ((mousePx > leftPanelW && mousePx <colorSpaceX) && (mousePy > zero && mousePy < height)) {

						gridColumn = (mousePx - leftPanelW) / gSquareSize;
						gridRow = mousePy / gSquareSize;
						if (currColorInteger >= 0) {
							set_gridPeace(leftPanelW + gridColumn * gSquareSize, gridRow * gSquareSize);//set Square at this location
							gridPeace.setFillColor(colorArray[currColorInteger]);
							grid[gridRow][gridColumn] = currColorInteger;
							window.draw(gridPeace);
							window.display();
						}


					}
				}


			}
			window.clear();//----draw static things after here
			
			
			int dsiplayColor=0;

			for (int r = 0;r < rows;r++) {
				for (int c = 0;c < colms;c++) {
					dsiplayColor=grid[r][c];
					if (dsiplayColor!=0) {
						gridDisplay[r][c].setFillColor(colorArray[grid[r][c]]);
					}
					else {
						gridDisplay[r][c].setFillColor(colorArray[zero]);
					}
					window.draw(gridDisplay[r][c]);
				}
			}
			//--------------------------------------------
			/*
			
			int pSize = 0;
			int yposs = 0;
			int temp = 0;
			int xposs = leftPanelW;
			xposs -= gSquareSize;

			for (int r = 0;r < rows;r++) {
				yposs += pSize;
				temp = xposs;
				for (int c = 0;c < colms;c++) {
					set_gridPeace(temp += gSquareSize, yposs);
					gridPeace.setFillColor(colorArray[grid[r][c]]);
					window.draw(gridPeace);
				}
				pSize = gSquareSize;
			}
			*/
			//-------------------------------------------
			for (index = 0;index < mainPanels.size();index++) {//draw panels
				window.draw(mainPanels[index]);

			}

			for (index = 0;index < panels.size();index++) {//draw panels
				window.draw(panels[index]);

			}
			for (int i = 0; i < numOfColourButtons; i++)
			{
				window.draw(colourButtons[i]);
			}	
			set_rectange(100,50,100,height-200);
			panel.setFillColor(Color::Red);
			panel.setOutlineThickness(one);
			panel.setOutlineColor(Color::Black);
			saveBtn = panel;
			window.draw(saveBtn);

			set_rectange(100, 50, 200, height - 200);
			panel.setFillColor(Color::Blue);
			panel.setOutlineThickness(one);
			loadBtn = panel;
			window.draw(loadBtn);

			
			window.draw(pencilButton);
			window.draw(rubberButton);
			window.draw(pickerButton);
			window.draw(rectangleBtn);
			//window.draw(exampleText);
			window.display();

			// reset the timeSinceLastUpdate to 0 
			timeSinceLastUpdate = Time::Zero;
		}
	}

	return zero;
}
