//============================================================================
// Name        : .cpp
// Author      : Dr. Sibt Ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game...
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include<iostream>
#include<fstream>
#include<string>
#include<cmath> // for basic math functions such as cos, sin, sqrt

using namespace std;

// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

class position{
	int xI ;
	int yI;
public:
	position():xI(400),yI(400){}
	int getXI()
	{
		return xI;
	}
	int getYI()
	{
		return yI;
	}
	void setX(int x)
	{
		xI = x;
	}
	void setY(int y)
	{
		yI = y;
	}
};
class Car {
	private:
		int x, y;               // For Position
		int width, height;      // Size of car 
		bool isTaxi;           // color
		float fuel;           // Will display the fuel of the car
		float* color;
	public:
		Car(int startX, int startY, bool istaxi, int w = 40, int h = 15 )            // constructor for car
		: x(startX), y(startY), width(w), height(h), isTaxi(istaxi), fuel(100.0){
			if(isTaxi)
				color = colors[YELLOW];
			else
				color = colors[RED];
			}
	
		void draw() const 
		{                                               // Will display user's car
			DrawCircle(x + 10, y + 10, 7, colors[BLACK]);
			DrawCircle(x + width - 10, y  + 10, 7, colors[BLACK]);
	
			DrawRectangle(x, y + 15, width, height , color);        
			DrawRectangle(x + 10, y + height + 15, width - 20, 15, colors[INDIAN_RED]);
			// Windows
			DrawRectangle(x + 13, y + height + 3 + 15, 15, 10, colors[SKY_BLUE]);
			DrawRectangle(x + width - 28, y + height + 3 + 15, 15, 10, colors[SKY_BLUE]);
			// Door line
			DrawLine(x + width / 2, y + 15, x + width / 2, y + height + 15, 2, colors[GRAY]);
		}
		void move(int dx, int dy) 
		{
			// Updating position of the user's car
			x += dx;
			y += dy;
			
			// Decreasing the  fuel
			fuel -= 0.1;
			
			// Keep car within bounds
			if (x < 0) x = 0;
			if (y < 0) y = 0;
			if (x > 1020 - width) x = 1020 - width;
			if (y > 840 - height) y = 840 - height;
		}
		int getX() const 
		{
			 return x;
		}
		int getY() const 
		{
			 return y; 
		}
		float getFuel() const 
		{
			 return fuel; 
		}
		void refuel(float amount) 
		{
			 fuel = min(100.0f, fuel + amount); 
		}
	};

class obstacles{            //Obstacles cars
 public:                    //Abstract class
		virtual void draw() const = 0;
		virtual ~obstacles(){}
};

class building : public obstacles{        //Building Class inherited from obstacles
		int blockwidth;
		int blockheight;
		int row;
		int col;
		int x;
		int y;
		int width;
		int height;
 public:
	building(int r , int c , int screenWidth = 1020, int screenHeight = 840){       
		row = r;
		col = c;
		blockwidth = screenWidth / 18;
		blockheight = screenHeight/ 14;
	
		
		
	
			x = c * blockwidth;
			y = r * blockheight; 
		}
		void draw()const override
		{
			
			DrawRectangle(x, y, blockwidth, blockheight, colors[BLACK]);
			
			// Roof stripe
			DrawRectangle(x, y, blockwidth, 5, colors[GRAY]);
	
			// Windows - optional
			DrawRectangle(x + 5, y + 5, 5, 5, colors[WHITE]);
			DrawRectangle(x + blockwidth - 10, y + 5, 5, 5, colors[WHITE]);
			DrawRectangle(x + 5, y + blockheight - 10, 5, 5, colors[WHITE]);
			DrawRectangle(x + blockwidth - 10, y + blockheight - 10, 5, 5, colors[WHITE]);
		
		}
};

class tree: public obstacles{
	int x;                              //Tree Class inherited from obstacles
	int y;
	int width;
	int height;
 public:
	tree(int x1 , int y1 , int w , int h){
		x = x1;
		y = y1;
		width = w;
		height = h;
	}
	void draw() const override
	{
		DrawCircle(x + width / 2, y + height / 2 + 3, min(width, height) / 4, colors[GREEN]);
        DrawRectangle(x + width / 2 - 2, y, 4, height / 4 + 5, colors[BROWN]);
	}
};
class ObstacleCar : public obstacles {              //Obstacle Car class inherited from obstacles
    int direction;     // 0=up, 1=right, 2=down, 3=left
    int tickCount;     // to slow down movement
    int maxTicks;      // how many ticks before actual move
	int x;
	int y;
	int width;
	int height;
	float* color;	
 public:
    ObstacleCar(int r, int c, int w, int h, float* clr = colors[GREEN]): obstacles() {
        x = c * w;
        y = r * h;
		color = clr;
        width = w;
        height = h;
        direction = rand() % 4;
        tickCount = 0;
        maxTicks = 20; // higher = slower
    }

    void move(int** grid, int rows, int cols, int blockW, int blockH) {
        tickCount++;
        if (tickCount < maxTicks) return;
        tickCount = 0;

        int currentR = y / blockH;
        int currentC = x / blockW;
        int nextR = currentR;
        int nextC = currentC;

        switch (direction) {
            case 0: // Up
                nextR++;
                break;
            case 1: // Right
                nextC++;
                break;
            case 2: // Down
                nextR--;
                break;
            case 3: // Left
                nextC--;
                break;
        }
        //checking if the next position is valid

       
        if (nextR >= 0 && nextR < rows && nextC >= 0 && nextC < cols && 
            grid[nextR][nextC] == 0) {
            
            grid[currentR][currentC] = 0;
            
            // Moving to new position
            x = nextC * blockW;
            y = nextR * blockH;
            
            // Updating the grid to store the new position
            grid[nextR][nextC] = 6;
        } else {
            // Changing the direction if can't move
            direction = (direction + 1) % 4;
        }
    }
    
    void draw() const override 
	{
    int centerX = x + width / 2;
    int centerY = y + height / 2;

    int carWidth = width * 0.7;   
    int carHeight = height * 0.5; 

    int topLeftX = centerX - carWidth / 2;
    int topLeftY = centerY - carHeight / 2;

    // Car body (main base rectangle)
    DrawRectangle(topLeftX, topLeftY, carWidth, carHeight - 10, color);
    DrawRectangle(topLeftX + 5, topLeftY + carHeight - 13, carWidth - 10, carHeight / 2, colors[BLUE]);         // Roof (drawn *above* the body)
 
    DrawCircle(topLeftX + 8, topLeftY - 4, 6, colors[BLACK]);                      // Left wheel
    DrawCircle(topLeftX + carWidth - 8, topLeftY - 4, 6, colors[BLACK]);           // Right wheel
    DrawRoundRect(topLeftX + 2, topLeftY + carHeight, carWidth - 4, 5, colors[MISTY_ROSE], 4);
	}

    int getX() const { return x; }
    int getY() const { return y; }
};

class Passenger : public obstacles {   // Passenger class inherited from obstacles
    int x, y, destX, destY;
    bool active;
 public:
    Passenger(int r, int c, int blockW, int blockH) {   // Constructor for passenger
        x = c * blockW;
        y = r * blockH;
        active = true;
        destX = -1;
        destY = -1;
    }
    void setDestination(int r, int c, int blockW, int blockH) 
	{
        destX = c * blockW;
        destY = r * blockH;
    }
    void draw() const override 
	{
        if (active) {
            DrawCircle(x + 20, y + 30, 8, colors[BLUE]); 
            DrawLine(x + 20, y, x + 20, y + 25, 3, colors[BLUE]);
            DrawLine(x + 10, y + 15, x + 30, y + 15, 3, colors[BLUE]);
        }
        // Only show destination if passenger is picked up
        if (!active && destX != -1 && destY != -1) {
            DrawRectangle(destX, destY, 20, 20, colors[GREEN]); // Destination
        }
    }
    bool isActive() const 
	{
		 return active; 
	}
    void deactivate() 
	{
		 active = false; 
	}
    int getX() const 
	{
		 return x; 
	}
    int getY() const 
	{
		 return y; 
	}
    int getDestX() const 
	{
		 return destX;
	}
    int getDestY() const 
	{
		 return destY;
	}
};
class DeliveryPoint : public obstacles {          // Delivery Point class inherited from obstacles
    int x, y;
    bool hasPackage;
 public:

    DeliveryPoint(int r, int c, int blockW, int blockH) {
        x = c * blockW;
        y = r * blockH;
        hasPackage = true;
    }
    void draw() const override
    {
        if (hasPackage) 
        {
            DrawRectangle(x, y, 20, 20, colors[GREEN]);
        }
        // Only show destination if package is picked up
        if (!hasPackage ) 
        {
            DrawRectangle(x, y, 20, 20, colors[RED]); // Destination
        }
    }
    bool hasPackageAvailable() const 
	{
		 return hasPackage; 
	}
    void pickPackage() 
	{
		 hasPackage = false; 
	}

    void resetPackage() 
	{
		 hasPackage = true; 
	}
    int getX() const 
	{
		 return x; 
	}
    int getY() const 
	{
		 return y; 
	}
};

class FuelStation : public obstacles {            // Fuel Station class inherited from obstacles
    int x, y;

 public:
    FuelStation(int r, int c, int blockW, int blockH) {
        x = c * blockW;
        y = r * blockH;
    }

    void draw() const override 
	{
        DrawRectangle(x, y, 40, 40, colors[RED]);
    }
    int getX() const 
	{ 
		return x; 
	}
    int getY() const 
	{
		 return y; 
	}
};
class Board {                                                // Board class
		int **grid;                                         // 0 = Road, 1=Building, 2 = Trees, 3 = passengers ,
		int blockW;											// 4 = Dilivery points , 5 = Fuel Station etc.
		int blockH;
		int rows;
		int cols;
		obstacles* obs[100];
		int obstacleCount;
		char change;
		ObstacleCar* obstacleCar[10];
		int obstacleCarCount;
		Passenger* passengers[3];
		int passengerCount;
		FuelStation* fuelStations[2];
		int fuelStationCount;
		DeliveryPoint* deliveryPoints[2];
		int DeliveryPointCount;
		int garageRow;
		int garageCol;

 public:
		Board(int screenWidth = 1020, int screenHeight = 840) {   //Board constructor
			rows = 14;
			cols = 18;
			grid = new int*[rows];
			for(int i = 0 ; i < rows ; i++)
			{
				grid[i] = new int[cols];
			}
			for(int i = 0; i < rows; ++i) {
				for(int j = 0; j < cols; ++j) {
					grid[i][j] = 0; 
				}
			}
			garageRow = 13;
        	garageCol = 0;
			grid[garageRow][garageCol] = 7;
			blockW = screenWidth / cols;
			blockH = screenHeight / rows;
			obstacleCount = 0;
			passengerCount = 0;
			fuelStationCount = 0;
			DeliveryPointCount = 0;
			obstacleCarCount = 0;
			change = '\0';
			placeBuildings(55);
			placeTree(25);
			placeObstacleCar(10);
			placePassengers(3);
			placeFuelStations(2);
			placeDeliveryPoints(2);
		
		}
		~Board(){
			for (int i = 0; i < obstacleCount; i++)   //Destructor
            {
				    delete obs[i];
			}
			for (int i = 0; i < obstacleCarCount; i++)   //deleting the obstacle cars
            {
				delete obstacleCar[i];
			}
			for (int i = 0; i < passengerCount; i++)   //deleting the passengers
            {
				delete passengers[i];
			}
			for (int i = 0; i < fuelStationCount; i++)   //deleting the fuel stations
            {
				delete fuelStations[i];
			}
			for (int i = 0; i < DeliveryPointCount; i++)   //deleting the delivery points
            {
				delete deliveryPoints[i];
			}
			for (int i = 0; i < rows; i++)   //deleting the grid
            {
				delete[] grid[i];
			}
			delete[] grid;   //deleting the grid
		}
	
		void placeBuildings(int count)   //Placing the buildings
		{
			int placed = 0;
			const int max = 1000;
			int attempts = 0;
			while(placed < count && attempts < max)      //Placing the buildings
			{
				int r = rand() % rows;
				int c = rand() % cols;
				if(grid[r][c] == 0 && !(r == garageRow && c == garageCol))   //Placing the buildings
				{
				obs[obstacleCount++] = new building(r,c,1020 , 840);
				grid[r][c] = 1;
				cout<<"Building : "<<r<<"  "<<c<<endl;
				placed++;
				}
				attempts++;     
			}
		}
		void placeTree(int count)  //Placing the trees
		{
			int placed = 0;
			const int max = 1000;
			int attempts = 0;
			while(placed<count && attempts < max)
			{
				int r = rand() % rows;
				int c = rand() % cols;
				int pixelX = c * blockW; // Calculate pixel coordinates
            	int pixelY = r * blockH;
				if(grid[r][c] == 0 && !(r == garageRow && c == garageCol))
				{
					obs[obstacleCount++] = new tree(pixelX,pixelY,blockW , blockH);
					grid[r][c] = 2;
					cout<<"Tree : "<<r<<"  "<<c<<endl;
					placed++;
				}
				attempts++;
				
			}			
		}
		void placeObstacleCar(int count)  //Placing the obstacle cars
		{
			int placed = 0;
			const int max = 500;
			int attempts = 0;
			while(placed < count && attempts < max)
			{
				int r = rand() % rows;
				int c = rand() % cols;
				if(grid[r][c] == 0 && !(r == garageRow && c == garageCol))
				{ 
				obstacleCar[obstacleCarCount++] = new ObstacleCar(r, c, blockW, blockH);
				placed++;
				grid[r][c] = 6;
				}
				attempts++;
		
			}
		}
		void placePassengers(int count)
		{
			int placed = 0, attempts = 0, max = 2000;
			while (placed < count && attempts < max) 
            {
				int r = rand() % rows;
				int c = rand() % cols;
				if (grid[r][c] == 0 && !(r == garageRow && c == garageCol))
				{
					passengers[passengerCount] = new Passenger(r, c, blockW, blockH);
					int destR, destC;
					do {
						destR = rand() % rows;
						destC = rand() % cols;
					} while (grid[destR][destC] != 0 || (destR == r && destC == c));
					passengers[passengerCount]->setDestination(destR, destC, blockW, blockH);
					grid[r][c] = 3;
					passengerCount++;
					placed++;
				}
				attempts++;
			}
		}
		void placeDeliveryPoints(int count)  //Placing the delivery points
		{
			int placed = 0;
			int attempts = 0;
			int max = 2000;
			while (placed < count && attempts < max)
			{
				int r = rand() % rows;
				int c = rand() % cols;
				if (grid[r][c] == 0 && !(r == garageRow && c == garageCol)) 
                {
					deliveryPoints[DeliveryPointCount++] = new DeliveryPoint(r, c, blockW, blockH);
					grid[r][c] = 4;
					placed++;
				}
				attempts++;
			}
		}
		void placeFuelStations(int count) 
		{
			int placed = 0;
			int attempts = 0;
			int max = 1000;
			while (placed < count && attempts < max) 
			{
				int r = rand() % rows;
				int c = rand() % cols;
				if (grid[r][c] == 0 && !(r == garageRow && c == garageCol)) 
				{
					fuelStations[fuelStationCount++] = new FuelStation(r, c, blockW, blockH);
					grid[r][c] = 5;
					placed++;
				}
				attempts++;
			}
		}
		
		void moveObstacleCars()  //Moving the obstacle cars
		{
			for (int i = 0; i < obstacleCarCount; i++) 
            {
				if (obstacleCar[i]) 
                {
					obstacleCar[i]->move(grid, rows, cols, blockW, blockH);  //Moving the obstacle cars
				}
			}
		}
		
		void draw(bool showingTaxiMode) const  //Drawing the board
		{	
			// Draw obstacles first (both in taxi and delivery mode)
			for (int i = 0; i < obstacleCount; ++i) 
            {
				if (obs[i]) 
                {
					obs[i]->draw();
				}
			}
			
			// Draw obstacle cars
			for (int i = 0; i < obstacleCarCount; ++i) 
            {
				if (obstacleCar[i]) 
                {
					obstacleCar[i]->draw();
				}
			}
			
    
			if (showingTaxiMode)   //Drawing the passengers
            {
				for (int i = 0; i < passengerCount; ++i) 
                {
					if (passengers[i]) 
                    {
						passengers[i]->draw();
					}
				}
			}
			
			
			if (!showingTaxiMode)   //Drawing the delivery points
            {
				for (int i = 0; i < DeliveryPointCount; ++i) 
                {
					if (deliveryPoints[i]) 
                    {
						deliveryPoints[i]->draw();
					}
				}
			}
			
			
			for (int i = 0; i < fuelStationCount; ++i) 
            {
				if (fuelStations[i]) 
                {
					fuelStations[i]->draw();   //Drawing the fuel stations
				}
			}
			
			for(int i = 0; i <= rows; i++) 
            {
				DrawLine(0, i*blockH, 1020, i*blockH, 2, colors[GRAY]);
			}
			
			for(int j = 0; j <= cols; j++) 
            {
				DrawLine(j*blockW, 0, j*blockW, 840, 2, colors[GRAY]);
			}
			
			DrawRectangle(0, 0, blockW, blockH - 10, colors[GRAY]);   //Drawing the garage
		}

		int getGridValue(int r, int c) const     //Getting the grid value
		{
			if (r >= 0 && r < rows && c >= 0 && c < cols) 
				return grid[r][c];
			return -1;
		}
		Passenger* getPassenger(int index)   //Getting the passenger
		{
			 return passengers[index];
		}
		int getPassengerCount() const   //Getting the passenger count
		{
			 return passengerCount;
		}
		DeliveryPoint* getDeliveryPoint(int index)   //Getting the delivery point
		{
			 return deliveryPoints[index]; 
		}
		int getDeliveryPointCount() const    //Getting the delivery point count
		{
			 return DeliveryPointCount; 
		}
		FuelStation* getFuelStation(int index)   //Getting the fuel station
		{
			 return fuelStations[index]; 
		}
		int getFuelStationCount() const    //Getting the fuel station count
		{
			 return fuelStationCount; 
		}
		ObstacleCar* getOtherCar(int index)   //Getting the other car
		{
			 return obstacleCar[index]; 
		}
		int getObstacleCarCount() const    //Getting the obstacle car count
		{
			 return obstacleCarCount; 
		}
	bool isRoad(int r, int c) const   //Checking if the road is valid
	{
			return r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] == 0;
	}
    void resetPassenger(int index, int r, int c)   //Resetting the passenger
	 {
        delete passengers[index];
        passengers[index] = new Passenger(r, c, blockW, blockH);
        int destR, destC;
        do 
        {
            destR = rand() % rows;
            destC = rand() % cols;
        } while (grid[destR][destC] != 0 || (destR == r && destC == c));
        passengers[index]->setDestination(destR, destC, blockW, blockH);
        grid[r][c] = 3;
    }
	void addOtherCar()              //Adding other cars
    {
        if (obstacleCarCount < 10) 
        {
            int r, c, attempts = 0, max = 2000;
            do 
            {
                r = rand() % rows;
                c = rand() % cols;
                attempts++;
            } while (grid[r][c] != 0 && attempts < max || (r == garageRow && c == garageCol));
            if (grid[r][c] == 0) 
            {
                obstacleCar[obstacleCarCount++] = new ObstacleCar(r, c, blockW, blockH);
                grid[r][c] = 6;
            }
        }
    }
	
	
};
enum GameState { MENU, PLAYING, GAME_OVER, LEADERBOARD, ROLE_MENU };
enum Role { TAXI, DELIVERY };

struct HighScore{
    char name[20];
    int score;
};

class Game {
    GameState state;
    Role role;
    Board* board;
    Car* playerCar;
    char playerName[20];
    int score;
	int earnings;
	int tasksCompleted;
    float gameTime;
    bool hasPassenger;
	bool hasPackage;
    int passengerIndex;
	int destX;
	int destY;
    HighScore highScores[10];
    int highScoreCount;
    char inputBuffer[20];
    int inputLength;
    bool isGameOver;

public:
    Game(): state(MENU), role(TAXI), score(0), earnings(0), tasksCompleted(0), gameTime(0), 
             hasPassenger(false), hasPackage(false), passengerIndex(-1), destX(-1), destY(-1), 
             highScoreCount(0), inputLength(0), isGameOver(false) {      //Constructor for the game
        board = new Board();
        playerCar = new Car(0, 13 * 840 / 14, true); // Top-left
        memset(playerName, 0, 20);
        memset(inputBuffer, 0, 20);
        loadHighScores();
    }
    ~Game() {                                             //Destructor for the game
        delete board;
        delete playerCar;
    }
    void draw() 
    {
        glClearColor(1, 1, 1, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (state == GAME_OVER) {                          //Drawing the game over screen
            char buffer[100];
            sprintf(buffer, "Game Over! Score: %d", score);
            DrawString(450, 500, buffer, colors[BLACK]);
            DrawString(400, 450, "Enter Name: ", colors[BLACK]);
            DrawString(520, 450, inputBuffer, colors[BLACK]);
            DrawString(400, 400, "Press ENTER to save", colors[BLACK]);
        } else if (state == MENU)                          //Drawing the menu screen
		{
            DrawString(400, 600, "Rush Hour Game", colors[BLACK]);
            DrawString(400, 500, "1. Play as Taxi Driver", colors[BLACK]);
            DrawString(400, 450, "2. Play as Delivery Driver", colors[BLACK]);
            DrawString(400, 400, "3. Random Role", colors[BLACK]);
            DrawString(400, 350, "4. View Leaderboard", colors[BLACK]);
            DrawString(400, 300, "Press 1-4", colors[BLACK]);
        } 
		else if (state == LEADERBOARD)                      //Drawing the leaderboard screen
		{
            DrawString(400, 600, "Leaderboard", colors[BLACK]);
            for (int i = 0; i < highScoreCount; i++) 
			{
                char buffer[50];
                sprintf(buffer, "%d. %s - %d", i + 1, highScores[i].name, highScores[i].score);
                DrawString(400, 550 - i * 50, buffer, colors[BLACK]);
            }
            DrawString(400, 100, "Press ESC to return", colors[BLACK]);
        } 
		else if (state == PLAYING)                          //Drawing the playing screen
		{	
            board->draw(role == TAXI);
            playerCar->draw();
            char buffer[50];
            sprintf(buffer, "Score: %d", score);
            DrawString(900, 800, buffer, colors[BLACK]);
            sprintf(buffer, "Earnings: $%d", earnings);
            DrawString(900, 780, buffer, colors[BLACK]);
            sprintf(buffer, "Fuel: %.1f", playerCar->getFuel());
            DrawString(900, 760, buffer, colors[BLACK]);
            sprintf(buffer, "Time: %.1f", gameTime);
            DrawString(900, 740, buffer, colors[BLACK]);
            sprintf(buffer, "Role: %s", role == TAXI ? "Taxi" : "Delivery");
            DrawString(900, 720, buffer, colors[BLACK]);
            
            // User instructions
            if (role == TAXI && !hasPassenger) 
                {
                DrawString(50, 800, "Press SPACE near a passenger to pick up", colors[BLACK]);
                } 
            else if (role == TAXI && hasPassenger) 
                {
                DrawString(50, 800, "Press SPACE at destination to drop off", colors[BLACK]);
                } 
            else if (role == DELIVERY && !hasPackage) 
                {
                DrawString(50, 800, "Press SPACE near a green box to pick up package", colors[BLACK]);
                } 
            else if (role == DELIVERY && hasPackage) 
                 {
                DrawString(50, 800, "Press SPACE at red destination to deliver", colors[BLACK]);
                }
        } 
		else if (state == ROLE_MENU) 
		{
            DrawString(400, 500, "Switch Role", colors[BLACK]);
            DrawString(400, 450, "1. Taxi Driver", colors[BLACK]);
            DrawString(400, 400, "2. Delivery Driver", colors[BLACK]);
            DrawString(400, 350, "Press 1-2", colors[BLACK]);
        }
        glutSwapBuffers();
    }
    void handleKey(unsigned char key, int x, int y) 
    {
        if (state == MENU) 
        {
            if (key == '1') 
            {
                role = TAXI;
                state = PLAYING;
                glutPostRedisplay();
            }
            else if (key == '2')
            {
                role = DELIVERY;
                state = PLAYING;
                glutPostRedisplay();
            }
            else if (key == '3') 
            {
                role = (rand() % 2) ? TAXI : DELIVERY;
                state = PLAYING;
                glutPostRedisplay();
            }
            else if (key == '4') 
            
            {
                state = LEADERBOARD;
                glutPostRedisplay();
            }
        }
        else if (state == LEADERBOARD) 
        {
            if (key == 27) 
            { // ESC key
                state = MENU;
                glutPostRedisplay();
            }
        }
        else if (state == GAME_OVER) 
        {
            if (key == 13 && inputLength > 0) 
            { // Enter key
                strcpy(playerName, inputBuffer);
                saveHighScore();
                state = PLAYING;
                resetGame();
                glutPostRedisplay();
            }
            else if (key == 8 && inputLength > 0) 
            { // Backspace
                inputBuffer[--inputLength] = '\0';
                glutPostRedisplay();
            }
            else if (inputLength < 19 && isprint(key)) 
            {
                inputBuffer[inputLength++] = key;
                inputBuffer[inputLength] = '\0';
                glutPostRedisplay();
            }
        }
        else if (state == PLAYING) 
        {
            if (key == 'p' || key == 'P') 
            {
                int r = playerCar->getY() / (840 / 14);
                int c = playerCar->getX() / (1020 / 18);
                if (board->getGridValue(r, c) == 7) 
                {
                    state = ROLE_MENU;
                    glutPostRedisplay();
                }
            }
            else if (key == 32) 
            {                               // Spacebar
                handlePickupDropoff();
                glutPostRedisplay();
            }
            else if (key == 27) 
            {
                 // ESC
                state = MENU;
                glutPostRedisplay();
            }
        }
        else if (state == ROLE_MENU) 
        {
            if (key == '1') 
            {
                role = TAXI;
                state = PLAYING;
                delete playerCar;
                playerCar = new Car(playerCar->getX(), playerCar->getY(), true);
                glutPostRedisplay();
            }
            else if (key == '2') 
            {
                role = DELIVERY;
                state = PLAYING;
                delete playerCar;
                playerCar = new Car(playerCar->getX(), playerCar->getY(), false);
                glutPostRedisplay();
            }
        }
    }
    void handleSpecialKey(int key, int x, int y)     //Handling the special keys
    {
        if (state == PLAYING) {
            int blockWidth = 1020 / 18;
            int blockHeight = 840 / 14;
            int currentR = playerCar->getY() / blockHeight;
            int currentC = playerCar->getX() / blockWidth;
            int newR = currentR;
            int newC = currentC;
            
            // Calculate new block position based on key press
            if (key == GLUT_KEY_LEFT) { newC--; }
            else if (key == GLUT_KEY_RIGHT) { newC++; }
            else if (key == GLUT_KEY_UP) { newR++; }
            else if (key == GLUT_KEY_DOWN) { newR--; }

            // Check if new position is valid
            if (newR >= 0 && newR < 14 && newC >= 0 && newC < 18) {
                int gridValue = board->getGridValue(newR, newC);
                
                // Allow movement only on roads (0) or garage (7)
                if (gridValue == 0 || gridValue == 7) {
                    // Calculate pixel movement
                    int newX = newC * blockWidth;
                    int newY = newR * blockHeight;
                    int dx = newX - playerCar->getX();
                    int dy = newY - playerCar->getY();
                    
                    playerCar->move(dx, dy);
                    glutPostRedisplay(); // Only redraw when car actually moves
                } else {
                    // Handle collisions
                    checkCollisions(gridValue);
                }
            }

            // Check fuel stations at current position
            for (int i = 0; i < board->getFuelStationCount(); i++) {
                FuelStation* fs = board->getFuelStation(i);
                if (abs(playerCar->getX() - fs->getX()) < blockWidth/2 && 
                    abs(playerCar->getY() - fs->getY()) < blockHeight/2 && 
                    earnings >= 20) {
                    playerCar->refuel(50.0);
                    earnings -= 20;
                    glutPostRedisplay(); // Only redraw when fuel is added
                }
            }
        }
    }
    void checkCollisions(int gridValue)                       //Checking the collisions
    {
        switch (gridValue) {
            case 1:                   // Building
                score -= (role == TAXI) ? 2 : 4;
                break;
            case 2:                     // Tree
                score -= (role == TAXI) ? 2 : 4;
                break;
            case 3:                 // Passenger
                if (!hasPassenger && role == TAXI) {
                    handlePickupDropoff();
                }
                break;
            case 4:                // Delivery Point
                if (!hasPackage && role == DELIVERY) {
                    handlePickupDropoff();
                }
                break;
            case 5:              // Fuel Station
                if (earnings >= 20) {
                    playerCar->refuel(50.0);
                    earnings -= 20;
                }
                break;
            case 6:              // Obstacle Car
                score -= (role == TAXI) ? 3 : 5;
                break;
        }
    }
    void update(int value) 
    {
        if (state == PLAYING) 
        {
            // Update game time - increment by 0.01 seconds
            gameTime += 0.01;
            
            // Move obstacle cars every 500ms
            static int obstacleMoveCounter = 0;
            if (++obstacleMoveCounter >= 5) 
            {  
                board->moveObstacleCars();
                obstacleMoveCounter = 0;
                glutPostRedisplay();
            }
            
            // Check game over conditions
            if (gameTime >= 180.0) 
            { // Exactly 180 seconds (3 minutes)
                state = GAME_OVER;
                inputLength = 0;
                memset(inputBuffer, 0, 20);
                glutPostRedisplay();
                return;
            } 
            else if (playerCar->getFuel() <= 0 || score < 0) 
            {
                state = GAME_OVER;
                inputLength = 0;
                memset(inputBuffer, 0, 20);
                glutPostRedisplay();
                return;
            }
            else if (score >= 100) 
            {
                state = GAME_OVER;
                inputLength = 0;
                memset(inputBuffer, 0, 20);
                glutPostRedisplay();
                return;
            }
        }
        // Set timer to update every 100ms
        glutTimerFunc(16, [](int v) 
            { Game::getInstance()->update(v); }, 0);
    }
    static Game* getInstance() 
    {
        static Game instance;
        return &instance;
    }
private:
    void loadHighScores() 
    {
        ifstream file("highscores.txt", ios::binary);
        if (file.is_open()) 
        {
            file.read((char*)&highScoreCount, sizeof(int));
            if (highScoreCount > 10) highScoreCount = 10;
            file.read((char*)highScores, highScoreCount * sizeof(HighScore));
            file.close();
        }
    }
    void saveHighScore()     //Saving the high scores using binary file handling
    {
        if (highScoreCount < 10) 
        {
            strcpy(highScores[highScoreCount].name, playerName);
            highScores[highScoreCount].score = score;
            highScoreCount++;
        } 
        else if (score > highScores[9].score) 
        {
            strcpy(highScores[9].name, playerName);
            highScores[9].score = score;
        }
        for (int i = 0; i < highScoreCount - 1; i++) 
        {
            for (int j = 0; j < highScoreCount - i - 1; j++) 
            {
                if (highScores[j].score < highScores[j + 1].score) 
                {
                    HighScore temp = highScores[j];
                    highScores[j] = highScores[j + 1];
                    highScores[j + 1] = temp;
                }
            }
        }
        ofstream file("highscores.txt", ios::binary);   //Saving the high scores using binary file handling

        if (file.is_open()) 
		{
            file.write((char*)&highScoreCount, sizeof(int));
            file.write((char*)highScores, highScoreCount * sizeof(HighScore));
            file.close();
        }
    }
    void resetGame()     //Resetting the game
    {
        score = 0;
        earnings = 0;
        tasksCompleted = 0;
        gameTime = 0;
        hasPassenger = false;
        hasPackage = false;
        passengerIndex = -1;
        destX = -1;
        destY = -1;
        delete board;
        delete playerCar;
        board = new Board();
        playerCar = new Car(0, 13 * 840 / 14, role == TAXI);
    }
    void handlePickupDropoff() 
    {                                                    //Handling the pickup and dropoff
        int r = playerCar->getY() / (840 / 14);
        int c = playerCar->getX() / (1020 / 18);
        
        if (role == TAXI) 
        {
            if (hasPassenger) 
            {
                // Check if at destination
                if (abs(playerCar->getX() - destX) < 20 && abs(playerCar->getY() - destY) < 20) 
                    {
                    score += 10;
                    earnings += 50;
                    tasksCompleted++;
                    if (tasksCompleted % 2 == 0) {
                        board->addOtherCar();
                    }
                    hasPassenger = false;
                    board->resetPassenger(passengerIndex, rand() % 14, rand() % 18);
                }
            } 
            else 
            {
                // Try to pick up passenger
                for (int i = 0; i < board->getPassengerCount(); i++) {
                    Passenger* p = board->getPassenger(i);
                    if (p->isActive() && 
                        abs(playerCar->getX() - p->getX()) < 20 && 
                        abs(playerCar->getY() - p->getY()) < 20) {
                        hasPassenger = true;
                        passengerIndex = i;
                        p->deactivate();
                        destX = p->getDestX();
                        destY = p->getDestY();
                        break;
                    }
                }
            }
        } 
        else 
        {
            if (hasPackage) 
            {
                // Check if at destination
                if (abs(playerCar->getX() - destX) < 20 && abs(playerCar->getY() - destY) < 20) 
                    {
                    score += 20;
                    earnings += 100;
                    tasksCompleted++;
                    if (tasksCompleted % 2 == 0) 
                    {
                        board->addOtherCar();
                    }
                    hasPackage = false;
                    destX = -1;
                    destY = -1;
                }
            } 
            else 
            {
                // Try  to pick up package
                for (int i = 0; i < board->getDeliveryPointCount(); i++) 
                {
                    DeliveryPoint* dp = board->getDeliveryPoint(i);
                    if (dp->hasPackageAvailable() && abs(playerCar->getX() - dp->getX()) < 20 && abs(playerCar->getY() - dp->getY()) < 20) 
                        {
                        hasPackage = true;
                        dp->pickPackage();
                        int destR, destC;
                        do 
                        {
                            destR = rand() % 14;
                            destC = rand() % 18;
                        } while (board->getGridValue(destR, destC) != 0);
                        destX = destC * (1020 / 18);
                        destY = destR * (840 / 14);
                        break;
                    }
                }
            }
        }
    }
};

/*
 * Main Canvas drawing function.
 * */
// void drawCar() {
//         Car c(pos.getXI(),pos.getYI());
//         c.draw();
// 	glutPostRedisplay();
// }

void GameDisplay() {
    static int lastTime = 0;
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    
    // Limit frame rate to 60 FPS for smooth rendering
    if (currentTime - lastTime < 16) { // 1000ms/60 ≈ 16ms
        return;
    }
    lastTime = currentTime;

    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    Game::getInstance()->draw();
    glutSwapBuffers();
}



/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {
	Game::getInstance()->handleSpecialKey(key, x, y);
	// if (key
	// 		== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
	// 	// what to do when left key is pressed...
	// 	pos.setX(pos.getXI() - 7);

	// } else if (key
	// 		== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
	// 			pos.setX(pos.getXI() + 7);
	// } else if (key
	// 		== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	// 	pos.setY(pos.getYI() + 7);
	// }

	// else if (key
	// 		== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	// 	pos.setY(pos.getYI() - 7);
	// }

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	//glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	Game::getInstance()->handleKey(key, x, y);
	// if (key == 27/* Escape key ASCII*/) {
	// 	exit(1); // exit the program when escape key is pressed.
	// }

	// if (key == 'b' || key == 'B') //Key for placing the bomb
	// 		{
	// 	//do something if b is pressed
	// 	cout << "b pressed" << endl;

	// }
	//glutPostRedisplay();
}



/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {
	Game::getInstance()->update(m);
	// implement your functionality here
	//moveCar();

	// once again we tell the library to call our Timer function after next 1000/FPS
	//glutTimerFunc(100, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
			{
		cout << GLUT_DOWN << " " << GLUT_UP << endl;

	} else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{
			cout<<"Right Button Pressed"<<endl;

	}
	glutPostRedisplay();
}
/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {

	int width = 1020, height = 840; // i have set my window size to be 800 x 600
    //playMusicInBackground("sounds/bg_music.ogg");

	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("OOP Project"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(100, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* RushHour_CPP_ */