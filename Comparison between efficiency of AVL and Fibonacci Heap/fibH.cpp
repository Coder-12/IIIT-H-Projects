#include<bits/stdc++.h>
#include <time.h>
#include <chrono>
#include <ctime>
#define ll long long
using namespace std;
const ll Mx=(ll)1e18+11;
struct Node{
    ll d,deg;
    bool m,ch;
    Node *l,*r,*c,*p;
    Node(){}
    //Node(Node *l,Node *r,Node *c,Node *p,ll x):l{l},r{r},c{c},p{p},d{x} {}
};
Node *root=NULL;
int n;
void insert_fib(ll x){
    Node *pr=new Node();
    pr->deg=0;pr->m=0;
    pr->ch=0;
    pr->c=0;pr->p=0;
    pr->d=x;
    pr->l=pr;pr->r=pr;
    if(root){
       (root->l)->r=pr;
       pr->r=root;
       pr->l=root->l;
       root->l=pr;
       if((pr->d)<(root->d))
         root=pr;
    }
    else root=pr;
    n++;
}
void print(){
     Node *p=root;
     if(!p){
         cout<<"No node in Fib. heap"<<"\n";
         return ;
     }
     cout<<"Root nodes of Fib. heap are:\n";
     do{
       cout<<(p->d);
       p=p->r;
       if(p!=root) cout<<"->";
     }while(p!=root and p->r!=NULL);
     cout<<"\n"<<"Fib. heap has "<<n<<" nodes"<<"\n";
} 
void cut(Node *ptr,Node *tmp){
    if(ptr==(ptr->r)) tmp->c=NULL;
    (ptr->l)->r=ptr->r;
    (ptr->r)->l=ptr->l;
    if(ptr==(tmp->c)) tmp->c=ptr->r;
    tmp->deg=tmp->deg-1;
    ptr->l=ptr;
    ptr->r=ptr;
    (root->l)->r=ptr;
    ptr->r=root;
    ptr->l=root->l;
    root->l=ptr;
    ptr->p=NULL;
    ptr->m=true;
}
void cascade_cut(Node *tmp){
    Node *ptr=tmp->p;
    if(ptr!=NULL){
       if(tmp->m==false)
          tmp->m=true;
       else{
          cut(tmp,ptr);
          cascade_cut(ptr);
       }
    }
}
void decr_key(Node *ptr,ll x){
    if(!root){
       cout<<"Empty Fib. heap\n";
       return;
     }
     if(!ptr){
       cout<<"Node not found\n";
       return;
     }
     ptr->d=x;
     Node *tmp=ptr->p;
     if(tmp!=NULL and (ptr->d)<(tmp->d)){
        cut(ptr,tmp);
        cascade_cut(tmp);
     }
     if((ptr->d)<(root->d)) root=ptr;
}
void fib_link(Node *p1,Node *p2){
     (p1->l)->r=p1->r;
     (p1->r)->l=p1->l;
     if(p2->r==p2) root=p2;
     p1->l=p1;
     p1->r=p1;
     p1->p=p2;
     if(p2->c==NULL) p2->c=p1;
     p1->r=p2->c;
     p1->l=(p2->c)->l;
     ((p2->c)->l)->r=p1;
     (p2->c)->l=p1;
     if((p1->d)<((p2->c)->d)) p2->c=p1;
     (p2->deg)++;
}
void consolidate(){
    ll t1;
    ll t2=(ll)log2(n);
    Node *arr[t2+1];
    for(int i=0;i<=t2;i++) arr[i]=0;
    Node *p1=root,*p2,*p3,*p4=p1;
    do{
       p4=p4->r;
       t1=p1->deg;
       while(arr[t1]){
            p2=arr[t1];
            if((p1->d)>(p2->d)){
                p3=p1;
                p1=p2;
                p2=p3;
            }
            if(p2==root) root=p1;
            fib_link(p2,p1);
            if((p1->r)==p1) root=p1;
            arr[t1]=NULL;
            t1++;
       }
       arr[t1]=p1;
       p1=p1->r;
    }while(p1!=root);
    root=NULL;
    for(int i=0;i<=t2;i++){
        if(arr[i]!=NULL){
           arr[i]->l=arr[i];
           arr[i]->r=arr[i];
           if(root!=NULL){
              (root->l)->r=arr[i];
              arr[i]->r=root;
              arr[i]->l=root->l;
              root->l=arr[i];
              if((arr[i]->d)<(root->d)) root=arr[i];
           }
           else root=arr[i];
           if(root==NULL) root=arr[i];
           else if((arr[i]->d)<(root->d)) root=arr[i];
        }
    }
}
void ext_min(){
    if(!root){
      cout<<"Empty Fib. heap\n";
      return;
    }
    Node *tmp=root,*ptr=tmp,*ptr1=NULL;
    if((tmp->c)!=NULL){
        ptr1=tmp->c;
        do{
           ptr=ptr1->r;
           (root->l)->r=ptr1;
           ptr1->r=root;
           ptr1->l=root->l;
           root->l=ptr1;
           if((ptr1->d)<(root->d)) root=ptr1;
           ptr1->p=NULL;
           ptr1=ptr;
        }while(ptr!=tmp->c);
    }
    (tmp->l)->r=tmp->r;
    (tmp->r)->l=tmp->l;
    root=tmp->r;
    if((tmp==tmp->r) and !(tmp->c)) root=NULL;
    else{
        root=tmp->r;
        consolidate();
    }
    n--;
}
void find_fibH(Node *root,int x,int y){
    Node *ptr=NULL,*tmp=root,*ptr1=NULL;
    tmp->ch=1;
    if(tmp->d==x){
       ptr1=tmp;
       tmp->ch=0;
       ptr=ptr1;
       decr_key(ptr,y);
    }
    if(ptr1==NULL){
       if(tmp->c!=NULL)
          find_fibH(tmp->c,x,y);
       if((tmp->r)->ch==false)
           find_fibH(tmp->r,x,y);
    }
    tmp->ch=false;
    ptr=ptr1;
}
ll find_min(){
    if(!root) return -1;
    return root->d;
}
void del_fibH(int x){
    if(!root){
        cout<<"The Fib. heap is empty\n";
        return;
    }
    find_fibH(root,x,0);
    ext_min();
    cout<<"key is deleted"<<endl;
}
int main(int argc,char *argv[]){
   FILE *ip,*ot;
   ip=fopen((char*)"/home/aklesh/input.txt",(char*)"a+");
   long long n=(ll)atoi(argv[1]);
   fprintf(ip,"%lld ",n);
   vector<ll> arr;
   for(int i=0;i<n;i++){
     arr.push_back(rand()%Mx+i);
     insert_fib(arr[i]);
   }
   srand(time(NULL));
   std::chrono::time_point<std::chrono::system_clock> start,end;
   start=std::chrono::system_clock::now();
   //find_fibH(root,arr[4],100);
   find_min();
   end=std::chrono::system_clock::now();
   std::chrono::duration<float> elapsed_seconds=(end-start);
   ot=fopen((char*)"/home/aklesh/output.txt",(char*)"a+");
   float t=(elapsed_seconds.count());
   fprintf(ot,"%f ",t);
   fclose(ip);fclose(ot);
  // print();
   /*cout << "Creating an initial Fib. heap" << endl; 
    insert_fib(5); 
    insert_fib(2); 
    insert_fib(8); 
  
    // Now we will display the root list of the heap 
    print(); 
  
    cout<<"\n";
    // Now we will extract the minimum value node from the heap 
    cout << "Extracting min" << endl; 
    ext_min(); 
    print(); 
  
    cout<<"\n";
    // Now we will decrease the value of node '8' to '7' 
    cout << "Decrease value of 8 to 7" << endl; 
    find_fibH(root, 8, 7); 
    print(); 
   
    cout<<endl;
    // Now we will delete the node '7' 
    cout << "Delete the node 7" << endl; 
    del_fibH(7); 
    print();*/
   //cout<<"\n"<<(root->d)<<"\n";
   return 0;
}
