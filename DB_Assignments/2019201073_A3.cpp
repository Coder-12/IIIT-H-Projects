#include<bits/stdc++.h>
using namespace std;

const int MAXNODES=1000001;

int MaxChildPtrOfNode;

struct TreeNode{
    int tNodes;
    TreeNode *next;
    TreeNode *parentNode; 
    int value[MAXNODES];
    TreeNode *childNode[MAXNODES];
    TreeNode(){ 
        tNodes = 0;
        next=NULL;
        parentNode = NULL;
        for(int i=0; i<MAXNODES; i++){
            value[i] = INT_MAX;
            childNode[i] = NULL;
        }
    }
};

TreeNode *rootNode;

void splitLeaf(TreeNode *curNode){
    int x, i, j;

    if(MaxChildPtrOfNode%2)
        x = (MaxChildPtrOfNode+1)/2;
    else x = MaxChildPtrOfNode/2;

    TreeNode *rightNode = new TreeNode();

    curNode->tNodes = x;
    rightNode->tNodes = MaxChildPtrOfNode-x;
    rightNode->parentNode = curNode->parentNode;

    for(i=x, j=0; i<MaxChildPtrOfNode; i++, j++){
        rightNode->value[j] = curNode->value[i];
        curNode->value[i] = INT_MAX;
    }
    int val = rightNode->value[0];

    if(curNode->parentNode==NULL){
        TreeNode *parentNode = new TreeNode();
        parentNode->parentNode = NULL;
        parentNode->tNodes=1;
        parentNode->value[0] = val;
        parentNode->childNode[0] = curNode;
        parentNode->childNode[1] = rightNode;
        curNode->parentNode = rightNode->parentNode = parentNode;

        curNode->next=rightNode;
        rootNode = parentNode;
        return;
    }
    else{   

        TreeNode *prevNode=curNode;
        curNode = curNode->parentNode;

        TreeNode *newChildNode = new TreeNode();
        newChildNode = rightNode;


        for(i=0; i<=curNode->tNodes; i++){
            if(val < curNode->value[i]){
                swap(curNode->value[i], val);
           }
        }

        curNode->tNodes++;

        for(i=0; i<curNode->tNodes; i++){
            if(newChildNode->value[0] < curNode->childNode[i]->value[0]){
                swap(curNode->childNode[i], newChildNode);
            }
        }
        curNode->childNode[i] = newChildNode;
        rightNode->next=prevNode->next;
        prevNode->next=rightNode;
        for(i=0;curNode->childNode[i]!=NULL;i++){
            curNode->childNode[i]->parentNode = curNode;
        }
    }

}
void splitNonLeaf(TreeNode *curNode){
    int x, i, j;

    x = MaxChildPtrOfNode/2;

    TreeNode *rightNode = new TreeNode();

    curNode->tNodes = x;
    rightNode->tNodes = MaxChildPtrOfNode-x-1;
    rightNode->parentNode = curNode->parentNode;


    for(i=x, j=0; i<=MaxChildPtrOfNode; i++, j++){
        rightNode->value[j] = curNode->value[i];
        rightNode->childNode[j] = curNode->childNode[i];
        curNode->value[i] = INT_MAX;
        if(i!=x) curNode->childNode[i] = NULL;
    }

    int val = rightNode->value[0];
    memcpy(&rightNode->value, &rightNode->value[1], sizeof(int)*(rightNode->tNodes+1));
    memcpy(&rightNode->childNode, &rightNode->childNode[1], sizeof(rootNode)*(rightNode->tNodes+1));


    for(i=0;curNode->childNode[i]!=NULL;i++){
        curNode->childNode[i]->parentNode = curNode;
    }
    for(i=0;rightNode->childNode[i]!=NULL;i++){
        rightNode->childNode[i]->parentNode = rightNode;
    }
    if(curNode->parentNode==NULL){
        TreeNode *parentNode = new TreeNode();
        parentNode->parentNode = NULL;
        parentNode->tNodes=1;
        parentNode->value[0] = val;
        parentNode->childNode[0] = curNode;
        parentNode->childNode[1] = rightNode;
        curNode->parentNode = rightNode->parentNode = parentNode;
        rootNode = parentNode;
        return;
    }
    else{  
        curNode = curNode->parentNode;
        TreeNode *newChildNode = new TreeNode();
        newChildNode = rightNode;
        for(i=0; i<=curNode->tNodes; i++){
            if(val < curNode->value[i]){
                swap(curNode->value[i], val);
            }
        }
        curNode->tNodes++;
        for(i=0; i<curNode->tNodes; i++){
            if(newChildNode->value[0] < curNode->childNode[i]->value[0]){
                swap(curNode->childNode[i], newChildNode);
            }
        }
        curNode->childNode[i] = newChildNode;

         for(i=0;curNode->childNode[i]!=NULL;i++){
            curNode->childNode[i]->parentNode = curNode;
        }
    }

}
void insertNode(TreeNode *curNode, int val){
    for(int i=0; i<=curNode->tNodes; i++){
        if(val < curNode->value[i] && curNode->childNode[i]!=NULL){
            insertNode(curNode->childNode[i], val);
            if(curNode->tNodes==MaxChildPtrOfNode)
                splitNonLeaf(curNode);
            return;
        }
        else if(val < curNode->value[i] && curNode->childNode[i]==NULL){
            swap(curNode->value[i], val);
            if(i==curNode->tNodes){
                    curNode->tNodes++;
                    break;
            }
        }
    }
    if(curNode->tNodes==MaxChildPtrOfNode){
            splitLeaf(curNode);
    }
}
bool Find(TreeNode *Broot,int x){
   if(Broot==NULL) return false;
   vector<int> arr(Broot->tNodes);
   for(int i=0;i<arr.size();i++)
      arr[i]=Broot->value[i];
   int idx=std::upper_bound(arr.begin(),arr.end(),x)-arr.begin();
   if(idx>0 and arr[idx-1]==x) return true;
   return Find(Broot->childNode[idx],x);
}

void Count(TreeNode *Broot,int x,int &tot_cnt){
   if(Broot==NULL) return;
   if(Broot->childNode[0]!=NULL)
      Count(Broot->childNode[0],x,tot_cnt);
   else{
       bool done=false;
       for(int i=0;i<Broot->tNodes;i++){
          if(Broot->value[i]==x)
             tot_cnt++;
      }
      TreeNode *nextNode=Broot->next;
      while(nextNode!=NULL){
         for(int i=0;i<nextNode->tNodes;i++){
             if(nextNode->value[i]==x)
                tot_cnt++;
         }
         nextNode=nextNode->next;
      }
    }
}

void Range_Count(TreeNode *Broot,int L,int R,int &tot_cnt){
   if(Broot->childNode[0]!=NULL)
      Range_Count(Broot->childNode[0],L,R,tot_cnt);
   else{
      bool done=false;
      for(int i=0;i<Broot->tNodes;i++){
          if(Broot->value[i]>=L and Broot->value[i]<=R)
             tot_cnt++;
      }
      TreeNode *nextNode=Broot->next;
      while(nextNode!=NULL){
         for(int i=0;i<nextNode->tNodes;i++){
             if(nextNode->value[i]>=L and nextNode->value[i]<=R)
                tot_cnt++;
         }
         nextNode=nextNode->next;
      }
   }
}

int main(int argc,char *argv[]){
    rootNode = new TreeNode();
    int num[MAXNODES];
    MaxChildPtrOfNode=atoi(argv[2]);
    FILE *p,*out;
    p = fopen((char*)argv[3], "r");

    out = fopen((char*)argv[4],"w+");
    vector<TreeNode*> Nodes;
    
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
  
       if(strcmp(op,(char*)"INSERT")==0){
             insertNode(rootNode, opd[0]);
       }
       else if(strcmp(op,(char*)"FIND")==0){
             char *str;
             if(Find(rootNode,opd[0])==true) str = (char*)"YES";
             else str = (char*)"NO";
             fprintf(out,"%s\n",str);
       }
        else if(strcmp(op,(char*)"COUNT")==0){
            int x,tot_cnt=0;
            Count(rootNode,opd[0],tot_cnt);
            fprintf(out,"%d\n",tot_cnt);
        }
        else{
            int tot_cnt=0;
            Range_Count(rootNode,opd[0],opd[1],tot_cnt);
            fprintf(out,"%d\n",tot_cnt);
        }
    }
}
