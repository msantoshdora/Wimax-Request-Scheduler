#include"request.h"
#include"network.h"
#include<iostream>
#include<bits/stdc++.h>
#include<ctime>
#include<random>
#include<stdlib.h>
#include<time.h>
#include<fstream>
#include<climits>
#include<sys/time.h> //For gettimeofday()
 std::vector<std::vector<double> > data1,data2,data3,data4,data5,data6;

#define V 6

//Network constructor
Network::Network(){}

//Network request generator
Request Network::generateRequest(int source, int r_number,bool fileEntry,int iterator){
/*	  for (int y = 0; y < data1.size(); y++)
    {
        for (int x = 0; x < data1[y].size(); x++)
        {
            std::cout<<data1[y][x]<< " ";
        }
        std::cout << "\n";
    }*/
		//std::cout<<"test\n";
	Request obj;
        std::random_device rd, rd1;			//For randomly generating default_random_engine
        std::default_random_engine generator(rd());
	std::default_random_engine generator1(rd1());
	std::poisson_distribution<int> distribution(10);
	std::poisson_distribution<int> distribution1(10);
	/* initialize random seed */
	srand(time(NULL));
	
	rnumber = r_number;
	if(!fileEntry){
	//	std::cout<<"test1\n";
		src = source;
	//dest = rand() % 6 +1;
		dest = distribution1(generator1)%6 + 1;
       	        br = clock();
		bw = allocateBandwidth();
	//std::cout<<bw<<"This is the bandwidth allocated\n";
		htime = distribution(generator);	
		sn = allocateSinr();
		dl = assignDeadline();
		typ = assignType();
		fl = 0;
		dr = assignDataRate();
	}else{
			
		
		src = source;
		
		dest = allocateValue(source,iterator,1);
		//std::cout<<"DEstination: "<<allocateValue(iterator,1)<<"\n";
       	        br = clock();
		bw = allocateValue(source,iterator,3);
		htime = allocateValue(source,iterator,2);	
		sn = allocateValue(source,iterator,4);
		dl = allocateValue(source,iterator,5);
		typ = allocateValue(source,iterator,6);
		fl = 0;
		dr = 1.22;
		calculateTrafficClass(typ);
		//std::cout<<"TYPE is: "<<typ<<"\n";
	}
	//std::cout<<"test2\n";
	ts = updateTotalScore(sn,typ,bw,br);
	obj.setRequest(src,dest,htime,bw,sn,dl,br,ts,typ,rnumber,fl,dr);
//	std::cout<<"Holding Time: "<<htime<<"\n";	
	return obj;
}


int Network::assignType(){

	/* initialize random seed */
	timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);             //Seeding is better in this case

	int randNum = (rand() % 25) + 1;
	//std::cout<<randNum<<"Checking\n";
	if(randNum>=0 && randNum<=5){
		ugs_generated++;
		return 10;
	}
	if(randNum>= 6 && randNum<=10){
		rtps_generated++;
		return 9;
	}
	if(randNum>=11 && randNum<=15){
		nrtps_generated++;
		return 8;
	}
	if(randNum>=16 && randNum<=20){
		ertps_generated++;
		return 7;
	}
	if(randNum>=21 && randNum<=25){
		be_generated++;
		return 6;
	}
}

void Network::calculateTrafficClass(int traffic){
	if(traffic == 10){
		ugs_generated++;	
	}
	if(traffic == 9){
		rtps_generated++;	
	}
	if(traffic == 8){
		nrtps_generated++;	
	}
	if(traffic == 7){
		ertps_generated++;	
	}
	if(traffic == 6){
		be_generated++;
	}
		
}
double Network::allocateBandwidth(){

	/* initialize random seed */
	timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);             //Seeding is better in this case

	int randNum = (rand() % 25) + 1;
	//std::cout<<randNum<<"Checking\n";
	if(randNum>=0 && randNum<=5)
		return 64;
	if(randNum>= 6 && randNum<=10)
		return 128;
	if(randNum>=11 && randNum<=15)
		return 256;
	if(randNum>=16 && randNum<=20)
		return 512;
	if(randNum>=21 && randNum<=25)
		return 1024;
}

double Network::assignDeadline(){
	/* initialize random seed */
	timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);             //Seeding is better in this case

	int randNum = (rand() % 25) + 1;
	//std::cout<<randNum<<"Checking\n";
	if(randNum>=1 && randNum<=5)
		return 6400;
	if(randNum>= 6 && randNum<=10)
		return 12800;
	if(randNum>=11 && randNum<=15)
		return 256000;
	if(randNum>=16 && randNum<=20)
		return 5120000;
	if(randNum>=21 && randNum<=25)
		return 10240000000;
	
}

float Network::allocateSinr(){
	/* initialize random seed */
	timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);             //Seeding is better in this case

	int randNum = (rand() % 25) + 1;
	//return randNum;
	//std::cout<<randNum<<"Checking\n";
	if(randNum>=1 && randNum<=5)
		return 6.4;
	if(randNum>= 6 && randNum<=10)
		return 12.8;
	if(randNum>=11 && randNum<=15)
		return 25.6;
	if(randNum>=16 && randNum<=20)
		return 5.12;
	if(randNum>=21 && randNum<=25)
		return 1.024;


}
void Network::initializeRequest(){

	
 	ugs_generated = 0;
	nrtps_generated= 0;
	ertps_generated = 0; 
	rtps_generated = 0;
	be_generated = 0;
}

//Initialize the serving
void Network::beginServing(){}

//Release the Resource after finishing the request
void Network::releaseResources(){}

//A utility function to find the vertex with minimum distance
//value, from the set of vertices not yet included in shortest
//path tree
int Network::minDistance(int dist[], bool sptSet[]){
    // Initialize min value
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;

}

// To print the shortest path from source to destination
void Network::printPath(int destin){
    // Base Case : If j is source
    if (parent[destin]==-1)
        return;
// 	std::cout<<"Destination: "<<destin<<" ";
    printPath( parent[destin]);
   
    printf("%d ", destin);
}


//Find shortest path to allocate the resources
void Network::findShortestPath(int graph[V][V], int src){

	
 
    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
    {
        parent[src] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }
 
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
 
    // Find shortest path for all vertices
    for (int count = 0; count < V-1; count++)
    {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to src
        // in first iteration.
        int u = minDistance(dist, sptSet);
 
        // Mark the picked vertex as processed
        sptSet[u] = true;
 
        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < V; v++)
 
            // Update dist[v] only if is not in sptSet, there is
            // an edge from u to v, and total weight of path from
            // src to v through u is smaller than current value of
            // dist[v]
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v]  = u;
                dist[v] = dist[u] + graph[u][v];
            }  
    }
	
}




double Network::updateTotalScore(float sinr,int type, double price,const clock_t start){
		double waitTime = calculateWaitTime(start);
		double totalScore,w1,w2,w3,w4;
	       	double sum = sinr + waitTime + type;
		w1 = sinr/sum;
		w2 = price/sum;
		w3 = waitTime/sum;
		w4  = 1 - (w1+w2+w3);
		totalScore = w1*sinr + w2*price + w3*waitTime+ w4*type;
		return totalScore;

}

double Network::calculateWaitTime(const clock_t start){
	clock_t now = clock();
	clock_t delta = now - start;
	double seconds_elapsed = static_cast<double>(delta) / CLOCKS_PER_SEC * 100.0;
	return seconds_elapsed;
}
//double Network::updateWaitTime();
//void Network::updateTotalScore();

void Network::readFile(){
	
    std::ifstream file("request1.txt");// file path
    std::string line;
    while (getline(file, line))
    {
        data1.push_back(std::vector<double>());
        std::istringstream ss(line);
        double value;
        while (ss >> value)
        {
            data1.back().push_back(value);
        }
    }

 /*   for (int y = 0; y < data1.size(); y++)
    {
        for (int x = 0; x < data1[y].size(); x++)
        {
            std::cout<<data1[y][x]<< " ";
        }
        std::cout << "\n";
    }
*/
	file.close();
	file.open("request2.txt");
//std::ifstream file("request2.txt");// file path
    //std::string line;
    while (getline(file, line))
    {
        data2.push_back(std::vector<double>());
        std::istringstream ss(line);
        double value;
        while (ss >> value)
        {
            data2.back().push_back(value);
        }
    }
	file.close();
std::cout<<"Data2:\n";
	for (int y = 0; y < data2.size(); y++)
    	{
		for (int x = 0; x < data2[y].size(); x++)
		{
		    std::cout<<data2[y][x]<< " ";
		}
		std::cout << "\n";
    	}
	file.open("request3.txt");
//std::ifstream file("request3.txt");// file path
    //std::string line;
    while (getline(file, line))
    {
        data3.push_back(std::vector<double>());
        std::istringstream ss(line);
        double value;
        while (ss >> value)
        {
            data3.back().push_back(value);
        }
    }
	file.close();

std::cout<<"Data3:\n";
	for (int y = 0; y < data3.size(); y++)
    	{
		for (int x = 0; x < data3[y].size(); x++)
		{
		    std::cout<<data3[y][x]<< " ";
		}
		std::cout << "\n";
    	}
	file.open("request4.txt");
//std::ifstream file("request4.txt");// file path
   // std::string line;
    while (getline(file, line))
    {
        data4.push_back(std::vector<double>());
        std::istringstream ss(line);
        double value;
        while (ss >> value)
        {
            data4.back().push_back(value);
        }
    }
	file.close();

std::cout<<"Data4:\n";
	for (int y = 0; y < data4.size(); y++)
    	{
		for (int x = 0; x < data4[y].size(); x++)
		{
		    std::cout<<data4[y][x]<< " ";
		}
		std::cout << "\n";
    	}
	file.open("request5.txt");
//std::ifstream file("request5.txt");// file path
    //std::string line;
    while (getline(file, line))
    {
        data5.push_back(std::vector<double>());
        std::istringstream ss(line);
        double value;
        while (ss >> value)
        {
            data5.back().push_back(value);
        }
    }

//Request 6
	file.close();
std::cout<<"Data5:\n";
	for (int y = 0; y < data5.size(); y++)
    	{
		for (int x = 0; x < data5[y].size(); x++)
		{
		    std::cout<<data5[y][x]<< " ";
		}
		std::cout << "\n";
    	}
	file.open("request6.txt");
//   std::ifstream file("request6.txt");// file path
    //std::string line;
    while (getline(file, line))
    {
        data6.push_back(std::vector<double>());
        std::istringstream ss(line);
        double value;
        while (ss >> value)
        {
            data6.back().push_back(value);
        }
    }


	

}


double Network::allocateValue(int src,int itr,int col){
	//std::cout<<data1[itr][col]<<"\n";
	if(src == 1){
		return data1[itr][col];	
		//std::cout<<"in 1\n";
	}
	if(src == 2){
		return data2[itr][col];	
	}
	if(src == 3){
		return data3[itr][col];	
	}
	if(src == 4){
		return data4[itr][col];	
	}
	if(src == 5){
		return data5[itr][col];	
	}
	if(src == 6){
		return data6[itr][col];	
	}
	//return data1[itr][col];
}

double Network::assignDataRate(){
	return 100;
}
