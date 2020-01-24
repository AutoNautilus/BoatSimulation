#ifndef CSVPARSER_H_
#define CSVPARSER_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class CSVColumn{
 public:
  CSVColumn();
  ~CSVColumn();
  std::string name;
  std::vector<std::string> data;
};
class CSVFile{
 public:
  CSVFile();
  ~CSVFile();
  std::vector<CSVColumn*> columns;
};

class CSVParser{
 public:
  CSVParser();
  ~CSVParser();
  CSVFile* parser(std::string);
};

#endif /* CSVPARSER_H_ */
