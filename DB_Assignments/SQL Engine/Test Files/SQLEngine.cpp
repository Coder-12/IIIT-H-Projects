#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
unordered_map<string,vector<string>> schemas;
unordered_map<string,vector<int>> table1,table2;
vector<string> strs;
void row_seperate(char *line,bool flag){
    char *token=strtok(line,",");
    int i=0;
    while(token!=NULL){
       if(flag==0) table1[schemas["table1"][i]].emplace_back(atoi(token));
       else table2[schemas["table2"][i]].emplace_back(atoi(token));
       token=strtok(NULL,",");
       i++;
    }
}
void pre_process(){
    FILE *md,*fd1,*fd2;
    char *line=NULL;
    size_t len=0;
    ssize_t read;
    int i=0;
    string str("");
    md=fopen("/home/aklesh/Downloads/SQLEngine/files/metadata.txt","r");
    if(md==NULL) exit(EXIT_FAILURE);
    while((read=getline(&line,&len,md))!=-1){
         i++;
         string temp("");
         if(line[strlen(line)-1]=='\n'){
            for(int j=0;j<=strlen(line)-3;j++)
                temp.push_back(line[j]);
         }
         else{
            for(int j=0;j<=strlen(line)-1;j++)
                temp.push_back(line[j]);

         }
         if(temp=="<end_table>") str.clear(),i=0;
         else if(i==2) str+=temp;
         else if(i>2) schemas[str].emplace_back(temp);
    }
    fclose(md);
    if(line) free(line);
    fd1=fopen("/home/aklesh/Downloads/SQLEngine/files/table1.csv","r");
    fd2=fopen("/home/aklesh/Downloads/SQLEngine/files/table2.csv","r");
    char buffer[1024];
    while(fgets(buffer,1024,(FILE*)fd1)){
      //char *temp=strdup(buffer);
      row_seperate(buffer,0);
      //free(temp);
    } 
    while(fgets(buffer,1024,(FILE*)fd2)) row_seperate(buffer,1);
    //for(auto &v:table1[schemas["table1"][0]]) cout<<v<<" ";
}
void query_tokenize(char *query){
    char *token=strtok_r(query," ",&query);
    while(token!=NULL){
       string s(token);
       strs.emplace_back(s);
       token=strtok_r(query," ",&query);
    }
}
void select(string table){
    std::string temp=string(table)+string(".");
    if(table=="table1"){
       cout<<(temp+schemas[table][0])<<","<<(temp+schemas[table][1])<<","<<(temp+schemas[table][2])<<"\n";
       for(int i=0;i<table1[schemas[table][0]].size();i++){
           for(int j=0;j<schemas["table1"].size();j++){
               if(j<schemas["table1"].size()-1) cout<<table1[schemas["table1"][j]][i]<<",";
               else cout<<table1[schemas["table1"][j]][i]<<"\n";
           }
       }
    }
    else{
       cout<<(temp+schemas[table][0])<<","<<(temp+schemas[table][1])<<"\n";
       for(int i=0;i<table2[schemas[table][0]].size();i++){
           for(int j=0;j<schemas["table2"].size();j++){
               if(j<schemas["table2"].size()-1) cout<<table1[schemas["table2"][j]][i]<<",";
               else cout<<table1[schemas["table2"][j]][i]<<"\n";
           }
       }
    }

}
void process_Query(char *query){
    query_tokenize(query);
    //cout<<strs[strs.size()-1].size()<<"\n"
      if(strs[strs.size()-1].size()==6){
         if(strs[strs.size()-1]=="table1"){
            if(strs.size()==4){
               if(strs[1]=="*"){
                   std::string temp=string("table1")+string(".");
                   cout<<(temp+schemas["table1"][0])<<","<<(temp+schemas["table1"][1])<<","<<(temp+schemas["table1"][2])<<"\n";
                   for(int i=0;i<table1[schemas["table1"][0]].size();i++){
                     for(int j=0;j<schemas["table1"].size();j++){
                         if(j<schemas["table1"].size()-1) cout<<table1[schemas["table1"][j]][i]<<",";
                         else cout<<table1[schemas["table1"][j]][i]<<"\n";
                     }
                  }          
               }
               else if(strs[1].size()<6){
                    vector<string> att;
                    for(auto &v:strs[1]) if(v!=',') att.push_back(string{v});
                    for(auto &v:att){
                        if(find(schemas["table1"].begin(),schemas["table1"].end(),v)==schemas["table1"].end()){
                           cout<<"Invalid Query\n";
                           return;
                        }
                    }
                    std::string temp=string("table1")+string(".");
                    for(int i=0;i<att.size();i++){
                        if(i<att.size()-1) cout<<(temp+att[i])<<",";
                        else cout<<(temp+att[i])<<"\n";
                    } 
                    for(int i=0;i<table1[schemas["table1"][0]].size();i++){
                        for(int j=0;j<att.size();j++){
                            if(j<att.size()-1) cout<<table1[att[j]][i]<<",";
                            else cout<<table1[att[j]][i]<<"\n";
                        }
                    }               
               }
               else{
                    string col;
                    if(strs[1].substr(0,3)=="max"){
                       col.push_back(strs[1][4]);
                       if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
                          cout<<"Invalid Query\n";
                          return;
                       }
                       int ans=INT_MIN;
                       cout<<strs[1]<<"\n";
                       for(int i=0;i<table1[schemas["table1"][0]].size();i++){
                           ans=max(ans,table1[col][i]);
                       }
                       cout<<ans<<"\n";
                    }
                    else if(strs[1].substr(0,3)=="min"){
                      col.push_back(strs[1][4]);
                       if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
                          cout<<"Invalid Query\n";
                          return;
                       }
                       int ans=INT_MAX;
                       cout<<strs[1]<<"\n";
                       for(int i=0;i<table1[schemas["table1"][0]].size();i++){
                           ans=min(ans,table1[col][i]);
                       }
                       cout<<ans<<"\n";
                    }
                    else if(strs[1].substr(0,3)=="sum"){
                      col.push_back(strs[1][4]);
                       if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
                          cout<<"Invalid Query\n";
                          return;
                       }
                       int sum=0;
                       cout<<strs[1]<<"\n";
                       for(int i=0;i<table1[schemas["table1"][0]].size();i++){
                           sum+=table1[col][i];
                       }
                       cout<<sum<<"\n";
                    }
                    else if(strs[1].substr(0,3)=="avg"){
                       col.push_back(strs[1][4]);
                       if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
                          cout<<"Invalid Query\n";
                          return;
                       }
                       int sum=0;
                       double avg;
                       cout<<strs[1]<<"\n";
                       for(int i=0;i<table1[schemas["table1"][0]].size();i++){
                           sum+=table1[col][i];
                       }
                       avg=(double)sum/(double)table1[schemas["table1"][0]].size();
                       cout<<fixed<<setprecision(6)<<avg<<"\n";
                    }
                    else if(strs[1].substr(0,4)=="dist"){
                        col.push_back(strs[1][5]);
                        if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
                          cout<<"Invalid Query\n";
                          return;
                        }
                        cout<<strs[1]<<"\n";
                        unordered_set<int> distrow;
                        for(int i=0;i<table1[schemas["table1"][0]].size();i++){
                            if(distrow.find(table1[col][i])==distrow.end()){
                               cout<<table1[col][i]<<"\n";
                               distrow.insert(table1[col][i]);
                            }
                        }
                    }
                    else{
                       cout<<"Invalid Query"<<"\n";
                       return;
                    }
               }
            }
            else if(strs.size()==5){ 
                   string col,rel,val;
                   int j=0,x;
                   while(strs[4][j]>='A' and strs[4][j]<='z') col.push_back(strs[4][j++]);
                   while(strs[4][j]=='>' or strs[4][j]=='=' or strs[4][j]=='<') rel.push_back(strs[4][j++]);
                   while(strs[4][j]>=48 and strs[4][j]<=57) val.push_back(strs[4][j]);
                   if(find(schemas["table1"].begin(),schemas["table2"].end(),col)==schemas["table1"].end()){
                      cout<<"Invalid Query\n";
                      return;
                   }
                   x=stoi(val);
                   vector<int> rows;
                   if(rows.empty()){
                      cout<<"No rows selected\n";
                      return;
                   }
                   for(int i=0;i<table1[col].size();i++){
                        if(rel==">" and table1[col][i]>x) rows.push_back(i);
                        else if(rel=="==" and table1[col][i]==x) rows.push_back(i);
                        else if(rel=="<" and table1[col][i]<x) rows.push_back(i);
                        else if(rel=="<" and table1[col][i]<x) rows.push_back(i);
                        else if(rel=="<" and table1[col][i]<x) rows.push_back(i);
                        else{
                           cout<<"Invalid Query"<<"\n";
                           return;
                        }
                   }
                   if(strs[1]=="*"){
                      std::string temp=string("table1")+string(".");
                      cout<<(temp+schemas["table1"][0])<<","<<(temp+schemas["table1"][1])<<","<<(temp+schemas["table1"][2])<<"\n";
                      for(int i=0;i<rows.size();i++){
                          for(int j=0;j<schemas["table1"].size();j++){
                              if(j<schemas["table1"].size()-1) cout<<table1[schemas["table1"][j]][rows[i]]<<",";
                              else cout<<table1[schemas["table1"][j]][rows[i]]<<"\n";
                          }
                      }      
                   }
                   else if(strs[1].size()<6){
                       vector<string> att;
                       for(auto &v:strs[1]) if(v!=',') att.push_back(string{v});
                       for(auto &v:att){
                           if(find(schemas["table1"].begin(),schemas["table1"].end(),v)==schemas["table1"].end()){
                              cout<<"Invalid Query\n";
                              return;
                           }
                       }
                       std::string temp=string("table1")+string(".");
                       for(int i=0;i<att.size();i++){
                           if(i<att.size()-1) cout<<(temp+att[i])<<",";
                           else cout<<(temp+att[i])<<"\n";
                       }
                       for(int i=0;i<rows.size();i++){
                          for(int j=0;j<att.size();j++){
                             if(j<att.size()-1) cout<<table1[att[j]][rows[i]]<<",";
                             else cout<<table1[att[j]][rows[i]]<<"\n";
                          }
                       }
                   }
                   else{
                       string col;
                       if(strs[1].substr(0,3)=="max"){
                          col.push_back(strs[1][4]);
                          if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
                             cout<<"Invalid Query\n";
                             return;
                          }
                          int ans=INT_MIN;
                          cout<<strs[1]<<"\n";
                          for(int i=0;i<rows.size();i++){
                              ans=max(ans,table1[col][rows[i]]);
                          }
                          cout<<ans<<"\n";
                       }
                       else if(strs[1].substr(0,3)=="min"){
                          col.push_back(strs[1][4]);
                          if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
                             cout<<"Invalid Query\n";
                             return;
                          }
                          int ans=INT_MAX;
                          cout<<strs[1]<<"\n";
                          for(int i=0;i<rows.size();i++){
                              ans=min(ans,table1[col][rows[i]]);
                          }
                          cout<<ans<<"\n";
                       }
                       else if(strs[1].substr(0,3)=="sum"){
                          col.push_back(strs[1][4]);
                          if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
                             cout<<"Invalid Query\n";
                             return;
                          }
                          int sum=0;
                          cout<<strs[1]<<"\n";
                          for(int i=0;i<rows.size();i++){
                              sum+=table1[col][rows[i]];
                          }
                          cout<<sum<<"\n";
                       }
                       else if(strs[1].substr(0,3)=="avg"){
                          col.push_back(strs[1][4]);
                          if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
                             cout<<"Invalid Query\n";
                             return;
                          }
                          int sum=0;
                          double avg;
                          cout<<strs[1]<<"\n";
                          for(int i=0;i<rows.size();i++){
                              sum+=table1[col][rows[i]];
                          }
                          avg=(double)sum/(double)rows.size();
                          cout<<fixed<<setprecision(6)<<avg<<"\n";
                       }
                       else if(strs[1].substr(0,4)=="dist"){
                          col.push_back(strs[1][5]);
                          if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
                             cout<<"Invalid Query\n";
                             return;
                          }
                          cout<<strs[1]<<"\n";
                          unordered_set<int> distrow;
                          for(int i=0;i<rows.size();i++){
                              if(distrow.find(table1[col][rows[i]])==distrow.end()){
                                 cout<<table1[col][rows[i]]<<"\n";
                                 distrow.insert(table1[col][rows[i]]);
                              }
                          }
                       }
                       else{
                          cout<<"Invalid Query\n";
                          return;
                       }
                 }
                 
            }
            else{
                 if(strs.size()==4){
                    if(strs[1]=="*"){ 
                       std::string temp=string("table2")+string(".");
                       cout<<(temp+schemas["table2"][0])<<","<<(temp+schemas["table2"][1])<<"\n";
                       for(int i=0;i<table1[schemas["table2"][0]].size();i++){
                          for(int j=0;j<schemas["table2"].size();j++){
                             if(j<schemas["table2"].size()-1) cout<<table2[schemas["table2"][j]][i]<<",";        
                             else cout<<table2[schemas["table2"][j]][i]<<"\n";        
                          }
                       }
                    }
                    else if(strs[1].size()<4){
                       vector<string> att;
                       for(auto &v:strs[1]) if(v!=',') att.push_back(string{v});
                       for(auto &v:att){
                          if(find(schemas["table2"].begin(),schemas["table2"].end(),v)==schemas["table2"].end()){
                             cout<<"Invalid Query\n";
                             return;
                          }
                       } 
                       std::string temp=string("table2")+string(".");
                       for(int i=0;i<att.size();i++){
                          if(i<att.size()-1) cout<<(temp+att[i])<<",";
                          else cout<<(temp+att[i])<<"\n";
                       }
                       for(int i=0;i<table2[schemas["table2"][0]].size();i++){
                          for(int j=0;j<att.size();j++){
                              if(j<att.size()-1) cout<<table2[att[j]][i]<<",";
                              else cout<<table2[att[j]][i]<<"\n";
                          }
                       }
                    }
                    else{
                       string col;
                       if(strs[1].substr(0,3)=="max"){
                         col.push_back(strs[1][4]);
                         if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
                            cout<<"Invalid Query\n";
                            return;
                         }
                         int ans=INT_MIN;
                         cout<<strs[1]<<"\n";
                         for(int i=0;i<table2[schemas["table2"][0]].size();i++){
                             ans=max(ans,table2[col][i]);
                         }
                         cout<<ans<<"\n";
                      }
                      else if(strs[1].substr(0,3)=="min"){
                         col.push_back(strs[1][4]);
                         if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
                            cout<<"Invalid Query\n";
                            return;
                         }
                         int ans=INT_MAX;
                         cout<<strs[1]<<"\n";
                         for(int i=0;i<table2[schemas["table2"][0]].size();i++){
                             ans=min(ans,table2[col][i]);
                         }
                         cout<<ans<<"\n";
                      } 
                      else if(strs[1].substr(0,3)=="sum"){
                         col.push_back(strs[1][4]);
                         if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
                            cout<<"Invalid Query\n";
                            return;
                         }
                         int sum=0;
                         cout<<strs[1]<<"\n";
                         for(int i=0;i<table1[schemas["table2"][0]].size();i++){
                             sum+=table2[col][i];
                         }
                         cout<<sum<<"\n";
                      }
                      else if(strs[1].substr(0,3)=="avg"){
                         col.push_back(strs[1][4]);
                         if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
                            cout<<"Invalid Query\n";
                            return;
                         }
                         int sum=0;
                         double avg;
                         cout<<strs[1]<<"\n";
                         for(int i=0;i<table2[schemas["table2"][0]].size();i++){
                             sum+=table2[col][i];
                         }
                         avg=(double)sum/(double)table2[schemas["table2"][0]].size();
                         cout<<fixed<<setprecision(6)<<avg<<"\n";
                      }
                      else if(strs[1].substr(0,4)=="dist"){
                          col.push_back(strs[1][5]);
                          if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
                             cout<<"Invalid Query\n";
                             return;
                          }
                          cout<<strs[1]<<"\n";
                          unordered_set<int> distrow;
                          for(int i=0;i<table2[schemas["table2"][0]].size();i++){
                             if(distrow.find(table2[col][i])==distrow.end()){
                                cout<<table2[col][i]<<"\n";
                                distrow.insert(table2[col][i]);
                             }
                         }
                     }
                     else{
                        cout<<"Invalid Query"<<"\n";
                        return;
                     }
                  }
                  else if(str.size()==5){
                  }
            }        
        }
    }
    /*for(auto &v:strs) cout<<v<<"\n";
    cout<<"\n";*/
}

int main(int argc,char* argv[]){
    pre_process();
    process_Query(argv[1]);
    exit(EXIT_SUCCESS);
}
