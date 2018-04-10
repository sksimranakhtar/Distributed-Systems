#include <iostream>
#include <bits/stdc++.h>
using namespace std;
queue<int>q;
void bfs(vector< vector < int > > v, vector<int> &parent,int t){
	int root=q.front();
	//cout << "ROOT= " << root << endl;
	q.pop();

	if(parent[root]==-1)
		parent[root]=root;
	/*for(int i=0;i<parent.size();i++){
		cout << i << " " << parent[i] << " ";
	}*/
	
	// node i is passing message to all connected node j and pushing in queue to call further
	for(int i=0;i<v[root].size();i++){

		// if parent is present then discard else make root as parent
		if(parent[v[root][i]]==-1){
			cout << "Message passed from " << root << " to " << v[root][i] <<" :parent set"<< endl;
			parent[v[root][i]]=root;
			q.push(v[root][i]);
		}
		else{
			cout << "Message from " << root << " to " << v[root][i] << " is rejected " << endl; 
		}
	}
	
	// message passing for further node
	while(!q.empty()){
		bfs(v,parent,t++);
	}
	
}

int main() {
	int n;
	// Taking total nodes input
	cout << "Enter number of Nodes:";
	cin>>n;
	
	// declaring 2d array data structure
	vector<vector<int> > v(n+1);
	int c;

	cout << "\nEnter number of Edges:" << endl;
	cin>>c;
	
	cout << "Enter edges\n" ;
	
	// forming graph
	while(c--)
	{
	    int x,y;
	    cin>>x;
	    cin>>y;
	    v[x].push_back(y);
	    v[y].push_back(x);
	}
	vector<int>parent(n+1,-1);
	cout << "\nENTER ROOT\n" << endl;
	int root;
	cin >> root;
	q.push(root);

	// iniating the message passing algorithm
	bfs(v,parent,0);
	
	// printing all parents
	cout << endl << " <----List of Parents---> " << endl;
	for(int i=1;i<parent.size();i++){
		cout << i << "'s parent is " <<parent[i]  << endl;
	}
	return 0;
}
