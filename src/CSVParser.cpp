#include "CSVParser.h"

CSVColumn::CSVColumn(){
  //data = new std::vector<float>();
}
CSVColumn::~CSVColumn(){

}

CSVFile::CSVFile(){
  //columns = new std::vector<CSVColumn*>();
}

CSVFile::~CSVFile(){

}

CSVParser::CSVParser(){

}
CSVParser::~CSVParser(){

}

CSVFile* CSVParser::parser(std::string filePath){
  int MAX_LINE_CHARS = 512;
  CSVFile* csvFile = new CSVFile();

  std::ifstream file (filePath);

  std::string line;
  std::getline(file, line); //get the first line, contains column names
  std::string::iterator itr;
  std::string columnName = "";
  for(itr = line.begin(); itr < line.end(); itr++){
    char c = *itr;
    if(c == ','){
      CSVColumn* column = new CSVColumn();
      column->name = columnName;
      std::cout<<"column named: " << columnName << std::endl;
      csvFile->columns.push_back(column);

      columnName.clear();
    }
    else if(c != ' '){
      columnName += c;
    }
  }

  int columnIndex = 0;
  while(!file.eof()){
    std::getline(file, line); //get the first line, contains column names
    if(file.fail()){
      std::cout<<"failed reading a line of file \n";
    }
    std::string::iterator itr;
    std::string currentData;
    int index = 0;
    for(itr = line.begin(); itr < line.end(); itr++){
      char c = *itr;
      if(c == ','){
        csvFile->columns[columnIndex]->data.push_back(currentData);
        columnIndex = (columnIndex + 1) % csvFile->columns.size();
        currentData.clear();
      }
      else if(c != ' '){
        currentData += c;
      }
    }
  }
  return csvFile;
}
