#include <bits/stdc++.h>
using namespace std;

int main(int argc,char *argv[]){
    //rootNode = new TreeNode();
    //int num[MAXNODES];
    //MaxChildPtrOfNode=atoi(argv[1]);
    FILE *p;
    p = fopen((char*)argv[1], "r");

    //vector<TreeNode*> Nodes;
    
    int ch;
    int j = 0;
    int totalValues = 0;
    
    char line[100];
    while(fgets(line,100,p)){
       int i=0;
       vector<int> opd;
       char *token = strtok(line,(const char*)" "),*op;
       while(token!=NULL){
          if(i==0) op=token;
          else opd.emplace_back(stoi(token));
          token = strtok(NULL,(const char*)" ");
          i++;
       }
       if(op=="RANGE")
          cout<<op<<"\n"<<opd[0]<<" "<<opd[1]<<"\n";
       else
          cout<<op<<"\n"<<opd[0]<<"\n";
       j++;
       if(j==3) break;
    }



    //printf("1.Insert a value\n2.Find a value\n3.Count number of occurance of a value\n4.Count number of elements in a range");
    /*while(fscanf(p, "%d", &ch)!=EOF){
        if(ch==1){
            fscanf(p, "%d", &num[i]);
            printf("**** Insert %d ****\n\n", num[i]);
            //insertNode(rootNode, num[i]);
            i++;
        }
        else if(ch==2){
            int x;
            fscanf(p, "%d", &x);
            //cout<<((Find(rootNode,x)==true)?"YES":"NO")<<"\n";
        }
        else if(ch==3){
            int x,tot_cnt=0;
            fscanf(p, "%d", &x);
            //Count(rootNode,x,tot_cnt);
            //cout<<tot_cnt<<"\n";
        }
        else{
            int L,R,tot_cnt=0;
            fscanf(p, "%d", &L);
            fscanf(p, "%d", &R);
            //Range_Count(rootNode,L,R,tot_cnt);
            //cout<<tot_cnt<<"\n";
        }
    }*/
} 
