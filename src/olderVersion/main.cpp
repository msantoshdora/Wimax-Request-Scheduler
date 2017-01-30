#include<iostream>
#include<bits/stdc++.h>
#include<algorithm>
#include<ctime>
#include<fstream>
#include"request.h"
#include"network.h"
#include<random>
#include<map>
#include<list>
#include<fstream>
#include"spt.h"


#define  NETWORK_MEAN 6
#define  NETWORK_NODES 6
#define UNIT_TIME 1
#define  REQUESTS 6
#define  WL 5                   //Number of Wave Lengths
#define ALLOWED_SERVE_TIME 5
#define BWT 6000

int wavelengths[WL*2];         //To store available Wavelengths
int currentWavelength;	       //Current Available Wavelength to allocate
std::priority_queue<Request> reqs;	//Data Structure to store Requests generated in unit time to serve
std::priority_queue<Request> reqsS1,reqsS2,reqsS3,reqsS4,reqsS5,reqsS6,reqSatisfy;  //Individual request storage for stations
std::priority_queue<Request> ugs,rtps,nrtps,be,ertps;
std::priority_queue<Request> reqServedQueue;
int bwugs,bwrtps,bwnrtps,bwertps,bwbe;
Request t;
int total_requests;
int stationRequest[6][10];               //To store station-request generated data for 10 unit time
int stationRequestSatisfy[6][10];        //To store request satisfy per station for 10 unit time
int sinrRequestSatisfy[6][10];		//To store request satisfy per sinr for 10 unit time
double reqData[50][8];
int currentStation;
int count;
int itr;
int TOTAL_SLOTS = 1000;
int req_count;
int reqServed;
int r_number;
double gHoldTime;
char option;
bool fileEntryy;
double bandwidthAllocated, ugsBandwidthAllocated, rtpsBandwidthAllocated, nrtpsBandwidthAllocated, ertpsBandwidthAllocated, beBandwidthAllocated;
Network n1;
int BWT_UGS,BWT_RTPS,BWT_NRTPS,BWT_ERTPS,BWT_BE;
int total_requests_generated;
int req_discarded;
int ugs_served,rtps_served, ertps_served,nrtps_served, be_served;
int ugs_discarded,rtps_discarded, ertps_discarded,nrtps_discarded, be_discarded;
double **station1request, **station2request, **station3request, **station4request, **station5request, **station6request;
int row1,row2, row3,row4,row5,row6;
void paus(double holdTime);
void assignWavelengths();
void displayResult(int total_requests,int count_number_of_requests_served);
double displayPath(Request temp,double elapsed_time,clock_t start,bool firstIteration);
bool checkDeadline(Request temp,double delayedTime);
void initializeTable();
void displayStationRequest();
void displayStationRequestSatisfy();
void displaySinrRequestSatisfy();
void generateRequest();
void calculateTotalScore();
int allocateBandwidth(int bandwidth_allocated,int bandwidth_available);
int allocateBandwidthUGS(int bandwidth_available,int check);
int allocateBandwidthNRTPS(int bandwidth_available,int check);
int allocateBandwidthERTPS(int bandwidth_available,int check);
int allocateBandwidthRTPS(int bandwidth_available,int check);
int allocateBandwidthBE(int bandwidth_available,int check);
bool isBandwidthAvailable(int bandwidth,int bandwidth_available);
void updateHoldingTime();
bool isRequestAvailable(Request r);
void saveAsType();
void initializeBWT(); 
void freeBandwidth(int type,int bandwidth,Request req_obj);
void makeEmpty();
int manualEntry();
void displayReqData();
void createRequestFile();
void initializeBWTNormalised();
void saveResult();
void displayResult();
void saveResultNormalised();
int readReq();
int writeReq();
void updateBandwidth(int size,double bw, int type);
void allocateSlots(double dataRate);
int convertDataRatetoSlots(double dataRate);
void initializeBandwidthAllocated();
//void initializeRequest();


/**
 * To Add a Priority to signal to noise ratio to server the request in that order.
 */
bool operator<(const Request& a, const Request& b) {
    return a.total_score < b.total_score;
}

int main(){
	int number_of_iteration = 0;

	while(number_of_iteration <= 1){

	number_of_iteration++;
	double temp_elapsed_time = 0;     
	int bandwidth_allocated = 0;             //Reqd for Serving or allocating into the frame
	int bandwidth_available =  BWT;		//Reqd for serving or allocating into the frame
	
	reqServed = 0;
	req_discarded = 0;
        count = 0;
	itr = 0;
	r_number = 0;
	ugs_served=0;
	rtps_served= 0;
	ertps_served = 0;
	nrtps_served= 0;
	be_served = 0;
	ugs_discarded=0;rtps_discarded=0 ;ertps_discarded=0;nrtps_discarded=0;be_discarded =0;
	initializeTable();      
	
	
	n1.initializeRequest();
	std::cout<<"UGS generted"<<n1.ugs_generated<<"\n";
	std::cout<<"Is this a file Entry(y/n)?\n";
	//std::cin>>option;
	if(number_of_iteration %2 !=0){
		option = 'n';	
	}else{
		option = 'y';	
	}
	std::cout<<option<<std::endl;
	if(option=='y' || option=='Y'){
		fileEntryy = true;
		n1.readFile();	
		initializeBWTNormalised();                      //initialize bandwidth threshold
	
	}else{
		fileEntryy = false;
		initializeBWT();                      //initialize bandwidth threshold	
	}
	//std::cout<<BWT_UGS<<"test\n";
	
	bool firstTime = true;		      //This is used to check if it's the first time for pop() of request
	bool firstIteration  =true;           //This is used to check if it's first iteration to serve the request
	total_requests_generated = 0;	
	initializeBandwidthAllocated();
	generateRequest();                    //Generate the request for the first time
		
	clock_t start = clock();
	bwugs = BWT_UGS;
	bwrtps = BWT_RTPS;
	bwnrtps = BWT_NRTPS;
	bwertps = BWT_ERTPS;
	bwbe = BWT_BE;
	int serial = 1, requests_to_satisfy = 0;
	temp_elapsed_time = 0;
        //To contiue for indefinite period
	while(count< UNIT_TIME || !reqSatisfy.empty() || !reqs.empty()){
			++count;                       //Number of Unit Time
	       		//bandwidth_available = allocateBandwidth(bandwidth_allocated,bandwidth_available);
	
		//currentStation = serial;                       //For stationRequestStatisfy table
		if(firstTime || reqSatisfy.empty() || (requests_to_satisfy > reqSatisfy.size()) ){
				calculateTotalScore();
				 start = clock();
				temp_elapsed_time = 0;
				firstTime = false;
				firstIteration = true;
				saveAsType();
				

	//DON'T DELETE BELOW COMMENT				
	//bandwidth_available = allocateBandwidth(bandwidth_allocated,bandwidth_available);
				bwugs = allocateBandwidthUGS(bwugs,0);
				if(UNIT_TIME == 1){
					updateBandwidth(ugs.size(),bwugs, 10);				
				}
				bwrtps = allocateBandwidthNRTPS(bwrtps,0);
				if(UNIT_TIME == 1){
					updateBandwidth(rtps.size(),bwrtps, 9);				
				}
				bwnrtps = allocateBandwidthRTPS(bwnrtps,0);
				if(UNIT_TIME == 1){
					updateBandwidth(nrtps.size(),bwnrtps, 8);				
				}
				bwertps =allocateBandwidthERTPS(bwertps,0);
				if(UNIT_TIME == 1){
					updateBandwidth(ertps.size(),bwertps, 7);				
				}
				bwbe = allocateBandwidthBE(bwbe,0);
								
				
				requests_to_satisfy = reqSatisfy.size();
				//std::cout<<"Requests to Satisfy: "<<requests_to_satisfy<<"\n";
				

//t = reqs.top();
				//reqs.pop();
			//std::cout<<"Request Number: "<<serial<<"\n";
			//++serial;
			}
			std::cout<<"Size of reqSatisfy: "<<reqSatisfy.size()<<"\n";
			temp_elapsed_time = displayPath(t,temp_elapsed_time,start,firstIteration);
			
			if(count < UNIT_TIME){
				generateRequest();
			}

	}
/* Write to a file */
	if(!fileEntryy){
		createRequestFile();
	}
	
	//if(ugs_served>n1.ugs_generated) ugs_served = n1.ugs_generated;
	//	if(be_served>n1.be_generated) be_served = n1.be_generated;
	//	if(nrtps_served>n1.nrtps_generated) nrtps_served = n1.nrtps_generated;
	//	if(rtps_served>n1.rtps_generated) rtps_served = n1.rtps_generated;
	//	if(ertps_served>n1.ertps_generated) ertps_served = n1.ertps_generated;

	if(!fileEntryy){
		saveResult(); //To save result in a file
	}else{
		saveResultNormalised();
	}
	displayResult();
	displayReqData();
        std::cout<<"total slots allocated: "<<1000-TOTAL_SLOTS<<"\n";
	if(fileEntryy){
		writeReq();
	}
	/*


	std::cout<<"Size of reqSatisfy: "<<reqSatisfy.size()<<"\n";
	std::cout<<"Size of ugs: "<<ugs.size()<<"\n";
	std::cout<<"Size of rtps: "<<rtps.size()<<"\n";
	std::cout<<"Size of nrtps: "<<nrtps.size()<<"\n";
	std::cout<<"Size of ertps: "<<ertps.size()<<"\n";
	std::cout<<"Size of be: "<<be.size()<<"\n";
	displayReqData(); */
	//displayStationRequest();
	//displayStationRequestSatisfy();
	//displaySinrRequestSatisfy();
}
		return 0;
}



/**
* Generates the Request
*/

void generateRequest(){

		double diff = 0;
		int req;
		 total_requests = 0;
		clock_t begin = clock();
		std::random_device rd;
	        std::default_random_engine generator(rd());
		//std::default_random_engine generator;
		std::default_random_engine request_generator(rd());
		std::poisson_distribution<int> distribution(NETWORK_MEAN);
		std::poisson_distribution<int> no_of_request(REQUESTS);
		
		
	  	
		//For first 6 seconds             
		for(int i = 1; i<= NETWORK_NODES; i++){                  // i is the Station number
			//if(fileEntryy){
			req = manualEntry();            //For Static entry
			//}else{
			//req = no_of_request(request_generator);
			//}
			if(i==1){
				row1 = req;
				if(!fileEntryy){
			 		station1request = new double*[req];
					for(int k = 0; k < req; ++k)
    					   station1request[k] = new double[8]; 
				}
			}
			if(i==2){
				 row2= req;
				if(!fileEntryy){
					 station2request = new double*[req];
					for(int k = 0; k < req; ++k)
	    				   station2request[k] = new  double[8];
				}
			}
			if(i==3){
				row3= req;
				if(!fileEntryy){
			  		station3request = new double*[req];
					for(int k = 0; k < req; ++k)
    				   		station3request[k] = new  double[8];
				}
			}
			if(i==4){
				row4= req;
				if(!fileEntryy){
			  		station4request = new double*[req];
					for(int k = 0; k < req; ++k)
    				   		station4request[k] = new  double[8];
				}
			}
			if(i==5){
			row5= req;
				if(!fileEntryy){
			  		station5request = new double*[req];
					for(int k = 0; k < req; ++k)
    					   station5request[k] = new  double[8];
				}
			}
			if(i==6){
			row6= req;
				if(!fileEntryy){
			  		station6request= new double*[req];
					for(int k = 0; k < req; ++k)
    				   		station6request[k] = new  double[8];
				}
			}
		       //req = no_of_request(request_generator);	//For Dynamic Entry
		       std::cout<<"For Station "<<i<<" , Number of New Requests Generated: "<<req<<"\n";
		      // stationRequest[i-1][count-1] = req;          //Fill the stationRequest matrix for station vs time unit data
		       total_requests_generated+=req;
	      	       total_requests+=req;
		       Request r1;
		       diff = 0;                         //Intializing to 0
		       begin = clock();		        // Recording the beginning Time
			int iterator = 0, colitr = 0;	
					
			while((diff < 1) ){
				//std::cout<<"here\n";
				while((diff<1) && (req>0) ){
					++r_number;
					r1 = n1.generateRequest(i,r_number,fileEntryy,iterator);	
					reqs.push(r1);	
					//std::cout<<"Again\n";
					//saveAsType(r1);
					//network.insert(std::pair<int,Request> (i,r1));             //Storing of request in network
				 	if(i == 1){
						//std::cout<<"Number of Requests Generated..."<<i<<"\n";
						reqsS1.push(r1);
						//for(int colitr =0;colitr<8;colitr++){
						if(!fileEntryy){
							station1request[iterator][0] =r1.source;
					 		station1request[iterator][1] =r1.destination;				
							station1request[iterator][2] =r1.holding_time;				
							station1request[iterator][3]=r1.bandwidth;
							station1request[iterator][4]=r1.sinr;
							station1request[iterator][5] =r1.deadline;
							station1request[iterator][6] =r1.type;
							station1request[iterator][7] =r1.birth;
						}

					
					}
					if(i == 2){
						reqsS2.push(r1);
							if(!fileEntryy){	
							station2request[iterator][0] =r1.source;
					 		station2request[iterator][1] =r1.destination;				
							station2request[iterator][2] =r1.holding_time;				
							station2request[iterator][3]=r1.bandwidth;
							station2request[iterator][4]=r1.sinr;
							station2request[iterator][5] =r1.deadline;
							station2request[iterator][6] =r1.type;
							station2request[iterator][7] =r1.birth;
							}
				
					}
					if(i == 3){
						reqsS3.push(r1);
							if(!fileEntryy){
							station3request[iterator][0] =r1.source;
					 		station3request[iterator][1] =r1.destination;				
							station3request[iterator][2] =r1.holding_time;				
							station3request[iterator][3]=r1.bandwidth;
							station3request[iterator][4]=r1.sinr;
							station3request[iterator][5] =r1.deadline;
							station3request[iterator][6] =r1.type;
							station3request[iterator][7] =r1.birth;
							}
					
					}
					if(i == 4){
						reqsS4.push(r1);
							if(!fileEntryy){
							station4request[iterator][0] =r1.source;
					 		station4request[iterator][1] =r1.destination;				
							station4request[iterator][2] =r1.holding_time;				
							station4request[iterator][3]=r1.bandwidth;
							station4request[iterator][4]=r1.sinr;
							station4request[iterator][5] =r1.deadline;
							station4request[iterator][6] =r1.type;
							station4request[iterator][7] =r1.birth;
							}
					
					}
					if(i == 5){
						reqsS5.push(r1);
						if(!fileEntryy){
							station5request[iterator][0] =r1.source;
					 		station5request[iterator][1] =r1.destination;				
							station5request[iterator][2] =r1.holding_time;				
							station5request[iterator][3]=r1.bandwidth;
							station5request[iterator][4]=r1.sinr;
							station5request[iterator][5] =r1.deadline;
							station5request[iterator][6] =r1.type;
							station5request[iterator][7] =r1.birth;
						}
					
					}
					if(i == 6){
						reqsS6.push(r1);
						if(!fileEntryy){
							station6request[iterator][0] =r1.source;
					 		station6request[iterator][1] =r1.destination;				
							station6request[iterator][2] =r1.holding_time;				
							station6request[iterator][3]=r1.bandwidth;
							station6request[iterator][4]=r1.sinr;
							station6request[iterator][5] =r1.deadline;
							station6request[iterator][6] =r1.type;
							station6request[iterator][7] =r1.birth;
						}
					
					}
					iterator++;
					clock_t end = clock();
					diff = double(end - begin) / CLOCKS_PER_SEC;
					req--;
				}
			clock_t end = clock();
			diff = double(end - begin)/CLOCKS_PER_SEC;
		}
	}

}


/**
 * Input: Object of the network which is a mapping from node number to Request Object
 * To Display the path and the allocated Resources 
 */
double displayPath(Request temp1,double se,clock_t start,bool firstIteration){

 	
	 int lsource,ldestination;      // For Iterator way
	 int ls,ld;                     //For Priority Queue way
	 int wl_used;
	 int time_hold;
	 int trequests;
	 int satisfiedRequest = 0;
	 Request r;

	 int count_number_of_requests_served = 0;
	 double process_time_seconds, delayedTime;
	 std::priority_queue<Request> tempQ;
	
	/*while(!reqSatisfy.empty()) {
			//Checks if deadline is over or not
			r = reqSatisfy.top();
			reqSatisfy.pop();
		 	std::cout<<"Within deadline check,flag = "<<r.flag<<"\n";
			if(isRequestAvailable(r)){
		 		tempQ.push(r);	
		 	}else{
				
				req_discarded++;
				if(r.type == 10) ugs_discarded++;
				if(r.type == 9) rtps_discarded++;
				if(r.type == 8) nrtps_discarded++;
				if(r.type == 7) ertps_discarded++;
				if(r.type == 6) be_discarded++;
		
				/*if(r.flag == -1){
					reqServed--;
					if(r.type == 10) ugs_served--;
					if(r.type == 9) rtps_served--;
					if(r.type == 8) nrtps_served--;
					if(r.type == 7) ertps_served--;
					if(r.type == 6) be_served--;
				}
					freeBandwidth(r.type,r.bandwidth);
						
			}
	} //While ends */

		//reqSatisfy = tempQ;

	 do {   
		std::priority_queue<Request> temQ;

		
		/*if(temp1.holding_time == 0){
			//To handle exception case
		    t.holding_time = 0;
		    gHoldTime = temp1.holding_time;
		    
		    return std::max(seconds_elapsed,se);		
		}*/


		//paus(5);

		
		 count_number_of_requests_served++;

		while(!reqSatisfy.empty() && reqSatisfy.size()>0){
   		 
			creat();
		 	
			r = reqSatisfy.top();
			reqSatisfy.pop();
			
		 	ls = r.source;
   		 	ld = r.destination;
		/* if(ls == ld || ((ls==1 && ld == 6) || (ls==6 && ld ==1)  ) ){
			temp.pop();
		  	trequests--;
			continue;
		 } */
		 
	/* //DON'T DELETE BELOW COMMENTS
			 std::cout<<"\n\t\t Request Number: "<<r.req_number<<"\n";
			 std::cout <<"\t\tSource: "<< ls<< " Destination: " << ld<<"\n";
			 SPT(ls,ld);
			 std::cout<<"\t\tTotal Score: "<<r.total_score<<"\n";
			 std::cout<<"\t\tBandwidth Given: "<<r.bandwidth<<"\n";
			 std::cout<<"\t\tProcessing Time: "<<r.holding_time<<"\n";
			 std::cout<<"\t\tSignal to Noise Ratio : "<<r.sinr<<"\n";
			 std::cout<<"\t\tTYPE : "<<r.type<<"\n";  */
			if(itr<50){			 
			reqData[itr][0] = r.req_number;
			 reqData[itr][1] = ls;
			 reqData[itr][2] = ld;
			 reqData[itr][3] = r.total_score;
			 reqData[itr][4] = r.bandwidth;
			 reqData[itr][5] = r.holding_time;
			 reqData[itr][6] = r.sinr;
	  		 reqData[itr][7] = r.type;
			
			 ++itr;
			}
		 //Wait for the holding time of the request then show path
	         // paus(5);           //Waiting For 5ms
		 // temp1.holding_time = temp1.holding_time - 5;
		 // gHoldTime = temp1.holding_time;
		 //For Displaying path
		  time_hold = r.holding_time;          //for priority queue

//END OF PRIORITY QUEUE DATA STRUCTURE


		 
		
		
	
	
   		 for(int k=top;k>=0;k--){
			if(k==top){
				std::cout<<"\t\tPath: ";
			}
			
 			std::cout<<stack[k];
			
			if(time_hold == 0){
				std::cout<<"Cannot allocate this resource!\n";
				break;			
			}
			if(k>0){
				
				std::cout<<"-->";
				//paus(clock(),&time_hold);
			}
		  } //For Loop Ends
		//std::cout<<"\n";
			firstIteration = false;
		//CHECK THE BELOW CODE IT IS DOING SAME AS updateHoldingTime(),I THINK IT'S REPEATATIVE
			if(r.holding_time >0){
				//std::cout<<"In display Path,Request Holding Time: "<<r.holding_time<<"\n";
				//std::cout<<"In display Path,Size of reqSatisfy: "<<reqSatisfy.size()<<"\n";
				temQ.push(r);
			}else{ 
				freeBandwidth(r.type,r.bandwidth,r);
			}
		}//While Loop Ends
 	 	//To check if under given serving time or not.
		reqSatisfy = temQ;
		clock_t now = clock();
        	clock_t process_time = now - start;

         	process_time_seconds = static_cast<double>(process_time) / CLOCKS_PER_SEC;  //+se deleted,as we require only 5ms time
		//std::cout<<"In display Path,Seconds Elapsed: "<<process_time_seconds<<"\n";
   		 
 	 } while (process_time_seconds < 1);//ALLOWED_SERVE_TIME);    //&& mycomp((*it++).first, highest), Run for 5ms
	
		

	  updateHoldingTime();      //UpdateHolding Time in reqSatisfy requests

	  //r.holding_time = r.holding_time - std::min(process_time_seconds,r.holding_time);
	//  std::cout<<"Holding Time: "<<r.holding_time<<"\n";
	  //gHoldTime = r.holding_time;

	//stationRequestSatisfy[currentStation-1][count-1] = satisfiedRequest;


	
	//std::cout<<"Size after removing: "<<temp.size()<<" Seconds Elapsed: "<<seconds_elapsed<<"\n";
 	//displayResult(trequests,count_number_of_requests_served);

return process_time_seconds;


}

void initializeBWT(){

	BWT_UGS = 0.4*6000;
	//std::cout<<"BWT_UGS: "<<BWT_UGS<<"\n";
	BWT_RTPS = 0.2*6000;//25%BWT;
	BWT_ERTPS = 0.2*6000;//15%BWT;
	BWT_NRTPS = 0.1*6000;//20%BWT;
	BWT_BE = 0.1*6000;//10%BWT;

}

void initializeBWTNormalised(){

   	BWT_UGS = (0.25)*6000;
	BWT_RTPS = (0.225)*6000;//25%BWT;
	BWT_ERTPS = (0.2)*6000;//15%BWT;
	BWT_NRTPS = (0.175)*6000;//20%BWT;
	BWT_BE = (0.15)*6000;//10%BWT;
	//std::cout<<BWT_UGS<<"\n";
}

void makeEmpty(){
	while(!ugs.empty()){
		ugs.pop();	
	}
	while(!rtps.empty()){
		rtps.pop();	
	}
	while(!ertps.empty()){
		ertps.pop();	
	}
	while(!nrtps.empty()){
		nrtps.pop();	
	}
	while(!be.empty()){
		be.pop();	
	}
	
}

/**
* Stores the request as per the container.
*/

void saveAsType(){

	int ltype ;
	std::priority_queue<Request> tempQ;
	Request r;
	//makeEmpty();
	while(!reqs.empty()){
		r = reqs.top();
		reqs.pop();
		tempQ.push(r);
		ltype = r.type;
		//std::cout<<"Within Save AS type: "<<ltype<<"\n";
		switch(ltype){

		case 10:ugs.push(r);
			break;
		case 9: rtps.push(r);
			break;
		case 8: nrtps.push(r);
			break;
		case 7: ertps.push(r);
			break;
		case 6: be.push(r);
			break;
		}
	}
	//reqs = tempQ;
}


int manualEntry(){

	int n;
	std::cout<<"Enter number of Requests: \n";
	std::cin>>n;

	return n;
}


/**
*  If deadline gets over,then it returns true;else false
*/

bool checkDeadline(Request temp,double delayedTime){

	//if(temp.empty()) return false;
	if(temp.deadline > delayedTime) return false;

	return true;
}

/**
* Checks if the Request has crossed it's deadline or not
*/

bool isRequestAvailable(Request r){
	
		 double delayedTime;
	         clock_t now1 = clock();
		
        	 clock_t wait_time = now1 - r.birth ;          //birth is the request generation time.(Time request was born)
         	 delayedTime = (double)wait_time / CLOCKS_PER_SEC * 100.0;                //To calculate the deadline

		// paus(5);
		 if(checkDeadline(r,delayedTime)){
		       //Checks if the Requests has not crossed the deadline
			   // std::cout<<delayedTime<<"Request Deadline: "<<r.deadline<<"\n";
                            std::cout<<"Request Discarded because deadline is over!!\n";
			
			     r.holding_time = 0;
			  //   gHoldTime = r.holding_time;
			return false;
			   
		 }
return true;

}

/**
* Update the Holding Time after each 5ms time
*/
void updateHoldingTime(){
	
	std::priority_queue<Request> q;
	Request r;
	while(!reqSatisfy.empty()){
		r = reqSatisfy.top();
		reqSatisfy.pop();
		r.holding_time = r.holding_time - 5;
		paus(5);
		if(r.holding_time <= 0){
			//std::cout<<"In updateHoldingTime, Request Served as Holding Time is over!\n";	
			//std::cout<<"Test within update Holding time,flag = "<<r.flag<<"\n";
			if(r.flag == -1){
				reqServed--;
				if(r.type == 10) ugs_served--;
				if(r.type == 9) rtps_served--;
				if(r.type == 8) nrtps_served--;
				if(r.type == 7) ertps_served--;
				if(r.type == 6) be_served--;
			}
			//if(r.flag != -1){                  //if r.flag = 1, then it is divided.
				freeBandwidth(r.type,r.bandwidth,r);
			//}
				
		}else{
			q.push(r);
		}		
	}
	reqSatisfy = q;
	//std::cout<<"In updateHoldingTime,reqSatisfy size: "<<reqSatisfy.size()<<"\n";
}

void allocateSlots(double dataRate){
	int slotsAllocated = 0;
	slotsAllocated = convertDataRatetoSlots(dataRate);
	std::cout<<"Slots allocated: "<<slotsAllocated<<"\n";
	TOTAL_SLOTS-=slotsAllocated;
}

void freeBandwidth(int type,int bandwidth,Request req_obj){

	if(req_obj.flag != -1){
		reqServedQueue.push(req_obj);	
		allocateSlots(req_obj.data_rate);
	}

	reqServed++;
	switch(type){

		case 10:bwugs+=bandwidth;
			ugs_served++;
			break;
		case 9: bwrtps+=bandwidth;
			rtps_served++;
			break;
		case 8: bwnrtps+=bandwidth;
			nrtps_served++;
			break;
		case 7: bwertps+=bandwidth;
			ertps_served++;
			break;
		case 6: bwbe+=bandwidth;
			be_served++;
			break;
	}
}


int convertDataRatetoSlots(double dataRate){

	int slots = (int)dataRate;
	return slots;
}

/**
*  Fills the slots in the Bandwidth Frame...It fills reqSatisfy
*/
int allocateBandwidth(int bandwidth_allocated,int bandwidth_available){
	std::priority_queue<Request> temp;
	Request r;
	double b;
	while(!reqs.empty()){
		r = reqs.top();
		reqs.pop();
		
		if(isBandwidthAvailable(r.bandwidth,bandwidth_available)){
			bandwidth_available -=r.bandwidth;
			reqSatisfy.push(r);		
		}else if(r.bandwidth > BWT){
				std::cout<<"Request Discarded!!\n";
		
		}else{
			temp.push(r);		
		}
	}
	
	reqs = temp;
	return bandwidth_available;
}


/**
*  Fills the slots in the Bandwidth Frame in UGS...It fills reqSatisfy
*/
int allocateBandwidthUGS(int bandwidth_available,int check){
	std::priority_queue<Request> temp;
	Request r;
	double b = -1;
	//std::cout<<"Allocating UGS BW"<<bandwidth_available<<"\n";
	while(!ugs.empty()){
		r = ugs.top();
		ugs.pop();
		while(!isRequestAvailable(r) && !ugs.empty()){
			if(r.flag == -1){
				ugs_served--;
				reqServed--;
			}
			std::cout<<"Request Discarded\n";
			req_discarded++;
			ugs_discarded++;
			r = ugs.top();
			ugs.pop();		
		}
		if(check == 1){
			b = r.bandwidth - bandwidth_available;
			r.bandwidth = bandwidth_available;
						
		}
		if(isBandwidthAvailable(r.bandwidth,bandwidth_available)){
			bandwidth_available -=r.bandwidth;
			reqSatisfy.push(r);
			if(b>0){
				r.bandwidth = b;
				temp.push(r);			
			}		
		}else if((r.bandwidth > bandwidth_available) && (r.bandwidth < BWT)){
			Request r1 = r;
			r.bandwidth -= bandwidth_available;			
			//temp.push(r); 
			r1.bandwidth = bandwidth_available;
			if(r1.flag == 0){
		        r1.flag = 1;
			}
			reqSatisfy.push(r1);
			if(r.flag == 0){
			r.flag = -1;
			}
			//r.deadline = std::max(0,r.deadline-5);
		//	if(r.deadline > 0){
				temp.push(r);
		//	}
			
		
		}else if(r.bandwidth >= BWT){
				std::cout<<"Request Discarded!!\n";
		}
	}
	
	ugs = temp;
	//std::cout<<"bandwidth Available: "<<bandwidth_available<<"\n";
	return bandwidth_available;
}

/**
*  Fills the slots in the Bandwidth Frame in RTPS...It fills reqSatisfy
*/
int allocateBandwidthRTPS(int bandwidth_available,int check){
	std::priority_queue<Request> temp;
	Request r;
	double b = -1;
	while(!rtps.empty()){
		r = rtps.top();
		rtps .pop();
		while(!isRequestAvailable(r) && !rtps.empty()){
			if(r.flag == -1){
				rtps_served--;
				reqServed--;
			}
			std::cout<<"Request Discarded\n";
			req_discarded++;
			rtps_discarded++;
			r = rtps.top();
			rtps.pop();		
		}
		if(check == 1){
			b = r.bandwidth - bandwidth_available;
			r.bandwidth = bandwidth_available;
						
		}
		if(isBandwidthAvailable(r.bandwidth,bandwidth_available)){
			bandwidth_available -=r.bandwidth;
			reqSatisfy.push(r);
			if(b>0){
				r.bandwidth = b;
				temp.push(r);			
			}		
		}else if((r.bandwidth > bandwidth_available) && (r.bandwidth < BWT)){
			Request r1 = r;
			r.bandwidth -= bandwidth_available;			
			//temp.push(r); 
			r1.bandwidth = bandwidth_available;
			if(r1.flag == 0){
			 r1.flag = 1;
			}
			reqSatisfy.push(r1);
			if(r.flag ==0){
			r.flag = -1;
			}
		//	r.deadline = std::max(0,r.deadline-5);
			//if(r.deadline > 0){
				temp.push(r);
		//	}
			
		
		}else if(r.bandwidth >= BWT){
				std::cout<<"Request Discarded!!\n";
		}
	}
	
	rtps = temp;
	return bandwidth_available;
}
/**
*  Fills the slots in the Bandwidth Frame in NRTPS...It fills reqSatisfy
*/
int allocateBandwidthNRTPS(int bandwidth_available,int check){
	std::priority_queue<Request> temp;
	Request r;
	double b = -1;
	while(!nrtps.empty()){
		r = nrtps.top();
		nrtps .pop();
		while(!isRequestAvailable(r) && !nrtps.empty()){
			if(r.flag == -1){
				nrtps_served--;
				reqServed--;
			}
			std::cout<<"Request Discarded\n";
			req_discarded++;
			nrtps_discarded++;
			r = nrtps.top();
			nrtps.pop();		
		}
		if(check == 1){
			b = r.bandwidth - bandwidth_available;
			r.bandwidth = bandwidth_available;
						
		}
		if(isBandwidthAvailable(r.bandwidth,bandwidth_available)){
			bandwidth_available -=r.bandwidth;
			reqSatisfy.push(r);
			if(b>0){
				r.bandwidth = b;
				temp.push(r);			
			}		
		}else if((r.bandwidth > bandwidth_available) && (r.bandwidth < BWT)){
			Request r1 = r;
			r.bandwidth -= bandwidth_available;			
			//temp.push(r); 
			r1.bandwidth = bandwidth_available;
			if(r1.flag == 0){
			 r1.flag = 1;
			}
			reqSatisfy.push(r1);
			if(r.flag == 0){
			r.flag = -1;
			}
			//r.deadline = std::max(0,r.deadline-5);
			//if(r.deadline > 0){
				temp.push(r);
			//}
			
		
		}else if(r.bandwidth >= BWT){
				std::cout<<"Request Discarded!!\n";
		}
	}
	
	nrtps = temp;
	return bandwidth_available;
}
/**
*  Fills the slots in the Bandwidth Frame in ERTPS...It fills reqSatisfy
*/
int allocateBandwidthERTPS(int bandwidth_available,int check){
	std::priority_queue<Request> temp;
	Request r;
	double b = -1;
	while(!ertps.empty()){
		r = ertps.top();
		ertps.pop();
		while(!isRequestAvailable(r) && !ertps.empty()){
			if(r.flag == -1){
				ertps_served--;
				reqServed--;
			}
			std::cout<<"Request Discarded\n";
			req_discarded++;
			ertps_discarded++;
			r = ertps.top();
			ertps.pop();		
		}
		if(check == 1){
			b = r.bandwidth - bandwidth_available;
			r.bandwidth = bandwidth_available;
						
		}
		if(isBandwidthAvailable(r.bandwidth,bandwidth_available)){
			bandwidth_available -=r.bandwidth;
			reqSatisfy.push(r);
			if(b>0){
				r.bandwidth = b;
				temp.push(r);			
			}		
		}else if((r.bandwidth > bandwidth_available) && (r.bandwidth < BWT)){
			Request r1 = r;
			r.bandwidth -= bandwidth_available;			
			//temp.push(r); 
			r1.bandwidth = bandwidth_available;
			if(r1.flag == 0){
			r1.flag = 1;
			}
			reqSatisfy.push(r1);
			if(r.flag == 0){
			r.flag = -1;
			}
			//r.deadline = std::max(0,r.deadline-5);
			//if(r.deadline > 0){
				temp.push(r);
			//}
			
		
		}else if(r.bandwidth >= BWT){
				std::cout<<"Request Discarded!!\n";
		}
	}
	
	ertps = temp;
	return bandwidth_available;
}
/**
*  Fills the slots in the Bandwidth Frame in BE...It fills reqSatisfy
*/
int allocateBandwidthBE(int bandwidth_available,int check){
	std::priority_queue<Request> temp;
	Request r;
	double b =-1;
	while(!be.empty()){
		r = be.top();
		be.pop();
		while(!isRequestAvailable(r) && !be.empty()){
			if(r.flag == -1){
				be_served--;
				reqServed--;
			}
			std::cout<<"Request Discarded\n";
			req_discarded++;
			be_discarded++;
			r = be.top();
			be.pop();		
		}
		if(check == 1){
			b = r.bandwidth - bandwidth_available;
			r.bandwidth = bandwidth_available;
						
		}
		if(isBandwidthAvailable(r.bandwidth,bandwidth_available)){
			bandwidth_available -=r.bandwidth;
			reqSatisfy.push(r);
			if(b>0){
				r.bandwidth = b;
				temp.push(r);			
			}		
		}else if((r.bandwidth > bandwidth_available) && (r.bandwidth < BWT)){
			Request r1 = r;
			r.bandwidth -= bandwidth_available;			
			//temp.push(r); 
			r1.bandwidth = bandwidth_available;
			if(r1.flag == 0){
			 r1.flag = 1;
			}
			reqSatisfy.push(r1);
			if(r.flag == 0){
			r.flag = -1;
			}
			//r.deadline = std::max(0,r.deadline-5);
			//if(r.deadline > 0){
				temp.push(r);
			//}
			
		
		}else if(r.bandwidth >= BWT){
				std::cout<<"Request Discarded!!\n";
		}
	}
	
	be = temp;
	return bandwidth_available;
}

bool isBandwidthAvailable(int bandwidth,int bandwidth_available){

	if(bandwidth_available >= bandwidth){
		return true;		
	}

return false;
}
/**
* It initializes table for the first time with 0
*/
void initializeTable(){
	//int stationRequest[6][10];               //To store station-request generated data for 10 unit time
//int stationRequestSatisfy[6][10];        //To store request satisfy per station for 10 unit time
//int sinrRequestSatisfy[6][10];		//To store request satisfy per sinr for 10 unit time


	for(int i=0;i< NETWORK_NODES;i++){
		for(int j=0;j< UNIT_TIME;j++){
			stationRequest[i][j] = 0;
			stationRequestSatisfy[i][j] = 0;
			sinrRequestSatisfy[i][j] = 0;
		}	
	}
	for(int i =0;i<50;i++){
		for(int j=0;j<8;j++){
			reqData[i][j] =0;		
		}
	}
	
}



void displayResult(int total_requests,int count_number_of_requests_served){

	std::cout<<"Total Requests: "<<total_requests<<"\n";
	std::cout<<"Requests Handled within 1 unit time: "<<count_number_of_requests_served<<"\n\n";
}



void displayStationRequest(){
	
	std::cout<<"-------------------Station-Request Table--------------"<<"\n";
	for(int i=0;i< NETWORK_NODES;i++){
		for(int j=0;j< UNIT_TIME;j++){
			std::cout<<stationRequest[i][j]<<" ";
			//stationRequestSatisfy[i][j] = 0;
			//sinrRequestSatisfy[i][j] = 0;
		}
		std::cout<<"\n";	
	}
}
void displayStationRequestSatisfy(){
	std::cout<<"-------------------Station-Request-Satisfy Table--------------"<<"\n";
	for(int i=0;i< NETWORK_NODES;i++){
		for(int j=0;j< UNIT_TIME;j++){
			std::cout<<stationRequestSatisfy[i][j]<<" ";
			//stationRequestSatisfy[i][j] = 0;
			//sinrRequestSatisfy[i][j] = 0;
		}
		std::cout<<"\n";	
	}

}
void displaySinrRequestSatisfy(){
	std::cout<<"-------------------Sinr-Request Table--------------"<<"\n";
	for(int i=0;i< NETWORK_NODES;i++){
		for(int j=0;j< UNIT_TIME;j++){
			std::cout<<sinrRequestSatisfy[i][j]<<" ";
			//stationRequestSatisfy[i][j] = 0;
			//sinrRequestSatisfy[i][j] = 0;
		}
		std::cout<<"\n";	
	}
	
}






/**
 *  Assign available wavelengths
 */
void assignWavelengths(){
	int wl1 = 64;
	for(int i = 0; i<WL;i++){
		wavelengths[i] = wl1;
		wavelengths[i+WL] = wl1;  //To keep a temporary Storage of the wavelengths 
		wl1= wl1*2;			
	}
}
/**
*  To give a pause till the holding time of the serving request gets finished.
*/
void paus(double holdTime){
	//std::cout<<"\n              Processing... \n";


	const clock_t start = clock();
	clock_t now = clock();

	clock_t delta = now - start;
	double seconds_elapsed = static_cast<double>(delta) / CLOCKS_PER_SEC;
	while(seconds_elapsed <= (holdTime/10)){
	
		clock_t now = clock();
		clock_t delta = now - start;
		seconds_elapsed = static_cast<double>(delta) / CLOCKS_PER_SEC;
	}
	
}

void displayReqData(){
	std::cout<<"SN\tSrc\tDest\tTS\tBw\tHT\tSinr\tType\n"; 
	for(int i= 0;i<10;i++){
		for(int j=0;j<8;j++){
			std::cout<<reqData[i][j]<<"\t";
					
		}		
		std::cout<<"\n";	
	}
	
}
void calculateTotalScore(){

	Request res;
	std::priority_queue<Request> reqsS, tempo,temp1,temp2,temp3,temp4,temp5;
	reqsS = reqs;    //Temporary Holding of requests

	while(!reqs.empty()){
		res = reqs.top();
		reqs.pop();
		res.total_score = n1.updateTotalScore(res.sinr,res.type, res.bandwidth,res.birth);
		tempo.push(res);
	}
	reqs = tempo;
	
	while(!ugs.empty()){
		res = ugs.top();
		ugs.pop();
		res.total_score = n1.updateTotalScore(res.sinr,res.type, res.bandwidth,res.birth);
		temp1.push(res);
	}
	ugs = temp1;
	while(!rtps.empty()){
		res = rtps.top();
		rtps.pop();
		res.total_score = n1.updateTotalScore(res.sinr,res.type, res.bandwidth,res.birth);
		temp2.push(res);
	}
	rtps = temp2;

	while(!nrtps.empty()){
		res = nrtps.top();
		nrtps.pop();
		res.total_score = n1.updateTotalScore(res.sinr,res.type, res.bandwidth,res.birth);
		temp3.push(res);
	}
	nrtps = temp3;
	
	while(!ertps.empty()){
		res = ertps.top();
		ertps.pop();
		res.total_score = n1.updateTotalScore(res.sinr,res.type, res.bandwidth,res.birth);
		temp4.push(res);
	}
	ertps = temp4;

	while(!be.empty()){
		res = be.top();
		be.pop();
		res.total_score = n1.updateTotalScore(res.sinr,res.type, res.bandwidth,res.birth);
		temp5.push(res);
	}
	be = temp5;
	
	
 	
}


void createRequestFile(){
	std::ofstream myfile;
	myfile.open ("request1.txt");
	int rowCount,colCount;
	//cout<<"Enter row and column numbers\n";
	//cin>>rowCount>>colCount;
			rowCount = row1;

	for(int i = 0; i< rowCount; i++){
		for(int j = 0;j<8; j++){
			//cin>>ary[i][j];	
			myfile << station1request[i][j];
			myfile << " ";	
		}	
		myfile << "\n";
	}
	myfile.close();	
	
	myfile.open ("request2.txt");
	//int rowCount,colCount;
	//cout<<"Enter row and column numbers\n";
	//cin>>rowCount>>colCount;
			rowCount = row2;

	for(int i = 0; i< rowCount; i++){
		for(int j = 0;j<8; j++){
			//cin>>ary[i][j];	
			myfile << station2request[i][j];
			myfile << " ";	
		}	
		myfile << "\n";
	}
	myfile.close();	
	myfile.open ("request3.txt");
	//int rowCount,colCount;
	//cout<<"Enter row and column numbers\n";
	//cin>>rowCount>>colCount;
			rowCount = row3;

	for(int i = 0; i< rowCount; i++){
		for(int j = 0;j<8; j++){
			//cin>>ary[i][j];	
			myfile << station3request[i][j];
			myfile << " ";	
		}	
		myfile << "\n";
	}
	myfile.close();	
	myfile.open ("request4.txt");
	//int rowCount,colCount;
	//cout<<"Enter row and column numbers\n";
	//cin>>rowCount>>colCount;
			rowCount = row4;

	for(int i = 0; i< rowCount; i++){
		for(int j = 0;j<8; j++){
			//cin>>ary[i][j];	
			myfile << station4request[i][j];
			myfile << " ";	
		}	
		myfile << "\n";
	}
	myfile.close();	
	myfile.open ("request5.txt");
	//int rowCount,colCount;
	//cout<<"Enter row and column numbers\n";
	//cin>>rowCount>>colCount;
			rowCount = row5;

	for(int i = 0; i< rowCount; i++){
		for(int j = 0;j<8; j++){
			//cin>>ary[i][j];	
			myfile << station5request[i][j];
			myfile << " ";	
		}	
		myfile << "\n";
	}
	myfile.close();	
	myfile.open ("request6.txt");
	//int rowCount,colCount;
	//cout<<"Enter row and column numbers\n";
	//cin>>rowCount>>colCount;
			rowCount = row6;

	for(int i = 0; i< rowCount; i++){
		for(int j = 0;j<8; j++){
			//cin>>ary[i][j];	
			myfile << station6request[i][j];
			myfile << " ";	
		}	
		myfile << "\n";
	}
	myfile.close();	

}

void saveResult(){
		 
	std::string ugs_result,rtps_result,nrtps_result,ertps_result,be_result;
	int temp,temp1;
	std::ofstream outfile;
//UGS
  	outfile.open("ugsresult.txt",std::ofstream::out | std::ofstream::app);
	temp = ugs_served-ugs_discarded+99999;
	temp1 = n1.ugs_generated-ugs_discarded;
	//temp = std::min(temp,temp1);
	//ugs_result += n1.ugs_generated+" "+temp;
	outfile << n1.ugs_generated <<"|" <<temp1<<std::endl;
	//ugs_result = n1.ugs_generated+" "+temp;
	//outfile << ugs_result;
	//outfile << "\n";
	outfile.close();
//RTPS
	outfile.open("rtpsresult.txt",std::ofstream::out | std::ofstream::app);
	temp = rtps_served-rtps_discarded+rtps.size();
	temp1 = n1.rtps_generated-rtps_discarded;
	temp = std::min(temp,temp1);
	//rtps_result = n1.rtps_generated+" "+temp;
	outfile << n1.rtps_generated <<"|" <<temp1<<std::endl;
	//rtps_result = n1.rtps_generated+" "+temp;
	//outfile << rtps_result;
	//outfile << "\n";
	outfile.close();
//NRTPS
	outfile.open("nrtpsresult.txt",std::ofstream::out | std::ofstream::app);
	temp = nrtps_served-nrtps_discarded+nrtps.size();
	temp1 = n1.nrtps_generated-nrtps_discarded;
	temp = std::min(temp,temp1);
	//nrtps_result = n1.nrtps_generated+" "+temp;
	outfile << n1.nrtps_generated <<"|" <<temp1<<std::endl;	
	//nrtps_result = n1.nrtps_generated+" "+temp;
	//outfile << nrtps_result;
	//outfile << "\n";
	outfile.close();
//ERTPS
	outfile.open("ertpsresult.txt",std::ofstream::out | std::ofstream::app);
	temp = ertps_served-ertps_discarded+ertps.size();
	temp1= n1.ertps_generated-ertps_discarded;
	temp = std::min(temp,temp1);
	outfile << n1.ertps_generated <<"|" <<temp1<<std::endl;
	//ertps_result = n1.ertps_generated+" "+temp;
	//outfile << ertps_result;
	//outfile << "\n";
	outfile.close();
//BE
	outfile.open("beresult.txt",std::ofstream::out | std::ofstream::app);
	temp = be_served-be_discarded+be.size();
	temp1 = n1.be_generated-be_discarded;
	temp = std::min(temp,temp1);
	outfile << n1.be_generated <<"|" <<temp1<<std::endl;
	//be_result = n1.be_generated+" "+temp;
	//outfile << be_result;
	//outfile << "\n";
	outfile.close();
}


void saveResultNormalised(){
		 
	std::string ugs_result = " ",rtps_result = " ",nrtps_result= " ",ertps_result=" ",be_result=" ";
	int temp,temp1;
	std::ofstream outfile;
//UGS
  	outfile.open("ugsresult1.txt",std::ofstream::out | std::ofstream::app);
	temp = ugs_served-ugs_discarded+ugs.size();
	temp1 = n1.ugs_generated-ugs_discarded;
	temp = std::min(temp,temp1);
	//ugs_result += n1.ugs_generated+" "+temp;
	outfile << n1.ugs_generated <<"|" <<temp1<<std::endl;
	//outfile <<"|";
	//outfile << temp;
	//outfile << "\n";
	//outfile << ugs_result;
	outfile.close();
//RTPS
	outfile.open("rtpsresult1.txt",std::ofstream::out | std::ofstream::app);
	temp = rtps_served-rtps_discarded+rtps.size();
	temp1 = n1.rtps_generated-rtps_discarded;
	temp = std::min(temp,temp1);
	//rtps_result = n1.rtps_generated+" "+temp;
	outfile << n1.rtps_generated <<"|" <<temp1<<std::endl;
	//outfile << rtps_result;
	//outfile << "\n";
	outfile.close();
//NRTPS
	outfile.open("nrtpsresult1.txt",std::ofstream::out | std::ofstream::app);
	temp = nrtps_served-nrtps_discarded+nrtps.size();
	temp1 = n1.nrtps_generated-nrtps_discarded;
	temp = std::min(temp,temp1);
	//nrtps_result = n1.nrtps_generated+" "+temp;
	outfile << n1.nrtps_generated <<"|" <<temp1<<std::endl;	
	//outfile << nrtps_result;
	//outfile << "\n";
	outfile.close();
//ERTPS
	outfile.open("ertpsresult1.txt",std::ofstream::out | std::ofstream::app);
	temp = ertps_served-ertps_discarded+ertps.size();
	temp1= n1.ertps_generated-ertps_discarded;
	temp = std::min(temp,temp1);
	outfile << n1.ertps_generated <<"|" <<temp1<<std::endl;
	//ertps_result = n1.ertps_generated+" "+temp;
	//outfile << ertps_result;
	//outfile << "\n";
	outfile.close();
//BE
	outfile.open("beresult1.txt",std::ofstream::out | std::ofstream::app);
	temp = be_served-be_discarded+be.size();
	temp1 = n1.be_generated-be_discarded;
	temp = std::min(temp,temp1);
	outfile << n1.be_generated <<"|" <<temp1<<std::endl;
	//be_result = n1.be_generated+" "+temp;
	//outfile << be_result;
	//outfile << "\n";
	outfile.close();
}

void displayResult(){
	int temp,temp1;
	std::cout<<"Total Requests generated: "<<total_requests_generated<<"\n";	
	std::cout<<"Total Requests served: "<<total_requests_generated - req_discarded<<"\n";
        std::cout<<"Requests Discarded Because of Deadline: "<<req_discarded<<"\n";
	std::cout<<"Type\tTotal\tServed\t\n";
	temp = ugs_served-ugs_discarded+999999;
	temp1 = n1.ugs_generated-ugs_discarded;
	std::cout<<"UGS\t"<<n1.ugs_generated<<"\t"<<std::min(temp,temp1)<<"\t\n";//<<ugs_discarded<<"\n";
	temp = rtps_served-rtps_discarded+999999;
	temp1 = n1.rtps_generated-rtps_discarded;
	std::cout<<"RTPS\t"<<n1.rtps_generated<<"\t"<<std::min(temp,temp1)<<"\t\n";//<<rtps_discarded<<"\n";
	temp = nrtps_served-nrtps_discarded+999999;
	temp1 = n1.nrtps_generated-nrtps_discarded;
	std::cout<<"NRTPS\t"<<n1.nrtps_generated<<"\t"<<std::min(temp,temp1)<<"\t\n";//<<nrtps_discarded<<"\n";
	temp = ertps_served-ertps_discarded+999999;
	temp1= n1.ertps_generated-ertps_discarded;
	std::cout<<"ERTPS\t"<<n1.ertps_generated<<"\t"<<std::min(temp,temp1)<<"\t\n";//<<ertps_discarded<<"\n";
	temp = be_served-be_discarded + 99999;
	temp1 = n1.be_generated-be_discarded;
	std::cout<<"BE\t"<<n1.be_generated<<"\t"<<std::min(temp,temp1)<<"\t\n";//<<be_discarded<<"\n";

	std::cout<<"Size of reqSatisfy: "<<reqSatisfy.size()<<"\n";
	std::cout<<"Size of ugs: "<<ugs.size()<<"\n";
	std::cout<<"Size of rtps: "<<rtps.size()<<"\n";
	std::cout<<"Size of nrtps: "<<nrtps.size()<<"\n";
	std::cout<<"Size of ertps: "<<ertps.size()<<"\n";
	std::cout<<"Size of be: "<<be.size()<<"\n";
	std::cout<<"ugs discarded: "<<ugs_discarded<<"\n";
	std::cout<<"rtps discarded: "<<rtps_discarded<<"\n";
	std::cout<<"nrtps discarded: "<<nrtps_discarded<<"\n";
	std::cout<<"ertps discarded: "<<ertps_discarded<<"\n";
	std::cout<<"be discarded: "<<be_discarded<<"\n";
}

int readReq(){
	std::vector<int> data;
	 int value;
   	 std::ifstream file("totalrequest.txt");// file path
    	 std::string line;
    while (getline(file, line))
    {
        data.push_back(int());
        std::istringstream ss(line);
       
        while (ss >> value)
        {
            data.push_back(value);
        }
    }
	req_count = value;	
	file.close();
	
	//std::cout<<value<<std::endl;
    return value;
}

int writeReq(){

	std::ofstream myfile;
	myfile.open ("totalrequest.txt");
	req_count++;
	myfile << req_count;
	myfile.close();
	return 0;
}

void updateBandwidth(int size,double bw, int type){
	double temp;
	double div;
	if(type == 10){
		div = 4;	
	}
	if(type == 9) div = 3;
	if(type == 8) div = 2;
	if(type == 7) div = 1;
	if(size == 0){
		temp = bw/div;
		switch(type){
		case 10:bwrtps+=temp;
			bwnrtps+=temp;
			bwertps+=temp;
			bwbe+=temp;
			break;
		case 9:bwnrtps+=temp;
			bwertps+=temp;
			bwbe+=temp;
			break;
		case 8: bwertps+=temp;
			bwbe+=temp;
			break;
		case 7:bwbe+=temp;
			break;		
		}
	}
}

void initializeBandwidthAllocated(){
	bandwidthAllocated = 0;
	ugsBandwidthAllocated = 0; 
	rtpsBandwidthAllocated =0; 
	nrtpsBandwidthAllocated =0;
        ertpsBandwidthAllocated = 0;
 	beBandwidthAllocated = 0;
}
