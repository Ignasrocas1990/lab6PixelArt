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
const int zero = 0,one=1,two=2,three=3,four=4;
const int colms = 48, rows = 60;
const int gSquareSize = 25;
int index = 0,r=0,c=0,i=0;
const int numOfColourButtons = 21, numOfTools = 4;
const float tickness = 4.f;
Font font;
const Color colorArray[] = { Color::White,Color(153,0,26),Color(51,26,0),Color::Red,Color(255,179,191),Color(255,128,128),Color(255,85,0),Color::Yellow,Color(234,255,128),Color(191,230,0),Color::Green,Color(0,51,0),Color(0,204,102),Color(0,77,230),Color(0,204,204),Color::Blue,Color(69,0,204),Color(77,0,153),Color(204,0,204),Color(191,0,230),Color::Black };
RectangleShape colourButtons[numOfColourButtons];
vector<RectangleShape> mainPanels;
vector<RectangleShape> panels;
RectangleShape saveBtn, loadBtn, gridPeace, panel;
Texture saveTexture, loadTexture;
Text text;
int grid[rows][colms] = {0};
vector<Sprite> btns;
Sprite btn;

void set_rectange(int width, int height, int x_poss, int y_poss) {//set's a Rectange properties
	panel.setPosition(x_poss, y_poss);
	panel.setSize(Vector2f(width, height));
	panel.setFillColor(colorArray[zero]);
}
void set_text(String words,int x_poss,int y_poss) {//set text properties
	text.setFont(font);
	text.setString(words);
	text.setCharacterSize(40);
	text.setOutlineThickness(1);
	text.setPosition(x_poss,y_poss);
	text.setFillColor(Color::White);
}
void set_gridPeace(int x_poss, int y_poss) {//set's a Rectange properties
	gridPeace.setPosition(x_poss, y_poss);
	gridPeace.setSize(Vector2f(gSquareSize, gSquareSize));
	gridPeace.setFillColor(colorArray[zero]);
	gridPeace.setOutlineColor(Color::Black);
	gridPeace.setOutlineThickness(1);
}
void setColors(int poss_x) {//create's color blocks with colors
	int blockSize = 50;
	int side_poss = 0;
	int top_poss = gSquareSize*5;
	for (int i = 1; i < numOfColourButtons; i++)
	{
		int a = poss_x  + (side_poss+=blockSize+10);
		colourButtons[i].setSize(Vector2f(blockSize, blockSize));
		colourButtons[i].setFillColor(colorArray[i]);
		colourButtons[i].setPosition(a, top_poss);
		if (i % 5 == 0) {
			side_poss = 0;
			top_poss += 55;
		}
		side_poss++;
	}
}
void createToolsBtns(int lPanelBlocksW) {//create left panel tool blocks
	int possX = gSquareSize * 2, possY = gSquareSize*5;
	for (int index = 1;index <= numOfTools; index++) {
		if (index % two == one) {
			set_rectange(lPanelBlocksW, lPanelBlocksW, possX, possY);
			possX += lPanelBlocksW + gSquareSize;
		}
		else {
			set_rectange(lPanelBlocksW, lPanelBlocksW, possX, possY);
			possX -= lPanelBlocksW + gSquareSize;
			possY += lPanelBlocksW + gSquareSize;
		}
		panel.setFillColor(colorArray[zero]);
		panel.setOutlineThickness(2);
		panels.push_back(panel);
	}
}

void saveToFile() {//save entire grid to file(saved as: Color,row Possition,Column Possition)
	ofstream myFile("save.txt");
	if (myFile.is_open()) {

		for (int r = zero;r < rows;r++) {
			for (int c = zero;c < colms;c++) {
				myFile << grid[r][c] << ','<< r <<","<< c << ','<<'\n';
			}
		}
		cout << "Saved\n";
	}
	else {
		cout << "unable to load save file\n";
	}
	
}
void loadFile() {//loads from file
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
int importImages(string name,float scale_x,float scale_y,int poss_x,int poss_y) {//create's texture,add it to sprite

	Texture* texture = new Texture();
	texture->setSmooth(true);
	if (!texture->loadFromFile(name))
	{
		cout << "Image not able to load \n";
		return 0;
	}
	else {
		btn.scale(scale_x, scale_y);
		btn.setTexture(*texture);
		btn.setPosition(poss_x, poss_y);
		btns.push_back(btn);
	}
}

int saveAndload(int lPanelBlocksW,int height) {//set's up save and load buttons

	if (!saveTexture.loadFromFile("save.png"))return 0;
	if (!loadTexture.loadFromFile("load.png"))return 0;

	set_rectange(lPanelBlocksW, lPanelBlocksW, 70, height - 200);
	saveBtn = panel;
	saveBtn.setFillColor(Color::Green);
	saveBtn.setTexture(&saveTexture);
	saveBtn.setOutlineThickness(four);


	set_rectange(lPanelBlocksW, lPanelBlocksW, 230, height - 200);
	loadBtn = panel;
	loadBtn.setFillColor(Color::White);
	loadBtn.setTexture(&loadTexture);
	loadBtn.setOutlineThickness(four);
}

int main()
{
	int width = 2000, height = 1500;
	int colorSpaceW = width / 5, colorSpaceH = height / 4;
	int colorSpaceX = width - colorSpaceW;
	int leftPanelW = colorSpaceW;
	int gridW = width - (colorSpaceW + leftPanelW);
	int mousePx = 0, mousePy = 0;
	int lPanelBlocksW = leftPanelW / 3;
	bool switched = false;
	const string tools[] = { "Rubber","Pencil","Picker","Rectangle"};


	RenderWindow window(VideoMode(width, height), "Pixel Draw");
	
	if (!font.loadFromFile("arial.ttf")) cout << "Error on loading Font file\n";
	
	setColors(colorSpaceX);

	set_rectange(leftPanelW, height, zero, zero);//draw left panel
	panel.setFillColor(colorArray[20]);
	mainPanels.push_back(panel);

	set_rectange(colorSpaceW, colorSpaceH, colorSpaceX,zero);
	panel.setFillColor(colorArray[20]);
	mainPanels.push_back(panel);

	importImages("square.png", 0.04f, 0.04f, leftPanelW / two + 15, (colorSpaceH-80));
	importImages("picker.png", 4.8f, 4.8f, gSquareSize * 2+10, colorSpaceH-80);
	importImages("rubber.png", 1.3f, 1.3f, gSquareSize * 2, gSquareSize*5);
	importImages("pen.png", 1.f, 1.f, leftPanelW / two + 5, gSquareSize*5);

	saveAndload(lPanelBlocksW,height);

	createToolsBtns(lPanelBlocksW);

	

	Time timePerFrame = seconds(0.5f / 60.0f);
	Time timeSinceLastUpdate = Time::Zero;
	Clock clock;
	clock.restart();

	int currColorInteger=-1;
	int prevColorInt = -1;
	int curToolIndex = -1;
	int gridRow = 0, gridColumn = 0;
	int rectangeX = 0, rectangeY = 0;
	string selectedTool="";
	multimap<int, int>show_map;
	multimap<int, int>keep_map;
	int row = 0, column = 0;
	int cAcross=0, rDown=0;
	bool set = false,draw=true;
	int orgColumn = 0, orgRow = 0;
	int prevColumn = 0, prevRow = 0;


	while (window.isOpen())
	{
		 gridRow = 0, gridColumn = 0;

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

				if (mainPanels[zero].getGlobalBounds().contains(mousePx, mousePy)) {//check if left panel clicked on
					for (int i = 0;i < numOfTools;i++) {
						if (panels[i].getGlobalBounds().contains(mousePx, mousePy)) {
							selectedTool = tools[i];//gets selected tool keyword
							panels[i].setFillColor(Color::Blue);
							if (curToolIndex != -1 && i != curToolIndex) {
								panels[curToolIndex].setFillColor(colorArray[zero]);

							}
							window.display();
							curToolIndex = i;
							break;
						}
					}
					if (saveBtn.getGlobalBounds().contains(mousePx, mousePy)) {
						saveToFile();
					}
					else if (loadBtn.getGlobalBounds().contains(mousePx, mousePy)) {
						loadFile();
					}
				}
				

			}
			if (selectedTool == "Pencil" || selectedTool=="Rectangle") {

				if (Mouse::isButtonPressed(Mouse::Left))
				{
					mousePx = Mouse::getPosition(window).x;
					mousePy = Mouse::getPosition(window).y;
					if (switched) {
						currColorInteger = -1;
						switched = false;
					}

					for (int i = 1; i < numOfColourButtons; i++)
					{
						if (colourButtons[i].getGlobalBounds().contains(mousePx, mousePy))
						{
							if (currColorInteger !=-1 && i != currColorInteger) {

								colourButtons[currColorInteger].setOutlineThickness(0.0f);
								window.draw(colourButtons[currColorInteger]);

							}
							currColorInteger = i;
							colourButtons[i].setOutlineThickness(tickness);
							colourButtons[i].setOutlineColor(Color::White);
							window.draw(colourButtons[i]);
							
							break;

						}

					}

				}
			}
			else if (selectedTool == "Rubber") 
			{
				if (currColorInteger !=-1) {
					colourButtons[currColorInteger].setOutlineThickness(0.f);
					window.draw(colourButtons[currColorInteger]);
				}
				currColorInteger = 0;
				switched = true;
			}
			else if (selectedTool == "Picker")
			{
				if (currColorInteger!=-1) {
					colourButtons[currColorInteger].setOutlineThickness(0.f);
					window.draw(colourButtons[currColorInteger]);
					currColorInteger = -1;
					switched = false;
				}
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					mousePx = Mouse::getPosition(window).x;
					mousePy = Mouse::getPosition(window).y;

					if ((mousePx > leftPanelW && mousePx < colorSpaceX) && (mousePy > zero && mousePy < height)) {
							gridColumn = (mousePx - leftPanelW) / gSquareSize;
							gridRow = mousePy / gSquareSize;
							currColorInteger= grid[gridRow][gridColumn];
							if (currColorInteger != 0) {
								colourButtons[currColorInteger].setOutlineThickness(tickness);
								colourButtons[currColorInteger].setOutlineColor(Color::White);
								window.draw(colourButtons[currColorInteger]);
								selectedTool = "Pencil";
								curToolIndex = one;
								panels[one].setFillColor(Color::Blue);
								panels[two].setFillColor(colorArray[zero]);
								window.display();
							}
							else {
								currColorInteger = -1;
							}

						
					}

				}
			}
			if (selectedTool=="Rectangle")
			{
				if (switched) {//check if swtiched and set color to null(so it does not draw)
					colourButtons[currColorInteger].setOutlineThickness(tickness);
					colourButtons[currColorInteger].setOutlineColor(Color::White);
					window.draw(colourButtons[currColorInteger]);
					currColorInteger = -1;
					switched = false;
				}
				show_map.clear();
				keep_map.clear();
				set = false;
				draw = true;
				orgColumn = 0, orgRow = 0;
				prevColumn = 0, prevRow = 0;

				while (Mouse::isButtonPressed(Mouse::Left) && currColorInteger > -1)
				{
					mousePx = Mouse::getPosition(window).x;
					mousePy = Mouse::getPosition(window).y;
					if ((mousePx > leftPanelW && mousePx < colorSpaceX) && (mousePy > zero && mousePy < height)) {

						if (set == false) {
							orgColumn = (mousePx - leftPanelW) / gSquareSize;//original x column
							orgRow = (mousePy / gSquareSize); // original y column
							set = true;

							set_gridPeace(leftPanelW + orgColumn * gSquareSize, orgRow* gSquareSize);
							grid[orgRow][orgColumn] = currColorInteger;
							gridPeace.setFillColor(colorArray[currColorInteger]);
							window.draw(gridPeace);
						}

						gridColumn = (mousePx - leftPanelW) / gSquareSize;
						gridRow = mousePy / gSquareSize;

						if (prevColumn == gridColumn && prevRow == gridRow) {
								
								draw = false;

						}
						else{
							prevColumn = gridColumn;
							prevRow = gridRow;
							show_map.insert(keep_map.begin(), keep_map.end());
							for (auto i = show_map.begin();i != show_map.end();i++) {

								set_gridPeace(leftPanelW + i->second * gSquareSize, i->first * gSquareSize);
								gridPeace.setFillColor(colorArray[zero]);
								window.draw(gridPeace);
							}
							keep_map.clear();
							draw = true;
						}


						if ((orgColumn != gridColumn || orgRow != gridRow)&& draw==true) {
							
							cAcross = (orgColumn - gridColumn);//gives you how many columns need till 0 (back to the start)
							rDown = (orgRow - gridRow);// gives you how many rows needed till 0(b to start)(use abs()back to 0)

							if (cAcross <= zero) {//right & (up || down)

								for (column = orgColumn+1;column <= gridColumn;column++) {//right
									if (grid[orgRow][column] == 0) {

										set_gridPeace(leftPanelW + column * gSquareSize, orgRow * gSquareSize);
										gridPeace.setFillColor(colorArray[currColorInteger]);
										window.draw(gridPeace);
										keep_map.insert({ orgRow,column });
									}
									if(grid[gridRow][column] == 0){
										set_gridPeace(leftPanelW + column * gSquareSize, gridRow * gSquareSize);
										gridPeace.setFillColor(colorArray[currColorInteger]);
										window.draw(gridPeace);
										keep_map.insert({ gridRow,column });
									}
									
								}
								if (rDown > zero) {
									for (row = orgRow;row >= gridRow; row--) {//up
										if (grid[row-1][orgColumn] == 0) {

										set_gridPeace(leftPanelW + orgColumn * gSquareSize, row * gSquareSize);
										gridPeace.setFillColor(colorArray[currColorInteger]);
										window.draw(gridPeace);
										keep_map.insert({ row,orgColumn });

										}
										if (grid[row][gridColumn] == 0) {
											set_gridPeace(leftPanelW + gridColumn * gSquareSize, row * gSquareSize);
											gridPeace.setFillColor(colorArray[currColorInteger]);
											window.draw(gridPeace);

											keep_map.insert({ row,gridColumn });
										}
									}
								}
								else {
									for (row = orgRow;row <= gridRow; row++) {//down
										if (grid[row+1][orgColumn] == 0) {
											set_gridPeace(leftPanelW + orgColumn * gSquareSize, row * gSquareSize);
											gridPeace.setFillColor(colorArray[currColorInteger]);
											window.draw(gridPeace);
											keep_map.insert({ row,orgColumn });
										}
										if (grid[row][gridColumn] == 0) {
											set_gridPeace(leftPanelW + gridColumn * gSquareSize, row * gSquareSize);
											gridPeace.setFillColor(colorArray[currColorInteger]);
											window.draw(gridPeace);

											keep_map.insert({ row,gridColumn });
										}
									}
								}
							}
							else if (cAcross > zero) { //left & (down || UP )

								for (column = orgColumn-1;column > gridColumn;column--) {

									if (grid[orgRow][column] == 0) {
										set_gridPeace(leftPanelW + column * gSquareSize, orgRow * gSquareSize);
										gridPeace.setFillColor(colorArray[currColorInteger]);
										window.draw(gridPeace);
										keep_map.insert({ orgRow,column });
									}
									if (grid[gridRow][column] == 0) {
										set_gridPeace(leftPanelW + column * gSquareSize, gridRow * gSquareSize);
										gridPeace.setFillColor(colorArray[currColorInteger]);
										window.draw(gridPeace);
										keep_map.insert({ gridRow,column });
									}
								}
								if (rDown <= zero) {
									for (row = orgRow;row <= gridRow; row++) {//down
										if (grid[row+1][orgColumn] == 0) {
											set_gridPeace(leftPanelW + orgColumn * gSquareSize, row * gSquareSize);
											gridPeace.setFillColor(colorArray[currColorInteger]);
											window.draw(gridPeace);
											keep_map.insert({ row,orgColumn });
										}
										if (grid[row][gridColumn] == 0) {
											set_gridPeace(leftPanelW + gridColumn * gSquareSize, row * gSquareSize);
											gridPeace.setFillColor(colorArray[currColorInteger]);
											window.draw(gridPeace);
											keep_map.insert({ row,gridColumn });
										}
									}
								}
								else {
									for (row = orgRow;row >= gridRow; row--) {//up

										if (grid[row][orgColumn-1] == 0) {
											set_gridPeace(leftPanelW + orgColumn * gSquareSize, row * gSquareSize);
											gridPeace.setFillColor(colorArray[currColorInteger]);
											window.draw(gridPeace);
											keep_map.insert({ row,orgColumn });
										}
										if (grid[row][gridColumn] == 0) {
											set_gridPeace(leftPanelW + gridColumn * gSquareSize, row * gSquareSize);
											gridPeace.setFillColor(colorArray[currColorInteger]);
											window.draw(gridPeace);
											keep_map.insert({ row,gridColumn });
										}
										}
									}
							}
							window.display();
							draw = false;
						}

					}


					
				}
				for (auto index = keep_map.begin();index != keep_map.end();index++) {

					grid[index->first][index->second] = currColorInteger;
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
			window.clear();

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
			
			for (index = 0;index < mainPanels.size();index++) {//draw panels
				window.draw(mainPanels[index]);

			}
			for (index = 0;index < panels.size();index++) {//draw panels
				window.draw(panels[index]);

			}
			for (i = 0; i < numOfColourButtons; i++)
			{
				window.draw(colourButtons[i]);
			}	

			window.draw(saveBtn);
			window.draw(loadBtn);

			for (auto index = btns.begin();index != btns.end();index++) {
				window.draw(*index);
			}
			set_text("Drawing Tools", leftPanelW / 5, gSquareSize);
			text.setOutlineColor(Color::Red);
			window.draw(text);

			set_text("Colors", colorSpaceX+leftPanelW/3, gSquareSize);
			text.setOutlineColor(Color::Red);
			window.draw(text);

			set_text("Save", 90, height-250);
			text.setOutlineColor(Color::Green);
			window.draw(text);

			set_text("Load", 250, height-250);
			text.setOutlineColor(Color::Blue);
			window.draw(text);



			window.display();
			timeSinceLastUpdate = Time::Zero;
		}
	}

	return zero;
}
