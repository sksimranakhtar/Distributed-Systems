#include<bits/stdc++.h>
using namespace std;
int main()
{
	
	cout << "\n\n\t\tLeader Election" << endl;
	int n;
	cout<<"Enter The Number Of Processes"<<endl;	//Number of processes
	cin>>n;

	vector<int> v(n+1);
	int i;
	for(i=1;i<=n;i++)	// We will shuffle the array so that process Pi will have random ID
		v[i]=i;

	random_shuffle(v.begin()+1,v.end());	//Assigning the process id ( preference ) to each process 

	cout<<"The Processor Ids are"<<endl;	
	for(i=1;i<=n;i++)						//Displaying the Id of all Process
			cout<<v[i]<<"  ";

	int x,ind;		// Taking Process Number as input which will initiate the leader election 
	cout<<"\nEnter the Id to check"<<endl;
	cin>>x;

	// Finding The Process Id of that input process
	for(i=1;i<=n;i++)
	{
		if(x==v[i])
			ind=i;
	}
	
	// prev and right to traverse left and right of that process Pi 
	int prev,post;
	prev=(ind-1);	// if the choosen process is in 1st position then left will be nth position else 1 less than Pi position
	if(prev<0)prev=prev+n;
	post=ind+1;		// if the choosen process is in nth position then right will be 1st position else 1 greater than Pi position
	if(post>n)post=post-n;
	int flag=0;		// To terminate the loop
	// if a process gets its Id Back in kth phase then terminate the loop
	while(flag<(n/2))
	{
		if(v[post]>v[ind] || v[prev]>v[ind])	//Sent Process Id is smaller than the process Id received then Packet is lost (swallowed)
		{
			cout<<v[ind]<<" Lost while the hop contain "<<v[post]<<" and "<<v[prev]<<endl;
			break;
		}
		if (v[prev]==0)		// if at 1st index then left will be last index
			v[prev]=1;
		if(v[post]==0)		// if at last index then left will be 1st index
			v[post]=1;

		cout<<v[ind]<< " Won To " << v[post]<<" and " <<v[prev]<<endl;	// Process Id was greater than both side 
		prev--;		// proceeding one more left
		if(prev<0)prev=prev+n;
		post++;		// proceeding one more right
		if(post>n)post=post-n;
		flag++;		// Won at next neighbour
	}
	if(post==prev || flag==(n/2))cout<<"Leader is "<<v[ind];	// printing the process Pi as leader if it won
	return 0;
}
