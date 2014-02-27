//
//  SUWT.h
//  test
//
//  Created by Arthur Câmara on 02/09/13.
//  Copyright (c) 2013 latin-DCC. All rights reserved.
//

#ifndef __test__SUWT__
#define __test__SUWT__

#include <iostream>
#include <fstream>
#include "wat_array.hpp"
#include "ISWT.h"


//SUWT - Score-User Wavelet Tree
using namespace std;
class SUWT {
  wat_array::WatArray tree;
public:
  ISWT::ISWT iswt;
  std::vector<int> range_report(uint64_t s, uint64_t e, wat_array::WatArray& tree);
  std::vector<uint64_t> Init(istream& in);
  std::vector<uint64_t> Users(vector<vector<uint64_t> > &matrix, uint64_t score, uint64_t occ);

  //Operation 6 - Obtain all users that ranked item i
  std::vector<uint64_t> Rankedi(uint64_t i);
  //Operation 7 - Obtain all users that ranked any item with a score greater than t
  std::vector<uint64_t> RankedGreaterThan(int i);
  //Operation 8 - Obtain all users that ranked any item with a score within t0 and t1
  std::vector<uint64_t> RankedWithinRange(int t0, int t1);
  //Operation 9 - Obtain users that ranked a particular item i with score t
  std::vector<uint64_t> RankediWitht(uint64_t i, int t);
  //Operation 10 - Obtain users that ranked a particular item i with score within a range t0 and t1
  std::vector<uint64_t> RankediWithRange(uint64_t t0, uint64_t t1,uint64_t i);
  //Operation 11 - Given a user j, obtain all items that she has ranked
  std::vector<uint64_t> iRankedByj(uint64_t j);
  //Operation 12 - Given a set of m users obtain the items that they have ranked
  std::vector<uint64_t> ItemsRankedBym(std::vector<uint64_t> m);
};

#endif /* defined(__test__SUWT__) */
