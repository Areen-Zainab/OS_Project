#include <sstream>
#include <fstream>
#include <iostream>
#include <time.h>
using namespace std;

string int2Str(int n){

  stringstream stream;
  stream << n;
  string str;
  stream >> str;
  return str;
}

int str2Int(string str){
  int n = stoi(str);  //converting string into integer value
  return n;

}

//writing scores to file(input to file)
void writeScoreToFile(int topScore){

    ofstream HighScoreFile("highscores.txt");  //using output stream here to ouput send tha data(scores)to file
    HighScoreFile << int2Str(topScore);    //converting the integer into string 
    HighScoreFile.close();
}

//function to read the high score from data
void loadTopScore(int topScore){
    string line;
    ifstream ReadHighScoreFile("highscores.txt");  //using ifstream here, to read data from file
    while (getline (ReadHighScoreFile, line)) 
    {
         topScore=str2Int(line);    //it will give us data back into integer
    }
    ReadHighScoreFile.close();
}