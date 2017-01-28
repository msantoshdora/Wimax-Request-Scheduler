#ifndef REQUEST_H
#define REQUEST_H

#include<ctime>
#include<bits/stdc++.h>

class Request{
	public:
		int req_number;
		int source;
		int destination;
		double holding_time;
		double bandwidth;
		float sinr;
		double deadline;
		double total_score;
		int type;
		clock_t birth;
		int flag; //to record if bandwidth divided
		double data_rate;
	public:
		Request();
		Request(int src, int dest, double h_time, double bw,float sn,double dl,clock_t br,double ts,int typ,
			int rnumber,int fl,double dr);
		void setRequest(int src, int dest, double h_time, double bw,float sn, double dl,clock_t br,double ts,int typ,
				int rnumber, int fl,double dr);
		
};

#endif
