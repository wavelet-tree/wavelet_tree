//
//  kAWT.cpp
//  test
//
//  Created by Arthur Câmara on 26/08/13.
//  Copyright (c) 2013 latin-DCC. All rights reserved.
//

#include "ISWT.h"
using namespace std;

std::vector<uint64_t> ISWT::Init(istream &in){
  std::vector<uint64_t> aux2;
  std::vector<uint64_t> L;
  std::vector<char> aux;
  std::vector<string>line;
  std::vector<std::vector<char> > matrix;
  uint64_t maxValue=0;
  uint64_t numberItens;
  uint64_t emptyCells;
  string input;
  string strAux;
  int64_t userCount=0;
  pipes.push_back(0);
  while (in.good()) {//for every line on file
    aux.clear();
    line.clear();
    getline(in, input); //This line is a single user. Add it to the matrix
    if (input.size()<1) continue;
    size_t pos = 0;
    std::string token;
    while((pos=input.find(" "))!=std::string::npos){
      token = input.substr(0,pos);
      line.push_back(token);
      input.erase(0, pos+1);
    }
    token = input.substr(0,pos);
    line.push_back(token);
    for (unsigned it = 0; it<line.size(); it++){
      if(line[it].size()<2) aux.push_back(line[it][0]);
      else{
        strAux= line[it].substr(0, line[it].size()-1);
        emptyCells = atoi(strAux.c_str());
        while(emptyCells>0){
          aux.push_back('-');
          emptyCells--;
        }
      }
    }
    
    numberItens = aux.size();
    matrix.push_back(aux);
    userCount++;
  }
  int current;
  //scan the transposed matrix
  for(unsigned i =0; i<numberItens; i++){
    aux2.clear();
    for (unsigned j=0; j<userCount; j++){
      //read whole line, sort it and append it on L.
      if (matrix[j][i]=='-') continue;
      current = matrix[j][i]-'0';
      if (current >maxValue) maxValue = current;
      aux2.push_back(current);
    }
    this->maxValue = maxValue;
    aux2 = BucketSort(aux2, true);
    for (unsigned it = 0 ; it<aux2.size(); it++)
      L.push_back(aux2[it]);
    pipes.push_back(L.size());
    
  }
  //after the array is initiated, create the wavelet tree
  tree.Init(L);
  
  
  //build the ranges hashing
  uint64_t lastPipe=1;
  for (unsigned it = 0; it<L.size(); it++){
    if (it>=pipes[lastPipe]){//end of this item
      lastPipe++;
    }
    ranges[it]=lastPipe;
  }
  return L;
  
}

std::vector<uint64_t> ISWT::range_report(uint64_t s, uint64_t e){
  std::vector<uint64_t> ret;
  if(s==0) s++;
  ret.reserve(e-s+1); //faster
  for (uint64_t i = s-1; i<e; i++){
    ret.push_back((int)tree.Lookup(i));
  }
  return ret;
}



//Operation 1 - Retrieve all scores from item i
std::vector<uint64_t> ISWT::Retrieve(uint64_t i){
  uint64_t start;
  if (i==0) start = 0;
  else
     start = pipes[i-1]+1;
  uint64_t end   = pipes[i];

  std::vector<uint64_t> ret = range_report(start, end);
  return ret;
}

//Operation 2. Retrieve the best k scores for item i
std::vector<uint64_t> ISWT::RetrieveKBest(uint64_t i, uint64_t k){
  uint64_t start = pipes[i-1]+1;
  uint64_t end   = pipes[i];
  
  std::vector<uint64_t> ret = range_report(start, end);
  
  ret.resize(k);
  return ret;
  
}

//Operation 3. Retrieve all items that have at least one score y
std::vector<uint64_t> ISWT::RetrieveScoreY(uint64_t y){
  std::vector<uint64_t> AllYs, ret;
  uint64_t i = 1, lastRank=0, end;
  uint64_t aux = tree.Select(y, i);
  uint64_t item;
  //start from the leaves, trevessing upwards, finding all of the occurences.
  //O(occ log(theta))

  while (aux!= wat_array::NOTFOUND) {
    item = getItem(aux); //item of the given position
    ret.push_back(item);
    end = pipes[item]; //last position of this item
    lastRank = tree.Rank(y, end) ; //Number of ys left on this item
    aux = tree.Select(y, lastRank+1);
  }
  return ret;
}

//Operation 4. Retrieve all items that have at least a score within a range [s,e]
std::vector<uint64_t> ISWT::RetrieveScoreRange(uint64_t s, uint64_t e){
  std::vector<uint64_t> auxVec;
  std::vector<uint64_t> ret;
  std::vector<bool> included;
  for (uint64_t it = s; it<=e; it++){
    auxVec = RetrieveScoreY(it);
    ret.insert(ret.end(), auxVec.begin(), auxVec.end());
  }
 for (unsigned it = 0; it<ret.size(); it++)
  ret = BucketSort_noSize(ret);
  for (unsigned it = 0; it<ret.size(); it++)
  ret.erase(unique(ret.begin() ,ret.end()), ret.end());
  return ret;
}


std::vector<uint64_t> ISWT::SharedScores(std::vector<uint64_t> m){
  
  //step 1: find the ranges for every one of the m items
  std::vector<pair<uint64_t, uint64_t> > ranges;
  ranges.reserve(m.size());
  pair<uint64_t, uint64_t> auxPair;
  std::vector<uint64_t> allScoresAux ;
  std::vector<uint64_t> intersects, aux2(this->maxValue+1, 0);

  //retrieve ranges
  for (unsigned i = 0; i<m.size(); i++){
    auxPair = make_pair(pipes[m[i]-1], pipes[m[i]]);
    ranges.push_back(auxPair);
  }

  //recover all of the scores and count each of them.
  for(uint64_t it = 0; it<m.size(); it++){
    allScoresAux = range_report(ranges[it].first, ranges[it].second);
    //count how many times each score happens
    for (uint64_t it2 = 0; it2<allScoresAux.size(); it2++){
      if (aux2[allScoresAux[it2]]<=it)
        aux2[allScoresAux[it2]]++;
    }
  }
  for (uint64_t it =0; it<aux2.size(); it++)
    if(aux2[it]==m.size()) intersects.push_back(it);
  
  
  return intersects;
}


bool ISWT::InVector(std::vector<uint64_t> vec, uint64_t key){
  return (std::find(vec.begin(), vec.end(), key) != vec.end());
}

//linear sort for building the array
std::vector<uint64_t> ISWT::BucketSort(std::vector<uint64_t>vec, bool inverted){

  std::vector<uint64_t> ret(vec.size(), 0),aux(maxValue+1, 0);
  uint64_t position = 0;
  //counting occurences
  for (uint64_t it = 0; it< vec.size(); ++it)
    aux[vec[it]]++;
  //print
  if(inverted){
    for (int64_t it = aux.size()-1; it>=0; it--){
      for (uint64_t it2 = 0; it2<aux[it]; it2++){
        ret[position]=it;
        ++position;
      }
    }
  }
  else{
    for(uint64_t it = 0; it<aux.size(); it++){
      for(uint64_t it2 = 0; it2<aux[it]; it2++){
        ret[position]=it;
        ++position;
      }
    }
  }
  return ret;
}


std::vector<uint64_t> ISWT::BucketSort_noSize(std::vector<uint64_t> vec){
  std::vector<uint64_t> ret, occ(0,0);
  ret.reserve(vec.size());
  for (unsigned it = 0; it<vec.size(); it++){
    if(vec[it]>=occ.size())
      occ.resize(vec[it]+1,0);
    occ[vec[it]]++;
  }
  for(uint64_t it =0; it<occ.size(); it++){
    for(uint64_t it2 = 0; it2<occ[it]; it2++){
      ret.push_back(it);
    }
  }
  return ret;
}

void ISWT::setRanges(){
  uint64_t start;
  uint64_t end;
  for (unsigned i =0; i<pipes.size(); i++){
    if (i==0) start = 0;
    else
      start = pipes[i-1]+1;
    end   = pipes[i];
    
  }
}
uint64_t ISWT::getItem(uint64_t index){
  return ranges.at(index);
}







