#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
unordered_map<string,vector<string>> schemas;
unordered_map<string,vector<int>> table1,table2;
vector<string> strs;
vector<int> rows;
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
      row_seperate(buffer,0);
    } 
    while(fgets(buffer,1024,(FILE*)fd2)) row_seperate(buffer,1);
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
               if(j<schemas["table2"].size()-1) cout<<table2[schemas["table2"][j]][i]<<",";
               else cout<<table2[schemas["table2"][j]][i]<<"\n";
           }
       }
    }

}
void project(string table){
    std::string temp=string(table)+string(".");
    vector<string> att;
    for(auto &v:strs[1]) if(v!=',') att.push_back(string{v});
    if(table=="table1"){
       for(auto &v:att){
           if(find(schemas["table1"].begin(),schemas["table1"].end(),v)==schemas["table1"].end()){
              cout<<"Invalid Query\n";
              return;
           }
       }
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
        for(auto &v:att){
           if(find(schemas["table2"].begin(),schemas["table2"].end(),v)==schemas["table2"].end()){
              cout<<"Invalid Query\n";
              return;
           }
       }
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
}
void Max(string table){
    string col{strs[1][4]};
    if(table=="table1"){
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
    else{
       if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       int ans=INT_MIN;
       cout<<strs[1]<<"\n";
       for(int i=0;i<table1[schemas["table2"][0]].size();i++){
           ans=max(ans,table2[col][i]);
       }
       cout<<ans<<"\n";
    }
}
void Min(string table){
    string col{strs[1][4]};
    if(table=="table1"){
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
    else{
       if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       int ans=INT_MAX;
       cout<<strs[1]<<"\n";
       for(int i=0;i<table1[schemas["table2"][0]].size();i++){
           ans=min(ans,table2[col][i]);
       }
       cout<<ans<<"\n";
    }
}
void Sum(string table){
    string col{strs[1][4]};
    if(table=="table1"){
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
    else{
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
}
void Avg(string table){
    string col{strs[1][4]};
    if(table=="table1"){
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
       cout<<avg<<"\n";
    }
    else{
       if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       int sum=0;
       double avg;
       cout<<strs[1]<<"\n";
       for(int i=0;i<table1[schemas["table2"][0]].size();i++){
           sum+=table2[col][i];
       }
       avg=(double)sum/(double)table1[schemas["table2"][0]].size();
       cout<<avg<<"\n";
    }
}
void Dist(string table){
    string col{strs[1][5]};
    if(table=="table1"){
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
}
void checkcond(string table,string col,string rel,int x){
    if(table=="table1"){
       if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
          cout<<"Invalid Query\n";
          cout<<"no\n";
          return;
       }
       for(int i=0;i<table1[col].size();i++){
           if(rel==">" and table1[col][i]>x) rows.push_back(i);
           else if(rel=="==" and table1[col][i]==x) rows.push_back(i);
           else if(rel=="<=" and table1[col][i]<=x) rows.push_back(i);
           else if(rel==">" and table1[col][i]>x) rows.push_back(i);
           else if(rel==">=" and table1[col][i]>=x) rows.push_back(i);
           else if(rel==">" and table1[col][i]>x) rows.push_back(i);
       }
    }
    else{
       if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       for(int i=0;i<table2[col].size();i++){
           if(rel==">" and table2[col][i]>x) rows.push_back(i);
           else if(rel=="==" and table2[col][i]==x) rows.push_back(i);
           else if(rel=="<" and table2[col][i]<x) rows.push_back(i);
           else if(rel=="<=" and table2[col][i]<=x) rows.push_back(i);
           else if(rel==">" and table2[col][i]>x) rows.push_back(i);
           else if(rel==">=" and table1[col][i]>=x) rows.push_back(i);
       }
    }
}
void checknestedcond(string table,string col,string rel,int x){
    vector<int> trows;
    if(table=="table1"){
       if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       for(int i=0;i<rows.size();i++){
           if(rel==">" and table1[col][rows[i]]>x) trows.push_back(rows[i]);
           else if(rel=="==" and table1[col][rows[i]]==x) trows.push_back(rows[i]);
           else if(rel=="<" and table1[col][rows[i]]<x) trows.push_back(rows[i]);
           else if(rel=="<=" and table1[col][rows[i]]<=x) trows.push_back(rows[i]);
           else if(rel==">" and table1[col][rows[i]]>x) trows.push_back(rows[i]);
           else if(rel==">=" and table1[col][i]>=x) trows.push_back(rows[i]);
       }
    }
    else{
       if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       for(int i=0;i<rows.size();i++){
           if(rel==">" and table2[col][rows[i]]>x) trows.push_back(rows[i]);
           else if(rel=="==" and table2[col][rows[i]]==x) trows.push_back(rows[i]);
           else if(rel=="<" and table2[col][rows[i]]<x) trows.push_back(rows[i]);
           else if(rel=="<=" and table2[col][rows[i]]<=x) trows.push_back(rows[i]);
           else if(rel==">" and table2[col][rows[i]]>x) trows.push_back(rows[i]);
           else if(rel==">=" and table1[col][i]>=x) trows.push_back(rows[i]);
       }
    }
    rows.clear();
    for(int i=0;i<trows.size();i++) rows[i]=trows[i];
}

void wherecond(string table){
    string col,rel,val;
    int j=0,x;
    while(j<strs[5].size() and strs[5][j]!='<' and strs[5][j]!='>' and strs[5][j]!='=') col.push_back(strs[5][j++]);
    while(j<strs[5].size() and (strs[5][j]=='>' or strs[5][j]=='=' or strs[5][j]=='<')) rel.push_back(strs[5][j++]);
    while(j<strs[5].size() and strs[5][j]>=48 and strs[5][j]<=57) val.push_back(strs[5][j++]);
    x=stoi(val);
    //cout<<col<<"\n"<<rel<<"\n"<<val<<"\n";
    checkcond(table,col,rel,x);
    if(rows.empty()) return;
    if(strs.size()>7 and strs[6]=="AND"){
       if(strs.size()==7){
          cout<<"Invalid Query\n";
          return;
       }
       string col,rel,val;
       int j=0,x;
       while(j<strs[7].size() and strs[7][j]!='<' and strs[7][j]!='>' and strs[7][j]!='=') col.push_back(strs[7][j++]);
       while(j<strs[7].size() and (strs[7][j]=='>' or strs[7][j]=='=' or strs[7][j]=='<')) rel.push_back(strs[7][j++]);
       while(j<strs[7].size() and strs[7][j]>=48 and strs[7][j]<=57) val.push_back(strs[7][j++]);
       x=stoi(val);
       checknestedcond(table,col,rel,x);
    }
}
void condselect(string table){
    std::string temp=string("table1")+string(".");
    if(table=="table1"){
       cout<<(temp+schemas["table1"][0])<<","<<(temp+schemas["table1"][1])<<","<<(temp+schemas["table1"][2])<<"\n";
       for(int i=0;i<rows.size();i++){
           for(int j=0;j<schemas["table1"].size();j++){
               if(j<schemas["table1"].size()-1) cout<<table1[schemas["table1"][j]][rows[i]]<<",";
               else cout<<table1[schemas["table1"][j]][rows[i]]<<"\n";
           }
       }
    }
    else{
       cout<<(temp+schemas["table2"][0])<<","<<(temp+schemas["table2"][1])<<"\n";
       for(int i=0;i<rows.size();i++){
           for(int j=0;j<schemas["table2"].size();j++){
               if(j<schemas["table2"].size()-1) cout<<table2[schemas["table2"][j]][rows[i]]<<",";
               else cout<<table2[schemas["table2"][j]][rows[i]]<<"\n";
           }
       }
    }
}
void condproject(string table){
    std::string temp=string("table1")+string(".");
    vector<string> att;
    for(auto &v:strs[1]) if(v!=',') att.push_back(string{v});
    if(table=="table1"){
       for(auto &v:att){
           if(find(schemas["table1"].begin(),schemas["table1"].end(),v)==schemas["table1"].end()){
              cout<<"Invalid Query\n";
              return;
           }
       }
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
      for(auto &v:att){
           if(find(schemas["table2"].begin(),schemas["table2"].end(),v)==schemas["table2"].end()){
              cout<<"Invalid Query\n";
              return;
           }
       }
       for(int i=0;i<att.size();i++){
           if(i<att.size()-1) cout<<(temp+att[i])<<",";
           else cout<<(temp+att[i])<<"\n";
       }
       for(int i=0;i<rows.size();i++){
           for(int j=0;j<att.size();j++){
               if(j<att.size()-1) cout<<table2[att[j]][rows[i]]<<",";
               else cout<<table2[att[j]][rows[i]]<<"\n";
           }
       }
    }
}
void condMax(string table){
    string col{strs[1][4]};
    if(table=="table1"){
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
    else{
       if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       int ans=INT_MIN;
       cout<<strs[1]<<"\n";
       for(int i=0;i<rows.size();i++){
           ans=max(ans,table2[col][rows[i]]);
       }
       cout<<ans<<"\n";
    }
}
void condMin(string table){
    string col{strs[1][4]};
    if(table=="table1"){
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
    else{
       if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       int ans=INT_MAX;
       cout<<strs[1]<<"\n";
       for(int i=0;i<rows.size();i++){
           ans=min(ans,table2[col][rows[i]]);
       }
       cout<<ans<<"\n";
    }
}
void condSum(string table){
    string col{strs[1][4]};
    if(table=="table1"){
       if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       int ans=0;
       cout<<strs[1]<<"\n";
       for(int i=0;i<rows.size();i++){
           ans+=table1[col][rows[i]];
       }
       cout<<ans<<"\n";
    }
    else{
       if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       int ans=0;
       cout<<strs[1]<<"\n";
       for(int i=0;i<rows.size();i++){
           ans+=table2[col][rows[i]];
       }
       cout<<ans<<"\n";
    }
}
void condAvg(string table){
    string col{strs[1][4]};
    if(table=="table1"){
       if(find(schemas["table1"].begin(),schemas["table1"].end(),col)==schemas["table1"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       int ans=0;
       double avg;
       cout<<strs[1]<<"\n";
       for(int i=0;i<rows.size();i++){
           ans+=table1[col][rows[i]];
       }
       avg=(double)ans/(double)rows.size();
       cout<<avg<<"\n";
    }
    else{
       if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       int ans=0;
       double avg;
       cout<<strs[1]<<"\n";
       for(int i=0;i<rows.size();i++){
           ans+=table2[col][rows[i]];
       }
       avg=(double)ans/(double)rows.size();
       cout<<avg<<"\n";
    }
}
void condDist(string table){
    string col{strs[1][5]};
    if(table=="table1"){
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
       if(find(schemas["table2"].begin(),schemas["table2"].end(),col)==schemas["table2"].end()){
          cout<<"Invalid Query\n";
          return;
       }
       cout<<strs[1]<<"\n";
       unordered_set<int> distrow;
       for(int i=0;i<rows.size();i++){
           if(distrow.find(table2[col][rows[i]])==distrow.end()){
              cout<<table2[col][rows[i]]<<"\n";
              distrow.insert(table2[col][rows[i]]);
           }
       }
    }
}
void process_Query(char *query){
      query_tokenize(query);
      cout<<strs.size()<<"\n";
      if(strs[3].size()==6){
         if(strs[3]=="table1"){
            if(strs.size()==4){
               if(strs[1]=="*") select("table1");
               else if(strs[1].size()<6) project("table1");
               else if(strs[1].substr(0,3)=="max") Max("table1");
               else if(strs[1].substr(0,3)=="min") Min("table1");
               else if(strs[1].substr(0,3)=="sum") Sum("table1");
               else if(strs[1].substr(0,3)=="avg") Avg("table1");
               else if(strs[1].substr(0,4)=="dist") Dist("table1");
            }
            else{
               //cout<<strs.size()<<"\n"; 
               wherecond("table1");
               if(rows.empty()){
                  cout<<"No rows selected\n";
                  return;
               }
               if(strs[1]=="*") condselect("table1");
               else if(strs[1].size()<6) condproject("table1");
               else if(strs[1].substr(0,3)=="max") condMax("table1");
               else if(strs[1].substr(0,3)=="min") condMin("table1");
               else if(strs[1].substr(0,3)=="sum") condSum("table1");
               else if(strs[1].substr(0,3)=="avg") condAvg("table1");
               else if(strs[1].substr(0,4)=="dist") condDist("table1");
               else{
                   cout<<"Invalid Query\n";
                   return;
               }
           }     
         }
         else{
           if(strs.size()==4){
              if(strs[1]=="*") select("table2");
              else if(strs[1].size()<6) project("table2");
              else if(strs[1].substr(0,3)=="max") Max("table2");
              else if(strs[1].substr(0,3)=="min") Min("table2");
              else if(strs[1].substr(0,3)=="sum") Sum("table2");
              else if(strs[1].substr(0,3)=="avg") Avg("table2");
              else if(strs[1].substr(0,4)=="dist") Dist("table2");
              else{
                   cout<<"Invalid Query\n";
                   return;
               }

           }
           else{
              wherecond("table2");
              if(rows.empty()){
                  cout<<"No rows selected\n";
                  return;
              }
              if(strs[1]=="*") condselect("table2");
              else if(strs[1].size()<6) condproject("table2");
              else if(strs[1].substr(0,3)=="max") condMax("table2");
              else if(strs[1].substr(0,3)=="min") condMin("table2");
              else if(strs[1].substr(0,3)=="sum") condSum("table2");
              else if(strs[1].substr(0,3)=="avg") condAvg("table2");
              else if(strs[1].substr(0,4)=="dist") condDist("table2");
              else{
                   cout<<"Invalid Query\n";
                   return;
              }
           }
        }        
    }
}

int main(int argc,char* argv[]){
    pre_process();
    process_Query(argv[1]);
    exit(EXIT_SUCCESS);
}
