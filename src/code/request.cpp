#include "request.h"
#include<bits/stdc++.h>


//Default Constructor
Request::Request(){ 
	setRequest(0,0,0,0,0,0,0,0,0,0,0,0);
}
// Request constructor
Request::Request(int src, int dest, double h_time, double bw,float sn,double dl,clock_t br,double ts,int typ,int rnumber,          			 int fl,double dr){

	setRequest(src,dest,h_time,bw,sn,dl,br,ts,typ,rnumber,fl,dr);

}
// Request member Function to set request
void Request::setRequest(int src, int dest, double h_time, double bw, float sn,double dl,clock_t br,double ts,int typ,
			 int rnumber,int fl, double dr){
	source = src;
	destination = dest;
	holding_time = h_time;
	bandwidth = bw;
	sinr = sn;
	deadline = dl;
	birth = br;
	total_score = ts;
	type = typ;
	req_number = rnumber;
	flag = fl;
	data_rate = dr;
	
}
