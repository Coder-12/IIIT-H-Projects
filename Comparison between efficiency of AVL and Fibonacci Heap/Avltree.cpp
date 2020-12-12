#include <bits/stdc++.h>
#include <chrono>
#include <ctime>
#include <time.h>
#define fast ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define ll long long
using namespace std;
const ll Mx=(ll)1e18+11;
struct TreeNode{
	TreeNode *left,*right,*parent;
	ll val,size,height,lo,hi;
	TreeNode(ll x,TreeNode *p,TreeNode *l,TreeNode *r,ll y,ll z,ll lo,ll hi):val{x},parent{p},left{l},right{r},height{y},size{z},lo{lo},hi{hi} {}
};
TreeNode *root;
int bal_fact(TreeNode *Node){
	if(!Node) return 0;
        ll lh=((Node->left)?Node->left->height:0),rh;
	rh=((Node->right)?Node->right->height:0);
	return (lh-rh);
}
void modify(TreeNode *Node){
	if(!Node) return;
	ll h=max(((Node->left)?Node->left->height:0),((Node->right)?Node->right->height:0))+1,sz;
       sz=((Node->left)?Node->left->size:0)+((Node->right)?Node->right->size:0)+1;
       Node->lo=(Node->left?Node->left->lo:Node->val);
       Node->hi=(Node->right?Node->right->hi:Node->val);
       Node->height=h;
       Node->size=sz;
}
void RR_rot(TreeNode *Node){
	if(!Node) return;
	if(Node->right){
		TreeNode *temp=Node->right;
		if(!Node->parent){
			root=Node->right;
			root->parent=NULL;
		}
		else if(Node->parent->left==Node){
			Node->parent->left=temp;
			temp->parent=Node->parent;
		}
		else{
			Node->parent->right=temp;
			temp->parent=Node->parent;
		}
		Node->right=temp->left;
		if(temp->left) temp->left->parent=Node;
		temp->left=Node;
		Node->parent=temp;
		modify(Node);
		modify(temp);
	}
}
void LL_rot(TreeNode *Node){
	if(!Node) return;
	if(Node->left){
		TreeNode *temp=Node->left;
		if(!Node->parent){
			root=Node->left;
			root->parent=NULL;
		}
		else if(Node->parent->left==Node){
			Node->parent->left=temp;
			temp->parent=Node->parent;
		}
		else{
			Node->parent->right=temp;
			temp->parent=Node->parent;
		}
		Node->left=temp->right;
		if(temp->right) temp->right->parent=Node;
		temp->right=Node;
		Node->parent=temp;
		modify(Node);
		modify(temp);
	}
}
void balance_node(TreeNode *Node){
	modify(Node);
	ll bf=bal_fact(Node);
	if(bf>1){
		ll lbf=bal_fact(Node->left);
		if(lbf<0) RR_rot(Node->left);
		modify(Node->left);
		LL_rot(Node);
	}
	else if(bf<-1){
		ll rbf=bal_fact(Node->right);
		if(rbf>0) LL_rot(Node->right);
	        modify(Node->right);
	        RR_rot(Node);
	}	
	modify(Node);
}
void balance_nodes_path(TreeNode *Node){
	while(Node){
		balance_node(Node);
		Node=Node->parent;
	}
}
void insert_Avl(ll x){
	if(!root){
             root=new TreeNode(x,NULL,NULL,NULL,0,1,x,x);
	     return;
	}
	TreeNode *lastnode=NULL,*new_node,*temp=root;
        while(temp){
		lastnode=temp;
		if(x<=temp->val)
			temp=temp->left;
		else
			temp=temp->right;
	}
        new_node=new TreeNode(x,lastnode,NULL,NULL,0,1,x,x);
	if(x<=lastnode->val) lastnode->left=new_node;
	else lastnode->right=new_node;
	balance_nodes_path(new_node);
}

TreeNode* find_val(ll x){
	TreeNode *temp=root;
	while(temp){
		if(x==temp->val)
			return temp;
		else if(x<(temp->val))
			temp=temp->left;
                else
			temp=temp->right;
	}
	return NULL;
}
bool Is_Available(ll val){
        TreeNode *naddr=find_val(val);
        return naddr?1:0;
}
void remove_Avl(TreeNode *node){
    if(!node) return;
    if(!node->left and !node->right){
	    if(!node->parent) root=NULL;
	    else{
		    if(node->parent->left==node)
			    node->parent->left=NULL;
		    else
			    node->parent->right=NULL;
		    balance_nodes_path(node->parent);
	    }
    }
    else if((!node->left + !node->right)==1){
	    TreeNode *temp=NULL;
	    temp=(node->left)?(node->left):(node->right);
	    if(!node->parent)
		    root=temp;
	    else{
		    if(node->parent->left==node)
		        node->parent->left=temp;
                    else 
		        node->parent->right=temp;
	    }
	    if(temp) temp->parent=node->parent;
	    balance_nodes_path(node->parent);
    }
    else{
	    TreeNode *temp=node->right;
            while(temp->left)
		    temp=temp->left;
            remove_Avl(temp);
	    node->val=temp->val;
    }
}
void inorder(TreeNode *root){
	if(!root) return;
	inorder(root->left);
	printf("%lld ",root->val);
	inorder(root->right);
}
TreeNode *find_min(){
       TreeNode *temp=root;
       if(!temp){
          cout<<"No element in AVL tree exist\n";
          return NULL;
       }
       while(temp->left){
             temp=temp->left;
       }
       return temp;
}
void decr_key(ll x,ll y){
    TreeNode *temp=find_val(x);
    if(!temp){
         cout<<"Element doesn't exist in AVL tree\n";
         return;
    }
    remove_Avl(temp);
    insert_Avl((x-y));
}
void ext_min(){
    TreeNode *x=find_min();
    if(x) remove_Avl(x);
}
int main(int argc,char *argv[]){
	fast;
        FILE *ip,*ot;
        ip=fopen("/home/aklesh/input1.txt","a+");
        vector<ll> arr;
        ll n=(ll)atoi(argv[1]);
        fprintf(ip,"%lld ",n);
        for(int i=0;i<n;i++){
            arr.push_back(rand()%Mx+i);
            insert_Avl(arr[i]);
        }
        srand(time(NULL));
        std::chrono::time_point<std::chrono::system_clock> start,end;
        start=std::chrono::system_clock::now();
        decr_key(arr[4],100);
        end=std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds=(end-start);
        ot=fopen("/home/aklesh/output1.txt","a+");
        float t=pow(10,6)*(elapsed_seconds.count());
        fprintf(ot,"%f ",t);
        fclose(ip);fclose(ot);
	/*int test,x,y,tcnt=0;
        cin>>test;
	root=NULL;
	while(test--){
		int type;
		cout<<"Enter the operation you want to perform on ordered set\nInsert : 1\nRemove : 2\nIs_Available on ordered set : 3\nfind_closest : 4\nkthelement : 5\nCardinality of elements in a range : 6\n";
		cin>>type;
		if(type==1){
			int val;
			//cout<<"Enter a value to insert: ";
			cin>>val;
			insert_Avl(val);
		}
		else if(type==2){
			int val;
                        //cout<<"Enter a value to delete: ";
			cin>>val;
			TreeNode *srchnode=find_val(x);
                        remove_Avl(srchnode);
		}
		else if(type==3){
                        int val;
			//cout<<"Enter a value to check its presence: ";
                        cin>>val;
                        cout<<(Is_Available(val)?"YES":"NO");
                }
                else if(type==4){
                        int val;
                        cin>>val;
                        cout<<findclosest(val);
                }
		else if(type==5){
                        int val;
                        cin>>val;
			if(!root) cout<<"Tree is Empty!!!";
			else{
				int x=root->size+1;
				cout<<kthsmallest(root,x-val);
			}
                }
                else{
                        int L,R;
                        cin>>L>>R;
                        cout<<range_count_logn(L,R);
                }
                cout<<std::endl;

	}*/
	return 0;
}
