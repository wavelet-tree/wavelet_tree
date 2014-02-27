//
//  main.cpp
//  test
//
//  Created by Arthur Câmara on 21/08/13.
//  Copyright (c) 2013 latin-DCC. All rights reserved.
//

#include <vector>
#include <ctime>

#include <iostream>
#include <string>
#include <fstream>
#include <ISWT.h>
#include <SUWT.h>
#include "wat_array.hpp"

using namespace std;
int main(int argc, const char * argv[])
{
  
  short option, operation;
  string inFilePath;
  ifstream input;
  vector<uint64_t> TestVector;
  uint64_t i, k;
  string line;
  SUWT::SUWT *tree;
  clock_t begin;

  
  while(true){
    cout<<"Choose one option:"<<endl;
    cout<<"  1 - Input"<<endl;
    cout<<"  2 - Make an operation on a tree"<<endl;
    cout<<flush;
    cin>>option;
    tree = NULL;
    
    switch (option) {
      case 1:{
        cout<<"\n\nType the file path:"<<endl;
        cin>>inFilePath;
        cout<<"Building Tree..."<<endl;
        ifstream input(inFilePath.c_str());

        tree = new SUWT::SUWT;
        tree->Init(input);
        cout<<"Done";
        break;
      }
      case 2:{
        cout<<"\n\nChoose an operation: (please note that not all operations make sense on a every tree)"<<endl;
        cout<<" 1  - Retrieve all scores from item i"<<endl;
        cout<<" 2  - Retrieve the k best scores for item i"<<endl;
        cout<<" 3  - Retrieve all itens that have at least one score y"<<endl;
        cout<<" 4  - Retrieve all itens that have at least a score within range [y0,y1]"<<endl;;
        cout<<" 5  - Given a set of m different items, find which scores are shared by all of them"<<endl;
        cout<<" 6  - Obtain all users that ranked item i"<<endl;
        cout<<" 7  - Obtain all users that ranked any item with a score greater than t"<<endl;
        cout<<" 8  - Obtain all users that ranked any item with a score within t0 and t1"<<endl;
        cout<<" 9  - Obtain users that ranked a particular item i with score t"<<endl;
        cout<<" 10 - Obtain users that ranked a particular item i with score within a range t0 and t1"<<endl;
        cout<<" 11 - Given a user j, obtain all itens that she has ranked"<<endl;
        cout<<" 12 - Given a set of m users obtain the items that they have ranked"<<endl;
        cin>>operation;
        switch (operation) {
          case 1:{
            cout<<"i= "<<endl;
            cin>>i;
            begin = clock();
            TestVector = tree->iswt.Retrieve(i);
            break;
          }
          case 2:{
            cout<<"k= "<<endl;
            cin>>k;
            cout<<"i= "<<endl;
            cin>>i;
            begin = clock();
            TestVector = tree->iswt.RetrieveKBest(i, k);
            break;
          }
          case 3:{
            cout<<"y= "<<endl;
            cin>>k;
            begin = clock();
            TestVector = tree->iswt.RetrieveScoreY(k);
            break;
          }
          case 4:{
            uint64_t y0, y1;
            cout<<"y0="<<endl;
            cin>>y0;
            cout<<"y1="<<endl;
            cin>>y1;
            begin = clock();
            TestVector = tree->iswt.RetrieveScoreRange(y0, y1);
            break;
          }
          case 5:{
            cout<<"set of m itens: (enter -1 to stop entering the set)"<<endl;
            int input;
            std::vector<uint64_t> m;
            while(input != -1){
              cin>>input;
              m.push_back(input);
            }
            TestVector = tree->iswt.SharedScores(m);
          }
          case 6:{
            cout<<"i="<<endl;
            cin>>i;
            begin = clock();
            TestVector = tree->Rankedi(i);
            break;
          }
          case 7:{
            cout<<"t= "<<endl;
            int t;
            cin>>t;
            begin=clock();
            TestVector = tree->RankedGreaterThan(t);
            break;
          }
          case 8:{
            int t0, t1;
            cout<<"t0="<<endl;
            cin>>t0;
            cout<<"t1="<<endl;
            cin>>t1;
            begin=clock();
            TestVector = tree->RankedWithinRange(t0, t1);
            break;
          }
          case 9:{
            int t;
            cout<<"i="<<endl;
            cin>>i;
            cout<<"t="<<endl;
            cin>>t;
            begin=clock();
            TestVector=tree->RankediWitht(i,t);
            break;
          }
          case 10:{
            uint64_t t0,t1;
            cout<<"i="<<endl;
            cin>>i;
            cout<<"t0="<<endl;
            cin>>t0;
            cout<<"t1="<<endl;
            cin>>t1;
            begin=clock();
            TestVector=tree->RankediWithRange(t0, t1, i);
            break;
          }
          case 11:{
            cout<<"j="<<endl;
            cin>>i;
            begin=clock();
            TestVector=tree->iRankedByj(i);
            break;
          }
          case 12:{
            cout<<"set of m users: (enter -1 to stop entering the set)"<<endl;
            int input;
            std::vector<uint64_t> m;
            while(input != -1){
              cin>>input;
              m.push_back(input);
            }
            begin=clock();
            TestVector=tree->ItemsRankedBym(m);
            break;
          }
          default:
          break;
        }
        clock_t end = clock();
        double elapsed_time = double(end-begin)/CLOCKS_PER_SEC;
        cout<<"Elapsed Time = "<<elapsed_time<<endl<<"OUTPUT: "<<endl;
        for (int it =0; it<TestVector.size(); it++)
          cout<<TestVector[it]<< " ";
        break;
      }
    }
  }
}

//  testVector2 = test->Rankedi(3);
//  for (unsigned int i = 0; i<testVector2.size(); i++){
//    cout<<testVector2[i]<<" ";
//  }
//  cout<<"\n";
//  
//  testVector2 = test->RankedGreaterThan(3);
//  for (unsigned int i = 0; i<testVector2.size(); i++){
//    cout<<testVector2[i]<<" ";
//  }
//  cout<<"\n";
//  
//  testVector2 = test->RankedWithinRange(1, 3);
//  for (unsigned int i = 0; i<testVector2.size(); i++){
//    cout<<testVector2[i]<<" ";
//  }
//  cout<<"\n";
//
//  testVector2 = test->RankediWitht(3, 4);
//  for (unsigned int i = 0; i<testVector2.size(); i++){
//    cout<<testVector2[i]<<" ";
//  }
//  cout<<"\n";
//  
//  testVector2 = test->RankediWithRange(1, 4, 3);
//  for (unsigned int i = 0; i<testVector2.size(); i++){
//    cout<<testVector2[i]<<" ";
//  }
//  cout<<"\n";
//  testVector2 = test->iRankedByj(3);
//  for (unsigned int i = 0; i<testVector2.size(); i++){
//    cout<<testVector2[i]<<" ";
//  }
//  cout<<"\n";
//  std::vector<uint64_t> m;
//  m.push_back(2);
//  m.push_back(3);
//  testVector2 = test->ItemsRankedBym(m);
//  for (unsigned int i = 0; i<testVector2.size(); i++){
//    cout<<testVector2[i]<<" ";
//  }
//  cout<<"\n";



