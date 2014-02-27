//
//  kAWT.h
//  test
//
//  Created by Arthur Câmara on 26/08/13.
//  Copyright (c) 2013 latin-DCC. All rights reserved.
//

#ifndef __test__kAWT__
#define __test__kAWT__

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <boost/unordered_map.hpp>
#include "wat_array.hpp"


using namespace std;
class ISWT{
private:
  boost::unordered_map<uint64_t, uint64_t> ranges;
  uint64_t maxValue;//this is the alphabet size
  
  bool InVector(std::vector<uint64_t>, uint64_t key);
  std::vector<int> rint(pair<uint64_t, uint64_t>, pair<uint64_t, uint64_t>);
  std::vector<uint64_t> BucketSort(std::vector<uint64_t>, bool inverted = false);
  std::vector<uint64_t> BucketSort_noSize(std::vector<uint64_t>);  //slightly slower. But can order any size of array
  void setRanges();
  uint64_t getItem(uint64_t index);
  
  friend class SUWT;
  
public:
  wat_array::WatArray tree;

  std::vector<uint64_t> range_report(uint64_t s, uint64_t e);
  vector<uint64_t> pipes;
  std::vector<uint64_t> Init(istream& in);
  
  //Retrieve all scores for Item i
  std::vector<uint64_t> Retrieve(uint64_t i);
  
  //Retrieve the k best scores for item i
  std::vector<uint64_t> RetrieveKBest(uint64_t i, uint64_t k);
  
  //Retrieve all items that have at least one score y
  std::vector<uint64_t> RetrieveScoreY(uint64_t y);
  
  //Retriece all items that have at least one score within [s,e]
  std::vector<uint64_t> RetrieveScoreRange(uint64_t s, uint64_t e);
  
  //Retrieve a list of scores that those items share among them.
  std::vector<uint64_t> SharedScores(std::vector<uint64_t> m);
  

  
};


#endif /* defined(__test__kAWT__) */
