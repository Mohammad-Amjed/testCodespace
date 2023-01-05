#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
using namespace std;
double GPSCOORD[9999][3];
double RADARCOORD[9999][3];
int RADARrows = 0; // Keep track of the number of rows in the array
int GPSrows = 0; // Keep track of the number of rows in the array
string GPSFileName;
string RADARFileName;
void readFiles(){

  cout << "Enter the GPS coordinates file name" << endl;
  cin >> GPSFileName;
  std::ifstream openGPSfile(GPSFileName);  // Open the file for reading
      if(!openGPSfile.good()){
      cout << GPSFileName << " does not exist in the directory" <<endl;
        abort();

     }else{
         double x;
         while (openGPSfile >> x) { // Read values from the file until the end is reached
         GPSCOORD[GPSrows][0] = x; // Store the value in the array
         for (int j = 1; j < 3; j++) { // Read the remaining values in the row
            openGPSfile >> GPSCOORD[GPSrows][j];
         }
          GPSrows++; // Increment the number of rows
          }
          openGPSfile.close(); // Close the file
    }


  // Print the array
 
  cout << "Enter the RADAR coordinates file name" << endl;
  cin >> RADARFileName;
  std::ifstream openRADARfile(RADARFileName);  // Open the file for reading
    if(GPSFileName != RADARFileName){
          if(!openRADARfile.good()){
            cout << RADARFileName << " does not exist in the directory" <<endl;
              abort();

          }else{

              double y;
                while (openRADARfile >> y) { // Read values from the file until the end is reached
                RADARCOORD[RADARrows][0] = y; // Store the value in the array
                for (int j = 1; j < 3; j++) { // Read the remaining values in the row
                  openRADARfile >> RADARCOORD[RADARrows][j];
                }
                RADARrows++; // Increment the number of rows
                }

                openRADARfile.close(); // Close the file
          }
    }else {
      cout << "GPS file and RADAR file cannot be the same"<<endl;
      abort();
    }


}

double distanceD;
void checkToleranceError(double Farr[][3],double Sarr[][3]);
double distance(double x1, double x2, double y1, double y2, double z1, double z2){
  distanceD = sqrt((x2-x1) * (x2-x1) + (y2-y1) * (y2-y1) + (z2-z1) * (z2-z1));
  return distanceD;
} 
void appendErrorValues(double error, int timestamp, double x1, double y1, double z1, double x2, double y2, double z2) {
  std::ofstream outfile("ERROR.log", std::ios::app);  // Open the file for output and append to it
  outfile << "Timestamp " << timestamp << ":";
  outfile << " Error: [" << error << "] ";
  outfile << "Coordinates: [GPS (" << x1 << ", " << y1 << ", " << z1 << ") ";
  outfile << " RADAR (" << x2 << ", " << y2 << ", " << z2 << ")]." << std::endl;
  outfile.close();
}

int main () {
   cout << "1. Calculate errors and save to ERROR.LOG\n";
   cout << "2. Display ERROR.LOG\n";
   cout << "3. Quit\n";
   cout << "Enter your choice: ";

   int choice;
   cin >> choice;
  if(choice == 1){
    checkToleranceError(GPSCOORD,RADARCOORD);
  }else if(choice == 2){
    std::ifstream openERRORLOG("Error.log");
    if(!openERRORLOG.good()){
      cout << "Error.log does not exist in the directory" <<endl;
    }
    else{
      cout << "openning Error.log ..." <<endl;
      FILE* file = fopen("ERROR.LOG", "r");
        if (file != NULL) {
          fclose(file);
          system("start ERROR.LOG");
        }

    }
  }
   
   return 0;
}
 
void checkToleranceError(double Farr[][3],double Sarr[][3]) {


  readFiles();
  double tolerance;
  cout << "please enter the tolarance value" << endl;
  cin >> tolerance;
  int cols = 3;
  double x1,x2,y1,y2,z1,z2;
  std::ofstream outfile("ERROR.log",std::ios::app);
  outfile << "ERROR LOG for GPS file " << GPSFileName << " and RADAR file " << RADARFileName; 
  outfile << " and TOLERANCE value = [" << tolerance <<  "].\n" << std::endl;
  
  for (int i = 0; i < min(GPSrows, RADARrows); i++) {
      x1 = Farr[i][0];
      x2 = Sarr[i][0];
      y1 = Farr[i][1];
      y2 = Sarr[i][1];
      z1 = Farr[i][2];
      z2 = Sarr[i][2];
      distance(x1, x2, y1, y2, z1, z2);
      if(distanceD > tolerance){

        appendErrorValues(distanceD, i, x1, x2, y1, y2, z1, z2);

      }

  }


  outfile << std::setw(100) << std::setfill('-') << "" << std::endl;  // Output a line of 80 '-' characters
  outfile.close();
 
}



