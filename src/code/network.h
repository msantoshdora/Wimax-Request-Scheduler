#ifndef NETWORK_H
#define NETWORK_H
#include<bits/stdc++.h>


#define V 6

class Network{
	private:
		int src;
		int dest;
		double bw;
		double htime;
		float sn;
		double dl;
		double ts;
		int typ;
		clock_t br;
		int dist[V];  // The output array. dist[i] will hold
                // the shortest distance from src to i
 
	          // sptSet[i] will true if vertex i is included / in shortest
	         // path tree or shortest distance from src to i is finalized
	         bool sptSet[V];
	 
	         // Parent array to store shortest path tree
	         int parent[V];	
		 int rnumber;
		 int fl;
		 double dr;
		
		 
		
	public:
		
		Network();
		int ugs_generated,nrtps_generated, ertps_generated, rtps_generated,be_generated;
		double ugs_bandwidth,nrtps_bandwidth,rtps_bandwidth, be_bandwidth,ertps_bandwidth;

		Request generateRequest(int source,int r_number, bool fileEntry, int iterator);
		double calculateTotalScore();
		double calculateWaitTime(const clock_t start);
		double updateWaitTime();
		double updateTotalScore(float sinr,int type, double price,const clock_t start);
		void findShortestPath(int graph[V][V], int source);
		double allocateBandwidth();
		double assignDeadline();
		float allocateSinr();
		void beginServing();
		void releaseResources();
		int minDistance(int dist[], bool sptSet[]);
		void printPath(int j);
		int assignType();
		void initializeRequest();
		void readFile();
		double allocateValue(int src,int itr,int col);
		void calculateTrafficClass(int typ);
		double assignDataRate(float snr, int typ1);
		void initializeBandwidth();
		void bandwidthRequired(int typ,double bw);

	
		
};

#endif
