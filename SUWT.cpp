//
//  SUWT.cpp
//  test
//
//  Created by Arthur Câmara on 02/09/13.
//  Copyright (c) 2013 latin-DCC. All rights reserved.
//

#include "SUWT.h"
#include <vector>

std::vector<int> SUWT::range_report(uint64_t s, uint64_t e, wat_array::WatArray& tree){
    std::vector<int> ret;
    ret.reserve(e-s);
    for (uint64_t i = s; i<=e; i++)
      ret.push_back((int)tree.Lookup(i));
    return ret;
}

std::vector<uint64_t> SUWT::Init(istream& in){
  //Init L
  std::vector<uint64_t> L= iswt.Init(in);
  for (unsigned it = 0; it<L.size(); it++)
    cout<<L[it]<<" ";
  cout<<"\n";
  in.clear();
  in.seekg(0);
  vector<uint64_t> occ, P, aux;
  std::vector<std::vector<uint64_t> > matrix;
  string input, strAux;
  std::vector<string> line;
  std::vector<wat_array::ListResult> res; //List of scores and frequence of them.
  
  iswt.tree.ListMinRange(0, iswt.tree.alphabet_num(), 0, iswt.tree.length(), iswt.tree.alphabet_num(), res);
  
  uint64_t score, emptyCells;
  
  //read input and create the given matrix
  uint64_t numberItens;
  while (in.good()) {
    aux.clear();
    getline(in, input);
    if (input.size()<1) continue;
    size_t pos = 0;
    std::string token;
    while((pos=input.find(" "))!=std::string::npos){
      token = input.substr(0, pos);
      line.push_back(token);
      input.erase(0, pos+1);
    }
    for(unsigned it =0; it<line.size(); it++){
      if(line[it].size()<2) aux.push_back(line[it][0]);
      else{
        strAux = line[it].substr(0, line[it].size()-1);
        emptyCells = atoi(strAux.c_str());
        while (emptyCells>0) {
          aux.push_back('-');
          emptyCells--;
        }
      }
    }
    numberItens = aux.size();
    matrix.push_back(aux);
  }
  //to create P, find the user that gave that rate (use pipes)
  for (unsigned i = 0; i<res.size(); i++){
    score = res[i].c;
    occ = Users(matrix, score, res[i].freq);
    P.insert(P.end(), occ.begin(), occ.end());
  }
  
  tree.Init(P);
  return P;
}

std::vector<uint64_t> SUWT::Users(vector<vector<uint64_t> > &matrix, uint64_t score, uint64_t occ){
  vector<uint64_t> ret;
  
  //trasverse the transposed matrix. if find the proper score, add j to ret.
  for (unsigned i = 0; i<matrix[0].size(); i++){
    for (unsigned j =0 ; j<matrix.size(); j++){
      if(matrix[j][i]==score){
        ret.push_back(j+1);
        
      }
    }
  }
  return ret;
}



std::vector<uint64_t> SUWT::Rankedi(uint64_t i){
  //Find all the scores from item i:
  std::vector<uint64_t> scores = iswt.Retrieve(i);
  std::vector<uint64_t> ret;
  uint64_t occBeforeItem, occInsideItem, occCounter=0;
  uint64_t startL, endL;
  if (i==0) startL = 0;
  else startL = iswt.pipes[i-1]+1;
  endL = iswt.pipes[i];
  //must find the position of the score on P. After that, lookup and add it to ret
  for (uint64_t it = 0; it<scores.size();){
    occCounter = 0;
    occBeforeItem = iswt.tree.Rank(scores[it], startL-1);//how many of this score occured before the item
    occInsideItem = iswt.tree.Rank(scores[it], endL)-occBeforeItem;//How many of the score is inside the same item
    
    //now, cont all the frequence of each score smaller than the one being looked for. This is pretty fast on the size of theta-score.
    for (uint64_t i = 1; i<scores[it]; i++){
      occCounter += iswt.tree.Freq(i);
    }
    for (uint64_t i = 0; i<occInsideItem; i++){
      ret.push_back(tree.Lookup(occCounter+i));
    }
    //increase it by the number of occ inside the item.
    it+=occInsideItem;
  }
  return ret;
}
std::vector<uint64_t> SUWT::RankedGreaterThan(int i){
  std::vector<uint64_t> ret;
  uint64_t count = 0;
  //Locate leaves with rank > i
  //essentially, count how many occurences of ranks smaller and equal than i occurs.
  for (uint64_t it =1; it<=i; it++){
    count+= iswt.tree.Freq(it);
  }
  //count is the first occurence of an score >=i.
  for (uint64_t it = count; it<tree.length(); it++){
    ret.push_back(tree.Lookup(it));
  }
  return ret;
}
std::vector<uint64_t> SUWT::RankedWithinRange(int t0, int t1){
  //count how many scores are there before t0
  uint64_t countBefore = 0, countAfter = 0, end;
  std::vector<uint64_t> ret;
  for (uint64_t it =1; it<t0; it++){
    countBefore+= iswt.tree.Freq(it);
  }
  //Count how many scores are there after t1
  for (uint64_t it = t1+1; it<=iswt.maxValue; it++){
    countAfter+= iswt.tree.Freq(it);
  }
  end = iswt.tree.length()-countAfter;
  for (uint64_t i = countBefore; i<end; i++){
    ret.push_back(tree.Lookup(i));
  }
  return ret;
}
std::vector<uint64_t> SUWT::RankediWitht(uint64_t i, int t){
  std::vector<uint64_t> ret;
  uint64_t startL, endL, occ, before=0, startP;
  //inside the range or item i, find the rank of each score t.
  //count how many scores are there before t
  for (unsigned it = 1; it<t; it++)
    before += iswt.tree.Freq(it);
  
  if (i==0) startL = 0;
  else startL = iswt.pipes[i-1]+1;
  endL = iswt.pipes[i];
  occ = iswt.tree.Rank(t, endL) - iswt.tree.Rank(t, startL-1); //occurences of t inside i
  startP = before + iswt.tree.Rank(t, endL)-occ;
  for (uint64_t it = 0; it<occ; it++){
    ret.push_back(tree.Lookup(startP+it));
  }
  
  return ret;
}


//For each rate inside the range for the item(it's stored on inverse range on ISWT),
//count how many of these are before, in order to find it's position on SUWT.

std::vector<uint64_t> SUWT::RankediWithRange(uint64_t t0, uint64_t t1,uint64_t i){
  std::vector<uint64_t> ret;
  uint64_t startL, endL, before =0, occ, beforeAux;
  if (i==0) startL = 0;
  else startL = iswt.pipes[i-1];
  endL = iswt.pipes[i]-1;
  //calculate frequncies before t0

  
  //for each score on range, find if there is any occurence of scores within the range
  for (int64_t it = t0; it<=t1; it++){
    //found the number of occurences before and after the range. if it happens, find where it happens on
    beforeAux = iswt.tree.Rank(it, startL);
    occ = iswt.tree.Rank(it, endL+1) - beforeAux;
    if(occ>0){
      //if there is some occurences, for each of these, find the rank of it.
      //first, find how many occurences are before on P.
      for (unsigned it2 =1; it2<it; it2++){
        before+=iswt.tree.Freq(it2);
      }
      for (unsigned i = 0; i<occ; i++){
        ret.push_back(tree.Lookup(before+i+beforeAux));
      }
    }
  }
  
  return ret;
}

std::vector<uint64_t> SUWT::iRankedByj(uint64_t j){
  std::vector<uint64_t> ret, scoresByj;
  //find the position of every occurence of user j on SUWT
  uint64_t pos = 0, count=1, freqCounter, temptingScore, occBefore, posWithin, posL;
  while (pos!= wat_array::NOTFOUND){
    pos = tree.Select(j, count);
    count++;
    scoresByj.push_back(pos);
  }
  scoresByj.pop_back();
  

  //for each score, find it's original value [1..5]
  for (uint64_t it = 0; it<scoresByj.size(); it++){
    //to find it's original value, count the frequence of every score on ISWT, until we pass it.
    freqCounter = 0;
    temptingScore =0;
    while(freqCounter <= scoresByj[it]){
      temptingScore++;
      freqCounter += iswt.tree.Freq(temptingScore);
    }
    //tempting score is the actual score.
    //Now, how many scores like this happened before it?
    //Count how many scores smaller than this one happened. (occBefore)
    occBefore = freqCounter-iswt.tree.Freq(temptingScore);
    //now, how many happened within it.
    posWithin = scoresByj[it]-occBefore;
    posWithin++;
    posL = iswt.tree.Select(temptingScore, posWithin);
    ret.push_back(iswt.getItem(posL));
  }
  return ret;
}
std::vector<uint64_t> SUWT::ItemsRankedBym(std::vector<uint64_t> m){
  std::vector<uint64_t> ret, allScores;
  uint64_t pos, count, freqCounter, temptingScore, occBefore, posWithin, posL;
  //find all of the indexes for every user in m
  for (unsigned it = 0; it<m.size(); it++){
    pos = 0;
    count = 1;
    while(pos!= wat_array::NOTFOUND){
      pos = tree.Select(m[it], count);
      count++;
      allScores.push_back(pos);
    }
    allScores.pop_back();
  }
  //now, repeat the same proccess as operation 11
  for (uint64_t it =0; it<allScores.size(); it++){
    freqCounter = 0;
    temptingScore = 0;
    while (freqCounter<=allScores[it]) {
      temptingScore++;
      freqCounter += iswt.tree.Freq(temptingScore);
    }
    occBefore = freqCounter-iswt.tree.Freq(temptingScore);
    posWithin = allScores[it]-occBefore;
    posWithin++;
    posL = iswt.tree.Select(temptingScore, posWithin);
    ret.push_back(iswt.getItem(posL));
  }
  return ret;
}



